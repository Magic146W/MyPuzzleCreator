#include "imageholderwidget.h"
#include <QPainter>
#include <QStandardItem>
#include <QListView>

/**
 * @class ImageHolderWidget
 * @brief The ImageHolderWidget class provides a widget for holding and handling drag-and-drop operations for images (puzzle shapes).
 *
 * This class allows users to drag and drop images onto the widget and handles the positioning of the dropped images.
 */


ImageHolderWidget::ImageHolderWidget(QSize& biggestShape, QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);
    setAutoFillBackground(true);
    shapeSize = biggestShape;
}

void ImageHolderWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QLabel *label = qobject_cast<QLabel*>(childAt(event->pos()));
        if (label)
        {
            offset = event->pos() - label->pos();

            myLabel = label;

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            QPixmap pixmap = label->pixmap(Qt::ReturnByValue);
            QString labelName = label->property("itemName").toString();
            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream << pixmap << labelName;

            mimeData->setData("application/x-custom-item-data", itemData);
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
        }
    }
}

void ImageHolderWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}


void ImageHolderWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-custom-item-data"))
    {
        QPoint newPos = event->position().toPoint() - offset;
        myLabel->move(newPos);
    }
    event->accept();
}

void ImageHolderWidget::dragLeaveEvent(QDragLeaveEvent *event)
{}

void ImageHolderWidget::dropEvent(QDropEvent *event)
{
    QPoint dropPos = event->position().toPoint();

    if (event->mimeData()->hasFormat("application/x-custom-listView-data"))
    {
        QByteArray itemData = event->mimeData()->data("application/x-custom-listView-data");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QString labelName;
        QPoint offset;
        dataStream >> pixmap >> labelName >> offset;

        if (!pixmap.isNull())
        {
            QLabel *label = new QLabel(this);
            label->setPixmap(pixmap);

            emit handleDropEvent(label, labelName, dropPos-offset);
        }
    }else if(event->mimeData()->hasFormat("application/x-custom-item-data"))
    {
        QByteArray itemData = event->mimeData()->data("application/x-custom-item-data");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QString labelName;
        dataStream >> pixmap >> labelName;

        foreach (QObject *child, children())
        {
            QLabel *existingLabel = qobject_cast<QLabel*>(child);
            if (existingLabel && existingLabel->property("itemName").toString() == labelName)
            {
                existingLabel->move(dropPos - offset);
                event->acceptProposedAction();
                this->update();
                return;
            }
        }

        QLabel *label = new QLabel(this);
        label->setPixmap(pixmap);

        emit handleDropEvent(label, labelName, dropPos);
    }    

    event->acceptProposedAction();
}
