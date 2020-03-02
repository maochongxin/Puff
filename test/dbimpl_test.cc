// Copyright (c) 2020 The Puff Authors. All rights reserved.

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include <map>

#include "includes/options.h"
#include "includes/comparator.h"
#include "includes/db.h"
using namespace puff;
std::string random_string( size_t length ) {
  auto randchar = []() -> char {
    const char charset[] = "0123456789"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[ rand() % max_index ];
  };
  std::string str(length,0);
  std::generate_n( str.begin(), length, randchar );
  return str;
}

int main() {
    Options opts;
    opts.comparator_ = new BytewiseComparator();
    std::string db_name = "puff_test";

    DB* db;
    DB::Open(db_name, opts, &db);

    std::map<std::string, std::string> _map;

    for (int32_t i = 0; i < 2; ++i) {
        auto key = random_string(5);
        auto value = random_string(8);
        _map[key] = value;
        auto res = db->Put(key, value);
        if (!res) {
            std::cout << "Write failed. " << key << std::endl;
        }

        std::string read_value;
        res = db->Get(key, &read_value);
        if (!res || value != read_value) {
            std::cout << "Read value failed: " << key << std::endl;
        }
    }

    delete db;

    DB* reopen_db;
    DB::Open(db_name, opts, &reopen_db);
    for (auto it = _map.begin(); it != _map.end(); ++it) {
        std::string read_value;
        auto s = reopen_db->Get(it->first, &read_value); 
        if (!s || it->second != read_value) {
        std::cout << "Reread value failed: " << it->first << std::endl;
        }
    }

    // Delete
    for (auto it = _map.begin(); it != _map.end(); ++it) {
        auto s = reopen_db->Delete(it->first);
        if (!s) {
        std::cout << "Delete value failed: " << it->first << std::endl;
        }
    }

    for (auto it = _map.begin(); it != _map.end(); ++it) {
        std::string read_value;
        auto s = reopen_db->Get(it->first, &read_value);
        if (s || read_value != "") {
        std::cout << "Delete value failed: " << it->first << std::endl;
        }
    }
    delete reopen_db;

    delete opts.comparator_;

}