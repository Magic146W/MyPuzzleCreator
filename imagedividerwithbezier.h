#ifndef IMAGEDIVIDERWITHBEZIER_H
#define IMAGEDIVIDERWITHBEZIER_H

#include <QObject>
#include <QPoint>
#include <QPainter>
#include <QImage>
#include <QPixmap>

class ImageDividerWithBezier : public QObject
{
    Q_OBJECT

public:
    explicit ImageDividerWithBezier(const QImage& image, QObject *parent = nullptr);
    ~ImageDividerWithBezier();

    void setBezierPoints(const QPoint &p1, const QVector<QPoint>& bezierPoints, const QPoint &p7);
    void prepareToCutImage();
    void imageCopyPreview();

private:
    QPoint calculateBezierPointLocationForBaseLine(const QPoint &point1, const QPoint &point2, const QPoint &distancePoint) const;
    QPoint calculateBezierPointLocationForPerpendicularLines(const QPoint &point1, const QPoint &point2, const QPoint &distancePoint) const;
    QPoint calculateBezierPointLocationForLinesBetweenPerpendicularLines(const QPoint &point1, const QPoint &point2, const QPoint &distancePoint) const;

    QPainterPath createBezierPath() const;

    QImage originalImage;
    QImage imageCopy;
    QImage imageToCut;
    QPoint controlPoint1, controlPoint2, controlPoint3, controlPoint4, controlPoint5, controlPoint6, controlPoint7;

signals:
    void saveEdge(const QPair<QPoint, QPoint>& edge, const QPainterPath& path);
    void loadPreviewImage(const QImage puzzlePreview);
};

#endif // IMAGEDIVIDERWITHBEZIER_H
