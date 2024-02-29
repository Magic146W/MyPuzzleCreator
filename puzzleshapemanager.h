#ifndef PUZZLESHAPEMANAGER_H
#define PUZZLESHAPEMANAGER_H

#include "puzzleedgedata.h"
#include <QObject>
#include <QVector>
#include <QImage>

class PuzzleShapeManager : public QObject
{
    Q_OBJECT

public:
    explicit PuzzleShapeManager(int columns, int rows, const QImage& image, QObject *parent = nullptr);
    ~PuzzleShapeManager();

public slots:
    void saveEdge(const QPair<QPoint, QPoint>& edge, const QPainterPath& path);
    void receivePreviewImage(const QImage puzzlePreview);

private:
    PuzzleEdgeData* puzzleEdgeData;
    QObject* parent;
    const QPainterPath loadEdge(const QPair<QPoint, QPoint>& edge) const;
    const QHash<int, QPainterPath> dividePuzzleIntoShapes();
    QHash<int, QPixmap> finalShapes;

    QVector<QPoint> generatePoints();
    QVector<QPoint> generateBezierFlowPoints(QPoint p1, QPoint p7);
    QHash<int, QPainter*> getShapeData();

    QImage drawCuttingShape(const QPainterPath &shape);
    QPixmap cutImage(const QPainterPath &shape);
    QRect calculateCuttingRegion(const QPainterPath& puzzleShape) const;
    void bezierShapes();

    QVector<QPoint> points;
    QImage myImage;
    QImage myImageCopy;
    int userShapes;
    int rows;
    int columns;
    int horizontalSpacing;
    int verticalSpacing;

signals:
};

#endif // PUZZLESHAPEMANAGER_H
