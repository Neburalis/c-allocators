//
// Created by Neburalis on 05.01.2026.
// https://github.com/Neburalis/c-allocators
// Licensed by GNU General Public License v3.0
//

#include "arena.h"

#include <io_utils.h>

#include <string.h>
#include <stdio.h>

struct Arena {
    void *buffer;
    size_t capacity, offset;
};

Arena * arena_init(const size_t capacity) {
    auto *a = static_cast<struct Arena *>(allocator(sizeof(struct Arena)));
    if (a == nullptr)
        return nullptr;
    void *arena = allocator(capacity);
    if (arena == nullptr) {
        free(a);
        return nullptr;
    }
    a->buffer = arena;
    a->offset = 0;
    a->capacity = capacity;
    return a;
}

struct Arena * arena_from_buffer(void *buf, const size_t capacity) {
    struct Arena *a = (struct Arena *) allocator(sizeof(struct Arena));
    if (a == nullptr)
        return nullptr;
    a->buffer = buf;
    a->offset = 0;
    a->capacity = capacity;
    return a;
}

void * arena_alloc(struct Arena *a, const size_t size, const size_t align) {
#ifdef _DEBUG
    if ((align & (align - 1)) != 0)
        return nullptr;
#endif //_DEBUG
    if (a != nullptr)
        if (a->buffer != nullptr) {
            /* Как это работает:
             * Остаток от деления:
             * Работает только если align - степень двойки.
             * Битовое представление для степеней двойки:
             * 16        -> 00010000
             * align - 1 -> 00001111
             * 8         -> 00001000
             * align - 1 -> 00000111
             * У степеней двойки только 1 бит равен 1, а у чисел на 1 меньше, все младшие биты равны 1
             * align - 1    -> 00001111
             * а именно младшие биты отвечают за остаток от деления.
             * Поэтому offset & (align - 1) эквивалентно offset % align, но работает сильно быстрее.
             */
            const size_t misalignment = a->offset & (align - 1);
            const size_t padding = (misalignment == 0) ? 0 : align - misalignment;

            if (a->offset > SIZE_MAX - padding)
                return nullptr;

            const size_t aligned_offset = a->offset + padding;

            if (size <= a->capacity - aligned_offset) {
                void *ptr = (char *)a->buffer + aligned_offset;
                a->offset = aligned_offset + size;
                return ptr;
            }
        }
    return nullptr;
}

void arena_reset(struct Arena *a) {
    if (a != nullptr)
        if (a->buffer != nullptr)
            a->offset = 0;
}

void arena_clear(struct Arena *a) {
    if (a != nullptr)
        if (a->buffer != nullptr) {
            memset(a->buffer, 0, a->capacity);
            a->offset = 0;
        }
}

void arena_destroy(struct Arena *a) {
    if (a != nullptr) {
        free(a->buffer);
        a->buffer = nullptr;
        a->offset = 0;
        a->capacity = 0;
        free(a);
    }
}

size_t arena_available(const struct Arena *a) {
    if (a != nullptr)
        return a->capacity - a->offset;
    return 0;
}

size_t arena_used(const struct Arena *a) {
    if (a != nullptr)
        return a->offset;
    return 0;
}

void arena_dump_impl(const struct Arena *a) {
    if (a != nullptr) {
        printf("arena at %p:\n", a);
        printf("  offset = %lu\n", a->offset);
        printf("  capacity = %lu\n", a->capacity);
        if (a->buffer != nullptr) {
            printf("  buffer at %p:\n", a->buffer);
            memdump(a->buffer, a->capacity);
        }
        else
            printf("  buffer = %p\n", a->buffer);
    }
    else {
        printf("arena: NULL\n");
    }
}
