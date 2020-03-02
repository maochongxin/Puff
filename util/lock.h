// Copyright (c) 2020 The Puff Authors. All rights reserved.

#ifndef PUFF_UTIL_LOCK_H_
#define PUFF_UTIL_LOCK_H_

#include <cassert>
#include <atomic>
#include <mutex>

namespace puff {

class SpinLock {
public:
    void Lock() {
		while(lock.test_and_set(std::memory_order_acquire)) {}
    }

	void Unlock() {
		lock.clear(std::memory_order_acquire);
	}
private:
	std::atomic_flag lock = ATOMIC_FLAG_INIT;
};

} // end of namespace puff


#endif // PUFF_UTIL_LOCK_H_