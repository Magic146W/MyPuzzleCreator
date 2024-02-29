#include "playpuzzlesshapes.h"
#include "customlistview.h"
#include "itemhidenamedelegate.h"
#include "ui_playpuzzlesshapes.h"
#include <QScrollArea>
#include <QListView>
#include <QDropEvent>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QMimeData>

PlayPuzzlesShapes::PlayPuzzlesShapes(QStandardItemModel *model, const QSize &biggestShape, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlayPuzzlesShapes)
    , scrollView(new QScrollArea(this))
    , listView(new CustomListView())
    , maxShapeNumber(model->rowCount())
{
    ui->setupUi(this);

    listView->setViewMode(QListView::IconMode);
    listView->setIconSize(biggestShape);
    listView->setModel(nullptr);
    listView->setModel(prepareItemsForView(model));

    listView->setDragEnabled(true);
    listView->setAcceptDrops(true);//
    listView->setDropIndicatorShown(true);//
    listView->setDragDropMode(QAbstractItemView::DragDrop);//
    listView->setDragDropOverwriteMode(true);//

    QPalette palette;
    palette.setColor(QPalette::Base, Qt::lightGray);
    listView->setPalette(palette);

    scrollView->setWidget(listView);
    scrollView->setWidgetResizable(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(scrollView);
    setLayout(layout);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int desiredWidth = screenGeometry.width() * 1 / 2;
    int desiredHeight = screenGeometry.height() * 0.9;
    resize(desiredWidth, desiredHeight);
    this->move(screenGeometry.width() - this->width(), 0);

    connect(listView, &CustomListView::itemDragEntered, this, &PlayPuzzlesShapes::handleItemDragEntered);
    connect(listView, &CustomListView::itemDropped, this, &PlayPuzzlesShapes::handleItemDropped);
}

PlayPuzzlesShapes::~PlayPuzzlesShapes()
{
    delete ui;
}

void PlayPuzzlesShapes::deleteItemWithName(QString name)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(listView->model());
    if (!model)
    {
        return;
    }

    QString nameWithoutSpaces = name.simplified();

    for (int row = 0; row < model->rowCount(); ++row)
    {
        QModelIndex index = model->index(row, 0);
        if (index.isValid())
        {
            QStandardItem *item = model->itemFromIndex(index);
            if (item)
            {
                QString itemName = item->text().simplified();
                if (itemName.compare(nameWithoutSpaces, Qt::CaseInsensitive) == 0)
                {
                    model->removeRow(row);
                    return;
                }
            }
        }
    }
}

void PlayPuzzlesShapes::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    updateListViewItems();
}

void PlayPuzzlesShapes::updateListViewItems()
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(listView->model());
    if (model)
    {
        listView->setModel(nullptr);
        listView->setModel(model);
    }
}

QStandardItemModel *PlayPuzzlesShapes::prepareItemsForView(QStandardItemModel *model)
{
    if (!model)
    {
        return nullptr;
    }

    QStandardItemModel *unsortedModel = new QStandardItemModel(this);
    QList<QStandardItem *> items;

    for (int i = 0; i < model->rowCount(); ++i)
    {
        for (int j = 0; j < model->columnCount(); ++j)
        {
            QStandardItem *item = model->item(i, j)->clone();
            item->setText(" " + item->text());
            items.append(item);
        }
    }

    QRandomGenerator generator = QRandomGenerator::securelySeeded();
    for (int i = items.size() - 1; i > 0; --i)
    {
        int randomIndex = generator.bounded(i + 1);
        items.swapItemsAt(i, randomIndex);
    }

    for (auto item : items)
    {
        unsortedModel->appendRow(item);
    }

    ItemHideNameDelegate *delegate = new ItemHideNameDelegate(this);
    delegate->displayRoleEnabled = false;
    listView->setItemDelegate(delegate);

    return unsortedModel;
}

void PlayPuzzlesShapes::handleItemDragEntered(QDragEnterEvent *event)
{}

void PlayPuzzlesShapes::handleItemDropped(QDropEvent *event)
{
    QPoint dropPos = event->position().toPoint();
    QModelIndex dropIndex = listView->indexAt(dropPos);

    QStandardItem* item = nullptr;
    if(event->mimeData()->hasFormat("application/x-custom-listView-data"))
    {
        QByteArray itemData = event->mimeData()->data("application/x-custom-listView-data");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QString labelName;
        QPoint offset;
        dataStream >> pixmap >> labelName >> offset;
        item = new QStandardItem(QIcon(pixmap), labelName);
    }else if(event->mimeData()->hasFormat("application/x-custom-item-data"))
    {
        QByteArray itemData = event->mimeData()->data("application/x-custom-item-data");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QString labelName;
        dataStream >> pixmap >> labelName;
        item = new QStandardItem(QIcon(pixmap), labelName);

        emit dropEventReceived(labelName);
    } else
    {
        return;
    }


    if (item)
    {
        QStandardItemModel *targetModel = qobject_cast<QStandardItemModel*>(listView->model());
        if (targetModel)
        {
            QString droppedItemText = item->text();

            for (int row = 0; row < targetModel->rowCount(); ++row)
            {
                for (int column = 0; column < targetModel->columnCount(); ++column)
                {
                    QModelIndex index = targetModel->index(row, column);
                    if (index.isValid())
                    {
                        QStandardItem* existingItem = targetModel->itemFromIndex(index);
                        if (existingItem && existingItem->text() == droppedItemText)
                        {
                            if (!dropIndex.isValid())
                            {
                                targetModel->removeRow(index.row(), index.parent());
                                targetModel->appendRow(item);
                            }else
                            {
                                targetModel->removeRow(index.row(), index.parent());
                                targetModel->insertRow(dropIndex.row(), item);
                            }
                            return;
                        }
                    }
                }
            }

            if(targetModel->rowCount()<maxShapeNumber)
            {
                if (!dropIndex.isValid())
                {
                    targetModel->appendRow(item);
                }else
                {
                    targetModel->insertRow(dropIndex.row(), item);
                }
            }
        }
    }
}
