#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "WorkMode.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QInputDialog>
#include <QLineEdit>

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
    //setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(657, 543);

    Mode = WorkMode::DEFAULT;
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(100 / 25);
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    QList<Edge *> edges;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
            edges << edge;
    }

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advancePosition())
            itemsMoved = true;
    }

    foreach (Node *node, nodes) {
        if (node->IWasClicked())
        {
            GetLastNodeClicked(node);
            node->SetWasClicked(false);
        }
    }

    foreach (Edge *edge, edges) {
        if (edge->IWasClicked())
        {
            GetLastEdgeClicked(edge);
            edge->SetWasClicked(false);
        }
    }

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

void GraphWidget::AddNode()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Ввод названия вершины"),
                            tr("Название вершины:"), QLineEdit::Normal,
                            "", &ok);
    if (ok && !text.isEmpty())
    {
        Node *node = new Node(this, text);

        scene()->addItem(node);
        node->setPos(0, 0);

        vectNodes.append(node);
        mathGraph.AddVertex(text);
        SetNewCommentAct("Вершина добавлена");
    }
    else if (ok && text.isEmpty())
        SetNewCommentAct("Название не может быть пустым");
    else
        SetNewCommentAct("Отмена создания вершины");
    itemMoved();
}

void GraphWidget::DeleteNodeSetup()
{
    Mode = WorkMode::NODEDELITION;
    lastNodeSelected = nullptr;
    SetNewCommentAct("Выберете вершину для удаления");
}

void GraphWidget::DeleteNode()
{ 
    Mode = WorkMode::DEFAULT;

    foreach (Edge *edge, lastNodeSelected->edges()) {
        DeleteEdge(edge);
    }

    mathGraph.RemoveVertex(lastNodeSelected->value());
    vectNodes.remove(vectNodes.indexOf(lastNodeSelected));
    scene()->removeItem(lastNodeSelected);
    delete lastNodeSelected;

    SetNewCommentAct("Вершина удалена");
}

void GraphWidget::SetEdgeSetup1()
{
    Mode = WorkMode::EDGEADDING1;
    lastNodeSelected = nullptr;
    SetNewCommentAct("Выберете вершину отправления");
}

void GraphWidget::SetEdgeSetup2()
{
    Mode = WorkMode::EDGEADDING2;
    lastNodeSelected2 = nullptr;
    SetNewCommentAct("Выберете вершину прибытия");
}

void GraphWidget::SetEdge()
{
    bool ok;
    int addValue = QInputDialog::getInt(this, tr("Ввод пути"),
                          tr("Введите значение пути (целое):"), 0, 0, 10000, 1, &ok);
    if (ok )
    {

        foreach(Edge* edge1, lastNodeSelected->edges())
        {
            if (edge1->sourceNode() == lastNodeSelected && edge1->targetNode() == lastNodeSelected2)
                DeleteEdge(edge1);
        }

        Edge* edge = new Edge(lastNodeSelected, lastNodeSelected2, addValue);
        scene()->addItem(edge);

        mathGraph.SetEdge(lastNodeSelected->value(), lastNodeSelected2->value(), addValue);

        SetNewCommentAct("Ребро создано");
    }
    else
        SetNewCommentAct("Отмена создания ребра");
    Mode = WorkMode::DEFAULT;
}

void GraphWidget::DeleteEdgeSetup()
{
    Mode = WorkMode::EDGEDELITION;
    lastEdgeSelected = nullptr;
    SetNewCommentAct("Выберете грань для удаления");
}

void GraphWidget::DeleteEdge(Edge* argv)
{
    Mode = WorkMode::DEFAULT;

    mathGraph.RemoveEdge(argv->sourceNode()->value(), argv->targetNode()->value());

    argv->sourceNode()->deleteEdge(argv);
    argv->targetNode()->deleteEdge(argv);

    scene()->removeItem(argv);
    delete argv;

    SetNewCommentAct("Грань удалена");
}

void GraphWidget::ShortestPathSetup1()
{
    Mode = WorkMode::SHORTESTFIND1;
    lastNodeSelected = nullptr;
    SetNewCommentAct("Выберете вершину отправления");
}

void GraphWidget::ShortestPathSetup2()
{
    Mode = WorkMode::SHORTESTFIND2;
    lastNodeSelected2 = nullptr;
    SetNewCommentAct("Выберете вершину прибытия");
}


void GraphWidget::ShortestPath()
{
    Mode = WorkMode::DEFAULT;

    SetPathListClear();
    QString source = lastNodeSelected->value();
    QString target = lastNodeSelected2->value();

    QVector<QString> path = mathGraph.FindShortestPath(source, target);

    if (path.size()!=0){
        SetNewCommentPath("Путь решения:");
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
            SetNewPathList(showValue);
        }

        SetNewPathList(QString::number(total));
    }
    else
        SetNewCommentAct("Невозможно решить задачу при текущих условиях");

}

void GraphWidget::KomiTask()
{
    Mode = WorkMode::DEFAULT;

    SetPathListClear();

    QMap <QString, QString> path = mathGraph.KommivoyagerTask();

    if (path.size()!=0){
        SetNewCommentPath("Путь решения:");
        QString start = mathGraph.GetVertexList().first();
        QString next;
        QString showValue;
        int total = 0;

        do
        {
            next = path[start];
            total += mathGraph.GetWeight(start, next);
            showValue = start + "->" + next + " (" + QString::number(mathGraph.GetWeight(start, next)) + ")" ;
            SetNewPathList(showValue);
            start = next;
        } while (start != mathGraph.GetVertexList().first());

        SetNewPathList(QString::number(total));
    }
    else
        SetNewCommentAct("Невозможно решить задачу при текущих условиях");

}

void GraphWidget::GetLastNodeClicked(Node* argv)
{
    if (!(Mode == WorkMode::EDGEADDING2 || Mode == WorkMode::SHORTESTFIND2))
        lastNodeSelected = argv;
    if (Mode == WorkMode::EDGEADDING2 || Mode == WorkMode::SHORTESTFIND2)
        lastNodeSelected2 = argv;
}

void GraphWidget::GetLastEdgeClicked(Edge* argv)
{
    lastEdgeSelected = argv;
}

