// ============================================================================
// Лабораторная работа № 2. Ввод-вывод в файлы (Си++), вариант 8.
//
// Структура «товар в магазине (на складе)»:
//     название, число единиц в наличии, стоимость одной единицы.
//
// Программа работает в двух режимах (запись / чтение) и с файлом в двух видах
// (текстовый / двоичный). Режимы задаются параметрами командной строки:
//
//     lab2 <write|read> <text|binary> <имя_файла>
//
//     lab2 write text   goods.txt   запись:  ввод с клавиатуры -> текстовый файл
//     lab2 write binary goods.dat   запись:  ввод с клавиатуры -> двоичный файл
//     lab2 read  text   goods.txt   чтение:  текстовый файл -> консоль
//     lab2 read  binary goods.dat   чтение:  двоичный файл  -> консоль
// ============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>     // numeric_limits — для cin.ignore(...)
#include <iomanip>    // setprecision
#include <locale.h>   // setlocale
using namespace std;

// ---------------------------------------------------------------------------
// Структура — товар
// ---------------------------------------------------------------------------
struct PRODUCT
{
	string Name;   // название
	int    Count;  // число единиц в наличии
	double Price;  // стоимость одной единицы
};

// ---------------------------------------------------------------------------
// Запись ОДНОГО товара в поток. Режим выбирается параметром binary.
//
// Текстовый формат (читаемый человеком), на каждый товар две строки:
//     Название
//     Число Цена
//
// Двоичный формат (в «сыром» виде, как данные лежат в памяти):
//     Название + нулевой байт '\0' | int (4 байта) | double (8 байт)
// Нулевой байт после строки нужен как признак конца: длина названия
// заранее не известна, а в двоичном файле нет символов перевода строки.
// ---------------------------------------------------------------------------
void write_product(ostream & out, const PRODUCT & p, bool binary)
{
	if (binary)
	{
		out.write(p.Name.c_str(), p.Name.size() + 1);        // +1 — захватываем и '\0'
		out.write((const char *)&p.Count, sizeof(p.Count));  // 4 байта
		out.write((const char *)&p.Price, sizeof(p.Price));  // 8 байт
	}
	else
	{
		out << p.Name << endl;                               // название — отдельной строкой
		out << p.Count << ' '                                // числа — в одну строку через пробел
		    << fixed << setprecision(2) << p.Price << endl;  // цена с двумя знаками после точки
	}
}

// ---------------------------------------------------------------------------
// Чтение ОДНОГО товара из потока (зеркально write_product).
// Возвращает true, если чтение прошло без ошибок.
// ---------------------------------------------------------------------------
bool read_product(istream & in, PRODUCT & p, bool binary)
{
	if (binary)
	{
		getline(in, p.Name, '\0');                           // читаем до '\0' (он извлекается)
		in.read((char *)&p.Count, sizeof(p.Count));
		in.read((char *)&p.Price, sizeof(p.Price));
	}
	else
	{
		getline(in, p.Name);                                 // название — вся строка (с пробелами)
		if (!p.Name.empty() && p.Name.back() == '\r')        // если файл создан в Windows, а читаем
			p.Name.pop_back();                               // в Linux — убираем лишний '\r'
		in >> p.Count >> p.Price;                            // два числа через пробел
		in.ignore(numeric_limits<streamsize>::max(), '\n');  // пропускаем остаток строки
	}
	return static_cast<bool>(in);                            // false, если поток в состоянии ошибки
}

// ---------------------------------------------------------------------------
// Печать массива товаров в произвольный выходной поток (консоль или файл)
// ---------------------------------------------------------------------------
void print_products(ostream & out, const vector<PRODUCT> & Goods)
{
	out << fixed << setprecision(2);                         // цена всегда с двумя знаками
	int i = 1;
	for (const auto & pos : Goods)
		out << i++ << ". " << pos.Name
		    << ", in stock: " << pos.Count << " pcs"
		    << ", price: " << pos.Price << endl;
}

int main(int argc, const char * argv[])
{
	setlocale(LC_ALL, "rus");   // как в учебном примере: русские символы на консоли Windows

	// --- 1. Проверка параметров командной строки ---------------------------
	// argv[0] — имя программы, поэтому всего должно быть 4 элемента
	if (argc != 4)
	{
		cerr << "Error: use three parameters: <write|read> <text|binary> <file name>\n";
		return 5;
	}
	const string mode(argv[1]);       // режим работы: write или read
	const string open_mode(argv[2]);  // вид файла: text или binary
	const string file_name(argv[3]);  // имя файла

	if ((mode != "write" && mode != "read") ||
	    (open_mode != "text" && open_mode != "binary"))
	{
		cerr << "Error: unknown mode. Use write|read and text|binary\n";
		return 5;
	}

	const bool binary = (open_mode == "binary");
	// Для двоичного режима добавляется флаг ios::binary (иначе в Windows
	// '\n' при записи превратился бы в пару '\r' '\n')
	const ios::openmode file_mode = binary ? ios::binary : ios::openmode(0);

	int n;                 // число товаров
	vector<PRODUCT> Goods; // массив (вектор) товаров

	if (mode == "write")
	{
		// --- 2. Режим записи: ввод с клавиатуры -----------------------------
		cout << "n="; cin >> n;                       // число товаров
		if (!cin || n <= 0)
		{
			cerr << "Error: n must be a positive integer\n";
			return 2;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');  // убираем '\n' после n, иначе
		                                                      // getline прочтёт пустую строку
		Goods.resize(n);                             // задаём размер вектора
		for (int i = 0; i < n; i++)
		{
			cout << "Product N=" << (i + 1) << endl;
			cout << "Name: ";  getline(cin, Goods[i].Name);   // название может содержать пробелы
			cout << "Count: "; cin >> Goods[i].Count;
			cout << "Price: "; cin >> Goods[i].Price;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (!cin)
			{
				cerr << "Error: wrong input data\n";
				return 2;
			}
		}

		// --- 3. Запись в файл -----------------------------------------------
		ofstream fout(file_name, file_mode);         // открываем файл для записи
		if (!fout)
		{
			cerr << "Error: can't open file " << file_name << endl;
			return 1;
		}
		if (binary) fout.write((const char *)&n, sizeof(n));  // n — 4 байта
		else        fout << n << endl;                        // n — строка текста
		for (const auto & pos : Goods)
			write_product(fout, pos, binary);        // по одному товару
		fout.close();                                // закрываем файл
	}
	else
	{
		// --- 4. Режим чтения: читаем файл, печатаем на консоль --------------
		ifstream fin(file_name, file_mode);          // открываем файл для чтения
		if (!fin)                                    // файл не найден / нет доступа
		{
			cerr << "Error, file not found" << endl;
			return 1;
		}
		if (binary) fin.read((char *)&n, sizeof(n));          // n — 4 байта
		else
		{
			fin >> n;                                         // n — число в тексте
			fin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if (!fin || n < 0 || n > 1000000)            // защита от «чужого» файла
		{
			cerr << "Error: wrong file format" << endl;
			return 3;
		}
		Goods.resize(n);
		for (auto & pos : Goods)
			if (!read_product(fin, pos, binary))
			{
				cerr << "Error: unexpected end of file" << endl;
				return 3;
			}
		fin.close();

		print_products(cout, Goods);                 // печать прочитанных товаров
	}
	return 0;
}
