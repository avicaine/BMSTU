//8 Вариант

#include <iostream>
#include <string> // По заданию, для работы со строками
#include <vector> // По заданию, для работы с динамическими массивами
#include <iomanip> // Для выставления количества знаков после запятой
#include <windows.h> // Для русских букв в консоли

using namespace std; // Не писать std::

struct Discipline {
    // Задаём структуру дисциплина по условию
    string name; // название предмета
    int grade; // оценка
};

struct Student {
    // Задаём структуру студент по условию
    string fullName; // ФИО
    vector<Discipline> subjects; // массив дисциплин студента (не меньше 4)
};

// Ввод целого числа с проверкой: если ввели не число (например, букву), просим ввести заново
int inputNumber() {
    int x;
    while (!(cin >> x)) {
        cout << "Это не число. Введите заново: ";
        cin.clear(); // сбрасываем ошибку ввода
        cin.ignore(1000, '\n'); // выбрасываем неверный ввод
    }
    cin.ignore(1000, '\n'); // убираем \n, чтобы следующий getline считался нормально
    return x;
}

// Ввод строки, которая не должна быть пустой
string inputString() {
    string s;
    getline(cin, s);
    while (s.empty()) {
        cout << "Строка не должна быть пустой. Повторите ввод: ";
        getline(cin, s);
    }
    return s;
}

// Проверка: есть ли среди первых count предметов студента предмет с названием name
bool hasSubject(const Student& s, int count, string name) {
    for (int i = 0; i < count; i++) {
        if (s.subjects[i].name == name) {
            return true; // нашли такой же предмет
        }
    }
    return false; // такого предмета еще нет
}

// Ввод данных одного студента (знак & нужен, чтобы функция меняла самого студента, а не его копию)
void inputStudent(Student& s) {
    cout << "Введите ФИО студента: ";
    s.fullName = inputString();

    cout << "Введите количество дисциплин (не меньше 4): ";
    int k = inputNumber(); // количество дисциплин
    while (k < 4) {
        cout << "Должно быть не меньше 4. Повторите ввод: ";
        k = inputNumber();
    }
    s.subjects.resize(k); // делаем размер вектора равным количеству дисциплин

    for (int i = 0; i < k; i++) {
        cout << "Предмет №" << i + 1 << endl;
        cout << "Введите название предмета: ";
        s.subjects[i].name = inputString();
        // один и тот же предмет у студента дважды быть не может, проверяем уже введенные (0...i-1)
        while (hasSubject(s, i, s.subjects[i].name)) {
            cout << "Такой предмет уже есть. Повторите ввод: ";
            s.subjects[i].name = inputString();
        }

        cout << "Введите оценку (от 2 до 5): ";
        s.subjects[i].grade = inputNumber();
        while (s.subjects[i].grade < 2 || s.subjects[i].grade > 5) {
            cout << "Оценка может быть только от 2 до 5. Введите заново: ";
            s.subjects[i].grade = inputNumber();
        }
    }
}

// Вывод данных одного студента (const - функция только выводит, ничего не меняет)
void printStudent(const Student& s) {
    cout << "Студент: " << s.fullName << endl;
    for (int i = 0; i < s.subjects.size(); i++) {
        cout << "  " << s.subjects[i].name << ": " << s.subjects[i].grade << endl;
    }
}

// Задание 8: определить, какой предмет сдан группой лучше всего
// (лучше всего = самый высокий средний балл по предмету)
void bestSubject(const vector<Student>& group) {
    vector<string> names; // названия всех предметов группы (каждое один раз)
    vector<int> sums; // сумма оценок по каждому предмету
    vector<int> counts; // сколько студентов сдавали каждый предмет

    // Проходим по всем оценкам всех студентов
    for (int i = 0; i < group.size(); i++) {
        for (int j = 0; j < group[i].subjects.size(); j++) {
            // Ищем этот предмет в списке names
            int k = -1; // номер предмета в списке (-1 значит "не нашли")
            for (int t = 0; t < names.size(); t++) {
                if (names[t] == group[i].subjects[j].name) {
                    k = t;
                }
            }
            // Если такого предмета еще нет, добавляем его в конец списков
            if (k == -1) {
                names.push_back(group[i].subjects[j].name);
                sums.push_back(0);
                counts.push_back(0);
                k = names.size() - 1;
            }
            sums[k] += group[i].subjects[j].grade; // прибавляем оценку к сумме
            counts[k] += 1; // еще один студент сдавал этот предмет
        }
    }

    // Считаем средний балл по каждому предмету и находим наибольший
    vector<double> avg(names.size()); // средний балл по каждому предмету
    double best = 0; // наибольший средний балл
    cout << "Средний балл группы по предметам:" << endl;
    for (int k = 0; k < names.size(); k++) {
        avg[k] = (double)sums[k] / counts[k]; // (double) нужен, чтобы деление было не целочисленным
        cout << names[k] << ": " << avg[k] << endl;
        if (avg[k] > best) {
            best = avg[k];
        }
    }

    // Выводим все предметы с наибольшим средним баллом (их может быть несколько)
    cout << "Лучше всего группа сдала: ";
    for (int k = 0; k < names.size(); k++) {
        if (avg[k] == best) {
            cout << names[k] << " ";
        }
    }
    cout << "(средний балл " << best << ")" << endl;
}

int main() {
    SetConsoleOutputCP(65001); // Устанавливаем UTF-8 для вывода русских букв в консоли
    SetConsoleCP(65001); // Устанавливаем UTF-8 для ввода
    cout << setprecision(2) << fixed; // дробные числа выводим с двумя знаками после запятой

    cout << "Введите количество студентов: ";
    int n = inputNumber(); // количество студентов в группе
    while (n <= 0) {
        cout << "Должно быть больше 0. Повторите ввод: ";
        n = inputNumber();
    }

    vector<Student> group(n); // группа студентов (вектор из n студентов)

    // Ввод данных
    for (int i = 0; i < n; i++) {
        cout << endl << "Студент №" << i + 1 << endl;
        inputStudent(group[i]);
    }

    // Вывод всех введенных данных для контроля ввода
    cout << endl << "Введенные данные:" << endl;
    for (int i = 0; i < n; i++) {
        printStudent(group[i]);
    }

    // Решение задания 8
    cout << endl;
    bestSubject(group);

    return 0;
}
