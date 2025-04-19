// AVL_Database.cpp
#include "AVL_Database.hpp"
#include <algorithm>
#include <cmath>

Record::Record(const std::string& k, int v) : key(k), value(v) {}

AVLNode::AVLNode(Record* r) : record(r), left(nullptr), right(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr), nodeCount(0), searchComparisonCount(0) {}

int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}


void AVLTree::updateHeight(AVLNode* node) 
{
    if (node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

int AVLTree::getBalance(AVLNode* node) 
{
    return node ? height(node->left) - height(node->right) : 0;
}

/*
Can be used to balance AVL trees
*/
AVLNode* AVLTree::rotateRight(AVLNode* y) 
{
    if(y == nullptr || y->left == nullptr)
    {
        return y;
    }

    AVLNode* l = y->left;
    y->left = l->right;
    l->right = y;
    updateHeight(y);
    updateHeight(l);
    return l;
    
}

/*
Can be used to balance AVL trees
*/
AVLNode* AVLTree::rotateLeft(AVLNode* x) 
{
    if(x == nullptr || x->right == nullptr)
    {
        return x;
    }

    AVLNode* r = x->right;
    x->right = r->left;
    r->left = x;
    updateHeight(x);
    updateHeight(r);
    return r;

    
}


void AVLTree::insert(Record* record) 
{
    root = insertHelper(root, record);
}

AVLNode* AVLTree::insertHelper(AVLNode* node, Record* record)
{
    //checks if the node is null. If so, we can insert right away.
    if(node == nullptr)
    {
        return new AVLNode(record);
    }

    //now we move through the tree based on the record value. This is done recursively.
    //if the record value is less than current, then we move left. We more right if its more.
    if(record->value < node->record->value)
    {
        node->left = insertHelper(node->left, record);
    }
    else
    {
        node->right = insertHelper(node->right, record);
    }

    //updates the height of the current node (this is also done in rotations, but we need it here)
    updateHeight(node);
    //we need a balance of the current node.
    int balance = getBalance(node);


    //now we check for imbalances. If it is more than one, we have a left imbalance. If its less 
    //than one its right imbalance. Further more (left as an example) if we are left imbalanced, and 
    //the record value is less than the left node, that means the new insert was left of the 
    //current, and left of the current->left. Now we know the imbalance is LL. If that isnt the 
    //case, then it is LR.
    if(balance > 1)
    {
        if(record->value < node->left->record->value)
        {
            //rotate the current node right and return.
            return rotateRight(node);
        }
        else
        {
            //rotate the current->left, the return the node rotated right.
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }
        //this all works the same as above.
    else if(balance < -1)
    {
        if(record->value > node->right->record->value)
        {
            return rotateLeft(node);
        }
        else
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    //returns the node. This happens if the node wasnt NUll and we didnt need to rotate.
    return node;
}




void AVLTree::deleteNode(const std::string& key, int value) {
    // to do ..
}



Record* AVLTree::search(const std::string& key, int value) {
    // to do..

    return nullptr;
}

// IndexedDatabase Implementation
void IndexedDatabase::insert(Record* record) {
    index.insert(record);
}

Record* IndexedDatabase::search(const std::string& key, int value) {
    return index.search(key, value);
}

void IndexedDatabase::deleteRecord(const std::string& key, int value) {
    index.deleteNode(key, value);
}



/* RangeQuery Hints
1. Base: if (!node) return
2. Key Traversal Logic:
   - If value >= start: check left subtree
   - If start <= value <= end: add to result
   - If value <= end: check right subtree
*/


std::vector<Record*> IndexedDatabase::rangeQuery(int start, int end) {
    //to do..

    return {};
}



void IndexedDatabase::clearHelper(AVLNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node->record;
    delete node;
}

void IndexedDatabase::clearDatabase() {
    clearHelper(index.root);
    index.root = nullptr;
}

int IndexedDatabase::calculateHeight(AVLNode* node) const {
    if (!node) return 0;
    return 1 + std::max(calculateHeight(node->left), calculateHeight(node->right));
}

int IndexedDatabase::getTreeHeight() const {
    return calculateHeight(index.root);
}

int IndexedDatabase::getSearchComparisons(const std::string& key, int value) {
    search(key, value);
    return index.getLastSearchComparisons();
}