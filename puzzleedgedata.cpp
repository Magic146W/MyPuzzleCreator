#include "puzzleedgedata.h"

/**
 * @brief The PuzzleEdgeData class manages puzzle edge data.
 *
 * This class is responsible for storing and retrieving puzzle edge information.
 */


PuzzleEdgeData::PuzzleEdgeData(QObject *parent)
{

}

PuzzleEdgeData::~PuzzleEdgeData() {}

/**
     * @brief Adds an edge with its corresponding path to the data.
     *
     * @param edge The edge represented by a pair of points.
     * @param path The QPainterPath corresponding to the edge.
     */
void PuzzleEdgeData::addEdge(const QPair<QPoint, QPoint>& edge, const QPainterPath& path)
{
    edges.insert(edge, path);
}

/**
     * @brief Gets all stored edges.
     *
     * @return A constant reference to the hash containing all edges.
     */
const QHash<QPair<QPoint, QPoint>, QPainterPath>& PuzzleEdgeData::getAllEdges() const
{
    return edges;
}

/**
     * @brief Gets the edge path given two points.
     *
     * @param point1 The first point of the edge.
     * @param point2 The second point of the edge.
     * @return The QPainterPath corresponding to the edge.
     */
const QPainterPath PuzzleEdgeData::getEdgeByPoints(const QPoint& point1, const QPoint& point2) const
{
    return edges.value(QPair<QPoint, QPoint>(point1, point2));
}

/**
     * @brief Gets the total count of stored edges.
     *
     * @return The count of edges.
     */
int PuzzleEdgeData::count()
{
    return edges.count();
}
