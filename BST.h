#pragma once

#include <stdexcept>

template<typename K, typename V>
class RedBlackBST {
private:
    enum class Color { RED, BLACK };
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        Color color;
        int size;
        Node(const K& key, const V& val, const Color& color, int size) :
                key(key), value(val),
                left(nullptr), right(nullptr),
                color(color), size(size) {};
    };
    Node* root_;
public:
    RedBlackBST() : root_(nullptr) {};
    ~RedBlackBST() {
        clear();
    }

    void insert(const K& key, const V& value) {
        root_ = insert(root_, key, value);
        root_->color = Color::BLACK;
    }

    bool contains(const K& key) {
        return contains(root_, key);
    }

    V& get(const K& key) const {
        return get(root_, key);
    }

    void remove(const K& key) {
        if (!contains(key)) return;

        if (!isRed(root_->left) && !isRed(root_->right))
            root_->color = Color::RED;

        root_ = remove(root_, key);
        if (root_ != nullptr)
            root_->color = Color::BLACK;
    }

    unsigned int size() const {
        if (root_ == nullptr)
            return 0;
        return root_->size;
    }

    void clear() {
        clear(root_);
        root_ = nullptr;
    }

    void print(std::ostream& os) {
        print(root_, os);
    }

private:
    void print(Node* node, std::ostream& os) {
        if (node == nullptr)
            return;

        print(node->left, os);
        os << node->value << " ";
        print(node->right, os);
    }
    void clear(Node* node) {
        if (node == nullptr)
            return;
        else {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    unsigned int size(Node* node) {
        if (node == nullptr)
            return 0;
        else
            return node->size;
    }

    Node* insert(Node* node, const K& key, const V& value) {
        if (node == nullptr)
            return new Node(key, value, Color::RED, 1);
        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else
            node->value = value;

        if (isRed(node->right) && !isRed(node->right))
            node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left->left))
            node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right))
            flipColors(node);
        node->size = size(node->left) + size(node->right) + 1;

        return node;
    }

    V& get(Node* node, const K& key) const {
        while (node != nullptr) {
            if (key < node->key)
                node = node->left;
            else if (key > node->key)
                node = node->right;
            else
                return node->value;
        }
        throw std::invalid_argument("No such element in tree");
    }

    bool contains(Node* node, const K& key) const {
        while (node != nullptr) {
            if (key < node->key)
                node = node->left;
            else if (key > node->key)
                node = node->right;
            else
                return true;
        }
        return false;
    }

    Node* remove(Node* node, const K& key) {
        if (key < node->key) {
            if (!isRed(node->left) && !isRed(node->left->left))
                node = moveRedLeft(node);
            node->left = remove(node->left, key);
        }
        else {
            if (isRed(node->left))
                node = rotateRight(node);
            if (key == node->key && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            if (!isRed(node->right) && !isRed(node->right->left))
                node = moveRedRight(node);
            if (key == node->key) {
                Node* temp = min(node->right);
                node->key = temp->key;
                node->value = temp->value;
                node->right = removeMin(node->right);
            }
            else {
                node->right = remove(node->right, key);
            }
        }
        return balance(node);
    }

    Node* rotateLeft(Node* h) {
        Node* x = h->right;
        h->right = x->left;
        x->left = h;
        x->color = h->color;
        h->color = Color::RED;
        x->size = h->size;
        h->size = 1 + size(h->left) + size(h->right);
        return x;
    }

    Node* rotateRight(Node* h) {
        Node* x = h->left;
        h->left = x->right;
        h->right = h;
        x->color = h->color;
        h->color = Color::RED;
        x->size = h->size;
        h->size = 1 + size(h->left) + size(h->right);
        return x;
    }

    Node* moveRedLeft(Node* node) {
        flipColors(node);
        if (isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
            flipColors(node);
        }
        return node;
    }

    Node* moveRedRight(Node* node) {
        flipColors(node);
        if (isRed(node->left->left)) {
            node = rotateRight(node);
            flipColors(node);
        }
        return node;
    }

    Node* balance(Node* node) {
        if (isRed(node->right))
            node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left->left))
            node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right))
            flipColors(node);

        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

    Node* min(Node* node) {
        if (node->left == nullptr)
            return node;
        else
            return min(node->left);
    }

    Node* removeMin(Node* node) {
        if (node->left == nullptr) {
            delete node;
            return nullptr;
        }
        if (!isRed(node->left) && !isRed(node->left->left))
            node = moveRedLeft(node);
        node->left = removeMin(node->left);
        return balance(node);
    }

    void flipColors(Node* h) {
        h->color = Color::RED;
        h->left->color = Color::BLACK;
        h->right->color = Color::BLACK;
    }

    bool isRed(Node* node) {
        if (node == nullptr)
            return false;
        return node->color == Color::RED;
    }
};