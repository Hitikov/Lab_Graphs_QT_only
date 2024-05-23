#ifndef TREE_H
#define TREE_H

#include <QPair>
#include <QMap>

//Класс дерева решения задачи коммивояжера (бинарное дерево)
template <typename T, typename TT>
class Tree
{
private:
    Tree<T, TT>* left = nullptr; //Левая ветвь, ветвь исключения пути
    Tree<T, TT>* right = nullptr; //Правая ветвь, ветвь включения пути
    Tree<T, TT>* parent = nullptr; //Предок
    int lowerBorder; //Текущая нижняя граница
    QMap<TT, TT> path; //Текущий пройденый путь
    T dataContainer; //Текущая матрица смежнотси, редуцированная
    QString lastPath;

    void delete_left();
    void delete_right();

public:
    Tree();
    ~Tree();

    Tree<T, TT>* get_left() {return left;}
    Tree<T, TT>* get_right() {return right;}
    Tree<T, TT>* get_parent() {return parent;}

    int get_border();
    void set_border(int);
    T get_data();
    void set_data(T);
    QMap<TT, TT> get_path();
    void set_path(QMap<TT, TT>);
    QString get_lastPath();
    void set_lastPath(QString);

    void insert_left(int, T, QMap<TT, TT>, QString);
    void insert_right(int, T, QMap<TT, TT>, QString);
    void delete_tree();

    bool haveParent();

    QPair<int, Tree<T, TT>*> find_min();

};

template <typename T, typename TT>
bool Tree<T, TT>::haveParent()
{
    if (parent!=nullptr) return true;
    else return false;
}

//Инициализация пустого дерева
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

//Получение значения нижней границы
template <typename T, typename TT>
int Tree<T, TT>::get_border() {
    return lowerBorder;
}

//Установка значения нижней границы
template <typename T, typename TT>
void Tree<T, TT>::set_border(int border) {
    lowerBorder = border;
}

//Получение значения матрицы смежности
template <typename T, typename TT>
T Tree<T, TT>::get_data() {
    return dataContainer;
}

//Установка значения матрицы смежности
template <typename T, typename TT>
void Tree<T, TT>::set_data(T data) {
    T tmp(data);
    dataContainer.swap(tmp);
}

//Получение значения пройденого пути
template <typename T, typename TT>
QMap<TT, TT> Tree<T, TT>::get_path()
{
    return path;
}

//Установка значения пройденого пути
template <typename T, typename TT>
void Tree<T, TT>::set_path(QMap<TT, TT> newpath)
{
    if (newpath.size()!=0)
        path = newpath;
}

template <typename T, typename TT>
QString Tree<T, TT>::get_lastPath()
{
    return lastPath;
}

template <typename T, typename TT>
void Tree<T, TT>::set_lastPath(QString newpath)
{
    lastPath = newpath;
}

//Удаление левого поддерева (для деструктора)
template <typename T, typename TT>
void Tree<T, TT>::delete_left() {
    if (left != nullptr) {
        left->delete_left();
        left->delete_right();
        delete left;
    }
}

//Удаление праволевого поддерева (для деструктора)
template <typename T, typename TT>
void Tree<T, TT>::delete_right() {
    if (right != nullptr) {
        right->delete_left();
        right->delete_right();
        delete right;
    }
}

//Удаление дерева (для деструктора)
template <typename T, typename TT>
void Tree<T, TT>::delete_tree() {
    this->delete_left();
    this->delete_right();
    delete this;
}

//Вставка левой ветви с заданными параметрами
template <typename T, typename TT>
void Tree<T, TT>::insert_left(int border, T data, QMap<TT, TT> newpath, QString lastPath) {
    Tree<T, TT>* new_node = new Tree();
    new_node->set_data(data);
    new_node->set_border(border);
    new_node->set_path(newpath);
    new_node->set_lastPath(lastPath);

    if (this->left != nullptr) {
        this->left->parent = new_node;
        new_node->left = this->left;
    }
    this->left = new_node;
    new_node->parent = this;
}

//Вставка правой ветви с заданными параметрами
template <typename T, typename TT>
void Tree<T, TT>::insert_right(int border, T data, QMap<TT, TT> newpath, QString lastPath) {
    Tree<T, TT>* new_node = new Tree();
    new_node->set_data(data);
    new_node->set_border(border);
    new_node->set_path(newpath);
    new_node->set_lastPath(lastPath);

    if (this->right != nullptr) {
        this->right->parent = new_node;
        new_node->right = this->right;
    }
    this->right = new_node;
    new_node->parent = this;
}

//Нахождение минимальной нижней границы
//Поиск производится в два этапа:
//1. Нахождение не ветвившихся вершин
//2. Сравнение значений их нижних границ и возврат наименьшего значения
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
