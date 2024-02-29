#include "imagedividerwithbezier.h"

#include <QPoint>
#include <QPainter>
#include <QPainterPath>

ImageDividerWithBezier::ImageDividerWithBezier(QObject *parent)
    : QObject{parent}
{}

ImageDividerWithBezier::ImageDividerWithBezier(const QImage &image)
    : originalImage(image)
{
    imageCopy = originalImage;
}

ImageDividerWithBezier::~ImageDividerWithBezier()
{

}

void ImageDividerWithBezier::setBezierPoints(const QPoint &p1, const QVector<QPoint>& bezierPoints, const QPoint &p7)
{
    controlPoint1 = p1;
    controlPoint7 = p7;

    controlPoint2 = bezierPoints[0];
    controlPoint3 = bezierPoints[1];
    controlPoint4 = bezierPoints[2];
    controlPoint5 = bezierPoints[3];
    controlPoint6 = bezierPoints[4];
}

void ImageDividerWithBezier::prepareToCutImage()
{
    QPainter painter(&imageCopy);

    QPainterPath bezierPath = createBezierPath();
    QPair<QPoint, QPoint> edge(controlPoint1, controlPoint7);

    emit saveEdge(edge, bezierPath);

    QPen blackPen(Qt::black);
    blackPen.setWidth(3);
    QPen whitePen(Qt::white);
    whitePen.setWidth(1);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(blackPen);
    painter.drawPath(bezierPath);
    painter.setPen(whitePen);
    painter.drawPath(bezierPath);

    painter.end();
}

void ImageDividerWithBezier::imageCopyPreview()
{
    QString filePath = "C:/Users/macwe/Pictures/puzzlePreview.jpg";
    imageCopy.save(filePath);
    emit loadPreviewImage(imageCopy);
}

QPainterPath ImageDividerWithBezier::createBezierPath() const
{
    QPainterPath bezierPath;
    bezierPath.moveTo(controlPoint1);

    if((controlPoint1.x() == 0 && controlPoint2.x() == 0) ||
        (controlPoint1.y() == 0 && controlPoint2.y() == 0) ||
        (controlPoint1.x() == originalImage.width() && controlPoint2.x() == originalImage.width()) ||
        (controlPoint1.y() == originalImage.height() && controlPoint2.y() == originalImage.height()))
    {
        bezierPath.lineTo(controlPoint7);
    } else
    {

        QPoint p12 = calculateBezierPointLocationForBaseLine(controlPoint1, controlPoint2, controlPoint3);
        QPoint p23 = calculateBezierPointLocationForPerpendicularLines(controlPoint2, controlPoint3, controlPoint4);
        QPoint p34 = calculateBezierPointLocationForLinesBetweenPerpendicularLines(controlPoint3, controlPoint4, controlPoint5);
        QPoint p45 = calculateBezierPointLocationForLinesBetweenPerpendicularLines(controlPoint5, controlPoint4, controlPoint3);
        QPoint p56 = calculateBezierPointLocationForPerpendicularLines(controlPoint5, controlPoint6, controlPoint4);
        QPoint p67 = calculateBezierPointLocationForBaseLine(controlPoint6, controlPoint7, controlPoint5);


        bezierPath.quadTo(p12, controlPoint2);
        bezierPath.quadTo(p23, controlPoint3);
        bezierPath.quadTo(p34, controlPoint4);
        bezierPath.quadTo(p45, controlPoint5);
        bezierPath.quadTo(p56, controlPoint6);
        bezierPath.quadTo(p67, controlPoint7);
    }

    return bezierPath;
}

QPoint ImageDividerWithBezier::calculateBezierPointLocationForBaseLine(const QPoint &point1, const QPoint &point2, const QPoint &distancePoint) const
{
    QPoint bezierPoint;
    int distance;
    int isConcave = (rand() % 2) == 0 ? 1 : -1;

    if(point1.y() == point2.y())
    {
        distance = rand() % (qAbs(point2.x() - point1.x()) / 4) + qAbs(point2.x() - point1.x()) / 2;

        bezierPoint.setX(point1.x() + distance);
        bezierPoint.setY(point1.y() - (point1.y() - distancePoint.y()) * isConcave);
    } else if(point1.x() == point2.x())
    {
        distance = rand() % (qAbs(point2.y() - point1.y()) / 4) + qAbs(point2.y() - point1.y()) / 2;

        bezierPoint.setX(point1.x() - (point1.x() - distancePoint.x()) * isConcave);
        bezierPoint.setY(point1.y() + distance);
    }

    return bezierPoint;
}

QPoint ImageDividerWithBezier::calculateBezierPointLocationForPerpendicularLines(const QPoint &point1, const QPoint &point2, const QPoint &distancePoint) const
{
    QPoint bezierPoint;
    int randomMidPoint;

    if(point1.y() == point2.y())
    {
        randomMidPoint = (point2.x() + point1.x()) / 2;

        bezierPoint.setX(randomMidPoint);
        bezierPoint.setY(point1.y() - (point1.y() - distancePoint.y()));
    } else if(point1.x() == point2.x())
    {
        randomMidPoint = (point2.y() + point1.y()) / 2;

        bezierPoint.setX(point1.x() - (point1.x() - distancePoint.x()));
        bezierPoint.setY(randomMidPoint);
    }

    return bezierPoint;
}

QPoint ImageDividerWithBezier::calculateBezierPointLocationForLinesBetweenPerpendicularLines(const QPoint &point1, const QPoint &point2, const QPoint &distancePoint) const
{
    QPoint bezierPoint;
    int distance;

    if(point1.y() == distancePoint.y())
    {
        distance = qAbs(point2.x() - point1.x()) * 0.25;
        bezierPoint.setX(point1.x()-distance);
        bezierPoint.setY(point2.y());
    } else
    {
        distance = qAbs(point2.y() - point1.y()) * 0.25;
        bezierPoint.setX(point2.x());
        bezierPoint.setY(point1.y()-distance);
    }

    return bezierPoint;
}
