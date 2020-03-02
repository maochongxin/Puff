// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef PUFF_UTIL_CODING_H_
#define PUFF_UTIL_CODING_H_

#include <iostream>

#include "includes/index.h"

namespace puff {

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

void EncodeData(char* buf,
        const std::string& key,
        const std::string& value);

void DecodeData(char* buf,
        uint32_t key_size,
        uint32_t value_size,
        std::string* value);

void EncodeIndex(char* buf,
        const std::string& key,
        uint32_t file_index,
        uint32_t file_offset,
        uint32_t key_size,
        uint32_t value_size);

void EncodeIndex(char* buf, const Index& index);

void DecodeIndex(char* buf,
        std::string* key,
        uint32_t* file_index,
        uint64_t* file_offset,
        uint32_t* key_size,
        uint32_t* value_size);

void DecodeIndex(char* buf, Index* index);

uint32_t DecodeFixed32(const char* buf);

uint64_t DecodeFixed64(const char* buf);

} // end of namespace puff


#endif // PUFF_UTIL_CODING_H_