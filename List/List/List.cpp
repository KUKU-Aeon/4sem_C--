/*Написать программу, содержащую следующие функции:
1. Функцию заполнения текстового файла N целыми случайными числами в
диапазоне от - M до M. Имя файла, N и M поступают на вход функции, файл
возвращается функцией как результат (использовать потоки ввода - вывода).
Реализовать два варианта генерации чисел — в цикле и с использованием
алгоритма generate.
2. Функцию, получающую файл на вход, и возвращающую последовательный
контейнер, заполненный числами из файла.
3. Функцию modify, выполняющую преобразование полученного контейнера. На вход
функции должен поступать контейнер, в качестве результата функция должна
возвращать модифицированный контейнер.
4. Перегруженную функцию modify, принимающую на вход итераторы начала и конца
обрабатываемой части контейнера.
5. Функцию, реализующую требуемое преобразование с использованием алгоритма
transform вместо функции modify.
6. Функцию, реализующую требуемое преобразование с использованием алгоритма
for_each вместо функции modify.
7. Функции, вычисляющие сумму и среднее арифметическое чисел, содержащихся в
контейнере.
8. Функцию вывода результата в файл и на экран(опционально).
9. Вычесть из каждого числа сумму всех чисел.
*/

#include <iostream>
#include <list>
#include <iterator> 
#include <algorithm>
#include <fstream>
#include <ctime> 

using namespace std;

ostream& operator<<(ostream& out, list<int> lst)
{
	for (int n : lst)
		out << n << " ";
	out << endl;
	return out;
}

ifstream create_file(string f, int n, int m) //заполнение текстового файла(в цикле)
{
	ofstream file(f);
	srand(time(NULL)); //начало генерации псевдослучайных чисел
	for (int i = 1; i < n; i++)
	{
		file << rand() % (2 * m + 1) - m << " ";
	}
	file.close();
	ifstream F(f);
	return F;
}

ifstream generate_file(string f, int n, int m) //заполнение текстового файла (generate)
{
	ofstream file(f);
	srand(time(NULL));
	list<int> lst(n);
	generate(lst.begin(), lst.end(), [&]() {return rand() % (2 * m + 1) - m; });
	file << lst;
	file.close();
	return ifstream(f);
}

list<int> read_file(istream& in) //вывод списка
{
	list<int> lst;
	int i;
	while (!in.eof())
	{
		in >> i;
		lst.push_back(i);
	}
	return lst;
}

int sum(list<int> lst) // сумма всех чисел
{
	int sum = 0;
	for (auto a : lst)
	{
		sum += a;
	}
	return sum;
}

double average(list<int> lst) // среднее всех чисел
{
	return ((double)sum(lst)) / lst.size();
}


list<int> modify(list<int> lst) // преобразование списка
{
	int avg = average(lst);
	list<int>::iterator i;
	for (i = lst.begin(); i != lst.end(); i++)
	{
		if (abs(*i % 2) != 0)
		{
			*i /= avg;
		}
	}
	return lst;
};

list<int> modify(list<int>& lst, list<int>::iterator start, list<int>::iterator end) // modify с итераторами начала и конца
{
	int avg = average(lst);
	for (list<int>::iterator i = start; i != end; ++i)
	{
		if (abs(*i % 2) != 0)
		{
			*i /= avg;
		}
	}
	return lst;
};

list<int> modify_transform(list<int> lst)// с помощью transform
{
	int avg = average(lst);
	transform(lst.begin(), lst.end(), lst.begin(), [&](int x) {if ((x % 2) != 0) return x / avg; });
	return lst;
}

list<int> modify_for_each(list<int> lst) // с помощью for each
{
	int avg = average(lst);
	for_each(lst.begin(), lst.end(), [&](int& x) {if ((x % 2) != 0) x /= avg; });
	return lst;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	string str;
	ifstream F;
	ofstream file;
	bool full = false;
	list<int> lst;
	list<int>::iterator start, end;
	int st, en;
	int n, m, index = -1;
	while (index != 0)
	{
		cout << "Выберите действие: " << endl;
		cout << "1. Создание файла с псевдорандомными числами" << endl;
		cout << "2. Запись данных из файла в список" << endl;
		if (full)
		{
			cout << "3. modify" << endl;
			cout << "4. перегруженный modify" << endl;
			cout << "5. transform" << endl;
			cout << "6. for_each" << endl;
			cout << "7. Сумма " << endl;
			cout << "8. Среднее арифметическое" << endl;
			cout << "9. Вывод списка" << endl;
		}
		cout << "0. Выход" << endl;
		cin >> index;
		{
			switch (index)
			{
			case 1:
				cout << "Введите имя файла" << endl;
				cin >> str;
				cout << "Введите количество чисел" << endl;
				cin >> n;
				cout << "Введите интервал M" << endl;
				cin >> m;
				F = create_file(str, n, m);
				lst = read_file(F);
				full = true;
				break;
			case 2:
				cout << "Введите имя файла" << endl;
				cin >> str;
				F.open(str);
				lst = read_file(F);
				full = true;
				break;
			case 3:
				lst = modify(lst);
				break;
			case 4:
				cout << "Введите начало и конец, обрабатываемой части" << endl;
				cin >> st;
				cin >> en;
				start = lst.begin();
				for (int i = 1; i < st; i++)
					start++;
				end = start;
				for (int i = st; i <= en; i++)
					end++;
				lst = modify(lst, start, end);
				break;
			case 5:
				lst = modify_transform(lst);
				break;
			case 6:
				lst = modify_for_each(lst);
				break;
			case 7:
				cout << "Сумма чисел в контейнере: " << sum(lst) << endl;
				system("PAUSE");
				break;
			case 8:
				cout << "Среднее арефметическое в контейнере: " << average(lst) << endl;
				system("PAUSE");
				break;
			case 9:
				int sw;
				cout << "1. На экран" << endl;
				cout << "2. В файл" << endl;
				cin >> sw;
				switch (sw)
				{
				case 1:
					cout << lst;
					system("PAUSE");
					break;
				case 2:
					cout << "Введите имя файла: ";
					cin >> str;
					file.open(str);
					file << lst;
					file.close();
					break;
				}
				break;
			}
		}
	}

	system("PAUSE");
	return 0;
}


