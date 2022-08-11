#ifndef TABLECELLCONTENT_H
#define TABLECELLCONTENT_H

#include <QPainter>

class TableCellContent
{
public:
    TableCellContent();

    virtual ~TableCellContent() = default;

    virtual void draw(QPainter *painter, QRectF rect) = 0;
};

#endif // TABLECELLCONTENT_H
