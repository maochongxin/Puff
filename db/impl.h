// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef PUFF_DB_DB_IMPL_H_
#define PUFF_DB_DB_IMPL_H_

#include <algorithm>
#include <atomic>
#include <vector>

#include "file/file.h"
#include "includes/db.h"
#include "includes/comparator.h"
#include "includes/options.h"
#include "includes/skiplist.h"
#include "util/arena.h"
#include "util/lock.h"
#include "includes/bitcask.h"

namespace puff {

const std::string IndexFileName = "/INDEX-";
const std::string DataFileName = "/DATA-";

using Table = SkipList<Index, Compare>;

class DBImpl: public DB {
public:
    DBImpl(const std::string& dbname, const Options& options);

    virtual ~DBImpl();

    bool Put(const std::string& key, const std::string& value) override;

    bool Get(const std::string& key, std::string* value) override;

    bool Delete(const std::string& key) override;

    void IndexCallback(File* file);

    bool Recover();

    void operator=(const DBImpl&) = delete;

    DBImpl(const DBImpl&) = delete;

private:
    friend DB;

    std::string dbname_;
    Options options_;

    std::vector<File* > data_files_;
    std::vector<File* > index_files_;
    bitcask::Bitcask<SkipList, Index, Compare> bitcask;
};

} // end of namespace puff

#endif // PUFF_DB_DB_IMPL_H_
