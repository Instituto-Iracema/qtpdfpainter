#ifndef TABLECELL_H
#define TABLECELL_H

#include "tablecellcontent.h"
#include <QPainter>


enum TableCellWalls {
    Top = 0b0001,
    Right = 0b0010,
    Bottom = 0b0100,
    Left = 0b1000,
    All = Top | Right | Bottom | Left,
    Horizontal =  Top | Bottom,
    Vertical = Left | Right,
    None = 0,
};

class TableCell
{
public:
    TableCell(QColor backgroundColor = Qt::transparent, qreal lineWidth = 1, QColor lineColor = Qt::black, TableCellWalls walls = All);
    TableCell(qreal flex, QColor backgroundColor = Qt::transparent, qreal lineWidth = 1, QColor lineColor = Qt::black, TableCellWalls walls = All);

    void addContent(TableCellContent *content);
    void removeContent(unsigned int contentIndex);

    void draw(QPainter *painter);

    qreal lineWidth() const;
    void setLineWidth(qreal newLineWidth);

    const QColor &lineColor() const;
    void setLineColor(const QColor &newLineColor);

    TableCellWalls walls() const;
    void setWalls(TableCellWalls newWalls);

    qreal flex() const;
    void setFlex(qreal newFlex);

    const QRectF &rect() const;
    void setRect(const QRectF &newRect);

    bool autoFlex() const;
    void setAutoFlex(bool newAutoFlex);

    const QColor &backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

private:
    void _drawWalls(QPainter *painter);
    void _paintBackground(QPainter *painter);

    qreal _lineWidth = 1;
    QColor _lineColor = Qt::black;
    QColor _backgroundColor = Qt::transparent;
    TableCellWalls _walls = All;
    qreal _flex = 0;
    QRectF _rect = QRectF();
    bool _autoFlex = true;
    QList<TableCellContent*> _contents;
};

#endif // TABLECELL_H
