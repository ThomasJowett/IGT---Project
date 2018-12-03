#pragma once

struct node {
	int value;
	node *left;
	node *right;
};

class BinaryTree
{
public:
	BinaryTree();
	~BinaryTree();

	void Insert(int key);
	node * Search(int key);
	void DestroyTree();
private:
	void DestroyTree(node *leaf);
	void Insert(int key, node *leaf);
	node *Search(int key, node *leaf);

	node * root;
};

BinaryTree::BinaryTree() {
	root = nullptr;
}

BinaryTree::~BinaryTree() {
	DestroyTree();
}

void BinaryTree::DestroyTree(node *leaf) {
	if (leaf != nullptr) {
		DestroyTree(leaf->left);
		DestroyTree(leaf->right);
		delete leaf;
	}
}

void BinaryTree::Insert(int key, node *leaf) {

	if (key < leaf->value) {
		if (leaf->left != nullptr) {
			Insert(key, leaf->left);
		}
		else {
			leaf->left = new node;
			leaf->left->value = key;
			leaf->left->left = nullptr;
			leaf->left->right = nullptr;
		}
	}
	else if (key >= leaf->value) {
		if (leaf->right != nullptr) {
			Insert(key, leaf->right);
		}
		else {
			leaf->right = new node;
			leaf->right->value = key;
			leaf->right->right = nullptr;
			leaf->right->left = nullptr;
		}
	}

}

void BinaryTree::Insert(int key) {
	if (root != nullptr) {
		Insert(key, root);
	}
	else {
		root = new node;
		root->value = key;
		root->left = nullptr;
		root->right = nullptr;
	}
}

node *BinaryTree::Search(int key, node *leaf) {
	if (leaf != nullptr) {
		if (key == leaf->value) {
			return leaf;
		}
		if (key < leaf->value) {
			return Search(key, leaf->left);
		}
		else {
			return Search(key, leaf->right);
		}
	}
	else {
		return nullptr;
	}
}

node *BinaryTree::Search(int key) {
	return Search(key, root);
}

void BinaryTree::DestroyTree() {
	DestroyTree(root);
}