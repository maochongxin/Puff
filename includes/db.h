// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef PUFF_INCLUDE_DB_H_
#define PUFF_INCLUDE_DB_H_

#include "includes/comparator.h"
#include "includes/env.h"
#include "includes/options.h"

namespace puff {

class DB {
public:
    DB() {}
    virtual ~DB() {}

    static bool Open(const std::string& dname, const Options& options, DB** dbptr);

    virtual bool Put(const std::string& key, const std::string& value) = 0;

    virtual bool Get(const std::string& key, std::string* value) = 0;
    
    virtual bool Delete(const std::string& key) = 0;

    // No copying allowed
    DB& operator=(const DB&) = delete;
    DB(const DB&) = delete;
};

} // end of namespace puff

#endif
