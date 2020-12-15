#include <iostream>
#include <queue>
#include <vector>
using namespace std;

//void printNSpace(int n)
//{
//    for (int i = 0; i < n - 1; i++)
//        cout << " ";
//}
//
//void printInteger(int& n)
//{
//    cout << n << " ";
//}

template <class K, class V>
class SplayTree {
public:
    class Entry {
    public:
        K key;
        V value;
        Entry(K key, V value) {
            this->key = key;
            this->value = value;
        }
    };
    class Node {
        Entry* entry;
        Node* left;
        Node* right;
        Node* parent;
        friend class SplayTree;

        Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL, Node* parent = NULL) {
            this->entry = entry;
            this->left = left;
            this->right = right;
            this->parent = parent;
        }
    };
private: 
    void zig(Node*& node) {
        Node* p = node->parent;
        if (p->left == node) {
            Node* a = node->left;
            Node* b = node->right;
            Node* c = p->right;
            node->parent = NULL;
            node->right = p;
            p->parent = node;
            p->left = b;
            if (b) b->parent = p;
        }
        else
        {
            Node* a = p->left;
            Node* b = node->left;
            Node* c = node->right;
            node->parent = NULL;
            node->left = p;
            p->parent = node;
            p->right = b;
            if (b) b->parent = p;
        }
    }
    void zig_zig(Node*& node)
    {
        Node* p = node->parent;
        Node* g = p->parent;
        if (p->left == node)
        {
            Node* a = node->left;
            Node* b = node->right;
            Node* c = p->right;
            Node* d = g->right;
            node->parent = g->parent;
            node->right = p;
            p->parent = node;
            p->left = b;
            p->right = g;
            g->parent = p;
            g->left = c;
            if (node->parent) {
                if (node->parent->left == g) node->parent->left = node;
                else node->parent->right = node;
            }
            if (b) b->parent = p;
            if (c) c->parent = g;
        }
        else
        {
            Node* a = g->left;
            Node* b = p->left;
            Node* c = node->left;
            Node* d = node->right;
            node->parent = g->parent;
            node->left = p;
            p->parent = node;
            p->left = g;
            p->right = c;
            g->parent = p;
            g->right = b;
            if (node->parent) {
                if (node->parent->left == g) node->parent->left = node;
                else node->parent->right = node;
            }
            if (b) b->parent = g;
            if (c) c->parent = p;
        }
    }
    void zig_zag(Node*& node)
    {
        Node* p = node->parent;
        Node* g = p->parent;
        if (p->right == node)
        {
            Node* a = p->left;
            Node* b = node->left;
            Node* c = node->right;
            Node* d = g->right;
            node->parent = g->parent;
            node->left = p;
            node->right = g;
            p->parent = node;
            p->right = b;
            g->parent = node;
            g->left = c;
            if (node->parent) {
                if (node->parent->left == g) node->parent->left = node;
                else node->parent->right = node;
            }
            if (b) b->parent = p;
            if (c) c->parent = g;
        }
        else
        {
            Node* a = g->left;
            Node* b = node->left;
            Node* c = node->right;
            Node* d = p->right;
            node->parent = g->parent;
            node->left = g;
            node->right = p;
            p->parent = node;
            p->left = c;
            g->parent = node;
            g->right = b;
            if (node->parent) {
                if (node->parent->left == g) node->parent->left = node;
                else node->parent->right = node;
            }
            if (b) b->parent = g;
            if (c) c->parent = p;
        }
    }
    void rebuild(Node*& node) {
        if (!node) return;

        while (node->parent) {
            Node* p = node->parent;
            Node* g = p->parent;
            if (!g) zig(node);
            else if (g->left == p && p->left == node) zig_zig(node);
            else if (g->right == p && p->right == node) zig_zig(node);
            else zig_zag(node);
        }
        this->root = node;
    }
    void add(Node*& node, Node* parent, Entry* entry) {
        if (!node) {
            node = new Node(entry, NULL, NULL, parent);
            return;
        }
        if (node->entry->key == entry->key) throw "Duplicate key";
        else if (node->entry->key > entry->key) add(node->left, node, entry);
        else add(node->right, node, entry);
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
    int countNode(Node* node) {
        if (!node) return 0;
        return 1 + countNode(node->left) + countNode(node->right);
    }
public:
    Node* root;

    SplayTree(): root(NULL) {};
    ~SplayTree() { this->clear(); };

    void add(K key, V value) {
        add(new Entry(key, value));
    }
    void add(Entry* entry) {
        Node* parent = NULL;
        add(this->root, parent, entry);
        search(entry->key);
    }
    void remove(K key) {
        search(key);

        //Find largest node in left subtree
        Node* temp = root->left;
        if (!temp) {
            Node* p = root;
            root = root->right;
            if(root) root->parent = NULL;
            delete p;
        }
        else {
            while (temp->right) temp = temp->right;
            Node* right = root->right;
            Node* deleteNode = root;
            root = root->left;
            root->parent = NULL;
            delete deleteNode;
            rebuild(temp);
            root->right = right;
            if(right) right->parent = root;
        }
    }
    V search(K key) {
        if (!this->root) throw "Not found";
        queue<Node*> q;
        Node* current;
        q.push(this->root);
        while (!q.empty())
        {
            current = q.front();
            q.pop();
            if (!current) continue;
            if (current->entry->key == key) {
                rebuild(current);
                return current->entry->value;
            }
            else if (current->entry->key > key) {
                if(current->left) q.push(current->left);
            }
            else {
                if(current->right) q.push(current->right);
            }
        }
        rebuild(current);
        throw "Not found";
    }
    void traverseNLR(void (*func)(K key, V value));
    void clear() {
        clear(this->root);
        this->root = NULL;
    }
    void printTreeStructure()
    {
        int height = getHeight(this->root);
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

//int main(){
//    SplayTree<int, string> splay;
//    int key[] = { 14,2,80,55,47,43,94,88,72,30,11,87,29,39 };
//    /*int key[] = { 14,2,80 };*/
//    
//    int size = sizeof(key)/sizeof(key[0]);
//    for(int i = 0; i < size; ++i) {
//        splay.add(key[i], "x");
//    }
//    
//    try {
//        splay.search(55);
//    }
//    catch (const char* e) {
//        cout << e << endl;
//    }
//    splay.printTreeStructure();
//    for (int i = 0; i < size; ++i) {
//        splay.remove(key[i]);
//    }
//    splay.printTreeStructure();
//    return 0;
//}

//int main() {
//    SplayTree<int, int> tree;
//    int keys[] = { 1, 3, 5, 7, 9, 2, 4 };
//    for (int i = 0; i < 7; i++) tree.add(keys[i], keys[i]);
//    tree.printTreeStructure();
//}