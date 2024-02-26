#ifndef IMAGEHOLDERWIDGET_H
#define IMAGEHOLDERWIDGET_H

#include <QWidget>
#include <QMimeData>
#include <QMouseEvent>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QLabel>

class ImageHolderWidget : public QWidget {
    Q_OBJECT

public:
    explicit ImageHolderWidget(QSize& biggestShape, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QSize shapeSize;
    QPoint offset;

    QLabel* myLabel;
    QPoint labelPickUpOffset;

signals:
    void handleDropEvent(QLabel* newImage, QString fileName, QPoint dropPos);
    void moveLabel(QLabel* moveImage, QPoint dropPos);
};

#endif // IMAGEHOLDERWIDGET_H
