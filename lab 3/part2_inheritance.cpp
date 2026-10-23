// Лабораторная работа №3, часть 2, вариант 8.
// Базовый класс «вектор на плоскости», производный класс
// «вектор в трехмерном пространстве». Два варианта переопределения функций:
//  1) статический полиморфизм (vec2, vec3) - функции без virtual,
//     вызываемая функция выбирается по типу указателя;
//  2) динамический полиморфизм (vec2v, vec3v) - функция print с virtual
//     в базовом классе, вызываемая функция выбирается по типу объекта.
#include <iostream>
#include <vector>
using namespace std;

// ---------- 1. Статический полиморфизм ----------
class vec2 // Базовый класс - «вектор на плоскости»
{
protected:
    double x, y; // Координаты конца вектора (начало - точка (0, 0))
public:
    vec2(double x, double y) // Конструктор для инициализации полей
    {
        this->x = x;
        this->y = y;
    }
    void print() // Печать координат (без перевода строки)
    {
        cout << "x=" << x << " y=" << y;
    }
};

class vec3 : public vec2 // Производный класс - «вектор в трехмерном пространстве»
{
    double z; // Дополнительная координата
public:
    vec3(double x, double y, double z) : vec2(x, y) // Явный вызов конструктора базового класса
    {
        this->z = z;
    }
    void print() // Переопределенная функция печати
    {
        vec2::print();       // Сначала функция из базового класса
        cout << " z=" << z;  // Затем дополнительная координата
    }
};

// ---------- 2. Динамический полиморфизм ----------
class vec2v // Базовый класс - «вектор на плоскости»
{
protected:
    double x, y;
public:
    vec2v(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
    virtual ~vec2v() {} // Виртуальный деструктор (нужен при удалении через указатель на базовый класс)
    virtual void print() // Виртуальная функция печати
    {
        cout << "x=" << x << " y=" << y;
    }
};

class vec3v : public vec2v // Производный класс - «вектор в трехмерном пространстве»
{
    double z;
public:
    vec3v(double x, double y, double z) : vec2v(x, y)
    {
        this->z = z;
    }
    void print() // Переопределяет виртуальную функцию базового класса
    {
        vec2v::print();
        cout << " z=" << z;
    }
};

int main()
{
    cout << "1. Static polymorphism (no virtual)" << endl;
    vec2 a(3, 4);           // Объект базового класса
    vec3 b(1, 2, 3);        // Объект производного класса
    cout << "a.print():   "; a.print(); cout << endl; // Вызов через объект
    cout << "b.print():   "; b.print(); cout << endl; // Вызов через объект
    vec2 *p = &a;           // Указатель на базовый класс
    cout << "p->print():  "; p->print(); cout << endl; // Метод класса vec2
    p = &b;                 // Указатель на объект производного класса (преобразование допустимо)
    cout << "p->print():  "; p->print(); cout << "  <- z is not printed: function chosen by pointer type" << endl;
    cout << endl;

    cout << "2. Dynamic polymorphism (virtual)" << endl;
    vec2v av(3, 4);
    vec3v bv(1, 2, 3);
    vec2v *pv = &av;
    cout << "pv->print(): "; pv->print(); cout << endl; // Метод класса vec2v
    pv = &bv;
    cout << "pv->print(): "; pv->print(); cout << "  <- z is printed: function chosen by object type" << endl;
    cout << endl;

    cout << "3. Vector of pointers to objects of base and derived classes" << endl;
    vec2v  r1(3, 4), r2(-5, 12);        // Объекты базового класса
    vec3v  s1(1, 2, 3), s2(2, -1, 0.5); // Объекты производного класса
    vector<vec2v *> v;                  // Вектор указателей на объекты базового класса
    v.push_back(&r1);                   // В одном векторе указатели
    v.push_back(&s1);                   // на объекты базового
    v.push_back(&r2);                   // и производного
    v.push_back(&s2);                   // классов
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << i + 1 << ") ";
        v[i]->print(); // Динамический полиморфизм: вызывается функция того класса, к которому относится объект
        cout << endl;
    }
    return 0;
}
