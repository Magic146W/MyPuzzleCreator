#ifndef DRAGGABLEPIXMAPITEM_H
#define DRAGGABLEPIXMAPITEM_H

#include <QObject>
#include <QLabel>

class DraggablePixmapItem : public QLabel
{
    Q_OBJECT
public:
    explicit DraggablePixmapItem(const QPixmap& pixmap, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

signals:
};

#endif // DRAGGABLEPIXMAPITEM_H
