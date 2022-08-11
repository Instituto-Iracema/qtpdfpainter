#include "chartpainterlineseries.h"

ChartPainterLineSeries::ChartPainterLineSeries()
{

}

ChartPainterLineSeries::ChartPainterLineSeries(qreal lineWidth, QColor lineColor, qreal yMin, qreal yMax)
{
    this->lineWidth = lineWidth;
    this->lineColor = lineColor;
    this->yMax = yMax;
    this->yMin = yMin;
}

void ChartPainterLineSeries::addData(QList<QPointF> data)
{
    for (QPointF point : data)
        addPoint(point);
}

const QList<QLineF> ChartPainterLineSeries::getData()
{
    return _data;
}

void ChartPainterLineSeries::addPoint(QPointF point)
{
    QLineF line;
    if (_data.empty())
        line.setP1(point);
    else
        line.setP1(_data.last().p2());

    line.setP2(point);
    _data.append(line);
}

qreal ChartPainterLineSeries::_convertValueToNewScale(qreal oldValue, qreal oldScaleBottom, qreal oldScaleTop, qreal newScaleBottom, qreal newScaleTop)
{
    qreal newValue = oldValue - oldScaleBottom;
    newValue *= newScaleTop - newScaleBottom;
    newValue /= oldScaleTop - oldScaleBottom;
    newValue += newScaleBottom;

    return newValue;
}
