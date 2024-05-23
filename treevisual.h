#ifndef TREEVISUAL_H
#define TREEVISUAL_H

#include <QGraphicsView>
#include <QGraphicsItem>

#include "graphwidget.h"

class InfoBlock;
class ConnectLine;
class TreeVisual;

class ConnectLine : public QGraphicsItem
{
public:
    ConnectLine(QPointF sourcePoint, QPointF destPoint);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

class InfoBlock : public QGraphicsItem
{
public:
    InfoBlock(TreeVisual *tree, QString Npath, QString Nborder);

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QString path;
    QString border;

    TreeVisual *tree;
};

class TreeVisual : public QGraphicsView
{
    Q_OBJECT

public:
    TreeVisual(QWidget *parent = 0, Tree<QVector<QVector<ElementOfMatrix>>, QString>* solvationTree = 0, Graph<QString>* mathgraph = new Graph<QString>);

private:
    //int timerId;
    InfoBlock *root;
};

#endif // TREEVISUAL_H
