#include "playpuzzlegamedialog.h"
#include "imageholderwidget.h"
#include "ui_playpuzzlegamedialog.h"
#include <QSplitter>
#include <QListView>
#include <QStringListModel>
#include <QScreen>
#include <QPainter>
#include <QStackedWidget>
#include <QDrag>
#include <QMimeData>

/**
 * @brief The PlayPuzzleGameDialog class represents a dialog for playing with the puzzle shapes.
 *
 * This dialog allows users to play puzzle games by dragging and dropping image pieces.
 */


PlayPuzzleGameDialog::PlayPuzzleGameDialog(int rows, int columns, QImage& image, QSize &biggestShape, QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::PlayPuzzleGameDialog)
    , imageHolderWidget(new ImageHolderWidget(biggestShape,this))
    , scrollArea(new QScrollArea)
    , rows(rows)
    , columns(columns)
    , width(image.width())
    , height(image.height())
    , biggestShape(biggestShape)
{
    ui->setupUi(this);
    this->setMaximumSize(width + (width * 0.1), height + (height * 0.1));

    scrollArea->setWidget(imageHolderWidget);

    imageHolderWidget->setFixedSize(image.width(), image.height());
    imageHolderWidget->setPalette(QPalette(Qt::darkRed));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(scrollArea);
    setLayout(layout);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int desiredWidth = screenGeometry.width() * 1 / 2;
    int desiredHeight = screenGeometry.height() * 0.9;
    resize(desiredWidth, desiredHeight);
    this->move(0,0);

    connect(imageHolderWidget, &ImageHolderWidget::handleDropEvent, this, &PlayPuzzleGameDialog::handleDropEvent);
}

PlayPuzzleGameDialog::~PlayPuzzleGameDialog()
{
    delete ui;
}

/**
 * @brief Resizes the dialog.
 */
void PlayPuzzleGameDialog::resizeDialog()
{
    //planned function
}

/**
 * @brief Handles the drop event by adding a new image label to the image holder widget.
 *
 * @param newImage The new image label.
 * @param fileName The name of the file associated with the image.
 * @param dropPos The position where the image is dropped.
 */
void PlayPuzzleGameDialog::handleDropEvent(QLabel *newImage, QString fileName, QPoint dropPos)
{
    newImage->setParent(imageHolderWidget);
    newImage->move(dropPos);
    newImage->setProperty("itemName", QVariant::fromValue(fileName));
    newImage->show();

    emit deleteShapeFromPool(fileName);
}

/**
 * @brief Deletes the label with the specified name from the image holder widget.
 *
 * @param name The name of the label to delete.
 */
void PlayPuzzleGameDialog::deleteLabelWithName(QString name)
{
    foreach (QObject *child, imageHolderWidget->children())
    {
        QLabel *existingLabel = qobject_cast<QLabel*>(child);
        if (existingLabel && existingLabel->property("itemName").toString() == name)
        {
            existingLabel->setParent(nullptr);
            delete existingLabel;
            imageHolderWidget->update();

            return;
        }
    }
}
