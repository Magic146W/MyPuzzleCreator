#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollBar>
#include <QLabel>
#include <QScrollArea>
#include <QListView>
#include <QPrinter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool loadFile(const QString &);

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void preparePuzzle(int row, int column);
    void receivePuzzlePreview(const QImage imagePreview);
    void receivePuzzleShapes(const QHash<int, QPixmap> shapes);

private slots:
    void open();
    void saveAs();
    void print();
    void copy();
    void paste();

    void zoomIn();
    void zoomOut();
    void normalSize();

    void preparePuzzleSetUp();
    void createPuzzle();
    void playPuzzle();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();
    void updateActions();

    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    void openHelpImage();
    void updateListViewItems();

    QImage image;
    QLabel *imageLabel;
    QListView *listView;
    QScrollArea *scrollArea;
    double scaleFactor = 1;
    QHash<int, QPixmap> puzzleShapes;
    QSize biggestShape;

    int rows;
    int columns;

#if defined(QT_PRINTSUPPORT_LIB)
    QPrinter printer;
#endif

    QAction *saveAsAction;
    QAction *printAction;
    QAction *copyAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *normalSizeAction;
    QAction *prepareAction;
    QAction *createAction;
    QAction *playAction;
};

#endif // MAINWINDOW_H
