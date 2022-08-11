#ifndef CHARTPAINTER_H
#define CHARTPAINTER_H

#include <QColor>
#include <QPainter>
#include <QRectF>
#include <QSizeF>
#include "chartpainterlineseries.h"
#include "abstractpainter.h"
#include <cmath>

class ChartPainter : public AbstractPainter
{
public:
    ChartPainter(QRectF newRect);
    ChartPainter();

    QColor backgroundColor = Qt::white;
    QString title = "Complacência Dinâmica";

    mutable QSizeF gridSize;
    qreal gridLineWidth = 1;
    QColor gridColor = Qt::lightGray;
    QSizeF gridOffset = QSizeF(0, 0);
    int gridHorizontalCount = 0;
    int gridVerticalCount = 0;
    qreal scalesOffset = 400;
    qreal scalesPixelSize = 100;
    qreal scalesMargin = 20;

    qreal xMax = 1000;
    qreal xMin = 0;

    void addLineSeries(ChartPainterLineSeries lineSeries);
    void removeLineSeries(unsigned int lineSeriesIndex);
    void addPoint(unsigned int lineSeriesIndex, QPointF point);
    void addData(unsigned int lineSeriesIndex, QList<QPointF> data);
    void draw(QPainter *painter) const override;
    const QRectF &rect() const override;
    void setRect(const QRectF &newRect) const override;
private:
    mutable QRectF _rect;
    mutable QRectF _realRect;
    mutable QList<QString> _horizontalScales;
    mutable QList<QString> _verticalScales;
    QList<ChartPainterLineSeries> _lines;
    QPainter *_painter;


    QList<QString> _calculateScales(double minValue, double maxValue, int count) const;
    qreal truncate(qreal value, int numberOfDigits) const;
    void _drawTitle(QPainter *painter, QString title) const;
    void _drawScaleHorizontal(QPainter *painter, qreal position, QString scaleValue) const;
    void _drawScaleVertical(QPainter *painter, qreal position, QString scaleValue) const;
    void _drawGridHorizontal(QPainter *painter) const;
    void _drawGridVertical(QPainter *painter) const;
    void _drawGrid(QPainter *painter) const;
    void _drawLineSeries(QPainter *painter, ChartPainterLineSeries line, bool invertY = true) const;
    void _drawLines(QPainter *painter) const;
    qreal _convertValueToNewScale(qreal oldValue, qreal oldScaleBottom, qreal oldScaleTop, qreal newScaleBottom, qreal newScaleTop) const;
};

#endif // CHARTPAINTER_H
