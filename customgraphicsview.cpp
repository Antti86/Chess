#include "customgraphicsview.h"
#include "qgraphicssceneevent.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (scene())
    {
        QGraphicsSceneMouseEvent sceneEvent(QEvent::GraphicsSceneMousePress);
        sceneEvent.setScenePos(mapToScene(event->pos()));
        QCoreApplication::sendEvent(scene(), &sceneEvent);
    }
    QGraphicsView::mousePressEvent(event);
}
