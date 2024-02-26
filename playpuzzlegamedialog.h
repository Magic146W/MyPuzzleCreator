#ifndef PLAYPUZZLEGAMEDIALOG_H
#define PLAYPUZZLEGAMEDIALOG_H

#include "imageholderwidget.h"
#include <QDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QListView>

namespace Ui {
class PlayPuzzleGameDialog;
}

class PlayPuzzleGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayPuzzleGameDialog(int rows, int columns, QImage& image, QSize &biggestShape, QWidget *parent = nullptr);
    ~PlayPuzzleGameDialog();

public slots:
    void resizeDialog();
    void handleDropEvent(QLabel* newImage, QString fileName, QPoint dropPos);
    void deleteLabelWithName(QString name);

private:
    Ui::PlayPuzzleGameDialog *ui;

    ImageHolderWidget *imageHolderWidget;
    QScrollArea *scrollArea;

    int rows;
    int columns;
    int width;
    int height;
    QSize biggestShape;

signals:
    void deleteShapeFromPool(QString fileName);

};

#endif // PLAYPUZZLEGAMEDIALOG_H
