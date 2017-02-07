
#include "stdafx.h"

#include <stddef.h>
#include <iostream>

typedef unsigned int ui;
//��������� ����� �������� ������
template <typename T>
class Tree
{
public://�������� ����� � ������ ������
	   //���������-���� ������
	struct Item
	{
		Item* left;//��������� �� ������ ����������
		Item* right;//��������� �� ������� ����������
		T data;//������, ������� ����� �������� � ���� ������
	};
	//����������� �� ���������
	Tree() : count(0), Root(nullptr)
	{}
	//����������� � �����������
	Tree(T value)
	{
		++count;//����������� �������� ����������, �������� ���������� ����� ������
		Item* temp = new Item;//������� ����� ����
		temp->data = value;//� ���������� data ���������� ���������� ��������
		temp->left = nullptr;//��������� �� ������� ���������� ������������� nullptr
		temp->right = nullptr;//��������� �� ������ ���������� ������������� nullptr
		Root = temp;//��������� �� ������� ������ ������������� �� ��������� ����
	}
	//�����, ����������� ���� � ������
	void AddElement(T value)
	{
		if (!Root)//���� � ������ ��� �����, ��������� ����� ���� ����� ��� � � ������������ � �����������
		{
			Item* temp = new Item;//������� ����� ����
			temp->data = value;//� ���������� data ���������� ���������� ��������
			temp->left = nullptr;//��������� �� ������� ���������� ������������� nullptr
			temp->right = nullptr;//��������� �� ������ ���������� ������������� nullptr
			Root = temp;//��������� �� ������� ������ ������������� �� ��������� ����
		}
		else//���� � ������ ��� ���� ���� ���� ����
			Add(Root, value);//� ������� ������ Add ��������� ����� ����	
		++count;//����������� �������� ����������, �������� ���������� ����� ������
	}
	//�����, ��������� ���� ������ � �������� ���������, � � ������ �������������� ���������� ������������ true, �������������� - false
	bool DelElement(T value)
	{
		if (!Root)//���� � ������ ������ ��� ����� ������������ false 
			return false;
		else//���� � ������ ���� ���� �� ���� ����
		{
			if (!SearchElement(value))//���� ���� � �������� ��������� ��� - ������������ false
				return false;
			else//���� � ������ ���� ���� � �������� ���������
			{	//������� 2 ��������������� ���������, ������� ����������� ����� ������� ������, ������� - nullptr 
				Item* current = Root;
				Item* parrent = nullptr;
				while (true)//� ������� ����� �������� ������, ������� �������� �������(current) � ��� ������������ ����(parrent)
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
				//������ 1 - ���� ������ �� ���������� ���� ��� �������
				if (!current->right)
				{
					if (!parrent)//���� ��������� ���� - ������� ������, ������ ��������� Root �� ������ ���������� ����������				   
						Root = current->left;
					else//���� ��������� ���� �� ������ ������
						if (current->data > parrent->data)//���� �������� ���������� ���� ������ �������� �������������
							parrent->right = current->left;
						else//���� �������� ���������� ���� ������ �������� �������������
							parrent->left = current->left;
				}
				//������ 2 - ���� � ���������� ���� ���� ������ ���������, �� � ���� ��� ������ ����������
				else if (!current->right->left)
				{//���� ��������� ���� - ������� ������, ������ ��������� Root �� ������� ����������				
					if (!parrent)
						Root = current->right;
					else//���� ��������� ������� �� ������ ������
					{
						current->right->left = current->left;//��������� ������ ���������� ���������� � ������ ��������� � ��� ����� ����
						if (current->right->data < parrent->data)//���� �������� ������� ���������� ���������� ���� ������ �������� ������������� ����					   
							parrent->left = current->right;
						else//���� �������� ������� ���������� ���������� ���� ������ �������� ������������� ����
							parrent->right = current->right;
					}
				}
				//������ 3 - ���� � ���������� ���� ���� ������ ���������, � ������� ���� ����� ���������, �� ����� ����� ���� � ����������� ��������� � ������ ����������
				else
				{	//� ������� ������ SearchMinAndDelete ������� ���� � ����������� ��������� � ������ ����������
					Item* Min = SearchMinForDelete(current->right);
					Min->left = current->left;//� ������������ ��� �� ����� ���������� ����, �������� � ������ � ������� ������������
					Min->right = current->right;
					if (Min->data < parrent->data)//���� �������� ������������ ���� ������ �������� �������������
						parrent->left = Min;
					else//���� �������� ������������ ���� ������ �������� �������������
						parrent->right = Min;
				}
				delete current;//������� ����, �� ��������� ���������� � ����� � �������� ���������
				--count;//��������� ����������, ���������� �� ���������� ����� ������
				return true;
			}
		}
	}
	//����� ��� ������, ���� �� � ������ ���� � �������� ���������
	bool SearchElement(T value) const
	{
		if (!Root)//���� � ������ ������ ��� ����� - ���������� false
			return false;
		else//���� � ������ ���� ����
		{//������� ��������������� ��������� � ����������� ��� ����� ������� ������
			Item* current = Root;
			while (current)//� ������� ����� �������� ��� ������
			{
				if (value < current->data)
					current = current->left;
				else if (value > current->data)
					current = current->right;
				else
					return true;//���� ���� � �������� ��������� ������ - ���������� true
			}
			return false;//���� � ������ ��� ���� � �������� ��������� - ���������� false
		}
	}
	//������ ��� ������ ���� � ����������� ���������
	T SearchMin() const
	{
		if (!Root)//���� ������ ������
		{
			std::cout << "Error. Tree is empty!!!!" << std::endl;//������� �� ����� ��������� �� ������
			//return null;
		}
		else
		{
			//������� 2 ��������������� ���������, ������� ����������� ����� ������� ������, ������� - ������ ���������� ������� ������ 
			Item* cur = Root;
			Item* low = Root->left;
			while (low)//� ������� ����� �������� ������, ������� ���� � �����, � ������� ���� � ����������� ���������
			{
				cur = low;
				low = cur->left;
			}
			return cur->data;//���������� ����������� �������� 
		}
	}
	//������ ��� ������ ���� � ������������ ���������
	T SearchMax() const
	{
		if (!Root)//���� ������ ������
		{
			std::cout << "Error. Tree is empty!!!!" << std::endl;//������� �� ����� ��������� �� ������
			//return null;
		}
		else
		{
			//������� 2 ��������������� ���������, ������� ����������� ����� ������� ������, ������� - ������� ���������� ������� ������ 
			Item* cur = Root;
			Item* low = Root->right;
			while (low)//� ������� ����� �������� ������, ������� ���� � ������, � ������� ���� � ������������ ���������
			{
				cur = low;
				low = cur->right;
			}
			return cur->data;//���������� ������������ ��������
		}
	}
	//�����, ������������ ���������� ����� ������
	ui CountElements() const
	{
		return count;
	}
	//�����, ��������� ������                                                                              
	void ClearTree()
	{
		if (count)//���� � ������ ���� ����
		{//������� ��������������� ��������� � ������������� ��� nullptr
			Item* temp = nullptr;
			while (count > 1)//���� �� ��������� ���� ������� - ������� ������, � ������� ����� � ������ SearchMinAndDelete ������� ���� ������ �����-�������
			{
				temp = SearchMinForDelete(Root);//��������� temp ����������� ����� ���� � ����������� ���������
				delete temp;//������ ����
				--count;//��������� ����������, ���������� �� ���������� ����� ������
			}
			delete Root;//������� ������� ������
			--count;//��������� ����������, ���������� �� ���������� ����� ������
		}
	}
	//���������� ������, ������������ ����� ClearTree
	~Tree()
	{
		ClearTree();
		std::cout << "Destructor!!!" << std::endl;
	}
private://�������� ����� � ������ ������
		//�������������������� ��� ���������� ���� � ������, ������������ ��������
	void Add(Item* ptr, T v)
	{
		if (v < ptr->data)//���� �������� �������� ������ �������� ������� ������(���������)
		{//� ������ ���������� �� ����������, �� ������� � ��������� ����� ����			 
			if (!ptr->left)
			{
				Item* temp = new Item;//������� ����� ����
				temp->data = v;//� ���������� data ���������� ���������� ��������
				temp->left = nullptr;//��������� �� ������� ���������� ������������� nullptr
				temp->right = nullptr;//��������� �� ������ ���������� ������������� nullptr
				ptr->left = temp;//��������� �� ������ ���������� ������� ������(���������) ������������� �� ��������� ����
			}
			else//���� ����� ��������� ���������� - ��������� ����, ���������� ������� ����� �dd, ��������� � �������� ������� ��������� ��������� �� ������ ���������� ������� ������(���������)
				Add(ptr->left, v);
		}
		else//���� �������� �������� ������ �������� ������� ������(���������)
		{
			if (!ptr->right)//���� ������� ���������� �� ����������, �� ������� � ��������� ����� ����				
			{
				Item* temp = new Item;//������� ����� ����
				temp->data = v;//� ���������� data ���������� ���������� ��������
				temp->left = nullptr;//��������� �� ������� ���������� ������������� nullptr
				temp->right = nullptr;//��������� �� ������ ���������� ������������� nullptr
				ptr->right = temp;//��������� �� ������� ���������� ������� ������(���������) ������������� �� ��������� ����
			}
			else//���� ������ ��������� ���������� - ��������� ����, ���������� ������� ����� �dd, ��������� � �������� ������� ��������� ��������� �� ������� ���������� ������� ������(���������)
				Add(ptr->right, v);
		}
	}
	//��������������� ����� ��� ������ ���� � ����������� ���������
	Item* SearchMinForDelete(Item* ptr) const
	{//������� ��������������� ��������� ��� ������ ���� � ����������� ���������
		Item* parrent = nullptr;
		Item* cur = ptr;
		Item* lowleft = ptr->left;
		Item* lowright = ptr->right;
		if (lowleft)//���� ��������� ����� ����������, ���� ���� � ����������� ��������� � ����� �������� ������(���������)
		{
			while (lowleft)//� ������� ����� ������� �� �������� ������ (� ����������� ���������) ���� ������
			{
				parrent = cur;
				cur = lowleft;
				lowleft = cur->left;
			}
		}
		else if (lowright)//� ��������� ������, ���� ��������� ������ ����������, ���� ���� � ����������� ��������� � ������ �������� ������(���������)
		{
			while (lowright)//� ������� ����� ������� �� �������� ������ (� ����������� ���������) ���� ������
			{
				parrent = cur;
				cur = lowright;
				lowright = cur->left;
			}
		}
		parrent->left = cur->right;//���������, ���� �� ���� ������� ���������� ���������� ����
		return cur;//���������� ��������� �� ���� � ����������� ���������
	}

	Item* Root;//��������� �� ������� ������
	ui count;//��������, �������� ����� ����� ������
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