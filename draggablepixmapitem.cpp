#include "draggablepixmapitem.h"
#include <QDrag>
#include <QMimeData>
#include <QLabel>
#include <QMouseEvent>

DraggablePixmapItem::DraggablePixmapItem(const QPixmap& pixmap, QWidget *parent)
    : QLabel(parent)
{
    setPixmap(pixmap);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
}

void DraggablePixmapItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setImageData(pixmap());
        drag->setMimeData(mimeData);
        drag->exec(Qt::MoveAction);
    }
}

void DraggablePixmapItem::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasImage())
    {
        event->acceptProposedAction();
    }
}

void DraggablePixmapItem::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasImage())
    {
        setPixmap(qvariant_cast<QPixmap>(event->mimeData()->imageData()));
        event->acceptProposedAction();
    }
}
