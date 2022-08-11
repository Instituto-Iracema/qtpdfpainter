#ifndef ABSTRACTPAINTER_H
#define ABSTRACTPAINTER_H

#include <QPainter>

class AbstractPainter
{
public:
    AbstractPainter() {};

    virtual ~AbstractPainter() = default;

    virtual void draw(QPainter *painter) const = 0;
    virtual const QRectF &rect() const = 0;
    virtual void setRect(const QRectF &newRect) const = 0;
};

#endif // ABSTRACTPAINTER_H
