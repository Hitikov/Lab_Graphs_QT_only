#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget, QString newValue);

    QString value() { return showValue; }

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    void deleteEdge(Edge *edge);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    bool advancePosition();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    GraphWidget * getGraph() {return graph;}

    bool IWasClicked();
    void SetWasClicked(bool);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;

    QString showValue;

    bool isClicked;
};

#endif // NODE_H
