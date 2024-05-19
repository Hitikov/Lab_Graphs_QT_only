#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

//Инициализация графической вершины
Node::Node(GraphWidget *graphWidget, QString newValue)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    showValue = newValue;
}

//Добавление грани
void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

//Получение списка указателей на связанные грани
QList<Edge *> Node::edges() const
{
    return edgeList;
}

//Регистрация позиции и обновление информации о позиции
bool Node::advancePosition()
{
    if (newPos == pos())
        return false;

    newPos = pos();
    return true;
}

//Формирование ограничивающего прямоугольника
QRectF Node::boundingRect() const
{
    return QRectF( 0, 0, 40, 40);
}

//Формирование формы вершины
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(0, 0, 40, 40);
    return path;
}

//Отрисовка вершины
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);

    if(isClicked) painter->setBrush(Qt::darkYellow);
    else painter->setBrush(Qt::yellow);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(0, 0, 40, 40);

    QFont t = painter->font();
    t.setPointSize(12);
    painter->setFont(t);

    painter->drawText(5, 26, showValue);
}

//Регистрация изменения состояния вершины и обновление граней
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

//Удаление информации о вершине
void Node::deleteEdge(Edge *edge)
{
    edgeList.removeOne(edge);
}

//Регистрация нажатия на вершину
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    SetWasClicked(true);
    if(!isUnderMouse()) SetWasClicked(false);
    update();
    QGraphicsItem::mousePressEvent(event);
    graph->itemMoved();
}

//Регистрация отжатия вершины
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isUnderMouse()) SetWasClicked(false);
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    graph->itemMoved();
}

//Проверка нажатия на грань
bool Node::IWasClicked()
{
    return isClicked;
}
//Установка значения переменной регистрации нажатия
void Node::SetWasClicked(bool bl)
{
    isClicked = bl;
}
