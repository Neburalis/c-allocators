# Способы подключения

## Способ 1: FetchContent (рекомендуется)

FetchContent автоматически скачивает библиотеку из GitHub во время конфигурации CMake. Это самый простой способ.

**Требования**: CMake 3.14+

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProject C)

set(CMAKE_C_STANDARD 17)

include(FetchContent)

# Скачать библиотеку с GitHub
FetchContent_Declare(
    myAllocators
    GIT_REPOSITORY https://github.com/Neburalis/c-allocators.git
    GIT_TAG main  # или конкретный тег/коммит для стабильности
)

FetchContent_MakeAvailable(myAllocators)

# Создать исполняемый файл
add_executable(my_app main.c)

# Прилинковать библиотеку
target_link_libraries(my_app myAllocators)
```

**Преимущества**:
- Автоматическое скачивание при сборке
- Всегда актуальная версия (или фиксированная по тегу)
- Не требует ручного управления зависимостями

**Сборка**:

```bash
cmake -B build
cmake --build build
./build/my_app
```

## Способ 2: Git Submodule

Подходит для проектов, где нужна версионность зависимостей в самом репозитории.

**Шаг 1**: Добавить submodule

```bash
cd ваш_проект
git submodule add https://github.com/Neburalis/c-allocators.git libs/myAllocators
git submodule update --init --recursive
git commit -m "Add myAllocators submodule"
```

**Шаг 2**: Настроить CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject C)

set(CMAKE_C_STANDARD 17)

# Подключить библиотеку из submodule
add_subdirectory(libs/myAllocators)

add_executable(my_app main.c)
target_link_libraries(my_app myAllocators)
```

**Шаг 3**: Клонирование проекта другими разработчиками

```bash
# Вариант 1: Клонировать с submodules сразу
git clone --recursive https://github.com/ВАШ_USERNAME/ваш_проект.git

# Вариант 2: Клонировать, затем инициализировать submodules
git clone https://github.com/ВАШ_USERNAME/ваш_проект.git
cd ваш_проект
git submodule update --init --recursive
```

**Обновление submodule**:

```bash
cd libs/myAllocators
git pull origin main
cd ../..
git add libs/myAllocators
git commit -m "Update myAllocators to latest version"
```

**Преимущества**:
- Фиксированная версия библиотеки в репозитории
- Работает без интернета после первого клонирования
- Полный контроль над версией зависимости

**Структура проекта**:

```
ваш_проект/
├── .gitmodules
├── CMakeLists.txt
├── main.c
└── libs/
    └── myAllocators/
        ├── include/
        ├── src/
        └── CMakeLists.txt
```

## Способ 3: Локальная копия

Простое копирование библиотеки в проект.

```bash
cd ваш_проект
cp -r /path/to/myAllocators libs/myAllocators
```

**CMakeLists.txt**:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(libs/myAllocators)

add_executable(my_app main.c)
target_link_libraries(my_app myAllocators)
```

**Преимущества**:
- Полная независимость от внешних репозиториев
- Можно модифицировать библиотеку под свои нужды

**Недостатки**:
- Ручное обновление библиотеки
- Сложнее отслеживать изменения
