// vector.cpp - реализация класса Vector.
// Лабораторная работа №3, часть 1, вариант 8.
#include "vector.h"
#include <utility> // std::swap

using namespace std;

// Закрытый конструктор: память выделяется, элементы не инициализируются
Vector::Vector(int n) : n(n)
{
    p = new double[n];
    cout << "Vector(int n)" << endl;
}

// Конструктор без параметров: поля уже равны nullptr и 0 (см. vector.h)
Vector::Vector()
{
    cout << "Vector()" << endl;
}

// Конструктор на основе обычного массива: копируем элементы в свою память
Vector::Vector(const double *arr, int size) : n(size)
{
    p = new double[n];
    for (int i = 0; i < n; i++) p[i] = arr[i];
    cout << "Vector(const double *arr, int size)" << endl;
}

// Конструктор копирования: создаем свой массив и копируем в него элементы,
// исходный вектор не меняется
Vector::Vector(const Vector &V) : n(V.n)
{
    p = new double[n];
    for (int i = 0; i < n; i++) p[i] = V.p[i];
    cout << "Vector(const Vector & V) n=" << n << endl;
}

// Конструктор перемещения: поля нового объекта пустые (nullptr, 0),
// после обмена с V новый объект владеет массивом, а V становится пустым
Vector::Vector(Vector &&V) noexcept
{
    swap(p, V.p);
    swap(n, V.n);
    cout << "Vector(Vector&& V)" << endl;
}

// Деструктор: освобождаем память
Vector::~Vector()
{
    cout << "~Vector() n=" << n << endl;
    if (p != nullptr) delete[] p;
}

// Операция []: возвращаем ссылку, чтобы элемент можно было и читать, и менять
double &Vector::operator[](int index)
{
    return p[index];
}

// Присваивание с копированием
Vector &Vector::operator=(const Vector &v2)
{
    if (this != &v2) // защита от присваивания вектора самому себе
    {
        if (p != nullptr) delete[] p; // освобождаем старую память
        n = v2.n;
        p = new double[n];
        for (int i = 0; i < n; i++) p[i] = v2.p[i];
    }
    cout << "Vector & operator =(const Vector& v2)" << endl;
    return *this;
}

// Присваивание с перемещением: забираем массив у v2, v2 становится пустым
// (подробная форма: после присваивания v2 пуст в любом случае)
Vector &Vector::operator=(Vector &&v2) noexcept
{
    if (this != &v2)
    {
        if (p != nullptr) delete[] p; // освобождаем старую память
        p = v2.p;                     // забираем массив
        n = v2.n;
        v2.p = nullptr;               // v2 больше не владеет массивом
        v2.n = 0;
    }
    cout << "Vector & operator =(Vector&& v2)" << endl;
    return *this;
}

// Индивидуальная операция: вектор * число.
// Результат - новый вектор той же размерности, исходный не меняется.
Vector Vector::operator*(double x) const
{
    Vector R(n); // вектор-результат, память выделена
    for (int i = 0; i < n; i++) R.p[i] = p[i] * x;
    return R;
}

// Вставка вектора в поток (cout или файл): "n=3: 1 2 3"
ostream &operator<<(ostream &os, const Vector &V)
{
    os << "n=" << V.n << ":";
    for (int i = 0; i < V.n; i++) os << " " << V.p[i];
    return os;
}

// Извлечение вектора из потока: сначала число элементов, затем сами элементы
istream &operator>>(istream &is, Vector &V)
{
    int k;
    is >> k;
    if (!is || k < 0) // неверные данные: помечаем поток как неисправный
    {
        is.setstate(ios::failbit);
        return is;
    }
    if (V.p != nullptr) delete[] V.p; // освобождаем память старого вектора
    V.n = k;
    V.p = new double[k];
    for (int i = 0; i < k; i++) is >> V.p[i];
    return is;
}
