#include <iostream>
#include <queue>
#include <vector>
using namespace std;

void printNSpace(int n)
{
    for (int i = 0; i < n - 1; i++)
        cout << " ";
}

void printInteger(int &n)
{
    cout << n << " ";
}
#include <math.h>

template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;

    class Entry {
    public:
        K key;
        V value;
        Entry(K key, V value) : key(key), value(value) {}
    };

public:
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    int maxNumOfKeys;
private: 
    void findKeyInQueue(K key) {
        int size = keys.size();
        queue<K> temp;
        while (size > 0 && keys.front() != key) {
            temp.push(keys.front());
            keys.pop();
        }
        while (!temp.empty()) {
            keys.push(temp.front());
            temp.pop();
        }
    }
public:
    BKUTree (int maxNumOfKeys = 5) {
        this->maxNumOfKeys = maxNumOfKeys;
        this->avl = new AVLTree();
        this->splay = new SplayTree();
    }
    ~BKUTree() {
        delete avl;
        delete splay;
        this-> clear(); 
    }

    void add(K key, V value) {
        Entry* entry = new Entry(key, value);
        typename SplayTree::Node* newSplayNode = new typename SplayTree::Node(entry);
        typename AVLTree::Node* newAVLNode = new typename AVLTree::Node(entry);
        newSplayNode->corr = newAVLNode;
        newAVLNode->corr = newSplayNode;
        avl->add(newAVLNode);
        splay->add(newSplayNode);
    }
    void remove(K key) {
        V value = splay->search(key);
        Entry* tempEntry = new Entry(key, value);
        splay->root->entry = tempEntry;
        avl->remove(key);
        splay->remove(key);
    }
    V search(K key, vector<K>& traversedList) {
        if (keys.size() == this->maxNumOfKeys) keys.pop();
        keys.push(key);
        V result;
        if (splay->root->entry->key == key) return splay->root->entry->value;
        findKeyInQueue(key);
        if (keys.front() == key) return splay->search(key, traversedList);
        else {
            try {
                result = avl->search(splay->root->corr, key, traversedList);
                splay->search(key);
            }
            catch (const char* e) {
                string temp = e;
                if (temp == "Not found") {
                    result = avl->search(avl->root, key, traversedList, splay->root->corr);
                    splay->search(key);
                }
            }
        }
        return result;
    }

    void traverseNLROnAVL(void (*func)(K key, V value)) {
        avl->traverseNLR(func);
    }
    void traverseNLROnSplay(void (*func)(K key, V value)) {
        splay->traverseNLR(func);
    }

    void clear() {
        avl->clear();
        splay->clear();
    }

    class SplayTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent;
            typename AVLTree::Node* corr;
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
                // Node* a = node->left;
                Node* b = node->right;
                // Node* c = p->right;
                node->parent = NULL;
                node->right = p;
                p->parent = node;
                p->left = b;
                if (b) b->parent = p;
            }
            else
            {
                // Node* a = p->left;
                Node* b = node->left;
                // Node* c = node->right;
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
                // Node* a = node->left;
                Node* b = node->right;
                Node* c = p->right;
                // Node* d = g->right;
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
                // Node* a = g->left;
                Node* b = p->left;
                Node* c = node->left;
                // Node* d = node->right;
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
                // Node* a = p->left;
                Node* b = node->left;
                Node* c = node->right;
                // Node* d = g->right;
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
                // Node* a = g->left;
                Node* b = node->left;
                Node* c = node->right;
                // Node* d = p->right;
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
        void add(Node*& node, Node* parent, Node* insertNode) {
            if (!node) {
                node = insertNode;
                node->parent = parent;
                return;
            }
            if (node->entry->key == insertNode->entry->key) throw "Duplicate key";
            else if (node->entry->key > insertNode->entry->key) add(node->left, node, insertNode);
            else add(node->right, node, insertNode);
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
        void preorder(Node* node, void (*func)(K key, V value)) {
            if (!node) return;
            func(node->entry->key, node->entry->value);
            preorder(node->left, func);
            preorder(node->right, func);
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

        SplayTree() : root(NULL) {};
        ~SplayTree() { this->clear(); };

        void add(K key, V value) {
            add(new Entry(key, value));
        }
        void add(Entry* entry) {
            Node* parent = NULL;
            add(this->root, parent, entry);
            search(entry->key);
        }
        void add(Node* insertNode) {
            Node* parent = NULL;
            add(this->root, parent, insertNode);
            search(insertNode->entry->key);
        }
        void remove(K key) {
            search(key);

            //Find largest node in left subtree
            Node* temp = root->left;
            if (!temp) {
                Node* p = root;
                root = root->right;
                if (root) root->parent = NULL;
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
                if (right) right->parent = root;
            }
        }
        V search(K key, vector<K>& traversedList) {
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
                traversedList.push_back(current->entry->key);
                if (current->entry->key > key) {
                    if (current->left) q.push(current->left);
                }
                else {
                    if (current->right) q.push(current->right);
                }
            }
            rebuild(current);
            throw "Not found";
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
                    if (current->left) q.push(current->left);
                }
                else {
                    if (current->right) q.push(current->right);
                }
            }
            rebuild(current);
            throw "Not found";
        }
        void traverseNLR(void (*func)(K key, V value)) {
            preorder(this->root, func);
        }
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

    class AVLTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
            typename SplayTree::Node* corr;
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
        bool add(Node*& node, Node* insertNode) {
            if (!node) {
                node = insertNode;
                return true;
            }
            // ??? value or key ???
            if (node->entry->key == insertNode->entry->key) throw "Duplicate key";
            else if (node->entry->key > insertNode->entry->key) {
                if (add(node->left, insertNode)) return balanceLeft(node);
                else return false;
            }
            else {
                if (add(node->right, insertNode)) return balanceRight(node);
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
            node->corr->entry = NULL;
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
        void preorder(Node* node, void (*func)(K key, V value)) {
            if (!node) return;
            func(node->entry->key, node->entry->value);
            preorder(node->left, func);
            preorder(node->right, func);
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
        void add(Node* insertNode) {
            add(this->root, insertNode);
        }
        void remove(K key) {
            if(!remove(this->root, key)) throw "Not found";
        }
        V search(Node* node, K key, vector<K>& traversedList) {
            if (!node) throw "Not found !!!";
            queue<Node*> q;
            Node* current;
            q.push(node);
            while (!q.empty())
            {
                current = q.front();
                q.pop();
                if (current->entry->key == key)
                    return current->entry->value;

                traversedList.push_back(current->entry->key);
                if (current->entry->key > key) {
                    if (current->left) q.push(current->left);
                }
                else {
                    if (current->right) q.push(current->right);
                }
            }
            throw "Not found !!!";
        }
        V search(Node* node, K key, vector<K>& traversedList, Node* _r) {
            if (!node) throw "Not found !!!";
            queue<Node*> q;
            Node* current;
            q.push(node);
            while (!q.empty())
            {   
                current = q.front();
                q.pop();

                if (current == _r) break;

                if (current->entry->key == key)
                    return current->entry->value;

                traversedList.push_back(current->entry->key);
                if (current->entry->key > key) {
                    if (current->left) q.push(current->left);
                }
                else {
                    if (current->right) q.push(current->right);
                }
            }
            throw "Not found !!!";
        }
        V search(K key, vector<K>& traversedList) {
            if (!this->root) throw "Not found !!!";
            queue<Node*> q;
            Node* current;
            q.push(this->root);
            while (!q.empty())
            {
                current = q.front();
                traversedList.push_back(current->entry->key);
                q.pop();
                if (current->entry->key == key)
                    return current->entry->value;

                if (current->entry->key > key) {
                    if (current->left) q.push(current->left);
                }
                else {
                    if (current->right) q.push(current->right);
                }
            }
            throw "Not found !!!";
        }

        void traverseNLR(void (*func)(K key, V value)) {
            preorder(this->root, func);
        }

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
};

void printKey(int key, int value) {
    cout << key << endl;
}
template<class T> 
void printVector(vector<T> v) {
    for(int i = 0; i < (int)v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
} 
int main() {
    vector<int> traversedKeys;
    BKUTree<int, int>* tree = new BKUTree<int, int>();
    int keys[] = { 1, 3, 5, 7, 9, 2, 4 };
    for (int i = 0; i < 7; i++) tree->add(keys[i], keys[i]);
    // try {
    //     tree->remove(5);
    //     tree->search(5, traversedKeys);
    // }
    // catch(const char* e) {
    //     cout << e << endl;
    // }
    tree->clear();
    tree->traverseNLROnSplay(printKey);
    tree->avl->printTreeStructure();
    tree->splay->printTreeStructure();
    
}