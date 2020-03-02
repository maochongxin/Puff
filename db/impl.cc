// Copyright (c) 2020 The Puff Authors. All rights reserved.

#include <iostream>
#include <thread>
#include <vector>

#include "impl.h"
#include "util/coding.h"
#include "util/hash.h"

namespace puff {

DBImpl::DBImpl(const std::string& dbname, const Options& options):
    dbname_(dbname),
    options_(options) {}

DBImpl::~DBImpl() {
    for (auto file : data_files_) {
        delete file;
    }
    for (auto file : index_files_) {
        delete file;
    }
}

bool DBImpl::Put(const std::string& key, const std::string& value) {
    return bitcask.Put(data_files_, index_files_, key, value);
}

bool DBImpl::Get(const std::string& key, std::string* value) {
    return bitcask.Get(data_files_, key, value);
}

bool DBImpl::Delete(const std::string& key) {
    return bitcask.Delete(key);
}

// bool DBImpl::Recover() {
//     return bitcask.Recover(index_files_);
// }

bool DB::Open(const std::string& dbname,
              const Options& options,
              DB** db) {
    *db = nullptr;
    DBImpl* impl = new DBImpl(dbname, options);

    // Get the number of processor
    int32_t num_processor = sysconf(_SC_NPROCESSORS_CONF);
    if (num_processor == -1) {
        return false;
    }

    impl->bitcask.InitBitcask(num_processor, options);

    // Create data directory
    std::string current_dir;
    if (!Env::GetCurrentDir(&current_dir)) {
        return false;
    }

    std::string data_dir = current_dir + "/" + dbname;
    if (!Env::FileExists(data_dir)) {
        auto s = Env::CreateDir(data_dir);
        if (!s) {
            return false;
        }
    }

    // Open data
    for (uint32_t i = 0; i < impl->bitcask.MaxFiles(); i++) {
        std::string data_file = data_dir + DataFileName + std::to_string(i );
        File* file;
        auto s = Env::NewFile(data_file, &file);
        if (s) {
            impl->data_files_.push_back(file);
        } else {
            return false;
        }
    }

    // Open index
    for (uint32_t i = 1; i <= impl->bitcask.MaxFiles(); i++) {
        std::string index_file = data_dir + IndexFileName + std::to_string(i);
        File* file;
        auto s = Env::NewFile(index_file, &file);
        if (s) {
            impl->index_files_.push_back(file);
        } else {
            return false;
        }
    }

    // Recover
    impl->bitcask.Recover(impl->index_files_);
    
    *db = impl;
    return true;
}

}