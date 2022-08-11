#include "chartpainter.h"
#include <QPen>
#include <QDebug>

ChartPainter::ChartPainter()
{
}

ChartPainter::ChartPainter(QRectF newRect)
{
    setRect(newRect);
}

void ChartPainter::addLineSeries(ChartPainterLineSeries lineSeries)
{
    _lines.append(lineSeries);
}

void ChartPainter::removeLineSeries(unsigned int lineSeriesIndex)
{
    _lines.removeAt(lineSeriesIndex);
}

void ChartPainter::addPoint(unsigned int lineSeriesIndex, QPointF point)
{
    _lines[lineSeriesIndex].addPoint(point);
}

void ChartPainter::addData(unsigned int lineSeriesIndex, QList<QPointF> data)
{
    _lines[lineSeriesIndex].addData(data);
}

QList<QString> ChartPainter::_calculateScales(double minValue, double maxValue, int count) const
{
    const double incrementValue = std::abs(minValue - maxValue) / count;
    double value = minValue;
    qreal truncValue = 0;
    int numberOfDigits = 2;

    QList<QString> list;
    list.append(QString::number(truncate(value, numberOfDigits)));

    for (int i = 0; i < count; i++) {
        if(minValue == maxValue)
            list.append("");
        else {
            // Finds the number of digits where they became equal
            while(truncate(value, numberOfDigits) != truncate(value + incrementValue, numberOfDigits)){
                if(numberOfDigits <= 0) break;
                --numberOfDigits;
            }

            // Finds the number of digits where they became different
            while(truncate(value, numberOfDigits) == truncate(value + incrementValue, numberOfDigits)){
                if(numberOfDigits >= 3) break;
                ++numberOfDigits;
            }

            value += incrementValue;
            truncValue = truncate(value, numberOfDigits);
            list.append(QString::number(truncValue));
        }
    }
    return list;
}

qreal ChartPainter::truncate(qreal value, int numberOfDigits) const
{
    double truncValue = 0;
    numberOfDigits = (int) std::pow(10, numberOfDigits);

    truncValue = int(value * numberOfDigits);
    return truncValue / numberOfDigits;
}

void ChartPainter::_drawTitle(QPainter *painter, QString title) const
{
    const QRectF rectangle = QRectF(_realRect.x(), _realRect.y(), _realRect.width(), _realRect.height() - _rect.height() - scalesOffset / 2);

    QFont font = QFont();
    font.setWeight(QFont::Bold);
    font.setPixelSize(scalesPixelSize * 1.5);

    painter->setPen(QPen(QBrush(Qt::black), gridLineWidth));
    painter->setFont(font);
    painter->drawText(rectangle, Qt::AlignCenter, title);
    painter->setPen(QPen(QBrush(gridColor), gridLineWidth));
}

void ChartPainter::_drawScaleHorizontal(QPainter *painter, qreal position, QString scaleValue) const
{
    const QRectF rectangle = QRectF(0, position - scalesPixelSize / 2, _rect.left() - scalesMargin, scalesPixelSize);

    QFont font = QFont();
    font.setWeight(QFont::Bold);
    font.setPixelSize(scalesPixelSize);

    painter->setPen(QPen(QBrush(Qt::black), gridLineWidth));
    painter->setFont(font);
    painter->drawText(rectangle, Qt::AlignRight, scaleValue);
    painter->setPen(QPen(QBrush(gridColor), gridLineWidth));
}

void ChartPainter::_drawScaleVertical(QPainter *painter, qreal position, QString scaleValue) const
{
    const QRectF rectangle = QRectF(position - (_rect.left() - scalesMargin) / 2, _rect.bottom(), _rect.left() - scalesMargin, scalesPixelSize);

    QFont font = QFont();
    font.setWeight(QFont::Bold);
    font.setPixelSize(scalesPixelSize);

    painter->setPen(QPen(QBrush(Qt::black), gridLineWidth));
    painter->setFont(font);
    painter->drawText(rectangle, Qt::AlignCenter, scaleValue);
    painter->setPen(QPen(QBrush(gridColor), gridLineWidth));
}

void ChartPainter::_drawGridHorizontal(QPainter *painter) const
{
    painter->setPen(QPen(QBrush(gridColor), gridLineWidth));

    int index = _horizontalScales.length() - 1;
    qreal currentPosition = _rect.top() + gridOffset.height();
    while(currentPosition <= _rect.bottom())
    {
        QLineF line(_rect.left(), currentPosition, _rect.right(), currentPosition);
        painter->drawLine(line);
        _drawScaleHorizontal(painter, currentPosition, _horizontalScales.at(index--));
        currentPosition += gridSize.height();
    }
}

void ChartPainter::_drawGridVertical(QPainter *painter) const
{
    painter->setPen(QPen(QBrush(gridColor), gridLineWidth));

    int index = 0;
    qreal currentPosition = _rect.left() + gridOffset.width();
    while(currentPosition <= _rect.right())
    {
        QLineF line(currentPosition, _rect.top(), currentPosition, _rect.bottom());
        painter->drawLine(line);
        _drawScaleVertical(painter, currentPosition, _verticalScales.at(index++));
        currentPosition += gridSize.width();
    }
}

void ChartPainter::_drawGrid(QPainter *painter) const
{
    if (gridSize.width() <= 0 || gridSize.height() <= 0) return;

    _drawGridHorizontal(painter);
    _drawGridVertical(painter);
}

void ChartPainter::_drawLineSeries(QPainter *painter, ChartPainterLineSeries lineSeries, bool invertY) const
{
    for (QLineF line : lineSeries.getData())
    {
        qreal newX1 = _convertValueToNewScale(line.p1().x(), xMin, xMax, _rect.left(), _rect.right());
        qreal newX2 = _convertValueToNewScale(line.p2().x(), xMin, xMax, _rect.left(), _rect.right());
        qreal newY1 = _convertValueToNewScale(line.p1().y(), lineSeries.yMin, lineSeries.yMax, _rect.top(), _rect.bottom());
        qreal newY2 = _convertValueToNewScale(line.p2().y(), lineSeries.yMin, lineSeries.yMax,  _rect.top(), _rect.bottom());

        //Clip values to view boundaries
        newY1 = std::max(_rect.top(), std::min(newY1, _rect.bottom()));
        newY2 = std::max(_rect.top(), std::min(newY2, _rect.bottom()));

        if (invertY) {
            newY1 = _rect.bottom() - (newY1 - _rect.top());
            newY2 = _rect.bottom() - (newY2 - _rect.top());
        }

        QPointF newP1(newX1, newY1);
        QPointF newP2(newX2, newY2);
        QLineF newLine(newP1, newP2);

        painter->setPen(QPen(QBrush(Qt::black), lineSeries.lineWidth));
        painter->drawLine(newLine);
    }

    painter->setPen(QPen(QBrush(Qt::black), 1, Qt::SolidLine, Qt::RoundCap));
}

void ChartPainter::_drawLines(QPainter *painter) const
{
    for (const auto &lineSeries : qAsConst(_lines))
    {
        _drawLineSeries(painter, lineSeries);
    }
}

qreal ChartPainter::_convertValueToNewScale(qreal oldValue, qreal oldScaleBottom, qreal oldScaleTop, qreal newScaleBottom, qreal newScaleTop) const
{
    qreal newValue = oldValue - oldScaleBottom;
    newValue *= newScaleTop - newScaleBottom;
    newValue /= oldScaleTop - oldScaleBottom;
    newValue += newScaleBottom;

    return newValue;
}

void ChartPainter::draw(QPainter *painter) const
{
    _verticalScales = _calculateScales(xMin, xMax, gridVerticalCount);
    _horizontalScales =  _calculateScales(_lines.first().yMin, _lines.first().yMax, gridHorizontalCount);
    _drawTitle(painter, title);
    _drawGrid(painter);
    _drawLines(painter);
}

const QRectF &ChartPainter::rect() const
{
    return _realRect;
}

void ChartPainter::setRect(const QRectF &newRect) const
{
    _realRect = newRect;
    QRectF chartRect = QRectF(newRect.x() + scalesOffset, newRect.y() + scalesOffset, newRect.width() - scalesOffset, newRect.height() - scalesOffset * 1.5);
    _rect = chartRect;
    gridSize = QSizeF(_rect.width() / gridVerticalCount, _rect.height() / gridHorizontalCount);
}
