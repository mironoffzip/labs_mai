// Вариант 7 - целочисленный однонаправленный

#include <iostream> // Подключаем библиотеку стандартного ввода-вывода
#include <fstream>  // Подключаем библиотеку для работы с файлами

using namespace std;

const char FNAME[30] = "lists.txt"; // Имя файла для вывода списка

// Структура для узла списка
struct Node {
    int data;      // Данные узла
    Node* next;    // Указатель на следующий узел
};

// Объявления функций
void PrintList(Node* head, const char* filename);      // Печать списка (файл + консоль)
void DeleteList(Node** head);                           // Удаление списка
void InsertAtTail(Node** head, Node** tail, int data);  // Вставка в конец
bool Contains(Node* head, int data);                   // Проверка на наличие элемента
void DeleteAtPos(Node** head, Node** tail, int pos);    // Удаление элемента на позиции
void MaxElement(Node* head);                           // Поиск максимального элемента
void ShowMenu();                                        // Показ меню

int main() {
    srand(time(NULL)); // Инициализируем генератор случайных чисел текущим временем

    Node* head = nullptr; // Указатель на голову списка
    Node* tail = nullptr; // Указатель на хвост списка
    int option = -1;      // Переменная для выбора операции
    int data, pos;        // Переменные для данных и позиции

    // Генерация списка из 20 случайных элементов (как положительных, так и отрицательных)
    for (int i = 0; i < 20; i++) {
        int El = -1000 + rand() % (2000 + 1); // Генерация случайного числа в диапазоне [-1000, 1000]
        InsertAtTail(&head, &tail, El); // Вставка сгенерированного элемента в конец списка
    }

    // Основной цикл программы
    while (option != 0) {
        ShowMenu(); // Отображение меню
        cout << "Введите номер операции: ";
        cin >> option; // Ввод выбора операции

        switch (option) {
            case 1:
                cout << "Введите элемент для вставки в конец списка: ";
                cin >> data; // Ввод элемента для вставки
                InsertAtTail(&head, &tail, data); // Вставка элемента в конец списка
                break;
            case 2:
                cout << "Введите позицию удаляемого элемента в списке: ";
                cin >> pos; // Ввод позиции для удаления
                DeleteAtPos(&head, &tail, pos); // Удаление элемента на заданной позиции
                break;
            case 3:
                PrintList(head, FNAME); // Печать списка
                break;
            case 4: // Поиск максимального элемента
                MaxElement(head); // Поиск и вывод максимального элемента
                break;
            case 5: // Удаление списка
                DeleteList(&head); // Удаление всего списка
                tail = nullptr; // Обнуляем указатель на хвост списка
                cout << "Список удален." << endl;
                break;
            case 0: // Выход
                cout << "Выход из программы..." << endl;
                break;
            default:
                cout << "Неверный выбор. Пожалуйста, попробуйте снова." << endl; // Обработка неверного выбора
        }
    }

    return 0; // Завершение программы
}

// Функция для печати списка
void PrintList(Node* head, const char* filename) {
    ofstream file(filename, ofstream::out); // Открытие файла для записи

    if (head == nullptr) { // Проверка на пустой список
        cout << "Список пуст" << endl;
        file << "Список пуст" << endl;
        file.close(); // Закрытие файла
        return;
    }

    Node* current = head; // Указатель на текущий узел
    int index = 0; // Индекс текущего узла
    cout << "Формат вывода: (индекс, данные) -> (индекс, данные) -> ... -> nullptr" << endl;
    while ( current != nullptr) { // Проход по всем узлам списка
        cout << "(" << index << ", " << current->data << ")" << " -> "; // Вывод данных узла в консоль
        file << "(" << index << ", " << current->data << ")" << " -> "; // Запись данных узла в файл
        current = current->next; // Переход к следующему узлу
        index++; // Увеличение индекса
    }
    cout << "nullptr" << endl; // Завершение вывода списка
    file << "nullptr" << endl; // Запись завершения списка в файл
    file.close(); // Закрытие файла
}

// Функция для удаления списка
void DeleteList(Node** head) {
    Node* auxilaryNode; // Временный указатель для хранения следующего узла
    Node* iterator = *head; // Указатель для прохода по списку
    while (iterator) { // Проход по всем узлам списка
        auxilaryNode = iterator->next; // Сохранение следующего узла
        delete iterator; // Удаление текущего узла
        iterator = auxilaryNode; // Переход к следующему узлу
    }
    *head = nullptr; // Обнуление указателя на голову списка
}

// Функция для проверки наличия элемента в списке
bool Contains(Node* head, int data) {
    Node* current = head; // Указатель на текущий узел
    while (current != nullptr) { // Проход по всем узлам списка
        if (current->data == data) { // Проверка на совпадение данных
            return true; // Элемент найден
        }
        current = current->next; // Переход к следующему узлу
    }
    return false; // Элемент не найден
}

// Функция для вставки элемента в конец списка
void InsertAtTail(Node** head, Node** tail, int data) {
    if (Contains(*head, data)) { // Проверка на наличие дубликата
        cout << "Элемент " << data << " уже существует в списке. Пропускаем вставку." << endl;
        return; // Не добавляем дубликат
    }

    Node* newNode = new Node; // Создание нового узла
    newNode->data = data; // Заполнение данных узла
    newNode->next = nullptr; // Указатель на следующий узел равен nullptr

    if (*head == nullptr) { // Если список пуст
        *head = newNode; // Новый узел становится головой списка
        *tail = newNode; // Новый узел также становится хвостом списка
        return;
    }

    (*tail)->next = newNode; // Присоединение нового узла в конец списка
    *tail = newNode; // Обновление указателя на хвост списка
}

// Функция для удаления элемента на заданной позиции
void DeleteAtPos(Node** head, Node** tail, int pos) {
    int k = 0; // Счетчик для отслеживания позиции
    Node* p; // Указатель на текущий узел
    Node* q; // Указатель на предыдущий узел
    if (*head == nullptr) { // Проверка на пустой список
        cout << "Список пуст" << endl;
        return;
    }
    p = *head; // Начинаем с головы списка
    if (pos == 0) { // Если удаляем первый элемент
        *head = (*head)->next; // Обновляем голову списка
        delete p; // Удаляем первый узел
        if (*head == nullptr) { // Если список стал пустым
            *tail = nullptr; // Обнуляем указатель на хвост
        }
        return;
    } else {
        while ((p != nullptr) && (k < pos)) { // Поиск узла на заданной позиции
            k++;
            q = p; // Сохраняем предыдущий узел
            p = p->next; // Переход к следующему узлу
        }
    }

    if (p == nullptr) { // Если позиция не существует
        cout << "Позиция не существует" << endl;
    } else {
        q->next = p->next; // Пропускаем узел p
        delete p; // Удаляем узел p
        if (q->next == nullptr) { // Если удаляем последний элемент
            *tail = q; // Обновляем указатель на хвост
        }
    }
}

// Функция для поиска максимального элемента
void MaxElement(Node* head) {
    if (head == nullptr) { // Проверка на пустой список
        cout << "Список пуст" << endl;
        return;
    }

    int max = head->data; // Инициализация максимального элемента
    Node* current = head->next; // Начинаем с второго узла

    while (current != nullptr) { // Проход по всем узлам списка
        if (current->data > max) { // Если текущий элемент больше максимального
            max = current->data; // Обновляем максимальный элемент
        }
        current = current->next; // Переход к следующему узлу
    }

    cout << "Максимальный элемент: " << max << endl; // Вывод максимального элемента
}

// Функция для отображения меню
void ShowMenu() {
    cout << "Меню:" << endl;
    cout << "1. Вставить элемент в конец списка" << endl;
    cout << "2. Удалить элемент по позиции" << endl;
    cout << "3. Печать списка" << endl;
    cout << "4. Поиск максимального элемента" << endl;
    cout << "5. Удалить список" << endl;
    cout << "0. Выход" << endl;
}
