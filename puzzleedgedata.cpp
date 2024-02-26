#include "puzzleedgedata.h"

PuzzleEdgeData::PuzzleEdgeData(QObject *parent)
{

}

PuzzleEdgeData::~PuzzleEdgeData() {}

void PuzzleEdgeData::addEdge(const QPair<QPoint, QPoint>& edge, const QPainterPath& path)
{
    edges.insert(edge, path);
}

const QHash<QPair<QPoint, QPoint>, QPainterPath>& PuzzleEdgeData::getAllEdges() const
{
    return edges;
}

const QPainterPath PuzzleEdgeData::getEdgeByPoints(const QPoint& point1, const QPoint& point2) const
{
    return edges.value(QPair<QPoint, QPoint>(point1, point2));
}

int PuzzleEdgeData::count()
{
    return edges.count();
}
