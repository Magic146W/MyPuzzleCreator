#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "playpuzzlegamedialog.h"
#include "playpuzzlesshapes.h"
#include "puzzlesetupsettingsdialog.h"
#include "puzzleshapemanager.h"
#include <windows.h>
#include <QScreen>
#include <QRect>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageWriter>
#include <QImageReader>
#include <QMessageBox>
#include <QColorSpace>
#include <QStatusBar>
#include <QClipboard>
#include <QMimeData>
#include <QPoint>
#include <QSplitter>
#include <QListView>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QListWidgetItem>
#include <QPrintDialog>
#include <QPainter>

/**
 * @class MainWindow
 * @brief Represents the main window of the application.
 */


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , imageLabel(new QLabel)
    , scrollArea(new QScrollArea)
{
    ui->setupUi(this);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    setCentralWidget(splitter);

    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    listView = new QListView;
    listView->setModel(new QStringListModel);
    listView->setDragEnabled(false);

    splitter->addWidget(scrollArea);
    splitter->addWidget(listView);

    QList<int> sizes;
    sizes << width() * 0.75 << width() * 0.25;
    splitter->setSizes(sizes);

    connect(splitter, &QSplitter::splitterMoved, this, &MainWindow::updateListViewItems);
    createActions();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int height = screenGeometry.height() * 3 / 5;
    int width = screenGeometry.width() * 3 / 5;
    this->resize(width, height);
    openHelpImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Handles resizing events for the main window.
 *
 * @param event The resize event.
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateListViewItems();
}

/**
 * @brief Updates the items in the list view.
 */
void MainWindow::updateListViewItems()
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(listView->model());
    if (model)
    {
        listView->setModel(nullptr);
        listView->setModel(model);
    }
}

/**
 * @brief Initializes the image file dialog.
 *
 * @param dialog The file dialog to initialize.
 * @param acceptMode The accept mode of the file dialog (AcceptOpen or AcceptSave).
 */
static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
                                                  ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

/**
 * @brief Opens a file dialog to select an image file.
 */
void MainWindow::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}

/**
 * @brief Loads an image file.
 *
 * @param fileName The name of the image file to load.
 * @return True if the file is successfully loaded; otherwise, false.
 */
bool MainWindow::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(newImage);
    return true;
}

/**
 * @brief Sets the main image.
 *
 * @param newImage The new image to set.
 */
void MainWindow::setImage(const QImage &newImage)
{
    image = newImage;
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    imageLabel->setPixmap(QPixmap::fromImage(image));

    scaleFactor = 1.0;

    scaleImage(scaleFactor);
    scrollArea->setVisible(true);
    printAction->setEnabled(true);
    prepareAction->setEnabled(true);
    updateActions();
}

/**
 * @brief Saves the current image to a file with the given file name.
 *
 * @param fileName The file name to save the image to.
 * @return True if the image was successfully saved, false otherwise.
 */
bool MainWindow::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                     .arg(QDir::toNativeSeparators(fileName)), QMessageBox::Ok);
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

/**
 * @brief Opens a dialog to save the current image as a file.
 */
void MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst())) {}
}

/**
 * @brief Prints the current image.
 */
void MainWindow::print()
{
    Q_ASSERT(!imageLabel->pixmap(Qt::ReturnByValue).isNull());

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    if (imageLabel->pixmap(Qt::ReturnByValue).isNull())
        qFatal("ASSERT: imageLabel->pixmap(Qt::ReturnByValue).isNull() in file ...");
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QPixmap pixmap = imageLabel->pixmap(Qt::ReturnByValue);
        QRect rect = painter.viewport();
        QSize size = pixmap.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(pixmap.rect());
        painter.drawPixmap(0, 0, pixmap);
    }
#endif
}

/**
 * @brief Copies the current image to the clipboard.
 */
void MainWindow::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(image);
#endif
}

#ifndef QT_NO_CLIPBOARD
/**
 * @brief Retrieves an image from the clipboard if available.
 *
 * @return The image retrieved from the clipboard.
 */
static QImage clipboardImage()
{
    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
        if (mimeData->hasImage()) {
            const QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull())
                return image;
        }
    }
    return QImage();
}
#endif

/**
 * @brief Pastes the image from the clipboard into the application.
 */
void MainWindow::paste()
{
#ifndef QT_NO_CLIPBOARD
    const QImage newImage = clipboardImage();
    if (newImage.isNull()) {
        statusBar()->showMessage(tr("No image in clipboard"));
    } else {
        setImage(newImage);
        setWindowFilePath(QString());
        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
                                    .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
        statusBar()->showMessage(message);
    }
#endif // !QT_NO_CLIPBOARD
}

/**
 * @brief Zooms in on the current image.
 */
void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

/**
 * @brief Zooms out on the current image.
 */
void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

/**
 * @brief Resets the image display to its normal size.
 */
void MainWindow::normalSize()
{
    scaleFactor = 1.0;
    imageLabel->adjustSize();
}

/**
 * @brief Opens a dialog to prepare the puzzle setup.
 */
void MainWindow::preparePuzzleSetUp()
{
    qreal dpiXMultiplier = image.dotsPerMeterX() * 0.0254 / 100; //image size with inches divided with standard dpi size
    qreal dpiYMultiplier = image.dotsPerMeterY() * 0.0254 / 100;

    QVector<int> imageSize = {static_cast<int>(image.height() / dpiYMultiplier), static_cast<int>(image.width() / dpiXMultiplier)};
    PuzzleSetUpSettingsDialog* dialog = new PuzzleSetUpSettingsDialog(imageSize);
    connect(dialog, &PuzzleSetUpSettingsDialog::acceptPuzzleDimensions, this, &MainWindow::preparePuzzle);
    dialog->open();
}

/**
 * @brief Prepares the puzzle with the given rows and columns.
 *
 * @param rows Number of rows in the puzzle.
 * @param columns Number of columns in the puzzle.
 */
void MainWindow::preparePuzzle(int rows, int columns)
{
    this->rows = rows;
    this->columns = columns;
    QSharedPointer<PuzzleShapeManager> puzzleShapes(new PuzzleShapeManager(rows, columns, image, this));
    createAction->setEnabled(true);
    playAction->setEnabled(false);
}

/**
 * @brief Receives the preview image of the puzzle shapes.
 *
 * @param imagePreview The preview image of puzzle shapes.
 */
void MainWindow::receivePuzzlePreview(const QImage imagePreview)
{
    setImage(imagePreview);
}

/**
 * @brief Receives the puzzle shapes from the puzzle setup dialog.
 *
 * @param shapes Hash map of puzzle shapes.
 */
void MainWindow::receivePuzzleShapes(const QHash<int, QPixmap> shapes)
{
    int index = 0;
    puzzleShapes.clear();
    for (int i = 0; i < shapes.count(); ++index)
    {
        if(!shapes[index].isNull())
        {
            puzzleShapes[i] = shapes[index];
            i++;
        }
    }

    foreach (QPixmap shape, puzzleShapes)
    {
        QSize shapeSize = shape.size();

        if (shapeSize.width() > biggestShape.width())
        {
            biggestShape.setWidth(shapeSize.width());
        }

        if (shapeSize.height() > biggestShape.height())
        {
            biggestShape.setHeight(shapeSize.height());
        }
    }
}

/**
 * @brief Creates the puzzle by populating the list view with puzzle shapes.
 */
void MainWindow::createPuzzle()
{
    listView->setViewMode(QListView::IconMode);


    listView->setIconSize(biggestShape);

    listView->setModel(nullptr);
    QStandardItemModel* model = new QStandardItemModel();

    QList<int> sortedKeys = puzzleShapes.keys();
    std::sort(sortedKeys.begin(), sortedKeys.end());

    for (int key : sortedKeys)
    {
        QPixmap pixmap = puzzleShapes.value(key);
        QIcon icon(pixmap);
        QStandardItem* item = new QStandardItem(icon, QString::number(key));

        model->appendRow(item);
    }

    listView->setModel(model);
    playAction->setEnabled(true);
    createAction->setEnabled(false);
}

/**
 * @brief Plays the puzzle game by opening the puzzle game dialog.
 */
void MainWindow::playPuzzle()
{
    PlayPuzzleGameDialog *playPuzzle = new PlayPuzzleGameDialog(rows, columns, image,biggestShape, this);
    playPuzzle->setAttribute(Qt::WA_DeleteOnClose);
    PlayPuzzlesShapes *playPuzzleShapes = new PlayPuzzlesShapes(qobject_cast<QStandardItemModel*>(listView->model()), biggestShape, this);
    playPuzzleShapes->setAttribute(Qt::WA_DeleteOnClose);

    connect(playPuzzle, &PlayPuzzleGameDialog::deleteShapeFromPool,playPuzzleShapes,&PlayPuzzlesShapes::deleteItemWithName);
    connect(playPuzzleShapes, &PlayPuzzlesShapes::dropEventReceived,playPuzzle,&PlayPuzzleGameDialog::deleteLabelWithName);

    playPuzzleShapes->show();
    playPuzzle->show();
}

/**
 * @brief Creates the main window actions for file, edit, view, and puzzle operations.
 */
void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    openAction->setShortcut(QKeySequence::Open);

    saveAsAction = fileMenu->addAction(tr("&Save As..."), this, &MainWindow::saveAs);
    saveAsAction->setEnabled(false);

    printAction = fileMenu->addAction(tr("&Print..."), this, &MainWindow::print);
    printAction->setShortcut(QKeySequence::Print);
    printAction->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAction = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    copyAction = editMenu->addAction(tr("&Copy"), this, &MainWindow::copy);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setEnabled(false);

    QAction *pasteAction = editMenu->addAction(tr("&Paste"), this, &MainWindow::paste);
    pasteAction->setShortcut(QKeySequence::Paste);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAction = viewMenu->addAction(tr("Zoom &In (25%)"), this, &MainWindow::zoomIn);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    zoomInAction->setEnabled(false);

    zoomOutAction = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &MainWindow::zoomOut);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    zoomOutAction->setEnabled(false);

    normalSizeAction = viewMenu->addAction(tr("&Normal Size"), this, &MainWindow::normalSize);
    normalSizeAction->setShortcut(tr("Ctrl+N"));
    normalSizeAction->setEnabled(false);

    QMenu *puzzleMenu = menuBar()->addMenu(tr("&Puzzle"));

    prepareAction = puzzleMenu->addAction(tr("&Prepare"), this, &MainWindow::preparePuzzleSetUp);
    prepareAction->setShortcut(tr("Ctrl+L"));
    prepareAction->setEnabled(false);

    createAction = puzzleMenu->addAction(tr("&Create"), this, &MainWindow::createPuzzle);
    createAction->setShortcut(tr("Ctrl+K"));
    createAction->setEnabled(false);

    playAction = puzzleMenu->addAction(tr("&Play"), this, &MainWindow::playPuzzle);
    playAction->setShortcut(tr("Ctrl+M"));
    playAction->setEnabled(false);
}

/**
 * @brief Updates the availability of actions based on the presence of an image.
 */
void MainWindow::updateActions()
{
    saveAsAction->setEnabled(!image.isNull());
    copyAction->setEnabled(!image.isNull());
    normalSizeAction->setEnabled(!image.isNull());
}

/**
 * @brief Scales the image according to the provided factor.
 *
 * @param factor The scaling factor to apply to the image.
 */
void MainWindow::scaleImage(double factor)
{
    scaleFactor *= factor;

    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());
    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAction->setEnabled(scaleFactor < 3.0);
    zoomOutAction->setEnabled(scaleFactor > 0.333);
}

/**
 * @brief Adjusts the scroll bar value based on the scaling factor.
 *
 * @param scrollBar The scroll bar to adjust.
 * @param factor The scaling factor.
 */
void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

/**
 * @brief Opens a help image stored in the resources.
 */
void MainWindow::openHelpImage()
{
    QImage introImage(":/help/exampleFile.jpg");

    if(!introImage.isNull())
    {
        setImage(introImage);
    }
}
