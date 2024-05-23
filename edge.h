#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
private:
    Node* source;
    Node* target;

    QPointF sourcePoint;
    QPointF targetPoint;
    qreal arrowSize;

    QPointF textPos;

    int showValue;

    bool isClicked;

    bool notSuit;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
public:
    Edge(Node* sourceNode, Node* targetNode, int newValue);

    Node* sourceNode() const;
    Node* targetNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

    bool IWasClicked();
    void SetWasClicked(bool);

    void SetNotSuit(bool);
};

#endif // EDGE_H
