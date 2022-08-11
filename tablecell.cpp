#include "tablecell.h"

TableCell::TableCell(QColor backgroundColor, qreal lineWidth, QColor lineColor, TableCellWalls walls)
{
    _lineColor = lineColor;
    _lineWidth = lineWidth;
    _walls = walls;
    _backgroundColor = backgroundColor;
}

TableCell::TableCell(qreal flex, QColor backgroundColor, qreal lineWidth, QColor lineColor, TableCellWalls walls)
{
    _lineColor = lineColor;
    _lineWidth = lineWidth;
    _walls = walls;
    _flex = flex;
    _autoFlex = false;
    _backgroundColor = backgroundColor;
}

void TableCell::addContent(TableCellContent *content)
{
    _contents.append(content);
}

void TableCell::removeContent(unsigned int contentIndex)
{
    TableCellContent *content = _contents[contentIndex];
    _contents.removeAt(contentIndex);
    delete content;
}

void TableCell::draw(QPainter *painter)
{
    _paintBackground(painter);
    _drawWalls(painter);
    for (auto content : qAsConst(_contents))
    {
        content->draw(painter, _rect);
    }
}

qreal TableCell::lineWidth() const
{
    return _lineWidth;
}

void TableCell::setLineWidth(qreal newLineWidth)
{
    _lineWidth = newLineWidth;
}

const QColor &TableCell::lineColor() const
{
    return _lineColor;
}

void TableCell::setLineColor(const QColor &newLineColor)
{
    _lineColor = newLineColor;
}

TableCellWalls TableCell::walls() const
{
    return _walls;
}

void TableCell::setWalls(TableCellWalls newWalls)
{
    _walls = newWalls;
}

qreal TableCell::flex() const
{
    return _flex;
}

void TableCell::setFlex(qreal newFlex)
{
    _flex = newFlex;
}

const QRectF &TableCell::rect() const
{
    return _rect;
}

void TableCell::setRect(const QRectF &newRect)
{
    _rect = QRectF(newRect);
}

bool TableCell::autoFlex() const
{
    return _autoFlex;
}

void TableCell::setAutoFlex(bool newAutoFlex)
{
    _autoFlex = newAutoFlex;
}

void TableCell::_drawWalls(QPainter *painter)
{
    for (auto i = 0U; i < 4; i++)
    {
//        if ((_walls >> i) & 0) continue;

        int mask = 1;

        if (!(_walls & (mask << i))) {
            continue;
        }


        switch (i) {
        case 0:
            painter->drawLine(_rect.topLeft(), _rect.topRight());
            break;
        case 1:
            painter->drawLine(_rect.topRight(), _rect.bottomRight());
            break;
        case 2:
            painter->drawLine(_rect.bottomRight(), _rect.bottomLeft());
            break;
        case 3:
            painter->drawLine(_rect.bottomLeft(), _rect.topLeft());
            break;
        default:
            break;
        }
    }
}

void TableCell::_paintBackground(QPainter *painter)
{
    painter->fillRect(_rect, QBrush(_backgroundColor));
}

const QColor &TableCell::backgroundColor() const
{
    return _backgroundColor;
}

void TableCell::setBackgroundColor(const QColor &newBackgroundColor)
{
    _backgroundColor = newBackgroundColor;
}
