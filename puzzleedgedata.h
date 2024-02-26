#ifndef PUZZLEEDGEDATA_H
#define PUZZLEEDGEDATA_H

#include <QObject>
#include <QHash>
#include <QPainterPath>
#include <QPoint>

class PuzzleEdgeData : public QObject
{
    Q_OBJECT

public:
    explicit PuzzleEdgeData(QObject *parent = nullptr);
    ~PuzzleEdgeData();

    void addEdge(const QPair<QPoint, QPoint>& edge, const QPainterPath& path);

    const QHash<QPair<QPoint, QPoint>, QPainterPath>& getAllEdges() const;
    const QPainterPath getEdgeByPoints(const QPoint& point1, const QPoint& point2) const;
    int count();

private:
    QHash<QPair<QPoint, QPoint>, QPainterPath> edges;
};

#endif // PUZZLEEDGEDATA_H
