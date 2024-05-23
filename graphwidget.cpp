#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "WorkMode.h"
#include "treevisual.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QInputDialog>
#include <QLineEdit>

//Инициализация виджета графического отоброжения графа
GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, parent->width(), parent->height());
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setMinimumSize(594, 471);

    Mode = WorkMode::DEFAULT;
}

//Регистрация информации о том, что объект был перемещен
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(100 / 25);
}

//Выполнение действий по истечению таймера событий
void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    //Получение списков вершин и граней
    QList<Node *> nodes;
    QList<Edge *> edges;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
            edges << edge;
    }

    //Проверка перемещения вершин
    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advancePosition())
            itemsMoved = true;
    }

    //Проверка нажатия вершин
    foreach (Node *node, nodes) {
        if (node->IWasClicked())
        {
            GetLastNodeClicked(node);
            node->SetWasClicked(false);
        }
    }

    //Проверка нажатия граней
    foreach (Edge *edge, edges) {
        if (edge->IWasClicked())
        {
            GetLastEdgeClicked(edge);
            edge->SetWasClicked(false);
        }
    }

    //Выполнение действи соответствующих режимов работы при соблюдении условий функционирования
    if (Mode == WorkMode::NODEDELITION && lastNodeSelected != nullptr)
    {
        DeleteNode();
    }
    if (Mode == WorkMode::EDGEDELITION && lastEdgeSelected != nullptr)
    {
        DeleteEdge(lastEdgeSelected);
    }
    if (Mode == WorkMode::EDGEADDING1 && lastNodeSelected != nullptr)
    {
        SetEdgeSetup2();
        itemsMoved = true;
    }
    if (Mode == WorkMode::EDGEADDING2 && lastNodeSelected2 != nullptr)
    {
        SetEdge();
        itemsMoved = true;
    }
    if (Mode == WorkMode::SHORTESTFIND1 && lastNodeSelected != nullptr)
    {
        ShortestPathSetup2();
        itemsMoved = true;
    }
    if (Mode == WorkMode::SHORTESTFIND2 && lastNodeSelected2 != nullptr)
    {
        ShortestPath();
        itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

//Добавление новой вершины
void GraphWidget::AddNode()
{
    bool ok;

    //Ввод названия новой вершины
    QString text = QInputDialog::getText(this, tr("Ввод названия вершины"),
                            tr("Название вершины:"), QLineEdit::Normal,
                            "", &ok);
    //Проверка корректности ввода
    if (ok && !text.isEmpty())
    {
        //Создание новой графической вершины
        Node *node = new Node(this, text);

        //Добавление вершины на сцену
        scene()->addItem(node);
        node->setPos(0, 0);

        //Добавление вершины в математический граф и вектор вершин
        vectNodes.append(node);
        mathGraph.AddVertex(text);
        emit SetNewCommentAct("Вершина добавлена");
    }
    else if (ok && text.isEmpty())
        emit SetNewCommentAct("Название не может быть пустым");
    else
        emit SetNewCommentAct("Отмена создания вершины");

    itemMoved();
}

//Подготовка к удалению вершины
void GraphWidget::DeleteNodeSetup()
{
    //Переключение в режим удаления и отчистка информации о последней нажатой вершине
    Mode = WorkMode::NODEDELITION;
    lastNodeSelected = nullptr;
    emit SetNewCommentAct("Выберете вершину для удаления");
}

//Удаление вершины
void GraphWidget::DeleteNode()
{ 
    //Переключение в стандартный режим работы
    Mode = WorkMode::DEFAULT;

    //Удаление прилегающих граней
    foreach (Edge *edge, lastNodeSelected->edges()) {
        DeleteEdge(edge);
    }

    //Отчистка математического графа, вектора вершин и сцены
    mathGraph.RemoveVertex(lastNodeSelected->value());
    vectNodes.remove(vectNodes.indexOf(lastNodeSelected));
    scene()->removeItem(lastNodeSelected);
    delete lastNodeSelected;

    emit SetNewCommentAct("Вершина удалена");
}

//Стадия 1 подготовки к добавлению грани
void GraphWidget::SetEdgeSetup1()
{
    //Переключение в режим стадии 1 добавления грани и отчистка информации о последней нажатой вершине
    Mode = WorkMode::EDGEADDING1;
    lastNodeSelected = nullptr;
    emit SetNewCommentAct("Выберете вершину отправления");
}

//Стадия 2 подготовки к добавлению грани
void GraphWidget::SetEdgeSetup2()
{
    //Переключение в режим стадии 2 добавления грани и отчистка информации о дополнительной последней нажатой вершине
    Mode = WorkMode::EDGEADDING2;
    lastNodeSelected2 = nullptr;
    emit SetNewCommentAct("Выберете вершину прибытия");
}

//Добавление грани
void GraphWidget::SetEdge()
{
    bool ok;
    //Ввод значения веса новой вершины
    int addValue = QInputDialog::getInt(this, tr("Ввод пути"),
                          tr("Введите значение пути (целое):"), 0, 0, 10000, 1, &ok);
    if (ok )
    {
        if (addValue == 0) addValue = rand() % 100;
        //Поиск грани с такими же вершиной отправления и вершиной прибытия
        foreach(Edge* edge1, lastNodeSelected->edges())
        {
            //Удаление грани с такими же вершиной отправления и вершиной прибытияпри нахождении
            if (edge1->sourceNode() == lastNodeSelected && edge1->targetNode() == lastNodeSelected2)
                DeleteEdge(edge1);
        }

        //Создание новой графической грани
        Edge* edge = new Edge(lastNodeSelected, lastNodeSelected2, addValue);
        //Добавление вершины на сцену
        scene()->addItem(edge);
        //Обновление информации о значении веса грани математического графа
        mathGraph.SetEdge(lastNodeSelected->value(), lastNodeSelected2->value(), addValue);

        emit SetNewCommentAct("Ребро создано");
    }
    else
        emit SetNewCommentAct("Отмена создания ребра");
    Mode = WorkMode::DEFAULT;
}

//Подготовка к удалению грани
void GraphWidget::DeleteEdgeSetup()
{
    //Переключение в режим удаления и отчистка информации о последней нажатой грани
    Mode = WorkMode::EDGEDELITION;
    lastEdgeSelected = nullptr;
    emit SetNewCommentAct("Выберете грань для удаления");
}

//Удаление грани
void GraphWidget::DeleteEdge(Edge* argv)
{
    //Переключение в стандартный режим работы
    Mode = WorkMode::DEFAULT;

    //Отчистка математического графа
    mathGraph.RemoveEdge(argv->sourceNode()->value(), argv->targetNode()->value());

    //Удаление информации о грании из связанных вершин
    argv->sourceNode()->deleteEdge(argv);
    argv->targetNode()->deleteEdge(argv);

    //Отчистка сцены
    scene()->removeItem(argv);
    delete argv;

    emit SetNewCommentAct("Грань удалена");
}

//Стадия 1 подготовки к нахождению кратчайшего пути между вершинами
void GraphWidget::ShortestPathSetup1()
{
    //Переключение в режим стадии 1 поиска пути и отчистка информации о последней нажатой вершине
    Mode = WorkMode::SHORTESTFIND1;
    lastNodeSelected = nullptr;
    emit SetNewCommentAct("Выберете вершину отправления");
}

//Стадия 2 подготовки к нахождению кратчайшего пути между вершинами
void GraphWidget::ShortestPathSetup2()
{
    //Переключение в режим стадии 2 поиска пути и отчистка информации о дополнительной последней нажатой вершине
    Mode = WorkMode::SHORTESTFIND2;
    lastNodeSelected2 = nullptr;
    emit SetNewCommentAct("Выберете вершину прибытия");
}

//Нахождение кратчайшего пути между двумя вершинами
void GraphWidget::ShortestPath()
{
    //Переключение в стандартный режим работы
    Mode = WorkMode::DEFAULT;

    emit SetPathListClear();
    //Определение названий вершин отправления и вершины приыбтия
    QString source = lastNodeSelected->value();
    QString target = lastNodeSelected2->value();

    //Получение кратчайшего пути между вершинами при помощи математического графа
    QVector<QString> path = mathGraph.FindShortestPath(source, target);

    //Вывод пути, если он был найден, в поле путей
    if (path.size()!=0){
        emit SetNewCommentPath("Путь решения:");
        QString start;
        QString next;
        QString showValue;
        int total = 0;

        for (int i = 0; i < path.size() - 1; i++)
        {
            start = path[i];
            next = path[i+1];
            total += mathGraph.GetWeight(start, next);
            showValue = start + "->" + next + " (" + QString::number(mathGraph.GetWeight(start, next)) + ")" ;
            emit SetNewPathList(showValue);
        }

        emit SetNewPathList(QString::number(total));
    }
    else
        emit SetNewCommentAct("Невозможно решить задачу при текущих условиях");

}

void GraphWidget::KomiTask()
{
    if (Mode == WorkMode::KOMMIBLOCK){
        emit SetPathListClear();
        emit UnBlockUI();
        Mode = WorkMode::DEFAULT;

        QList<Edge *> edges;
        foreach (QGraphicsItem *item, scene()->items()) {
            if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
                edge->SetNotSuit(false);
        }

        scene()->update(-400, -250, 800, 500);

        return;
    }

    //Получение решения задачи коммивояжера
    QMap <QString, QString> path = mathGraph.KommivoyagerTask().first;

    Tree<QVector<QVector<ElementOfMatrix>>, QString>* solvationTree = mathGraph.KommivoyagerTask().second;

    //Вывод пути, если он был найден, в поле путей
    if (path.size()!=0){

        //Переключение в режим работы блокировки действий
        Mode = WorkMode::KOMMIBLOCK;
        emit BlockUI();
        emit SetPathListClear();

        emit SetNewCommentPath("Путь решения:");
        QString start = mathGraph.GetVertexList().first();
        QString next;
        QString showValue;
        int total = 0;

        do
        {
            next = path[start];
            total += mathGraph.GetWeight(start, next);
            showValue = start + "->" + next + " (" + QString::number(mathGraph.GetWeight(start, next)) + ")" ;
            emit SetNewPathList(showValue);
            start = next;
        } while (start != mathGraph.GetVertexList().first());

        emit SetNewPathList(QString::number(total));

        QList<Edge *> edges;
        foreach (QGraphicsItem *item, scene()->items()) {
            if (Edge *edge = qgraphicsitem_cast<Edge *>(item)){
                if (edge->targetNode()->value() != path[edge->sourceNode()->value()])
                {
                    edge->SetNotSuit(true);
                }
            }
        }
        scene()->update(-400, -250, 800, 500);

        TreeVisual *widget = new TreeVisual(0, solvationTree, &mathGraph);
        widget->show();

    }
    else
        emit SetNewCommentAct("Невозможно решить задачу при текущих условиях");

}

//Сохранение информации о последней нажатой вершине
void GraphWidget::GetLastNodeClicked(Node* argv)
{
    if (!(Mode == WorkMode::EDGEADDING2 || Mode == WorkMode::SHORTESTFIND2))
        lastNodeSelected = argv;
    if (Mode == WorkMode::EDGEADDING2 || Mode == WorkMode::SHORTESTFIND2)
        lastNodeSelected2 = argv;
}

//Сохранение информации о последней нажатой грани
void GraphWidget::GetLastEdgeClicked(Edge* argv)
{
    lastEdgeSelected = argv;
}

