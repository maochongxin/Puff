// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef PUFF_INCLUDES_BITCASK_H_
#define PUFF_INCLUDES_BITCASK_H_

#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "includes/options.h"
#include "util/coding.h"
#include "util/hash.h"


namespace puff {
namespace bitcask {

template <template <typename Key, typename Hash> typename HashTable, typename Key, typename Hash>
class Bitcask {
public:
    Bitcask(): max_files_(0), cursor_(0) {}
    ~Bitcask() {
        for (auto table : tables_) {
            delete table;
        }
    }

    template <typename Container>
    bool Put (Container data_container, 
        Container index_container,
        const std::string& key,
        const std::string& value) {
        uint32_t cursor = cursor_++;
        uint32_t file_index = (cursor & (max_files_ - 1));
        uint64_t file_offset;
        
        auto res = data_container[file_index]->AppendData(key, value, &file_offset);
        if (likely(res)) {
            res = index_container[file_index]->AppendIndex(key, file_index, file_offset, key.size(), value.size());
            if (likely(res)) {
                // to store put info
                Key index(key, file_index, file_offset, key.size(), value.size());
                int32_t slot = (std::hash<std::string>{}(key) & (tables_.size() - 1));
                tables_[slot]->Insert(index); // duck type
            }
        }
        return res;
    }
    
    template <typename Container>
    bool Get(Container data_container,
    const std::string& key,
    std::string* value) {
        int32_t slot = (std::hash<std::string>{}(key) & (table_size_ -1));
        Key target(key), result;
        auto res = tables_[slot]->Get(target, &result);
        if (likely(res)) {
            if (unlikely(result.KeySize() == 0 &&
                result.ValueSize() == 0)) {
                *value = "";
                return false;
            }
            char buf[result.DataSize()];
            res = data_container[result.FileIndex()]->
                    Read(result.FileOffset(), result.DataSize(), buf);
            if (likely(res)) {
                DecodeData(buf, result.KeySize(), result.ValueSize(), value);
            }
        }
        return res;
    };
    
    bool Delete(const std::string& key) {
        int32_t slot = (std::hash<std::string>{}(key) & (table_size_ - 1));
        Key target(key), result;
        auto res = tables_[slot]->Get(target, &result);
        if (likely(res)) {
            result.SetDeleted();
            tables_[slot]->Insert(result);
        }
        return res;
    }
    template <typename File>
    void IndexCallBack(File* file) {
        uint64_t file_offset = file->FileOffset();
        if (file_offset) {
            uint64_t pos = 0;
            while (pos < file_offset) {
                char size_buf[sizeof(uint32_t)];
                auto res = file->Read(pos, sizeof(uint32_t), size_buf);
                if (likely(res)) {
                    pos += sizeof(uint32_t);
                    uint32_t index_size = DecodeFixed32(size_buf);
                    char index_buf[index_size];
                    res = file->Read(pos, index_size, index_buf);
                    if (likely(res)) {
                        pos += index_size;
                        Key key;
                        DecodeIndex(index_buf, &key);
                        int32_t slot = (std::hash<std::string>{}(key.Key()) & (table_size_ - 1));
                        tables_[slot]->Insert(key);
                    }
                }
            }
        }
    }

    template <typename Container>
    bool Recover(Container index_files) {
        std::vector<std::thread> threads;
        using type = typename std::remove_pointer<typename Container::value_type>::type;
        for (uint32_t i = 0; i < max_files_; ++i) {
            std::thread recover(&Bitcask::IndexCallBack<type>, this, index_files[i]);
            threads.push_back(std::move(recover));
        }
        for (uint32_t i = 0; i < max_files_; ++i) {
            threads[i].join();
        }
        return true;
    }

    inline void InitBitcask(uint32_t num_processor, const Options& option) {
        max_files_ = num_processor * 2;
        table_size_ = num_processor;
        for (uint32_t i = 0; i < table_size_; ++i) {
            HashTable<Key, Hash>* key = new HashTable<Key, Hash>(Hash(option.comparator_));
            tables_.push_back(key);
        }
    }

    inline uint32_t MaxFiles() const { return max_files_; }

private:
    
    uint32_t table_size_;
    uint32_t max_files_;
    std::atomic<uint32_t> cursor_;
    std::vector<HashTable<Index, Hash>*> tables_;
};


} // end of namespace bitcask 
} // end of namespace puff 


#endif // PUFF_INCLUDES_BITCASK_H_
