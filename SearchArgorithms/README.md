### Лабораторная работа №2 (Программирование) "Алгоритмы поиска"
## Задание

Цель работы: изучить основные принципы работы алгоритмов поиска, исследовать
их свойства:
● алгоритм BLS - последовательный поиск (Better_Linear_Search);
● алгоритм SLS - быстрый последовательный поиск (Sentinel_ Linear_Search);
● алгоритм OAS - последовательный поиск в упорядоченном массиве
(Ordered_Array_Search);
● алгоритм ВS - бинарный поиск ( Binary Search).

Для алгоритмов BLS и SLS в качестве входного массива использовать одну и ту же
последовательность значений (функция rand( ), можно использовать
соответствующую функцию из первой лабораторной работы).
Для алгоритмов OAS и ВS – значения массива должны быть отсортированы по
неубыванию, одна и та же последовательность чисел (можно использовать
соответствующую функцию из первой лабораторной работы).
Оценить длительность поиска для различных значений размеров
последовательностей (начиная с 10000 до 200000 элементов массива, провести
измерения не менее, чем для 10 разных размерностей).
Для каждой размерности рассматриваются случаи нахождения ключа поиска в
начале, в середине и в конце массива, случай отсутствия ключа в массиве.
Для алгоритмов BLS и SLS кроме подсчета времени, необходимого для поиска,
требуется определить сколько раз выполняются операции сравнения (сравнение
ключа с элементом массива, а также в BLS добавляется подсчет сравнений при
анализе индекса элемента массива в цикле… ).
Все результаты оформить в виде таблиц и графиков. На графиках - только
временные характеристики поиска.
По результатам сделать выводы об эффективности того или иного алгоритма
поиска.

Отчет по лабораторной работе должен содержать:
● задание;
● структурные схемы главной функции, всех алгоритмов поиска;
● тексты программ;
● результаты работы программ и результаты сопоставительного анализа – в виде
таблиц, графиков;
● выводы по работе.