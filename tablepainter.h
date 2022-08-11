#ifndef TABLEPAINTER_H
#define TABLEPAINTER_H

#include "tablerow.h"
#include "abstractpainter.h"

class TablePainter : public AbstractPainter
{
public:
    TablePainter();
    TablePainter(QRectF rect);

    void draw(QPainter *painter) const override;

    void addRow(TableRow *row);
    void removeRow(unsigned int rowIndex);

    qreal flex() const;
    void setFlex(qreal newFlex);

    const QRectF &rect() const override;
    void setRect(const QRectF &newRect) const override;

private:
    qreal _flex = 0;
    QList<TableRow *> _rows;

    mutable QRectF _rect = QRectF();

    void _applyFlex() const;
};

#endif // TABLEPAINTER_H
