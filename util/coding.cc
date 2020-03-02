// Copyright (c) 2020 The Puff Authors. All rights reserved.

#include <cassert>
#include <cstring>

#include "util/coding.h"

namespace puff {

    void EncodeData(char* buf, const std::string& key, const std::string& value) {
        memcpy(buf, key.data(), key.size());
        memcpy(buf + key.size(), value.data(), value.size());
    }

    void DecodeData(char* buf,
            uint32_t key_size,
            uint32_t value_size,
            std::string* value) {
        *value = std::string(buf + key_size, value_size);
    }

    void EncodeIndex(char* buf,
        const std::string& key,
        uint32_t file_index,
        uint32_t file_offset,
        uint32_t key_size,
        uint32_t value_size) {
        assert(buf);
        size_t pos = 0;

        uint32_t index_size = key_size + sizeof(uint32_t) * 3 + sizeof(uint64_t);
        // The size of index
        memcpy(buf, &index_size, sizeof(uint32_t));
        pos += sizeof(uint32_t);
        // File Offset
        memcpy(buf + pos, &file_offset, sizeof(uint64_t));
        pos += sizeof(uint64_t);
        // The size of key
        memcpy(buf + pos, &key_size, sizeof(uint32_t));
        pos += sizeof(uint32_t);
        // Key
        memcpy(buf + pos, key.data(), key_size);
        pos += key_size;
        // The size of value
        memcpy(buf + pos, &value_size, sizeof(uint32_t));
    }

    void EncodeIndex(char* buf, const Index& index) {
        assert(buf);
        size_t pos = 0;

        uint32_t index_size = index.KeySize() + sizeof(uint32_t) * 3 + sizeof(uint64_t);
        // The size of index
        memcpy(buf, &index_size, sizeof(uint32_t));
        pos += sizeof(uint32_t);
        // File Offset
        memcpy(buf + pos, &index.file_offset_, sizeof(uint64_t));
        pos += sizeof(uint64_t);
        // The size of key
        memcpy(buf + pos, &index.key_size_, sizeof(uint32_t));
        pos += sizeof(uint32_t);
        // Key
        memcpy(buf + pos, index.Key().data(), index.KeySize());
        pos += index.KeySize();
        // The size of value
        memcpy(buf + pos, &index.value_size_, sizeof(uint32_t));
    }

    void DecodeIndex(char* buf,
            std::string* key,
            uint32_t* file_index,
            uint64_t* file_offset,
            uint32_t* key_size,
            uint32_t* value_size) {
        assert(buf);

        size_t pos = 0;

        *file_index = DecodeFixed32(buf);
        pos += sizeof(uint32_t);

        *file_offset = DecodeFixed64(buf + pos);
        pos += sizeof(uint64_t);

        *key = std::string(buf + pos, *key_size);
        pos += *key_size;

        *value_size = DecodeFixed32(buf + pos);
    }

    void DecodeIndex(char* buf, Index* index) {
        assert(buf);
        size_t pos = 0;

        index->file_index_ = DecodeFixed32(buf);
        pos += sizeof(uint32_t);

        index->file_offset_ = DecodeFixed64(buf + pos);
        pos += sizeof(uint64_t);

        index->key_ = std::string(buf + pos, index->key_size_);
        pos += index->key_size_;

        index->value_size_ = DecodeFixed32(buf + pos);
    }

    uint32_t DecodeFixed32(const char* ptr) {
        uint32_t result;
        memcpy(&result, ptr, sizeof(result));
        return result;
    }

    uint64_t DecodeFixed64(const char* ptr) {
        uint64_t result;
        memcpy(&result, ptr, sizeof(result));
        return result;
    }



} // end of namespace puff