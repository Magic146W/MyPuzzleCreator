#include "customlistview.h"
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QStandardItem>
#include <QDrag>
#include <QCoreApplication>
#include <QMimeData>
#include <QDebug>


/**
 * @brief Subclass of QListView providing custom drag-and-drop functionality.
 */

/**
     * @brief Event handler for drag enter events.
     *
     * @param event The drag enter event.
     */
void CustomListView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-custom-item-data") || event->mimeData()->hasFormat("application/x-custom-listView-data"))
    {
        event->setAccepted(true);
        emit itemDragEntered(event);
    } else
    {
        event->ignore();
    }

    event->acceptProposedAction();
}

/**
     * @brief Event handler for drag move events.
     *
     * @param event The drag move event.
     */
void CustomListView::dragMoveEvent(QDragMoveEvent *event)
{
    event->setAccepted(true);
    event->acceptProposedAction();

    emit itemDragMoved(event);
}

/**
     * @brief Event handler for drop events.
     *
     * @param event The drop event.
     */
void CustomListView::dropEvent(QDropEvent *event)
{
    event->setAccepted(true);
    event->acceptProposedAction();

    emit itemDropped(event);
}

/**
 * @brief Event handler for mouse press events.
 *
 * Creates a drag object with custom mime data
 *
 * @param event The mouse press event.
 */
void CustomListView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QModelIndex index = indexAt(event->position().toPoint());
        if (index.isValid())
        {
            QStandardItem *item = qobject_cast<QStandardItemModel*>(this->model())->itemFromIndex(index);
            QPixmap pixmap = item->icon().pixmap(this->iconSize());
            QString name = item->text();
            QPoint offset = event->position().toPoint() - visualRect(index).topLeft();

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            QByteArray encodedData;
            QDataStream stream(&encodedData, QIODevice::WriteOnly);
            stream << pixmap << name <<  QPoint(offset);


            mimeData->setData("application/x-custom-listView-data", encodedData);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(offset);
            drag->exec(Qt::CopyAction);
        }
    }

    QListView::mousePressEvent(event);
}
