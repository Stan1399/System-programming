// L3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <string>

template <typename T>
class tree_node
{
public:
	T key;
	//V value;
	int height;
	tree_node* ancestor;
	tree_node* left;
	tree_node* right;
	tree_node<T>(int T)
	{
		key = T;
		left = right = 0;
		height = 1;
	}
	//friend const tree_node& operator++(tree_node& x);
	//friend const tree_node& operator--(tree_node& x);
};

template <typename T>
tree_node<T>* find_min_node(tree_node<T>* p);

template <typename T>
tree_node<T>* find_max_node(tree_node<T>* p);

template <typename T>
tree_node<T>* next(tree_node<T> *x)
{
	if (x->right)
		return find_min_node(x->right);
	if (x->ancestor && x->ancestor->right != x)
		return x->ancestor;
	while (x->ancestor && x->ancestor->right == x)
		x = x->ancestor;
	return x->ancestor;
}

template <typename T>
tree_node<T>* previous(tree_node<T> *x)
{
	if (x->left)
		return find_max_node(x->left);
	if (x->ancestor && x->ancestor->left != x)
		return x->ancestor;
	while (x->ancestor && x->ancestor->left == x)
		x = x->ancestor;
	return x->ancestor;
}

template <typename T>
const tree_node<T>* operator++(tree_node<T>& x)
{
	/*if (x.left)
		return *(x.left);*/
	if (x.right)
		return find_min_node(x.right);
	if (x.ancestor.right.key != x.key)
		return x.ancestor;
	while (x.ancestor != NULL && x.ancestor.right.key == x.key)
		x = *(x.ancestor);
	return x.ancestor;
	/*if (x.right)
		return *(find_min_node(x.right));
	return *(x.ancestor);*/
}

template <typename T>
const tree_node<T>& operator--(tree_node<T>& x)
{
	/*if (x.left)
		return *(x.left);*/
	if (x.right)
		return *(find_min_node(x.right));
	return *(x.ancestor);
}

template <typename T>
int height(tree_node<T>* p)
{
	if (p)
		return p->height;
	return 0;
}

template <typename T>
int subtree_heigth_difference(tree_node<T>* p)
{
	return height(p->right) - height(p->left);
}

template <typename T>
void fix_height(tree_node<T>* p)
{
	int left_subtree_heigth = height(p->left);
	int right_subtree_heigth = height(p->right);
	if (left_subtree_heigth > right_subtree_heigth)
		p->height = left_subtree_heigth + 1;
	else
		p->height = right_subtree_heigth + 1;
}

template <typename T>
tree_node<T>* rotate_right(tree_node<T>* p)
{
	tree_node<T>* p_ancestor = p->ancestor;	//
	tree_node<T>* q = p->left;
	p->left = q->right;
	q->right = p;
	q->ancestor = p_ancestor;				//
	p->ancestor = q;						//
	if (p->left)
		p->left->ancestor = p;				//
	fix_height(p);
	fix_height(q);
	return q;
}

template <typename T>
tree_node<T>* rotate_left(tree_node<T>* q)
{
	tree_node<T>* q_ancestor = q->ancestor;
	tree_node<T>* p = q->right;
	q->right = p->left;
	p->left = q;
	p->ancestor = q_ancestor;
	q->ancestor = p;
	if (q->right)
		q->right->ancestor = q;
	fix_height(q);
	fix_height(p);
	return p;
}

template <typename T>
tree_node<T>* balance(tree_node<T>* p)
{
	fix_height(p);
	if (subtree_heigth_difference(p) == 2)
	{
		if (subtree_heigth_difference(p->right) < 0)
			p->right = rotate_right(p->right);
		return rotate_left(p);
	}
	if (subtree_heigth_difference(p) == -2)
	{
		if (subtree_heigth_difference(p->left) > 0)
			p->left = rotate_left(p->left);
		return rotate_right(p);
	}
	return p;
}

template <typename T>
tree_node<T>* insert(tree_node<T>* p, int k, tree_node<T>* anc)
{
	if (!p)
	{
		tree_node<T>* t = new tree_node<T>(k);
		t->ancestor = anc;
		return t;
	}
	if (k < p->key)
		p->left = insert(p->left, k, p);
	else
		p->right = insert(p->right, k, p);
	return balance(p);
}

template <typename T>
tree_node<T>* find_min_node(tree_node<T>* p)
{
	if (p->left)
		return find_min_node(p->left);
	return p;
}

template <typename T>
tree_node<T>* find_max_node(tree_node<T>* p)
{
	if (p->right)
		return find_max_node(p->right);
	return p;
}

template <typename T>
tree_node<T>* remove_min(tree_node<T>* p)
{
	if (p->left == 0)
		return p->right;
	p->left = remove_min(p->left);
	return balance(p);
}

template <typename T>
tree_node<T>* remove(tree_node<T>* p, int k)
{
	if (!p)
		return 0;
	if (p -> left && k < p->key)
		p->left = remove(p->left, k);
	else if (p->right && k > p->key)
		p->right = remove(p->right, k);
	else
	{
		tree_node<T>* q = p->left;
		tree_node<T>* r = p->right;
		tree_node<T>* a = p->ancestor;
		delete p;
		if (!r)
			return q;
		tree_node<T>* min = find_min_node(r);
		min->right = remove_min(r);
		if (min->right)
			min->right->ancestor = min;
		min->left = q;
		if (min->left)
			min->left->ancestor = min;
		min->ancestor = a;
		return balance(min);
	}
	return balance(p);
}

template <typename T>
void clear(tree_node<T>* p)
{
	while (p != NULL)
		p = remove(p, p->key);
}

template <typename T>
void show(tree_node<T>* p)
{
	if (p)
	{
		tree_node<T>* cur;
		cur = find_min_node(p);
		while (cur != NULL)
		{
			std::cout << cur->key << ' ';
			cur = next(cur);
		}
		if (cur != NULL)
			std::cout << cur->key << '\n';
	}
}

int main()
{
	tree_node<double> *tree = NULL, *iter = NULL;

	/*int n;
	std::cin >> n;*/
	//std::cout << RAND_MAX;

	do
	{
		double t;
		std::string s;
		std::cin >> s;
		switch (s[0])
		{
		case 'A':
		case 'a':
			std::cin >> t;
			tree = insert(tree, t, iter);
			break;
		case 'D':
		case 'd':
			std::cin >> t;
			tree = remove(tree, t);
			break;
		case 'C':
		case 'c':
			clear(tree);
			break;
		case 'S':
		case 's':
			show(tree);
			break;
		case 'E':
		case 'e':
			return 0;
		default:
			std::cout << "\nThere is no such command\n";
		}
		//double t1 = rand(), t2 = rand();
		//tree = insert(tree, t1 * t2 / 10/* % 10*/, iter);
	} while (true);
	//iter = find_min_node(tree);
	//for (int i = 0; i < n - 1; i++)
	//	iter = next(iter);

	//while (iter != NULL)
	//{
	//	std::cout << iter->key << ' ';
	//	//iter = next(iter);
	//	iter = previous(iter);
	//	//iter++;
	//}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
