//
// Created by Neburalis on 05.01.2026.
// https://github.com/Neburalis/c-allocators
// Licensed by GNU General Public License v3.0
//

#ifndef MYALLOCATORS_ARENA_H
#define MYALLOCATORS_ARENA_H

#include <stdalign.h>

#ifndef allocator
#include <stdlib.h>
#define allocator malloc
#endif

struct Arena;

#define arena_alloc_type(a, type) \
    (type *) arena_alloc(a, sizeof(type), alignof(type))

#define arena_alloc_array(a, type, count) \
    (type *) arena_alloc(a, count * sizeof(type), alignof(type))

#ifdef _DEBUG
    #define arena_dump(a) arena_dump_impl(a)
#else
    #define arena_dump(a) (void) a
#endif

// Создает арену (по-умолчанию память не очищается при создании)
struct Arena * arena_init(size_t capacity);
// Выделяет память в арене. Если align не степень двойки, то UB. Возвращает NULL если не удалось
void * arena_alloc(struct Arena *a, size_t size, size_t align);
// Освобождает все, что было выделено в арене (арена остается валидной)
void arena_reset  (struct Arena *a);
// Сбрасывает все значения к 0
void arena_clear  (struct Arena *a);
// Освобождает арену
void arena_destroy(struct Arena *a);

// Свободной памяти в арене
size_t arena_available(const struct Arena *a);
// Занято памяти в арене
size_t arena_used     (const struct Arena *a);

void arena_dump_impl(const struct Arena *a);

#endif //MYALLOCATORS_ARENA_H