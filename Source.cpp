#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
using namespace std;
#define INF INT_MAX

struct Comand
{
	int x;
	int y;
	int z;
	Comand(int x, int y, int z) : x(x), y(y), z(z) {}
	Comand() : x(0), y(0), z(0) {}
	Comand& operator =(const Comand& comand);
	bool operator < (const Comand& comand)const;
};

struct PairSecond
{
	int index;
	int y;
	PairSecond(int index, int y) : index(index), y(y) {}
	PairSecond() : index(0), y(0){}
	PairSecond& operator =(const PairSecond& pair);
	bool operator < (const PairSecond& pair)const;
	void set(int index, int y);
};

void vectorOut(vector<Comand> comands);
int getMin(int a, int b);
int getMin(vector<int>& tree, int l, int r);
void buildTree(vector<int>& tree, vector<int>& tmpArray);
void updateTree(vector<int>& tree, int position, int newValue);
int realNumber(vector<Comand> comands);

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");
	int n;
	in >> n;
	vector<Comand> comands(n);
	for (int i = 0; i < n; i++)
	{
		in >> comands.at(i).x >> comands.at(i).y >> comands.at(i).z;
	}
	//vectorOut(comands);
	sort(comands.begin(), comands.end());
	//vectorOut(comands);
	int newN;
	_asm
	{
		bsr eax, n
		bsf ebx, n
		cmp eax, ebx
		jne first
		dec eax
		first :
		mov newN, eax
	}
	newN = 1 << newN + 1;
	vector<int> tree(2 * newN - 1, INF);
	int count = 0;
	//updateTree(tree, 0, comands.at(0).z);
	count++;
	priority_queue<PairSecond> secondQueue;
	PairSecond tmp;
	for (int i = 0; i < n; i++)
	{
		tmp.set(i, comands.at(i).y);
		secondQueue.push(tmp);
	}
	for (int i = 0; i < n; i++)
	{
		tmp = secondQueue.top();
		secondQueue.pop();
		if (getMin(tree, 0, tmp.index - 1) > comands.at(tmp.index).z)
		{
			updateTree(tree, tmp.index, comands.at(tmp.index).z);
		}
	}
	//for (int i = 0; i < n; i++)
	//{
	//	if (tree.at(i + newN - 1) == INF)
	//	{
	//		if (getMin(tree, 0, i - 1) > comands.at(i).z)
	//		{
	//			updateTree(tree, i, comands.at(i).z);
	//		}
	//	}
	//}
	for (int i = 1; i < n; i++)
	{
		if (tree.at(i + newN - 1) != INF)
		{
			count++;
			//cout << '*' << i << endl;
		}
	}

	out << count;
	//cout << realNumber(comands) << endl;

	//system("pause");
	return 0;
}

Comand & Comand::operator=(const Comand & comand)
{
	this->x = comand.x;
	this->y = comand.y;
	this->z = comand.z;
	return *this;
}

bool Comand::operator<(const Comand & comand) const
{
	return this->x < comand.x;
}

PairSecond & PairSecond::operator=(const PairSecond & pair)
{
	this->y = pair.y;
	this->index = pair.index;
	return *this;
}

bool PairSecond::operator<(const PairSecond & pair) const
{
	return this->y > pair.y;
}

void PairSecond::set(int index, int y)
{
	this->index = index;
	this->y = y;
}

void vectorOut(vector<Comand> comands)
{
	for (int i = 0; i < comands.size(); i++)
	{
		cout << "{" << setw(3) << comands.at(i).x << ", " << setw(3) << comands.at(i).y << ", " << setw(3) << comands.at(i).z << " }   ";
	}
	cout << endl;
}

int getMin(int a, int b)
{
	return (a < b) ? a : b;
}

int getMin(vector<int>& tree, int l, int r)
{
	if (r < l)

	{
		return INF;
	}
	l++;
	r++;
	int result = INF;
	int n = tree.size() / 2;
	l += n - 1, r += n - 1;
	bool flag = true;
	while (l <= r)
	{
		// если l - правый сын своего родителя, 
		// учитываем его фундаментальный отрезок
		if (l % 2 == 0)
			result = getMin(result, tree[l++]);
		// если r - левый сын своего родителя, 
		// учитываем его фундаментальный отрезок
		if (r % 2 == 1)
			result = getMin(result, tree[r--]);
		// сдвигаем указатели на уровень выше
		l = (l - 1) / 2;
		r = (r - 1) / 2;
		if (!flag)
		{
			break;
		}
		if (l == r)
		{
			flag = false;
		}
	}
	return result;
}

void buildTree(vector<int>& tree, vector<int>& tmpArray)
{
	int newN = (tree.size() + 1) / 2;
	int n = tmpArray.size();
	fill(tree.begin(), tree.end(), 0);
	copy(tmpArray.begin(), tmpArray.end(), tree.begin() + newN - 1);
	for (int i = newN - 1; i > 0; i--)
	{
		tree[i - 1] = max(tree[2 * i], tree[2 * i - 1]);
	}
}

void updateTree(vector<int>& tree, int position, int newValue)
{
	int newN = (tree.size() + 1) / 2;
	position += newN - 1;
	tree[position] = newValue;
	position = (position - 1) / 2;
	while (position)
	{
		tree[position] = getMin(tree[2 * position + 1], tree[2 * position + 2]);
		position = (position - 1) / 2;
	}
	tree[0] = getMin(tree[1], tree[2]);
}

int realNumber(vector<Comand> comands)
{
	int result = 0;
	vector<bool> flags(comands.size(), false);
	for (int i = 0; i < comands.size(); i++)
	{
		for (int j = 0; j < comands.size(); j++)
		{
			if (i != j)
			{
				if (comands[i].x < comands[j].x && comands[i].y < comands[j].y && comands[i].z < comands[j].z)
				{
					if (!flags[j])
					{
						result++;
						flags[j] = true;
					}
				}
			}
		}
	}
	return comands.size() - result;
}

