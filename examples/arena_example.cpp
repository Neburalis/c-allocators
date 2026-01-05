//
// Created by Neburalis on 05.01.2026.
// https://github.com/Neburalis/c-allocators
// Licensed by GNU General Public License v3.0
//

#include "allocators.h"

#include <stdio.h>

int main() {
    struct Arena *arena = arena_init(100);
    if (arena == NULL) {
        printf("arena is NULL\n");
        return -1;
    }

    const int ARR_SIZE = 15;

    int *arr = arena_alloc_array(arena, int, ARR_SIZE);
    if (arr == NULL) {
        printf("arena not allocate memory\n");
        return -1;
    }

    arena_dump(arena);

    for (int i = 0; i < ARR_SIZE; ++i) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    for (int i = 0; i < ARR_SIZE; ++i) {
        arr[i] = i*i;
    }

    for (int i = 0; i < ARR_SIZE; ++i) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    char *str = arena_alloc_array(arena, char, 6);
    if (str == NULL) {
        printf("arena not allocate memory\n");
        return -1;
    }
    str[0] = 'h';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = '\0';

    printf("str = %s\n", str);

    arena_dump(arena);

    double *val = arena_alloc_type(arena, double);
    if (val == NULL) {
        printf("arena not allocate memory\n");
        return -1;
    }

    printf("val(at %p) = %lg\n", val, *val);
    *val = 3.14;
    printf("val(at %p) = %lg\n", val, *val);

    ++val;
    printf("val(at %p) = %lg\n", val, *val);

    for (int i = 0; i < ARR_SIZE; ++i) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    arena_clear(arena);

    for (int i = 0; i < ARR_SIZE; ++i) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    arena_destroy(arena);

    return 0;
}