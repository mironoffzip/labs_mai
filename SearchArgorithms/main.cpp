 #include <iostream> // Подключаем библиотеку стандартного ввода-вывода
#include <chrono>   // Подключаем библиотеку для измерения времени
#include <fstream>  // Подключаем библиотеку для работы с файлами

using namespace std;

// Объявления функций
void increasing(int* arr, int size, int min, int max); // Генерирует возрастающий массив
void random(int* arr, int size, int min, int max);     // Генерирует случайный массив

// Объявления функций для алгоритмов поиска
int BLS(int* arr, int n, int x, int* count);      // Улучшенный линейный поиск
int SLS(int* arr, int n, int x, int* count);      // Поиск с использованием стража
int OAS(int* arr, int n, int x, int* count);      // Поиск в отсортированном массиве
int BS(int* arr, int n, int x, int* count);       // Бинарный поиск

// Функция для вывода массива в файл
void file_output(const char* filename, int* arr, int size);

// Функция для измерения времени, затраченного на генерацию массива
time_t time_count(void (*func)(int*, int, int, int), int* arr, int size, int min, int max, const char* filename);

// Функция для измерения времени, затраченного на выполнение алгоритма поиска
time_t algo_time(int (*func)(int*, int, int, int*), int* arr, int size, int key, int* count);

// Массив указателей на функции для генерации массивов
void (*functions[])(int*, int, int, int) = {
        increasing, // Указатель на функцию increasing
        random      // Указатель на функцию random
};

int main() {
    srand(time(NULL)); // Инициализируем генератор случайных чисел текущим временем

    // Массив имен файлов для вывода
    const char fnames[2][30]{
            "increasing.txt", // Имя файла для возрастающего массива
            "random.txt"      // Имя файла для случайного массива
    };

    int count = 0; // Переменная для отслеживания количества сравнений, сделанных во время поиска

    int size = 10000; // Размер массива
    int min = 0;      // Минимальное значение для генерации случайных чисел
    int max = 100000; // Максимальное значение для генерации случайных чисел

    // Динамические массивы для случайных и упорядоченных данных
    int* Randoms = new int[size];
    int* Ordered = new int[size];

    // Генерация массивов и измерение времени, затраченного на их создание
    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            // Генерация возрастающего массива и измерение времени
            cout << fnames[i] << " время генерации: "
                 << time_count(functions[i], Ordered, size, min, max, fnames[i])
                 << "мкс" << endl;
        } else {
            // Генерация случайного массива и измерение времени
            cout << fnames[i] << " время генерации: "
                 << time_count(functions[i], Randoms, size, min, max, fnames[i])
                 << "мкс" << endl;
        }
    }

    cout << endl; // Печатаем новую строку для форматирования

    // Определяем ключи для поиска
    int wantedInRand = Randoms[9008]; // Случайное значение из случайного массива
    int wantedInOrdered = 250;         // Фиксированное значение для поиска в упорядоченном массиве

    // Поиск в упорядоченном массиве
    cout << "\t\tОтсортированная последовательность" << endl; // Заголовок для отсортированной последовательности
    cout << "Binary_Search\t ключ: " << wantedInOrdered
         << "\t индекс: " << BS(Ordered, size, wantedInOrdered, &count)
         << "\t время поиска: " << algo_time(BS, Ordered, size, wantedInOrdered, &count)
         << " нс" << endl;
    cout << "Ordered_Array_Search\t ключ: " << wantedInOrdered
         << "\t индекс: " << OAS(Ordered, size, wantedInOrdered, &count)
         << "\t время поиска: " << algo_time(OAS, Ordered, size, wantedInOrdered, &count)
         << " нс" << endl;

    cout << endl; // Печатаем новую строку для форматирования

    // Поиск в случайном массиве
    cout << "\t\tСлучайная последовательность" << endl; // Заголовок для случайной последовательности
    cout << "Better_Linear_Search\t ключ: " << wantedInRand
         << "\t индекс: " << BLS(Randoms, size, wantedInRand, &count)
         << "\t время поиска: " << algo_time(BLS, Randoms, size, wantedInRand, &count)
         << " нс" << "\tКоличество сравнений " << count << endl;

    count = 0;

    cout << "Sentinel_Array_Search\t ключ: " << wantedInRand
         << "\t индекс: " << SLS(Randoms, size, wantedInRand, &count)
         << "\t время поиска: " << algo_time(SLS, Randoms, size, wantedInRand, &count)
         << " нс" << "\tКоличество сравнений " << count << endl;
    delete[] Randoms;
    delete[] Ordered;

    return 0;
}

// Функция для генерации возрастающего массива
void increasing(int* arr, int size, int min, int max) {
    for (int i = 0; i < size; i++) {
        arr[i] = min + i * (max - min) / size;
    }
}

// Функция для генерации случайного массива
void random(int* arr, int size, int min, int max) {
    for (int i = 0; i < size; i++) {
        arr[i] = min + rand() % (max - min + 1); // Генерация случайного числа в диапазоне [min, max]
        if (i == 9008) {
            arr[i] = 6445;
        }
    }
}

// Функция для улучшенного линейного поиска
int BLS(int* arr, int n, int x, int* count) {
    int c = 0; // Счетчик сравнений
    for (int i = 0; i < n; i++) {
        c++; // Увеличиваем счетчик при каждом сравнении
        if (arr[i] == x) {
            *count = c; // Сохраняем количество сравнений
            return i; // Возвращаем индекс найденного элемента
        }
    }
    *count = c; // Если элемент не найден, сохраняем количество сравнений
    return -1; // Возвращаем -1, если элемент не найден
}

// Функция для поиска с использованием стража
int SLS(int* arr, int n, int x, int* count) {
    int c = 0; // Счетчик сравнений
    int last = arr[n - 1];
    arr[n - 1] = x; // Устанавливаем последний элемент в искомое значение
    int i = 0;

    while (arr[i] != x) {
        c++; // Увеличиваем счетчик при каждом сравнении
        i++;
    }

    arr[n - 1] = last; // Восстанавливаем последний элемент
    c++; // Увеличиваем счетчик для последнего сравнения

    // Сохраняем количество сравнений
    *count = c;

    if (i < n - 1 || arr[n - 1] == x) {
        return i; // Возвращаем индекс найденного элемента
    }
    return -1; // Возвращаем -1, если элемент не найден
}

// Функция для поиска в отсортированном массиве
int OAS(int* arr, int n, int x, int* count) {
    int last = arr[n - 1];
    int i = 0;
    while (x > arr[i]) {
        i++;
    }
    arr[n - 1] = last;
    *count = i;
    if (i < n - 1 || arr[n - 1] == x) {
        return i;
    }
    return -1;
}

// Функция для бинарного поиска
int BS(int* arr, int n, int x, int* count) {
    int p = 0, r = n;
    int q;
    while (p <= r) {
        q = (p + r) / 2;
        if (arr[q] == x) {
            return q;
        } else if (arr[q] > x) {
            r = q - 1;
        } else {
            p = q + 1;
        }
    }
    return -1;
}

// Функция для вывода массива в файл
void file_output(const char* filename, int* arr, int size) {
    ofstream file(filename);
    for (int i = 0; i < size; i++) {
        file << arr[i] << endl;
    }
    file.close();
}

// Функция для измерения времени, затраченного на генерацию массива
time_t time_count(void (*func)(int*, int, int, int), int* arr, int size, int min, int max, const char* filename) {
    auto start = chrono::steady_clock::now();
    func(arr, size, min, max);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file_output(filename, arr, size);
    return duration.count();
}

// Функция для измерения времени, затраченного на выполнение алгоритма поиска
time_t algo_time(int (*func)(int*, int, int, int*), int* arr, int size, int key, int* count) {
    auto start = chrono::steady_clock::now();
    func(arr, size, key, count);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    return duration.count();
}
