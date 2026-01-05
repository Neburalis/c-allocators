# myAllocators

Библиотека аллокаторов памяти на C.

### Arena Allocator

Arena allocator (также известный как linear/bump allocator) - это простой и быстрый аллокатор, который выделяет память последовательно из непрерывного буфера. Основные преимущества:

- **Скорость**: O(1) выделение памяти без накладных расходов на метаданные
- **Производительность**: нет фрагментации, отличная локальность кэша
- **Простота**: вся память освобождается одной операцией
- **Выравнивание**: автоматическое выравнивание по границам для любых типов

Идеально подходит для:
- Временных вычислений с предсказуемым временем жизни
- Парсинга и обработки данных
- Фреймворков с четкими фазами выделения/освобождения памяти

#### API

```c
// Создает арену заданного размера
struct Arena * arena_init(size_t size);

// Выделяет память с указанным выравниванием
void * arena_alloc(struct Arena *a, size_t size, size_t align);

// Удобные макросы для типобезопасного выделения
#define arena_alloc_type(a, type)              // Выделить один объект
#define arena_alloc_array(a, type, count)       // Выделить массив

// Сбросить арену (offset = 0, память можно переиспользовать)
void arena_reset(struct Arena *a);

// Очистить арену (заполнить нулями + reset)
void arena_clear(struct Arena *a);

// Полностью освободить арену
void arena_destroy(struct Arena *a);
```

#### Базовый пример

```c
#include "allocators.h"
#include <stdio.h>

int main() {
    // Создать арену на 1KB
    struct Arena *arena = arena_init(1024);

    // Выделить массив целых чисел
    int *numbers = arena_alloc_array(arena, int, 10);
    for (int i = 0; i < 10; i++) {
        numbers[i] = i * i;
    }

    // Выделить строку
    char *str = arena_alloc_array(arena, char, 6);
    strcpy(str, "hello");

    // Выделить одно значение с автоматическим выравниванием
    double *value = arena_alloc_type(arena, double);
    *value = 3.14159;

    // Освободить всю память одной операцией
    arena_destroy(arena);
    return 0;
}
```

#### Переиспользование арены

```c
struct Arena *arena = arena_init(4096);

// Фаза 1: обработка первого запроса
int *data1 = arena_alloc_array(arena, int, 100);
// ... работа с данными ...

// Сбросить арену для следующего запроса
arena_reset(arena);

// Фаза 2: обработка второго запроса
float *data2 = arena_alloc_array(arena, float, 200);
// ... работа с данными ...

arena_destroy(arena);
```

## Использование в своем проекте

Рекомендуемый способ: *FetchContent*

```cmake
include(FetchContent)

FetchContent_Declare(
    myAllocators
    GIT_REPOSITORY https://github.com/Neburalis/c-allocators.git
    GIT_TAG main
)

FetchContent_MakeAvailable(myAllocators)

add_executable(my_app main.c)
target_link_libraries(my_app myAllocators)
```

Подробнее см. [USAGE.md](USAGE.md)

## Сборка

```bash
# Клонировать репозиторий
git clone https://github.com/Neburalis/c-allocators.git
cd c-allocators

# Собрать проект (Debug режим с санитайзерами)
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Или Release режим (оптимизированная версия)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Запустить тесты
./build/test_allocators
```

## Лицензия

[GNU General Public License v3.0](LICENSE) 
