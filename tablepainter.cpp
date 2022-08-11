#include "tablepainter.h"

TablePainter::TablePainter()
{

}

TablePainter::TablePainter(QRectF rect)
{
    _rect = rect;
}

void TablePainter::draw(QPainter *painter) const
{
    _applyFlex();
    for (auto row : qAsConst(_rows))
    {
        row->draw(painter);
    }
}

void TablePainter::addRow(TableRow *row)
{
    _rows.append(row);
}

void TablePainter::removeRow(unsigned int rowIndex)
{
    _rows.removeAt(rowIndex);
}

qreal TablePainter::flex() const
{
    return _flex;
}

void TablePainter::setFlex(qreal newFlex)
{
    _flex = newFlex;
}

const QRectF &TablePainter::rect() const
{
    return _rect;
}

void TablePainter::setRect(const QRectF &newRect) const
{
    _rect = QRectF(newRect);
}

void TablePainter::_applyFlex() const
{
    qreal totalFixedFlexValue = 0;
    unsigned int fixedFlexCount = 0;

    for (const auto &row : qAsConst(_rows))
    {
        if (!row->autoFlex())
        {
            totalFixedFlexValue += row->flex();
            fixedFlexCount++;
        }
    }
    qreal remainingFlexPerCell = (1 - totalFixedFlexValue) / (_rows.size() - fixedFlexCount);

    qreal currentY = 0;
    for (auto i = 0; i < _rows.size(); i++)
    {
        auto row = _rows[i];

        if (row->autoFlex())
        {
            row->setFlex(remainingFlexPerCell);
        }

        auto rowHeight = _rect.height() * row->flex();
        auto newRect = QRectF(_rect.topLeft() + QPointF(0, currentY), QSizeF(_rect.width(), rowHeight));
        row->setRect(newRect);

        currentY += rowHeight;
    }
}
