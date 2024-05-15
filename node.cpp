#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(GraphWidget *graphWidget, QString newValue)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    showValue = newValue;
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

bool Node::advancePosition()
{
    if (newPos == pos())
        return false;

    newPos = pos();
    //setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    return QRectF( 0, 0, 40, 40);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(0, 0, 40, 40);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);

    painter->setBrush(Qt::yellow);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(0, 0, 40, 40);

    QFont t = painter->font();
    t.setPointSize(12);
    painter->setFont(t);

    painter->drawText(5, 26, showValue);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::deleteEdge(Edge *edge)
{
    edgeList.removeOne(edge);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    SetWasClicked(true);
    graph->itemMoved();
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

bool Node::IWasClicked()
{
    return isClicked;
}
void Node::SetWasClicked(bool bl)
{
    isClicked = bl;
}
