// Copyright (c) 2018 The atendb Authors. All rights reserved.

#ifndef PUFF_INCLUDE_OPTIONS_H_
#define PUFF_INCLUDE_OPTIONS_H_

#include "includes/comparator.h"
#include "includes/env.h"

namespace puff {

class Options {
public:
    Options():
        comparator_(nullptr) {}
    Comparator* comparator_;
};


} // end of namespace puff

#endif // PUFF_INCLUDE_OPTIONS_H_
