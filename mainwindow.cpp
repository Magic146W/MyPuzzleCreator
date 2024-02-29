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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateListViewItems();
}

void MainWindow::updateListViewItems()
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(listView->model());
    if (model)
    {
        listView->setModel(nullptr);
        listView->setModel(model);
    }
}

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

void MainWindow::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}

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

void MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst())) {}
}

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

void MainWindow::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(image);
#endif
}

#ifndef QT_NO_CLIPBOARD
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


void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::normalSize()
{
    scaleFactor = 1.0;
    imageLabel->adjustSize();
}

void MainWindow::preparePuzzleSetUp()
{
    qreal dpiXMultiplier = image.dotsPerMeterX() * 0.0254 / 100; //image size with inches divided with standard dpi size
    qreal dpiYMultiplier = image.dotsPerMeterY() * 0.0254 / 100;

    QVector<int> imageSize = {static_cast<int>(image.height() / dpiYMultiplier), static_cast<int>(image.width() / dpiXMultiplier)};
    PuzzleSetUpSettingsDialog* dialog = new PuzzleSetUpSettingsDialog(imageSize);
    connect(dialog, &PuzzleSetUpSettingsDialog::acceptPuzzleDimensions, this, &MainWindow::preparePuzzle);
    dialog->open();
}

void MainWindow::preparePuzzle(int rows, int columns)
{
    this->rows = rows;
    this->columns = columns;
    QSharedPointer<PuzzleShapeManager> puzzleShapes(new PuzzleShapeManager(rows, columns, image, this));
    createAction->setEnabled(true);
}

void MainWindow::receivePuzzlePreview(const QImage imagePreview)
{
    setImage(imagePreview);
}

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

void MainWindow::updateActions()
{
    saveAsAction->setEnabled(!image.isNull());
    copyAction->setEnabled(!image.isNull());
    normalSizeAction->setEnabled(!image.isNull());
}

void MainWindow::scaleImage(double factor)
{
    scaleFactor *= factor;

    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());
    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAction->setEnabled(scaleFactor < 3.0);
    zoomOutAction->setEnabled(scaleFactor > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::openHelpImage()
{
    QImage introImage(":/help/exampleFile.jpg");

    if(!introImage.isNull())
    {
        setImage(introImage);
    }
}
