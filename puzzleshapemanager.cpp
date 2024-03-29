#include "mainwindow.h"
#include "puzzleshapemanager.h"
#include "imagedividerwithbezier.h"

/**
 * @class PuzzleShapeManager
 * @brief Manages the creation of shapes for puzzle.
 *
 * The PuzzleShapeManager class is responsible for managing the shapes of a puzzle.
 * It handles the generation of puzzle shapes, cutting the puzzle image into individual
 * shapes, and providing the necessary functionality to work with puzzle shapes.
 */


PuzzleShapeManager::PuzzleShapeManager(int rows, int columns, const QImage& image, QObject *parent)
    : myImage(image)
    , myImageCopy(image)
    , rows(rows)
    , columns(columns)
{
    this->parent = parent;
    userShapes = columns*rows;
    puzzleEdgeData = new PuzzleEdgeData();
    points = generatePoints();
    bezierShapes();
}

PuzzleShapeManager::~PuzzleShapeManager()
{

}

/**
 * @brief Saves an edge along with its associated QPainterPath.
 *
 * @param edge The edge represented by a pair of QPoint.
 * @param path The QPainterPath associated with the edge.
 */
void PuzzleShapeManager::saveEdge(const QPair<QPoint, QPoint>& edge, const QPainterPath& path)
{
    puzzleEdgeData->addEdge(edge, path);
}

/**
 * @brief Loads the QPainterPath associated with the given edge.
 *
 * @param edge The edge represented by a pair of QPoint.
 * @return The QPainterPath associated with the edge.
 */
const QPainterPath PuzzleShapeManager::loadEdge(const QPair<QPoint, QPoint>& edge) const
{
    return puzzleEdgeData->getEdgeByPoints(edge.first, edge.second);
}

/**
 * @brief Generates the points for the puzzle grid.
 *
 * @return A QVector containing the generated points.
 */
QVector<QPoint> PuzzleShapeManager::generatePoints()
{
    int imageWidth = myImage.width();
    int imageHeight = myImage.height();

    horizontalSpacing = imageWidth / columns;
    verticalSpacing = imageHeight / rows;

    int leftoverXSpacing = imageWidth - horizontalSpacing;
    int leftoverYSpacing = imageHeight - verticalSpacing;

    for (int row = 0; row <= rows; ++row)
    {
        for (int col = 0; col <= columns; ++col)
        {
            int x = col * horizontalSpacing;
            int y = row * verticalSpacing;

            if(col != 0 && leftoverXSpacing != 0)
            {
                x += 1;
                leftoverXSpacing--;
            }


            if(row != 0 && leftoverYSpacing != 0)
            {
                y += 1;
                leftoverYSpacing--;
            }

            if(row == rows)
            {
                y = imageHeight;
            }

            if (col == columns)
            {
                x = imageWidth;
            }
            points.append(QPoint(x, y));
        }
    }

    points.last() = QPoint(imageWidth, imageHeight);

    return points;
}

/**
 * @brief Generates Bezier shapes for the puzzle edges.
 *
 * Uses the generated points to create Bezier shapes for the puzzle edges.
 */
void PuzzleShapeManager::bezierShapes()
{
    ImageDividerWithBezier classicPuzzles(myImage, this);
    connect(&classicPuzzles, &ImageDividerWithBezier::saveEdge,this, &PuzzleShapeManager::saveEdge);
    connect(&classicPuzzles, &ImageDividerWithBezier::loadPreviewImage, this, &PuzzleShapeManager::receivePreviewImage);

    for (int i = 0; i < points.length(); ++i)
    {
        if((i+1) < points.length() && points[i].y() == points[i+1].y())
        {
            classicPuzzles.setBezierPoints(points[i], generateBezierFlowPoints(points[i],points[i+1]), points[i+1]);
            classicPuzzles.prepareToCutImage();
        }

        if ((i + (columns+1)) < points.length())
        {
            classicPuzzles.setBezierPoints(points[i], generateBezierFlowPoints(points[i],points[i+(columns+1)]), points[i+(columns+1)]);
            classicPuzzles.prepareToCutImage();
        }
    }
    classicPuzzles.imageCopyPreview();
}

/**
 * @brief Generates the Bezier flow points for an edge.
 *
 * Generates intermediate points for Bezier curves between two given points.
 *
 * @param p1 The start point.
 * @param p7 The end point.
 * @return A QVector containing the generated Bezier flow points.
 */
QVector<QPoint> PuzzleShapeManager::generateBezierFlowPoints(QPoint p1, QPoint p7)
{
    QVector<QPoint> bezierPoints;
    bool isVertical = (p1.x() == p7.x());
    int multiplier = (rand() % 2 == 0) ? 1 : -1;

    QPoint p2;
    if (isVertical)
    {
        p2.setX(p1.x());
        p2.setY(p1.y() + verticalSpacing * (0.4 + (rand() % 21) / 100.0));
    } else
    {
        p2.setX(p1.x() + horizontalSpacing * (0.4 + (rand() % 21) / 100.0));
        p2.setY(p1.y());
    }
    bezierPoints << p2;

    QPoint p3;
    if (isVertical)
    {
        p3.setX(p2.x() + multiplier * horizontalSpacing * 0.1);
        p3.setY(p2.y());
    } else
    {
        p3.setX(p2.x());
        p3.setY(p2.y() - multiplier * verticalSpacing * 0.1);
    }
    bezierPoints << p3;


    QPoint p4;
    if (isVertical)
    {
        p4.setX(p3.x() + multiplier * horizontalSpacing * 0.05);
        p4.setY(p3.y() + verticalSpacing * 0.05);
    } else
    {
        p4.setX(p3.x() + horizontalSpacing * 0.05);
        p4.setY(p3.y() - multiplier * verticalSpacing * 0.05);
    }
    bezierPoints << p4;

    QPoint p5;
    if (isVertical)
    {
        p5.setX(p3.x());
        p5.setY(p3.y() + verticalSpacing * 0.1);
    } else
    {
        p5.setX(p3.x() + horizontalSpacing * 0.1);
        p5.setY(p3.y());
    }
    bezierPoints << p5;

    QPoint p6;
    if (isVertical)
    {
        p6.setX(p2.x());
        p6.setY(p2.y() + verticalSpacing * 0.1);
    } else
    {
        p6.setX(p2.x() + horizontalSpacing * 0.1);
        p6.setY(p2.y());
    }
    bezierPoints << p6;

    return bezierPoints;
}


/**
 * @brief Receives the puzzle preview image and processes it.
 *
 * @param puzzlePreview The puzzle preview image.
 */
void PuzzleShapeManager::receivePreviewImage(const QImage puzzlePreview)
{
    MainWindow *mainWindow = qobject_cast<MainWindow*>(parent);
    if (mainWindow)
    {
        mainWindow->receivePuzzlePreview(puzzlePreview);
    }

    QHash<int, QPainterPath> puzzleShapes = dividePuzzleIntoShapes();

    for (auto it = puzzleShapes.begin(); it != puzzleShapes.end(); ++it)
    {
        finalShapes.insert(it.key(), cutImage(it.value()));
    }

    mainWindow->receivePuzzleShapes(finalShapes);
}

/**
 * @brief Divides the puzzle into shapes based on its edges.
 *
 * @return A hash map containing the puzzle shapes.
 */
const QHash<int, QPainterPath> PuzzleShapeManager::dividePuzzleIntoShapes()
{
    QHash<int, QPainterPath> puzzleShapes = *new QHash<int, QPainterPath>();
    int count = puzzleEdgeData->count();

    for (int i = 0; i < count; ++i)
    {
        if((i+1)+(columns+1) < points.count())
        {
            if(i%(columns+1) != columns)
            {
                QPainterPath shape;

                shape = puzzleEdgeData->getEdgeByPoints(points[i],points[i+1]);
                shape.connectPath(puzzleEdgeData->getEdgeByPoints(points[i+1],points[i+1+(columns+1)]));
                shape.connectPath((puzzleEdgeData->getEdgeByPoints(points[i+(columns+1)],points[i+1+(columns+1)]).toReversed()));
                shape.connectPath(puzzleEdgeData->getEdgeByPoints(points[i],points[i+(columns+1)]).toReversed());

                puzzleShapes[i] = shape;
            }
        } else
        {
            break;
        }
    }

    return puzzleShapes;
}

/**
 * @brief Calculates the cutting region for a puzzle shape.
 *
 * @param puzzleShape The QPainterPath representing the puzzle shape.
 * @return The QRect representing the cutting region.
 */
QRect PuzzleShapeManager::calculateCuttingRegion(const QPainterPath& puzzleShape) const
{
    QRect pathBounds = puzzleShape.boundingRect().toRect();

    int expansion = 4;
    pathBounds.adjust(-expansion, -expansion, expansion, expansion);

    return pathBounds.intersected(myImage.rect());
}

/**
 * @brief Cuts the image based on the puzzle shape.
 *
 * @param puzzleShape The QPainterPath representing the puzzle shape.
 * @return The QPixmap representing the cutout image.
 */
QPixmap PuzzleShapeManager::cutImage(const QPainterPath& puzzleShape)
{
    myImageCopy = myImage;
    QImage cutShape = drawCuttingShape(puzzleShape);
    QRect cuttingRegion = calculateCuttingRegion(puzzleShape);
    QImage cutoutImage = cutShape.copy(cuttingRegion);

    return QPixmap::fromImage(cutoutImage);
}

/**
 * @brief Draws the cutting shape on a transparent image.
 *
 * @param puzzleShape The QPainterPath representing the puzzle shape.
 * @return The QImage containing the cutting shape.
 */
QImage PuzzleShapeManager::drawCuttingShape(const QPainterPath& puzzleShape)
{
    QImage shape(myImage.size(), QImage::Format_ARGB32);
    shape.fill(Qt::transparent);

    QPainter painter(&shape);
    painter.setClipPath(puzzleShape);
    painter.drawImage(QPoint(), myImage);
    painter.end();

    return shape;
}
