#pragma once

#ifdef LAZY_MEMORY_CHECKER

#include <memory>

struct AllocationMetrics {
    uint32_t allocated = 0;
    uint32_t freed = 0;

    uint32_t GetCurrentAllocated() { return allocated - freed; }
};

static AllocationMetrics s_allocations;

#endif