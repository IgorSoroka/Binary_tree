
#include "stdafx.h"

#include <stddef.h>
#include <iostream>

typedef unsigned int ui;
//шаблонный класс бинарное дерево
template <typename T>
class Tree
{
public://открытые члены и методы класса
	   //структура-узел дерева
	struct Item
	{
		Item* left;//указатель на левого наследника
		Item* right;//указатель на правого наследника
		T data;//данные, которые будут хранится в узле дерева
	};
	//конструктор по умолчанию
	Tree() : count(0), Root(nullptr)
	{}
	//конструктор с параметрами
	Tree(T value)
	{
		++count;//увеличиваем значение переменной, хранящей количество узлов дерева
		Item* temp = new Item;//создаем новый узел
		temp->data = value;//в переменную data записываем переданное значение
		temp->left = nullptr;//указатель на правого наследника устанавливаем nullptr
		temp->right = nullptr;//указатель на левого наследника устанавливаем nullptr
		Root = temp;//указатель на вершину дерева устанавливаем на созданный узел
	}
	//метод, добавляющий узел в дерево
	void AddElement(T value)
	{
		if (!Root)//если в дереве нет узлов, добавляем новый узел также как и в конструкторе с параметрами
		{
			Item* temp = new Item;//создаем новый узел
			temp->data = value;//в переменную data записываем переданное значение
			temp->left = nullptr;//указатель на правого наследника устанавливаем nullptr
			temp->right = nullptr;//указатель на левого наследника устанавливаем nullptr
			Root = temp;//указатель на вершину дерева устанавливаем на созданный узел
		}
		else//если в дереве уже есть хоть один узел
			Add(Root, value);//с помощью метода Add добавляем новый узел	
		++count;//увеличиваем значение переменной, хранящей количество узлов дерева
	}
	//метод, удаляющий узел дерева с заданным значением, и в случае положительного разультата возвращается true, отрицательного - false
	bool DelElement(T value)
	{
		if (!Root)//если в дереве вообще нет узлов возвращается false 
			return false;
		else//если в дереве есть хотя бы один узел
		{
			if (!SearchElement(value))//если узла с заданным значением нет - возвращается false
				return false;
			else//если в дереве есть узел с заданным значением
			{	//создаем 2 вспомогательных указателя, первому присваиваем адрес вершины дерева, второму - nullptr 
				Item* current = Root;
				Item* parrent = nullptr;
				while (true)//с помощью цикла проходим дерево, находим удаляеый элемент(current) и его родительский узел(parrent)
				{
					if (current->data < value)
					{
						parrent = current;
						current = parrent->right;
					}
					else if (value < current->data)
					{
						parrent = current;
						current = parrent->left;
					}
					else
						break;
				}
				//случай 1 - если справа от удаляемого узла нет потомка
				if (!current->right)
				{
					if (!parrent)//если удаляемый узел - вершина дерева, меняем указатель Root на левого наслндника удаляемого				   
						Root = current->left;
					else//если удаляемый узел не начало дерева
						if (current->data > parrent->data)//если значение удаляемого узла больше значения родительского
							parrent->right = current->left;
						else//если значение удаляемого узла меньше значения родительского
							parrent->left = current->left;
				}
				//случай 2 - если у удаляемого узла есть правый наследник, но у него нет левого наследника
				else if (!current->right->left)
				{//если удаляемый узел - вершина дерева, меняем указатель Root на правого наследника				
					if (!parrent)
						Root = current->right;
					else//если удаляемый элемент не начало дерева
					{
						current->right->left = current->left;//переносим левого наследника удаляемого в правый наследник в его левый узел
						if (current->right->data < parrent->data)//если значение правого наследника удаляемого узла меньше значения родительского узла					   
							parrent->left = current->right;
						else//если значение правого наследника удаляемого узла больше значения родительского узла
							parrent->right = current->right;
					}
				}
				//случай 3 - если у удаляемого узла есть правый наследник, у которой есть левый наследник, то нужно найти узел с минимальным значением в правом наследнике
				else
				{	//с помощью метода SearchMinAndDelete находим узел с минимальным значением в правом наследнике
					Item* Min = SearchMinForDelete(current->right);
					Min->left = current->left;//и подсставляем его на место удаляемого узла, связывая с левыми и правыми наследниками
					Min->right = current->right;
					if (Min->data < parrent->data)//если значение минимального узла меньше значения родительского
						parrent->left = Min;
					else//если значение минимального узла больше значения родительского
						parrent->right = Min;
				}
				delete current;//удаляем узел, со значением переданным в метод в качестве параметра
				--count;//уменьшаем переменную, отвечающую за количество узлов дерева
				return true;
			}
		}
	}
	//метод для поиска, есть ли в дереве узел с заданным значением
	bool SearchElement(T value) const
	{
		if (!Root)//если в дереве вообще нет узлов - возвращаем false
			return false;
		else//если в дереве есть узлы
		{//создаем вспомогательный указатель и присваиваем ему адрес вершины дерева
			Item* current = Root;
			while (current)//с помощью цикла проходим все дерево
			{
				if (value < current->data)
					current = current->left;
				else if (value > current->data)
					current = current->right;
				else
					return true;//если узел с заданным значением найден - возвращаем true
			}
			return false;//если в дереве нет узла с заданным значением - возвращаем false
		}
	}
	//метода для поиска узла с минимальным значением
	T SearchMin() const
	{
		if (!Root)//если дерево пустое
		{
			std::cout << "Error. Tree is empty!!!!" << std::endl;//выводим на экран сообщение об ошибке
			//return null;
		}
		else
		{
			//создаем 2 вспомогательных указателя, первому присваиваем адрес вершины дерева, второму - левого наследника вершины дерева 
			Item* cur = Root;
			Item* low = Root->left;
			while (low)//с помощью цикла проходим дерева, спуская вниз и влево, и находим узел с минимальным значением
			{
				cur = low;
				low = cur->left;
			}
			return cur->data;//возвращаем минимальное значение 
		}
	}
	//метода для поиска узла с максимальным значением
	T SearchMax() const
	{
		if (!Root)//если дерево пустое
		{
			std::cout << "Error. Tree is empty!!!!" << std::endl;//выводим на экран сообщение об ошибке
			//return null;
		}
		else
		{
			//создаем 2 вспомогательных указателя, первому присваиваем адрес вершины дерева, второму - правого наследника вершины дерева 
			Item* cur = Root;
			Item* low = Root->right;
			while (low)//с помощью цикла проходим дерева, спуская вниз и вправо, и находим узел с максимальным значением
			{
				cur = low;
				low = cur->right;
			}
			return cur->data;//возвращаем максимальное значение
		}
	}
	//метод, возвращающий количество узлов дерева
	ui CountElements() const
	{
		return count;
	}
	//метод, очищающий дерево                                                                              
	void ClearTree()
	{
		if (count)//если в дереве есть узлы
		{//создаем вспомогательный указатель и устанавливаем его nullptr
			Item* temp = nullptr;
			while (count > 1)//пока не останется один элемент - вершина списка, с помощью цикла и метода SearchMinAndDelete удаляем узлы дерева слева-направо
			{
				temp = SearchMinForDelete(Root);//указателю temp присваиваем адрес узла с минимальным значением
				delete temp;//удалем узел
				--count;//уменьшаем переменную, отвечающую за количество узлов дерева
			}
			delete Root;//удаляем вершину дерева
			--count;//уменьшаем переменную, отвечающую за количество узлов дерева
		}
	}
	//деструктор класса, использующий метод ClearTree
	~Tree()
	{
		ClearTree();
		std::cout << "Destructor!!!" << std::endl;
	}
private://закрытые члены и методы класса
		//вспомогательныйметод для добавления узла в дерево, использующий рекурсию
	void Add(Item* ptr, T v)
	{
		if (v < ptr->data)//если заданное значение меньше значения вершины дерева(поддерева)
		{//и левого наследника не существует, то создаем и добавляем новый узел			 
			if (!ptr->left)
			{
				Item* temp = new Item;//создаем новый узел
				temp->data = v;//в переменную data записываем переданное значение
				temp->left = nullptr;//указатель на правого наследника устанавливаем nullptr
				temp->right = nullptr;//указатель на левого наследника устанавливаем nullptr
				ptr->left = temp;//указатель на левого наследника вершины дерева(поддерева) устанавливаем на созданный узел
			}
			else//если левый наследник существует - переходим ниже, рекурсивно вызывая метод Аdd, передавая в качестве первого параметра указатель на левого наследника вершины дерева(поддерева)
				Add(ptr->left, v);
		}
		else//если заданное значение больше значения вершины дерева(поддерева)
		{
			if (!ptr->right)//если правого наследника не существует, то создаем и добавляем новый узел				
			{
				Item* temp = new Item;//создаем новый узел
				temp->data = v;//в переменную data записываем переданное значение
				temp->left = nullptr;//указатель на правого наследника устанавливаем nullptr
				temp->right = nullptr;//указатель на левого наследника устанавливаем nullptr
				ptr->right = temp;//указатель на правого наследника вершины дерева(поддерева) устанавливаем на созданный узел
			}
			else//если правый наследник существует - переходим ниже, рекурсивно вызывая метод Аdd, передавая в качестве первого параметра указатель на правого наследника вершины дерева(поддерева)
				Add(ptr->right, v);
		}
	}
	//вспомогательный метод для поиска узла с минимальным значением
	Item* SearchMinForDelete(Item* ptr) const
	{//создаем вспомогательный указатели для поиска узла с минимальным значением
		Item* parrent = nullptr;
		Item* cur = ptr;
		Item* lowleft = ptr->left;
		Item* lowright = ptr->right;
		if (lowleft)//если наследник слева существует, ищем узел с минимальным значением в левой половине дерева(поддерева)
		{
			while (lowleft)//с помощью цикла доходим до крайнего левого (с минимальным значением) узла дерева
			{
				parrent = cur;
				cur = lowleft;
				lowleft = cur->left;
			}
		}
		else if (lowright)//в противном случае, если наследник справа существует, ищем узел с минимальным значением в правой половине дерева(поддерева)
		{
			while (lowright)//с помощью цикла доходим до крайнего левого (с минимальным значением) узла дерева
			{
				parrent = cur;
				cur = lowright;
				lowright = cur->left;
			}
		}
		parrent->left = cur->right;//сохраняем, если он есть правого наследника удаляемого узла
		return cur;//возвращаем указатель на узел с минимальным значением
	}

	Item* Root;//указатель на вершину дерева
	ui count;//перенная, хранящая число узлов дерева
};

int main()
{
	Tree <int>Myfirst;
	Myfirst.AddElement(10);
	Myfirst.AddElement(6);
	Myfirst.AddElement(15);
	Myfirst.AddElement(3);
	Myfirst.AddElement(8);
	Myfirst.AddElement(12);
	Myfirst.AddElement(16);
	Myfirst.AddElement(1);
	Myfirst.AddElement(2);
	Myfirst.AddElement(9);
	Myfirst.AddElement(11);
	Myfirst.AddElement(18);
	Myfirst.AddElement(7);

	std::cout << "Searching element 15 (1-true, 0-false) - " << Myfirst.SearchElement(15) << std::endl;
	std::cout << "Searching element 6 (1-true, 0-false) - " << Myfirst.SearchElement(6) << std::endl;
	std::cout << "Searching element 8 (1-true, 0-false) - " << Myfirst.SearchElement(8) << std::endl;
	std::cout << "Searching element 12 (1-true, 0-false) - " << Myfirst.SearchElement(12) << std::endl;
	std::cout << "Searching element 1 (1-true, 0-false) - " << Myfirst.SearchElement(1) << std::endl;
	std::cout << "Searching element 18 (1-true, 0-false) - " << Myfirst.SearchElement(18) << std::endl;

	std::cout << "Searching element 20 (1-true, 0-false) - " << Myfirst.SearchElement(20) << std::endl;
	std::cout << "Searching element 21 (1-true, 0-false) - " << Myfirst.SearchElement(21) << std::endl;

	std::cout << std::endl;

	std::cout << "Minimum element in the tree - " << Myfirst.SearchMin() << std::endl;
	std::cout << "Maximum element in the tree - " << Myfirst.SearchMax() << std::endl;

	std::cout << std::endl;

	std::cout << "Amount of elements in the tree - " << Myfirst.CountElements() << std::endl;

	std::cout << "Removes the item with a value of 12. Removal was successful - " << std::boolalpha << Myfirst.DelElement(12) << std::endl;
	std::cout << "Amount of elements in the tree - " << Myfirst.CountElements() << std::endl;
	std::cout << "Removes the item with a value of 15. Removal was successful - " << std::boolalpha << Myfirst.DelElement(15) << std::endl;
	std::cout << "Amount of elements in the tree - " << Myfirst.CountElements() << std::endl;
	std::cout << "Removes the item with a value of 6. Removal was successful - " << std::boolalpha << Myfirst.DelElement(6) << std::endl;
	std::cout << "Amount of elements in the tree - " << Myfirst.CountElements() << std::endl;
	std::cout << "Removes the item with a value of 3. Removal was successful - " << std::boolalpha << Myfirst.DelElement(3) << std::endl;
	std::cout << "Amount of elements in the tree - " << Myfirst.CountElements() << std::endl;
	std::cout << "Removes the item with a value of 21. Removal was successful - " << std::boolalpha << Myfirst.DelElement(21) << std::endl;

	std::cout << "Searching element 15 (1-true, 0-false) - " << Myfirst.SearchElement(15) << std::endl;
	std::cout << "Searching element 6 (1-true, 0-false) - " << Myfirst.SearchElement(6) << std::endl;
	std::cout << "Searching element 3 (1-true, 0-false) - " << Myfirst.SearchElement(3) << std::endl;
	std::cout << "Searching element 18 (1-true, 0-false) - " << Myfirst.SearchElement(18) << std::endl;
	std::cout << "Searching element 1 (1-true, 0-false) - " << Myfirst.SearchElement(1) << std::endl;

	std::cout << std::endl;

	std::cout << "Minimum element in the tree - " << Myfirst.SearchMin() << std::endl;
	std::cout << "Maximum element in the tree - " << Myfirst.SearchMax() << std::endl;

	Myfirst.ClearTree();

	std::cout << "Amount of elements in the tree - " << Myfirst.CountElements() << std::endl;

	return 0;
}