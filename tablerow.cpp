#include "tablerow.h"

TableRow::TableRow()
{
}

TableRow::TableRow(qreal flex)
{
    _flex = flex;
    _autoFlex = false;
}

TableRow::TableRow(QVector<TableCell *> cells)
{
    _cells = cells;
}

void TableRow::draw(QPainter *painter)
{
    _applyFlex();
    for (auto cell : qAsConst(_cells))
    {
        cell->draw(painter);
    }
}

void TableRow::addCell(TableCell *cell)
{
    _cells.append(cell);
}

void TableRow::removeCell(unsigned int cellIndex)
{
    auto cell = _cells[cellIndex];
    _cells.removeAt(cellIndex);
    delete cell;
}

qreal TableRow::flex() const
{
    return _flex;
}

void TableRow::setFlex(qreal newFlex)
{
    _flex = newFlex;
    _autoFlex = false;
}

bool TableRow::autoFlex() const
{
    return _autoFlex;
}

void TableRow::setAutoFlex(bool newAutoFlex)
{
    _autoFlex = newAutoFlex;
}

void TableRow::_applyFlex()
{
    qreal totalFixedFlexValue = 0;
    unsigned int fixedFlexCount = 0;

    for (const auto &cell : qAsConst(_cells))
    {
        if (!cell->autoFlex())
        {
            totalFixedFlexValue += cell->flex();
            fixedFlexCount++;
        }
    }
    qreal remainingFlexPerCell = (1 - totalFixedFlexValue) / (_cells.size() - fixedFlexCount);

    qreal currentX = 0;
    for (auto i = 0; i < _cells.size(); i++)
    {
        auto cell = _cells[i];

        if (cell->autoFlex())
        {
            cell->setFlex(remainingFlexPerCell);
        }

        auto cellWidth = _rect.width() * cell->flex();
        auto newRect = QRectF(_rect.topLeft() + QPointF(currentX, 0), QSizeF(cellWidth, _rect.height()));
        cell->setRect(newRect);

        currentX += cellWidth;
    }
}

const QRectF &TableRow::rect() const
{
    return _rect;
}

void TableRow::setRect(const QRectF &newRect)
{
    _rect = QRectF(newRect);
}
