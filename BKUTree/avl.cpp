#include <iostream>
#include <math.h>
#include <queue>
using namespace std;
#define SEPARATOR "#<ab@17943918#@>#"

//void printNSpace(int n)
//{
//    for (int i = 0; i < n - 1; i++)
//        cout << " ";
//}
//
//void printInteger(int &n)
//{
//    cout << n << " ";
//}

template <class K, class V>
class AVLTree {
public:
    class Entry {
    public:
        K key;
        V value;
        Entry(K key, V value) : key(key), value(value) {}
    };
    class Node {
        Entry* entry;
        Node* left;
        Node* right;
        int balance;
        friend class AVLTree;

        Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
            this->entry = entry;
            this->left = left;
            this->right = right;
            this->balance = 0;
        }
    };
private:
    void leftRotate(Node*& node) {
        Node* temp = node;
        node = node->right;
        temp->right = node->left;
        node->left = temp;
    }
    void rightRotate(Node*& node) {
        Node* temp = node;
        node = node->left;
        temp->left = node->right;
        node->right = temp;
    }

    void leftRightRotate(Node*& node) {
        leftRotate(node->left);
        rightRotate(node);
    }
    void rightLeftRotate(Node*& node) {
        rightRotate(node->right);
        leftRotate(node);
    }
    bool balanceLeft(Node*& node) {
        if (node->balance == 0) {
            node->balance = -1;
            return true;
        }
        else if (node->balance == 1) {
            node->balance = 0;
            return false;
        }
        else {
            if (node->left->balance == -1) {
                rightRotate(node);
                node->balance = node->right->balance = 0;
                return false;
            }
            else if (node->left->balance == 0) {
                rightRotate(node);
                node->balance = 1;
                node->right->balance = -1;
                return true;
            }
            leftRightRotate(node);
            if (node->balance == 0) {
                node->left->balance = node->right->balance = 0;
            }
            else if (node->balance == -1) {
                node->balance = node->left->balance = 0;
                node->right->balance = 1;
            }
            else {
                node->balance = node->right->balance = 0;
                node->left->balance = -1;
            }
            return false;
        }
    }
    bool balanceRight(Node*& node) {
        if (node->balance == 0) {
            node->balance = 1;
            return true;
        }
        else if (node->balance == -1) {
            node->balance = 0;
            return false;
        }
        else {
            if (node->right->balance == 1) {
                leftRotate(node);
                node->balance = node->left->balance = 0;
                return false;
            }
            else if (node->right->balance == 0) {
                leftRotate(node);
                node->balance = -1;
                node->left->balance = 1;
                return true;
            }
            rightLeftRotate(node);
            if (node->balance == 0) {
                node->right->balance = node->left->balance = 0;
            }
            else if (node->balance == 1) {
                node->balance = node->right->balance = 0;
                node->left->balance = -1;
            }
            else {
                node->balance = node->left->balance = 0;
                node->right->balance = 1;
            }
            return false;
        }
    }
    bool add(Node*& node, Entry* entry) {
        if (!node) {
            node = new Node(entry);
            return true;
        }

        if (node->entry->key == entry->key) throw "Duplicate key";
        else if (node->entry->key > entry->key) {
            if (add(node->left, entry)) return balanceLeft(node);
            else return false;
        }
        else {
            if (add(node->right, entry)) return balanceRight(node);
            else return false;
        }
    }
    bool remove(Node*& node, K key) {
        bool r = false, removeLeft = false;
        
        if (node) {
            if (node->entry->key == key) {
                if (node->left && node->right) {
                    Node* temp = node->left;
                    while (temp->right) temp = temp->right;
                    node->entry->key = temp->entry->key;
                    node->entry->value = temp->entry->value;
                    removeLeft = true;
                    r = remove(node->left, node->entry->key);
                }
                else if (node->left) {
                    Node* temp = node;
                    node = node->left;
                    delete temp;
                    return true;
                }
                else if (node->right) {
                    Node* temp = node;
                    node = node->right;
                    delete temp;
                    return true;
                }
                else {
                    delete node;
                    node = NULL;
                    return true;
                }
            }
            else if (node->entry->key > key) {
                removeLeft = true;
                r = remove(node->left, key);
            }
            else {
                removeLeft = false;
                r = remove(node->right, key);
            }
        }
        if (r) {
            if (removeLeft) return !balanceRight(node);
            else return !balanceLeft(node);
        }
        return false;
    }
    void clear(Node* node) {
        if (!node) return;
        Node* left = node->left;
        Node* right = node->right;
        delete node->entry;
        delete node;
        clear(left);
        clear(right);
    }
    int getHeight(Node* node) {
        if (!node) return 0;
        int lh = getHeight(node->left) + 1;
        int rh = getHeight(node->right) + 1;
        return (lh > rh) ? lh : rh;
    }

public:
    Node* root;

    AVLTree() : root(NULL) {};
    ~AVLTree() { this->clear(); };


    void add(K key, V value) {
        Entry* entry = new Entry(key, value);
        add(this->root, entry);
    }
    void add(Entry* entry) {
        add(this->root, entry);
    }
    void remove(K key) {
        if (!remove(this->root, key))throw "Not found";
    }
    V search(K key) {
        if (!root) throw "Not found !!!";
        queue<Node*> q;
        Node* current;
        q.push(root);
        while (!q.empty())
        {
            current = q.front();
            q.pop();
            if (current->entry->key == key) return current->entry->value;
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        throw "Not found !!!";
    }

    void traverseNLR(void (*func)(K key, V value));

    void clear() {
        clear(this->root);
        this->root = NULL;
    }
    void printTreeStructure()
    {
        int height = this->getHeight(this->root);
        if (this->root == NULL)
        {
            cout << "NULL\n";
            return;
        }
        queue<Node*> q;
        q.push(root);
        Node* temp;
        int count = 0;
        int maxNode = 1;
        int level = 0;
        int space = pow(2, height);
        printNSpace(space / 2);
        while (!q.empty())
        {
            temp = q.front();
            q.pop();
            if (temp == NULL)
            {
                cout << " ";
                q.push(NULL);
                q.push(NULL);
            }
            else
            {
                cout << temp->entry->key;
                q.push(temp->left);
                q.push(temp->right);
            }
            printNSpace(space);
            count++;
            if (count == maxNode)
            {
                cout << endl;
                count = 0;
                maxNode *= 2;
                level++;
                space /= 2;
                printNSpace(space / 2);
            }
            if (level == height)
                return;
        }
    }
};

//int main() {
//    AVLTree<int, string> avl;
//    int key[] = { 14,2,80,55,47,43,94,88,72,30,2 };
//    int size = sizeof(key) / sizeof(key[0]);
//    for (int i = 0; i < size; ++i) {
//        try {
//            avl.add(key[i], "x");
//        }
//        catch (const char* e) {
//            cout << e << endl;
//        }
//    }
//    try {
//        avl.remove(14);
//        avl.remove(0);
//    }
//    catch (const char* e) {
//        cout << e << endl;
//    }
//    /*for (int i = size - 1; i >= 0; --i) {
//        avl.remove(key[i]);
//    }*/
//    avl.printTreeStructure();
//}

