// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef PUFF_INCLUDE_COMPARATOR_H_
#define PUFF_INCLUDE_COMPARATOR_H_

#include <string.h>

#include "includes/index.h"

namespace puff {

class Comparator {
public:
    virtual ~Comparator() {}
    virtual int compare(const std::string& lhs, const std::string& rhs) const = 0;
};

class BytewiseComparator: public Comparator {
public:
    int compare(const std::string& lhs, const std::string& rhs) const override {
        return lhs.compare(rhs);
    }
};

class Compare {
public:
    Compare(Comparator* comparator): comparator_(comparator) {}

    int operator()(const Index& lhs, const Index& rhs) const {
        return comparator_->compare(lhs.Key(), rhs.Key());
    }
    int operator()(const std::string& lhs, const std::string& rhs) const {
        return strcmp(lhs.c_str(), rhs.c_str());
    }
private:
    Comparator* comparator_;
};

} // end of namespace puff

#endif // PUFF_INCLUDE_COMPARATOR_H_
