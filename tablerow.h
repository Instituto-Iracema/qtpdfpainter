#ifndef TABLEROW_H
#define TABLEROW_H

#include "tablecell.h"

class TableRow
{
public:
    TableRow();
    TableRow(qreal flex);
    TableRow(QVector<TableCell *> cells);

    void draw(QPainter *painter);

    void addCell(TableCell *cell);
    void removeCell(unsigned int cellIndex);

    qreal flex() const;
    void setFlex(qreal newFlex);

    bool autoFlex() const;
    void setAutoFlex(bool newAutoFlex);

    const QRectF &rect() const;
    void setRect(const QRectF &newRect);

private:
    void _applyFlex();

    qreal _flex = 0;
    bool _autoFlex = true;

    QRectF _rect  = QRectF();

    QVector<TableCell*> _cells;
};

#endif // TABLEROW_H
