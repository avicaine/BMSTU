// main.cpp - демонстрация работы класса Vector.
// Лабораторная работа №3, часть 1, вариант 8 (операция: вектор * число).
// Все входные данные читаются из input.txt, результаты выводятся в output.txt.
// Сообщения конструкторов, операций и деструкторов выводятся на консоль.
#include <iostream>
#include <fstream>
#include <vector>
#include <utility> // std::move
#include "vector.h"

using namespace std;

int main()
{
    ifstream fin("input.txt");   // Входной файл
    ofstream fout("output.txt"); // Выходной файл
    if (!fin || !fout)
    {
        cerr << "Error: can't open input.txt or output.txt" << endl;
        return 1;
    }

    // ---------- Создание векторов ----------
    Vector V0;                    // Конструктор без параметров: пустой вектор
    fout << "V0 = " << V0 << endl;

    Vector V1;                    // Пустой вектор, в него читаем данные из файла
    fin >> V1;                    // Операция >>: число элементов, затем элементы
    fout << "V1 = " << V1 << endl;

    int k;                        // Число элементов обычного массива
    fin >> k;
    vector<double> m(k);          // Обычный массив из файла
    for (int i = 0; i < k; i++) fin >> m[i];
    double x;                     // Число, на которое умножаем
    fin >> x;
    if (!fin)
    {
        cerr << "Error: wrong data in input.txt" << endl;
        return 2;
    }
    Vector V2(m.data(), k);       // Конструктор на основе обычного массива
    fout << "V2 = " << V2 << endl;
    fout << "x  = " << x << endl;

    // ---------- Операция [] ----------
    double old = V1[0];           // Чтение элемента
    fout << "V1[0] = " << old << endl;
    V1[0] = 7;                    // Запись элемента (слева от =)
    fout << "V1 after V1[0]=7: " << V1 << endl;
    V1[0] = old;                  // Возвращаем прежнее значение

    // ---------- Индивидуальная операция: вектор * число ----------
    Vector V3 = V1 * x;           // Результат - новый вектор
    fout << "V3 = V1 * x: " << V3 << endl;
    fout << "V1 unchanged: " << V1 << endl;
    Vector V4 = V2 * x;
    fout << "V4 = V2 * x: " << V4 << endl;

    // ---------- Конструктор копирования и перемещения ----------
    Vector C1(V1);                // Копирование: V1 не меняется
    fout << "copy ctor: V1 = " << V1 << endl;
    fout << "           C1 = " << C1 << endl;
    Vector C2(move(V1));          // Перемещение: V1 становится пустым
    fout << "move ctor: V1 = " << V1 << endl;
    fout << "           C2 = " << C2 << endl;

    // ---------- Присваивание с копированием и перемещением ----------
    Vector A1;                    // Пустой вектор для присваивания с копированием
    A1 = V3;                      // V3 не меняется
    fout << "copy =:    V3 = " << V3 << endl;
    fout << "           A1 = " << A1 << endl;
    Vector A2;                    // Пустой вектор для присваивания с перемещением
    A2 = move(V3);                // V3 становится пустым
    fout << "move =:    V3 = " << V3 << endl;
    fout << "           A2 = " << A2 << endl;

    return 0;
}
