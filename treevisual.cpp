#include "treevisual.h"

#include <qmath.h>
#include <QPainter>

int swiftY = 20;
int swiftX = 60;

void DrawElements(TreeVisual* vis, QGraphicsScene *scene, int height, int xpos, int ypos, Tree<QVector<QVector<ElementOfMatrix>>, QString>* tree)
{

    InfoBlock* newBlock = new InfoBlock(vis, tree->get_lastPath(), QString::number(tree->get_border()));
    newBlock->setPos(xpos, ypos);
    scene->addItem(newBlock);

    if (tree->get_right()!=nullptr)
    {
        ConnectLine* newLine = new ConnectLine(QPointF(xpos + swiftX * height, ypos + swiftY + 80 - 40), QPointF(xpos + 40, ypos));
        scene->addItem(newLine);
        DrawElements(vis, scene, height - 1, xpos + swiftX * height, ypos + swiftY + 80 , tree->get_right());
    }
    if (tree->get_left()!=nullptr)
    {
        ConnectLine* newLine = new ConnectLine( QPointF(xpos - swiftX * height, ypos + swiftY + 80 - 40),QPointF(xpos - 40, ypos));
        scene->addItem(newLine);
        DrawElements(vis, scene, height - 1, xpos - swiftX * height, ypos + swiftY + 80 , tree->get_left());
    }
}

TreeVisual::TreeVisual(QWidget *parent, Tree<QVector<QVector<ElementOfMatrix>>, QString>* solvationTree, Graph<QString>* mathgraph)
    : QGraphicsView(parent)
{
    int sizeofpath = mathgraph->GetVertexList().size();

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-(200 + 200 * pow(2,sizeofpath)) * 0.5, -(200 + 200 * sizeofpath) * 0.5, 200 + 200 * pow(2,sizeofpath), 200 + 200 * sizeofpath);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(400, 400);
    setMaximumSize(1200, 600);
    setWindowTitle(tr("Tree of solvation"));

    int highestY = -(sizeofpath * 80 + swiftY * (sizeofpath - 1)) * 0.5;

    DrawElements(this, scene, sizeofpath, 0, highestY, solvationTree );
}

ConnectLine::ConnectLine(QPointF NsourcePoint, QPointF NdestPoint)
    : arrowSize(10)
{
    sourcePoint = NsourcePoint;
    destPoint = NdestPoint;
}

QRectF ConnectLine::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void ConnectLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);

}

InfoBlock::InfoBlock(TreeVisual *graphWidget, QString Npath, QString Nborder)
    : tree(graphWidget)
{
    path = Npath;
    border = Nborder;
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

QRectF InfoBlock::boundingRect() const
{
    return QRectF( -40, -40, 80, 80);
}

QPainterPath InfoBlock::shape() const
{
    QPainterPath path;
    path.addRect( -40, -40, 80, 80);
    return path;
}

void InfoBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option)

    //painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::lightGray);
    painter->drawRoundRect( -40, -40, 80, 80);

    QFont t = painter->font();
    t.setPointSize(12);
    painter->setFont(t);

    painter->setBrush(Qt::black);
    painter->drawText(-20, -15, path);
    painter->drawText(-20, 5, "Border:");
    if(border.toInt() > 30000) painter->drawText(-20, 25, "∞");
    else painter->drawText(-20, 25, border);
}
