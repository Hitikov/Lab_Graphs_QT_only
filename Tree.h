#ifndef TREE_H
#define TREE_H

#include <QPair>
#include <QMap>

template <typename T, typename TT>
class Tree
{
private:
    Tree<T, TT>* left = nullptr;
    Tree<T, TT>* right = nullptr;
    Tree<T, TT>* parent = nullptr;
    int lowerBorder;
    QMap<TT, TT> path;
    T dataContainer;

    void delete_left();
    void delete_right();

public:
    Tree();
    ~Tree();

    int get_border();
    void set_border(int);
    T get_data();
    void set_data(T);
    QMap<TT, TT> get_path();
    void set_path(QMap<TT, TT>);

    void insert_left(int, T, QMap<TT, TT>);
    void insert_right(int, T, QMap<TT, TT>);
    void delete_tree();

    QPair<int, Tree<T, TT>*> find_min();

};

template <typename T, typename TT>
Tree<T, TT>::Tree() {
    left = right = parent = nullptr;
    lowerBorder = 0;
    dataContainer = {{}};
    path = {};
}

template <typename T, typename TT>
Tree<T, TT>::~Tree() {
    delete_tree();
    delete this;
}

template <typename T, typename TT>
int Tree<T, TT>::get_border() {
    return lowerBorder;
}

template <typename T, typename TT>
void Tree<T, TT>::set_border(int border) {
    lowerBorder = border;
}

template <typename T, typename TT>
T Tree<T, TT>::get_data() {
    return dataContainer;
}

template <typename T, typename TT>
void Tree<T, TT>::set_data(T data) {
    T tmp(data);
    dataContainer.swap(tmp);
}

template <typename T, typename TT>
QMap<TT, TT> Tree<T, TT>::get_path()
{
    return path;
}

template <typename T, typename TT>
void Tree<T, TT>::set_path(QMap<TT, TT> newpath)
{
    if (newpath.size()!=0)
        path = newpath;
}

template <typename T, typename TT>
void Tree<T, TT>::delete_left() {
    if (left != nullptr) {
        left->delete_left();
        left->delete_right();
        delete left;
    }
}

template <typename T, typename TT>
void Tree<T, TT>::delete_right() {
    if (right != nullptr) {
        right->delete_left();
        right->delete_right();
        delete right;
    }
}

template <typename T, typename TT>
void Tree<T, TT>::delete_tree() {
    this->delete_left();
    this->delete_right();
    delete this;
}

template <typename T, typename TT>
void Tree<T, TT>::insert_left(int border, T data, QMap<TT, TT> newpath) {
    Tree<T, TT>* new_node = new Tree();
    new_node->set_data(data);
    new_node->set_border(border);
    new_node->set_path(newpath);

    if (this->left != nullptr) {
        this->left->parent = new_node;
        new_node->left = this->left;
    }
    this->left = new_node;
    new_node->parent = this;
}

template <typename T, typename TT>
void Tree<T, TT>::insert_right(int border, T data, QMap<TT, TT> newpath) {
    Tree<T, TT>* new_node = new Tree();
    new_node->set_data(data);
    new_node->set_border(border);
    new_node->set_path(newpath);

    if (this->right != nullptr) {
        this->right->parent = new_node;
        new_node->right = this->right;
    }
    this->right = new_node;
    new_node->parent = this;
}

template <typename T, typename TT>
QPair<int, Tree<T, TT>*> Tree<T, TT>::find_min()
{
    if (this->left == nullptr && this->right == nullptr)
    {
        return QPair<int, Tree<T, TT>*>(lowerBorder, this);
    }
    else
    {
        QPair<int, Tree<T, TT>*> left = this->left->find_min();
        QPair<int, Tree<T, TT>*> right = this->right->find_min();
        if (left.first < right.first)
            return left;
        else
            return right;
    }
}

#endif // TREE_H
