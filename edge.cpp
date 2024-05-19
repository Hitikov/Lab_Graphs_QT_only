#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include <qmath.h>
#include <QPainter>

//Инициализация графической грани графа
//Параметры - связываемые вершины и значение веса грани
Edge::Edge(Node *sourceNode, Node *targetNode, int newValue) : arrowSize(10)
{
    setAcceptedMouseButtons(Qt::MouseButtons(1));
    source = sourceNode;
    target = targetNode;
    showValue = newValue;
    source->addEdge(this);
    target->addEdge(this);
    adjust();
}

//Получение указателя на вершину отправления
Node *Edge::sourceNode() const
{
    return source;
}

//Получение указателя на вершину прибытия
Node *Edge::targetNode() const
{
    return target;
}

//Соединение двух вершин гранью
void Edge::adjust()
{
    if (!source || !target)
        return;

    QLineF line(mapFromItem(source, 20, 20), mapFromItem(target, 20, 20));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(40.)) {
        QPointF edgeOffset((line.dx() * 20) / length, (line.dy() * 20) / length);
        sourcePoint = line.p1() + edgeOffset;
        targetPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = targetPoint = line.p1();
    }
}

//Формирование ограничивающего прямоугольника
QRectF Edge::boundingRect() const
{
    if (!source || !target)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0 + 15;

    QPointF tmp = sourcePoint;

    tmp.setX(tmp.x() - 20);
    tmp.setY(tmp.y() - 20);

    return QRectF(sourcePoint, QSizeF(targetPoint.x() - sourcePoint.x() ,
                                      targetPoint.y() - sourcePoint.y() ))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

//Отрисовка грани
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !target)
        return;

    QLineF line(sourcePoint, targetPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    QPointF textPos = ((targetPoint + sourcePoint) / 2 + targetPoint)/2 + QPointF(0, 10);

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    QFont t = painter->font();
    t.setPointSize(12);
    painter->setFont(t);
    painter->drawText(textPos, QString::number(showValue));

    // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF targetArrowP1 = targetPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF targetArrowP2 = targetPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                             cos(angle - M_PI + M_PI / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2() << targetArrowP1 << targetArrowP2);
}

//Регистрация нажатия на грань
void Edge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    SetWasClicked(true);
    source->getGraph()->itemMoved();
    update();
    QGraphicsItem::mousePressEvent(event);
}

//Проверка нажатия на грань
bool Edge::IWasClicked()
{
    return isClicked;
}
//Установка значения переменной регистрации нажатия
void Edge::SetWasClicked(bool bl)
{
    isClicked = bl;
}

