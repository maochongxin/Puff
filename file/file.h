// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef PUFF_FILE_FILE_H_
#define PUFF_FILE_FILE_H_

#include <string>

namespace puff {

class File {
public:
    File() {}
    virtual ~File() {}

    virtual bool Read(uint64_t offset, uint32_t n, char* buf) = 0;

    virtual bool AppendData(const std::string& key,
            const std::string& value,
            uint64_t* file_offset) = 0;
    
    virtual bool AppendIndex(const std::string& key,
            uint32_t file_index,
            uint64_t file_offset,
            uint32_t key_size,
            uint32_t value_size) = 0;

    virtual bool Close() = 0;

    virtual uint64_t FileOffset() = 0;
};

} // end of namespace puff

#endif // PUFF_FILE_FILE_H_
