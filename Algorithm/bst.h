#pragma once

// binary search tree

#include <functional>
#include <type_traits>

template<typename Key, typename Comp = std::less<Key>>
class BST
{
	struct Node
	{
		Node* parent = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;
		Key key;
	};

	Node* root = nullptr;
	Comp comp;

	void removeSubTree(Node* node);
	Node* findSubTree(Node* node, const Key& key);

public:
	
	//static bool isBST(const BST& tree);

	BST() = default;
	BST(const BST& src) = delete;	
	BST(BST&& src) = delete;
	BST& operator=(const BST& src) = delete;
	BST& operator=(BST&& src) = delete;

	~BST() 
	{
		clear();
	}

	bool insert(const Key& key);
	
	bool find(const Key& key) { return findSubTree(root, key); }
	
	void remove(const Key& key);
	void clear();
	
	bool empty() { return !root; }

/*	 
	template<class F, class ...Arg>
	typename std::result_of<F(Arg...)>::type traverse(Node* node, F(Arg...) f)
	{
		if ()
	}
*/	
	template<typename F>
	void traverse(F f)
	{
		traverse(root, f);
	}

	template<typename F>
	void traverse(Node* node, F f)
	{
		if (!node)
			return;

		traverse(node->left, f);
		f(node->key);
		traverse(node->right, f);
	}

};

/*
template<typename Key, typename Comp>
bool BST<Key, Comp>::isBST(const BST<Key, Comp>& tree)
{
	return tree.traverse(tree.root, )	
}
*/
template<typename Key, typename Comp>
bool BST<Key, Comp>::insert(const Key& key)
{
	Node* node = root;
	Node* parent = nullptr;
	bool isleft = true;
	
	while (node)
	{
		parent = node;
		if (comp(key, node->key))
		{
			node = node->left;
			isleft = true;
		}
		else if (comp(node->key, key))
		{
			node = node->right;
			isleft = false;
		}
		else
		{
			return false;
		}
	}

	node = new Node;
	node->key = key;
	node->parent = parent;
	if (parent)
		(isleft? parent->left : parent->right) = node;
	else
		root = node;

	return true;
}

template<typename Key, typename Comp>
typename BST<Key, Comp>::Node* BST<Key, Comp>::findSubTree(Node* node, const Key& key)
{
	if (!node)
		return nullptr;

	if (comp(key, node->key))
		return findSubTree(node->left, key);	
	else if (comp(node->key, key))
		return findSubTree(node->right, key);
	else
		return node;
}

template<typename Key, typename Comp>
void BST<Key, Comp>::remove(const Key& key)
{
	Node* node = findSubTree(root, key);

	if (!node)
		return;

	if (!node->left)	// doesn't have left subtree			
	{
		if (auto p = node->parent)
		{
			(p->left == node ? p->left : p->right) = node->right;
			if (node->right)
				node->right->parent = p;
		}
		else
		{
			if (root = node->right)
				root->parent = nullptr;
		}
	}
	else      
	{
		Node* n = node->left;      
		while (n->right)				// find maximum key in the left subtree      
			n = n->right;

		(n == node->left ? node->left : n->parent->right) = n->left;
		if (n->left)
			n->left->parent = n->parent;
		
		node->key = n->key;
		node = n;
	}

	delete node;
	node = nullptr;
}

template<typename Key, typename Comp>
void BST<Key, Comp>::clear()
{
	removeSubTree(root);
	root = nullptr;
}

template<typename Key, typename Comp>
void BST<Key, Comp>::removeSubTree(Node* node)
{
	if (!node)
		return;

	removeSubTree(node->left);
	removeSubTree(node->right);
	delete node;
	
}