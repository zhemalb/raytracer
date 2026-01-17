# Raytracer

Это проект на C++23 с трассировщиком лучей. Основной API это функция Render, она читает сцену из файла и возвращает изображение. В репозитории есть набор тестов на Catch2.

## Возможности

- Ввод: OBJ и MTL.
- Расширение формата сцены: строки S добавляют сферы, строки P добавляют источники света.
- Примитивы: треугольники и сферы.
- Режимы рендера: Full, Depth, Normal.
- Освещение: Phong, тени, отражения и преломления с ограничением глубины.
- Ускорение пересечений: BVH для треугольников.
- Вывод: PNG, tone mapping и gamma correction 2.2.

## Структура репозитория

- libs/utils/include. Утилиты и Image, чтение и запись PNG.
- libs/geom/include. Векторы, лучи, пересечения, треугольники, сферы.
- libs/reader/include. Загрузка сцен, OBJ и MTL, материалы и источники света.
- libs/core/include. Камера, BVH, трассировка, освещение, постобработка, функция Render.
- tests/geom. Юнит тесты геометрии.
- tests/reader. Юнит тесты загрузки сцены.
- tests/core. Тесты рендера в режиме Full.
- tests/debug. Тесты рендера в режимах Depth и Normal.
- tests/bvh. Тесты рендера на сценах с большим числом треугольников.
- tests/commons.h. Общие функции сравнения изображений.
- testdata. Входные файлы сцен и эталоны изображений для тестов.
- docs/images. Диаграммы и картинки проекта.
- cmake. CMake модули.

## Сборка

### Требования

- CMake 3.25 или новее.
- Компилятор с поддержкой C++23.
- libpng.
- Интернет нужен при первой конфигурации, чтобы CMake скачал Catch2.

### Шаги

1. cmake --preset debug
2. cmake --build --preset debug -j 8

Для release используй preset release. Для санитайзеров используй preset asan.

### Запуск тестов

- ctest --preset debug
- ./build/debug/test_raytracer_geom
- ./build/debug/test_raytracer_reader
- ./build/debug/test_raytracer_asan
- ./build/debug/test_raytracer_release
- ./build/debug/test_raytracer_debug_asan
- ./build/debug/test_raytracer_debug_release
- ./build/debug/test_raytracer_bvh_asan
- ./build/debug/test_raytracer_bvh_release
