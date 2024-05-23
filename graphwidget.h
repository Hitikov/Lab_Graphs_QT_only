#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QListWidgetItem>
#include <QGraphicsView>
#include "Graph.h"

class Node;
class Edge;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget* parent = nullptr);
    void itemMoved();
public slots:

    void AddNode();
    void DeleteNodeSetup();
    void SetEdgeSetup1();
    void SetEdgeSetup2();
    void DeleteEdgeSetup();
    void ShortestPathSetup1();
    void ShortestPathSetup2();
    void ShortestPath();
    void KomiTask();
    void GetLastNodeClicked(Node* argv);
    void GetLastEdgeClicked(Edge* argv);

signals:

    void SetNewCommentAct(QString comment);
    void SetNewCommentPath(QString comment);
    void SetNewPathList(QString path);
    void SetPathListClear();
    void BlockUI();
    void UnBlockUI();

protected:
    void timerEvent(QTimerEvent *event) override;

    void DeleteNode();

    void DeleteEdge(Edge*);

    void SetEdge();

    void FindEdge(Node* source, Node* target);

private:
    int timerId;
    QVector<Node*> vectNodes;
    Graph<QString> mathGraph;

    Node* lastNodeSelected;
    Node* lastNodeSelected2;
    Edge* lastEdgeSelected;

    int Mode;

    QWidget *parentRecord;
};

#endif // GRAPHWIDGET_H
