// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef REDIS_UTIL_HASH_H_
#define REDIS_UTIL_HASH_H_

namespace puff {


	unsigned int SDBMHash(const char* str) {
		unsigned int hash_code = 0;
		while (*str) {
			hash_code = (*str++) + (hash_code << 6) + (hash_code << 16) - hash_code;
		}
		return (hash_code & 0x7FFFFFFF);
	}

	unsigned int Hash(const char* str) {
		unsigned int a = 63689;
		unsigned int b = 378551;
		unsigned int hash_code = 0;
		while (*str) {
			hash_code = hash_code * a + (*str++);
			a *= b;
		}
		return (hash_code & 0x7FFFFFFF);
	}

}


#endif
