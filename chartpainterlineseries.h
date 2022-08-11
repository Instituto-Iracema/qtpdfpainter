#ifndef CHARTPAINTERLINESERIES_H
#define CHARTPAINTERLINESERIES_H

#include <QColor>
#include <QLineF>
#include <QList>
#include <QPointF>

class ChartPainterLineSeries
{
public:
    ChartPainterLineSeries();
    ChartPainterLineSeries(qreal lineWidth, QColor lineColor, qreal yMax, qreal yMin);

    void addPoint(QPointF point);
    void addData(QList<QPointF> data);
    const QList<QLineF> getData();

    qreal lineWidth = 1;
    QColor lineColor = Qt::red;

    qreal yMax = 100;
    qreal yMin = 0;

private:
    QList<QLineF> _data;
    qreal _convertValueToNewScale(qreal oldValue, qreal oldScaleBottom, qreal oldScaleTop, qreal newScaleBottom, qreal newScaleTop);
};

#endif // CHARTPAINTERLINESERIES_H
