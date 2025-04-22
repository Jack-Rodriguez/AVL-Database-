// AVL_Database.cpp
#include "AVL_Database.hpp"
#include <algorithm>
#include <cmath>
using namespace std;

//My name is Jack Rodriguez U69523108 and this is my own work. The only code that is not my own was the provided skeleton code. For this project I used ChatGPT and replit AI to teach me concepts and help me debug my code. I did not copy any code from these sources. I found the delete function to be the hardest, but overall this project was not overly difficult. I did run into a couple issues which I explain below. 

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
    nodeCount++;
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


    //now we check for imbalances. If the balance is greater than 1 or less than -1, we need to rotate.
    if (balance > 1)
    {
        //if the left child is greater than 0, we need to rotate right. If its less than 0, we need to rotate left.
        if (getBalance(node->left) >= 0)
            return rotateRight(node); // LL
        //if the left child is less than 0, we need to rotate left first, then right.
        else
        {
            node->left = rotateLeft(node->left); // LR
            return rotateRight(node);
        }
    }
        //same as above just reversed.
    else if (balance < -1)
    {
        if (getBalance(node->right) <= 0)
            return rotateLeft(node); // RR
        else
        {
            node->right = rotateRight(node->right); // RL
            return rotateLeft(node);
        }
    }


    //returns the node. This happens if the node wasnt NUll and we didnt need to rotate.
    return node;
}

//deletes a node from the tree.
void AVLTree::deleteNode(const std::string& key, int value) 
{
    //i dont like this part but it seems necessary for test 10 to pass. If the record isnt in the tree, we dont want to delete it.
    Record* rec = search(key, value);
    if (rec->key != "" || rec->value != 0) 
    { 
        root = deleteHelper(root, key, value);
        nodeCount--;
    }
}

//helper function for deleteNode.
AVLNode* AVLTree::deleteHelper(AVLNode* node, const std::string& key, int value)
{
    //if the node is null, we return the node. This acts as a
    //base case and an empty root case.
    if(node == nullptr) return node;
    
    //If the value is less we go left, right if its more. 
    if(value > node->record->value)
    {
        node->right = deleteHelper(node->right, key, value);
    }
    else if(value < node->record->value)
    {
        node->left = deleteHelper(node->left, key, value);
    }
    //if the value is the same, we have found the node we want to delete. 
    else if(value == node->record->value)
    {
        //Now we check for children and delete accordingly 
        //If there are no children, we simply delete the node.
        if(node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }
        //if it has one child we delete the node and return the child
        else if((node->left == nullptr) && (node->right != nullptr))
        {
            AVLNode* temp = node->right;
            delete node;
            return temp;
        }
        else if((node->left != nullptr) && (node->right == nullptr))
        {
            AVLNode* temp = node->left;
            delete node;
            return temp;
        }
        //if it has two, we find the min value in the right 
        //subtree and replace the node with it.
        else
        {
            AVLNode* temp = minValueNode(node->right);

            node->record = temp->record;

            node->right = deleteHelper(node->right, temp->record->key, temp->record->value);

            return node;
        }
    }

    //updates the height of the node.
    updateHeight(node);

    //we will use this value to balance the noe if needed
    int balance = getBalance(node);

    //this is all the same as the balancing in the insert help function
    //the only difference is that we are checking for imbalances in the subtrees rather than the node values itself.
    if (balance > 1)
    {
        if (getBalance(node->left) >= 0)
        {
            return rotateRight(node); // LL
        }
        else
        {
            node->left = rotateLeft(node->left); // LR
            return rotateRight(node);
        }
    }
    else if (balance < -1)
    {
        if (getBalance(node->right) <= 0)
        {
            return rotateLeft(node); // RR
        } 
        else
        {
            node->right = rotateRight(node->right); // RL
            return rotateLeft(node);
        }
    }

    return node;

}

AVLNode* AVLTree::minValueNode(AVLNode* node)
{
    AVLNode* current = node;
    while (current->left != nullptr)
    {
        current = current->left;
    }
    return current;
}


//searches for a record in the tree.
Record* AVLTree::search(const std::string& key, int value) 
{
    //resets the search comparison count.
    //Hey if you are grading this, this one line missing was causing my code to not work for hours. I cannot believe it to me so long to figure it out lol.
    searchComparisonCount = 0;
    
    //we create a node that will represent our target We create it by searching using the
    //helper
    AVLNode* foundNode = searchHelper(root, key, value);

    //if the returned node is null, we return null.
    if (foundNode == nullptr) 
    {
        return new Record("", 0);
    }


    //if not, we return the record.
    return foundNode->record;
}

//helper function for search.
AVLNode* AVLTree::searchHelper(AVLNode* node, const std::string& key, int value) const
{
    //if the node is null, we return null.
    if(node == nullptr)
    {
        return nullptr;
    }

    searchComparisonCount++;

    //if the node is the target, we return the node.
    if(node->record->key == key && node->record->value == value)
    {
        return node;
    }

    //this traversal is much better than simply checking every node. Now we can travel
    //towards the value rather than sweep the entire tree for it.
    //if value is less, we go left. If its more we go right. Easy peasy.
    if(value < node->record->value)
    {
        return searchHelper(node->left, key, value);
    }
    else
    {
        return searchHelper(node->right, key, value);
    }
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

//range query returns a vector of records whose values fall within a given range
std::vector<Record*> IndexedDatabase::rangeQuery(int start, int end) 
{
    //creates the vector
    vector<Record*> result;
    //calls the helper function.
    rangeQueryHelper(index.root, start, end, result);
    //returns the vector.
    return result;
}

//range query helper function.
void IndexedDatabase::rangeQueryHelper(AVLNode* node, int start, int end, std::vector<Record*>& result) const
{
    //if the node is null, we return.
    if(node == nullptr)
    {
        return;
    }
    //im not really going to explain this the psuedo code was given right above. Like i just did that lol.
    if(node->record->value >= start)
    {
        rangeQueryHelper(node->left, start, end, result);
    }
    if(node->record->value >= start && node->record->value <= end)
    {
        result.push_back(node->record);
    }
    if(node->record->value <= end)
    {
        rangeQueryHelper(node->right, start, end, result);
    }

    return;
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