#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H

#include <QListView>
#include <QObject>

class CustomListView : public QListView
{
    Q_OBJECT

signals:
    void itemDragEntered(QDragEnterEvent *event);
    void itemDragMoved(QDragMoveEvent *event);
    void itemDropped(QDropEvent *event);
    void dropData(const QMimeData *data, const QModelIndex &modelIndex) const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

};

#endif // CUSTOMLISTVIEW_H

