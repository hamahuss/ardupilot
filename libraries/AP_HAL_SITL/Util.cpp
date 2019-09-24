#include "Util.h"

uint64_t HALSITL::Util::get_hw_rtc() const
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    const uint64_t seconds = ts.tv_sec;
    const uint64_t nanoseconds = ts.tv_nsec;
    return (seconds * 1000000ULL + nanoseconds/1000ULL);
}
<<<<<<< HEAD
=======

void *HALSITL::Util::heap_realloc(void *heap_ptr, void *ptr, size_t new_size)
{
    if (heap_ptr == nullptr) {
        return nullptr;
    }

    struct heap *heapp = (struct heap*)heap_ptr;

    // extract appropriate headers
    size_t old_size = 0;
    heap_allocation_header *old_header = nullptr;
    if (ptr != nullptr) {
        old_header = ((heap_allocation_header *)ptr) - 1;
        old_size = old_header->allocation_size;
    }

    if ((heapp->current_heap_usage + new_size - old_size) > heapp->scripting_max_heap_size) {
        // fail the allocation as we don't have the memory. Note that we don't simulate fragmentation
        return nullptr;
    }

    heapp->current_heap_usage -= old_size;
    if (new_size == 0) {
       free(old_header);
       return nullptr;
    }

    heap_allocation_header *new_header = (heap_allocation_header *)malloc(new_size + sizeof(heap_allocation_header));
    if (new_header == nullptr) {
        // total failure to allocate, this is very surprising in SITL
        return nullptr;
    }
    heapp->current_heap_usage += new_size;
    new_header->allocation_size = new_size;
    void *new_mem = new_header + 1;

    if (ptr == nullptr) {
        return new_mem;
    }
    memcpy(new_mem, ptr, old_size > new_size ? new_size : old_size);
    free(old_header);
    return new_mem;
}

#endif // ENABLE_HEAP
>>>>>>> upstream/master
