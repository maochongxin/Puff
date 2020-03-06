// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef PUFF_INCLUDE_INDEX_H_
#define PUFF_INCLUDE_INDEX_H_

#include <string>

namespace puff {

class Index {
public:
    Index() {}

    Index(std::string key):
        key_(key),
        file_index_(0),
        file_offset_(0),
        key_size_(0),
        value_size_(0) {}
    
    Index(const std::string& key, uint32_t file_index, 
        uint64_t file_offset, uint32_t key_size, uint32_t value_size):
        key_(key),
        file_index_(file_index),
        file_offset_(file_offset),
        key_size_(key_size),
        value_size_(value_size) {}
    
    ~Index() {}

    std::string Key() const { return key_; }

    uint32_t FileIndex() const { return file_index_; }

    uint64_t FileOffset() const { return file_offset_; }

    uint32_t KeySize() const { return key_size_; }

    uint32_t ValueSize() const { return key_size_; }

    uint32_t DataSize() const { return key_size_ + value_size_; }

    uint32_t IndexSize() const {
        return key_size_ + sizeof(uint32_t) * 3 + sizeof(uint64_t);
    }

    void SetDeleted() {
        key_size_ = 0;
        value_size_ = 0;
    }

    Index& operator=(const Index& index) {
        key_ = index.key_;
        file_index_ = index.file_index_;
        file_offset_ = index.file_offset_;
        key_size_ = index.key_size_;
        value_size_ = index.value_size_;
        return *this;
    }

    friend void EncodeIndex(char* buf, const Index& index);
    friend void DecodeIndex(char* buf, Index* index);
private:
    std::string key_;
    uint32_t file_index_;
    uint64_t file_offset_;

    uint32_t key_size_;
    uint32_t value_size_;
};

} // end of namespace puff

#endif // PUFF_INCLUDE_INDEX_H_
