#ifndef PLAYPUZZLESSHAPES_H
#define PLAYPUZZLESSHAPES_H

#include "customlistview.h"
#include <QDialog>
#include <QScrollArea>
#include <QListView>
#include <QStandardItemModel>

namespace Ui {
class PlayPuzzlesShapes;
}

class PlayPuzzlesShapes : public QDialog
{
    Q_OBJECT

public:
    explicit PlayPuzzlesShapes(QStandardItemModel *model, const QSize &biggestShape ,QWidget *parent = nullptr);
    ~PlayPuzzlesShapes();

public slots:
    void deleteItemWithName(QString name);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action);

private:
    Ui::PlayPuzzlesShapes *ui;

    QScrollArea *scrollView;
    CustomListView *listView;
    int maxShapeNumber;

    void updateListViewItems();
    QStandardItemModel* prepareItemsForView(QStandardItemModel *model);

private slots:
    void handleItemDragEntered(QDragEnterEvent *event);
    void handleItemDropped(QDropEvent *event);

signals:
    void dropEventReceived(QString fileName);
    void deleteShapeFromPool(QString fileName);
};

#endif // PLAYPUZZLESSHAPES_H
