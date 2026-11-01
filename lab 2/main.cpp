//8 Вариант
// Лабораторная работа №2. Структура «товар в магазине (на складе)»:
// название, число единиц в наличии, стоимость одной единицы.
// Параметры командной строки: write|read text|binary имя_файла
// Например: lab2.exe write text goods.txt   или   lab2.exe read binary goods.dat

#include <iostream>
#include <fstream> // Для работы с файлами (ofstream - запись, ifstream - чтение)
#include <string> // По заданию, для работы со строками
#include <vector> // По заданию, для работы с динамическими массивами
#include <iomanip> // Для вывода цены с двумя знаками после запятой
#include <windows.h> // Для русских букв в консоли

using namespace std; // Чтобы не писать std::

struct Product {
    // Задаём структуру товар по условию
    string name; // название
    int count; // число единиц в наличии
    double price; // стоимость одной единицы (руб.)
};

// Ввод целого числа с проверкой: если ввели не число (например, букву или 12.5), просим ввести заново
int inputNumber() {
    int x;
    while (!(cin >> x) || cin.peek() != '\n') { // после числа сразу должен быть конец строки
        cout << "Это не целое число. Введите заново: ";
        cin.clear(); // сбрасываем ошибку ввода
        cin.ignore(1000, '\n'); // выбрасываем неверный ввод
    }
    cin.ignore(1000, '\n'); // убираем \n, чтобы следующий getline считался нормально
    return x;
}

// Ввод дробного числа с проверкой (дробная часть пишется через точку: 45.90)
// Если ввести 45,90, то прочитается 45, а ",90" останется в потоке - это считаем ошибкой
double inputDouble() {
    double x;
    while (!(cin >> x) || cin.peek() != '\n') {
        cout << "Это не число (дробную часть пишите через точку). Введите заново: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
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

// Ввод данных одного товара с клавиатуры (знак & нужен, чтобы функция меняла сам товар, а не его копию)
void inputProduct(Product& p) {
    cout << "Введите название товара: ";
    p.name = inputString();

    cout << "Введите число единиц в наличии: ";
    p.count = inputNumber();
    while (p.count < 0) {
        cout << "Число единиц не может быть отрицательным. Введите заново: ";
        p.count = inputNumber();
    }

    cout << "Введите стоимость одной единицы (руб.): ";
    p.price = inputDouble();
    while (p.price <= 0) {
        cout << "Стоимость должна быть больше 0. Введите заново: ";
        p.price = inputDouble();
    }
}

// Вывод всех товаров на консоль (const - функция только выводит, ничего не меняет)
void printGoods(const vector<Product>& goods) {
    for (size_t i = 0; i < goods.size(); i++) {
        cout << "Товар №" << i + 1 << ": " << goods[i].name
             << ", в наличии: " << goods[i].count << " шт."
             << ", цена: " << goods[i].price << " руб." << endl;
    }
}

// Запись одного товара в файл
// binary = false - текстовый режим, binary = true - двоичный режим
void writeProduct(ofstream& fout, const Product& p, bool binary) {
    if (binary) {
        fout << p.name << ends; // строка, после нее символ '\0' - признак конца строки
        fout.write((char*)&p.count, sizeof(p.count)); // int записываем так, как он лежит в памяти, - 4 байта
        fout.write((char*)&p.price, sizeof(p.price)); // double - 8 байт
    }
    else {
        fout << p.name << endl; // название отдельной строкой (в нем могут быть пробелы)
        fout << p.count << ' ' << fixed << setprecision(2) << p.price << endl; // числа в одну строку через пробел
    }
}

// Чтение одного товара из файла (в том же порядке, в котором товар записывался)
void readProduct(ifstream& fin, Product& p, bool binary) {
    if (binary) {
        getline(fin, p.name, '\0'); // читаем строку до символа '\0' (сам '\0' тоже извлекается)
        fin.read((char*)&p.count, sizeof(p.count));
        fin.read((char*)&p.price, sizeof(p.price));
    }
    else {
        getline(fin, p.name); // читаем всю строку с названием
        fin >> p.count >> p.price;
        fin.ignore(1000, '\n'); // пропускаем конец строки после цены
    }
}

// Размер файла в байтах: открываем файл в двоичном режиме и переходим в его конец
long long fileSize(const string& fileName) {
    ifstream f(fileName, ios::binary);
    f.seekg(0, ios::end); // переходим в конец файла
    return f.tellg(); // позиция конца файла = размер файла
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001); // Устанавливаем UTF-8 для вывода русских букв в консоли
    SetConsoleCP(65001); // Устанавливаем UTF-8 для ввода
    cout << setprecision(2) << fixed; // цены выводим с двумя знаками после запятой

    // argv[0] - имя программы, после него должно быть еще 3 параметра
    if (argc != 4) {
        cerr << "Ошибка: нужно 3 параметра: write|read text|binary имя_файла" << endl;
        return 5;
    }
    string mode = argv[1]; // режим работы: write - запись, read - чтение
    string openMode = argv[2]; // режим открытия файла: text - текстовый, binary - двоичный
    string fileName = argv[3]; // имя файла

    if ((mode != "write" && mode != "read") || (openMode != "text" && openMode != "binary")) {
        cerr << "Ошибка: первый параметр write или read, второй text или binary" << endl;
        return 5;
    }
    bool binary = (openMode == "binary"); // true - двоичный режим, false - текстовый

    int n; // количество товаров
    vector<Product> goods; // массив (вектор) товаров

    if (mode == "write") {
        // ---------- Режим записи ----------
        cout << "Введите количество товаров: ";
        n = inputNumber();
        while (n <= 0) {
            cout << "Должно быть больше 0. Повторите ввод: ";
            n = inputNumber();
        }
        goods.resize(n); // делаем размер вектора равным количеству товаров

        for (int i = 0; i < n; i++) {
            cout << endl << "Товар №" << i + 1 << endl;
            inputProduct(goods[i]);
        }

        // Вывод всех введенных данных для контроля ввода
        cout << endl << "Введенные данные:" << endl;
        printGoods(goods);

        // Открываем файл для записи (если такой файл уже есть, он очищается)
        ofstream fout;
        if (binary) fout.open(fileName, ios::binary);
        else fout.open(fileName);
        if (!fout) {
            cerr << "Ошибка: не удалось открыть файл " << fileName << endl;
            return 1;
        }

        // Сначала записываем n, чтобы при чтении знать, сколько товаров читать
        if (binary) fout.write((char*)&n, sizeof(n));
        else fout << n << endl;
        for (int i = 0; i < n; i++) {
            writeProduct(fout, goods[i], binary);
        }
        fout.close(); // закрываем файл

        cout << endl << "Данные записаны в файл " << fileName;
        cout << (binary ? " (двоичный режим)" : " (текстовый режим)") << endl;
        cout << "Размер файла в байтах: " << fileSize(fileName) << endl;
    }
    else {
        // ---------- Режим чтения ----------
        ifstream fin;
        if (binary) fin.open(fileName, ios::binary);
        else fin.open(fileName);
        if (!fin) { // файл не открылся
            cerr << "Ошибка: файл " << fileName << " не найден" << endl;
            return 1;
        }

        if (binary) fin.read((char*)&n, sizeof(n));
        else {
            fin >> n;
            fin.ignore(1000, '\n'); // пропускаем конец строки после n
        }
        // Если n не прочиталось или получилось слишком большим, то файл другого формата
        // (например, текстовый файл открыли в двоичном режиме)
        if (!fin || n <= 0 || n > 100000) {
            cerr << "Ошибка: неверный формат файла" << endl;
            return 3;
        }

        goods.resize(n);
        for (int i = 0; i < n; i++) {
            readProduct(fin, goods[i], binary);
        }
        if (!fin) { // файл закончился раньше, чем прочитали n товаров
            cerr << "Ошибка: файл поврежден" << endl;
            return 3;
        }
        fin.close(); // закрываем файл

        cout << "Данные из файла " << fileName;
        cout << (binary ? " (двоичный режим):" : " (текстовый режим):") << endl;
        printGoods(goods);
        cout << "Размер файла в байтах: " << fileSize(fileName) << endl;
    }

    return 0;
}
