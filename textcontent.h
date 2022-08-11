#ifndef TEXTCONTENT_H
#define TEXTCONTENT_H

#include <QFont>

#include "tablecellcontent.h"

class TextContent : public TableCellContent
{
public:
    TextContent(QString text, Qt::Alignment alignmentFlags = Qt::AlignCenter, QMarginsF margins = QMarginsF(), bool multiLine = false, QColor textColor = Qt::black, QFont font = QFont());
    TextContent(QString text, QTextOption textOption = QTextOption(), QMarginsF margins = QMarginsF(), bool multiLine = false, QColor textColor = Qt::black, QFont font = QFont());

    void draw(QPainter *painter, QRectF rect);
private:
    bool _multiLine = false;
    QString _text = "";
    QFont _font = QFont();
    QMarginsF _margins = QMarginsF();
    QTextOption _textOption = QTextOption();
    QColor _textColor = QColor(Qt::black);
};

#endif // TEXTCONTENT_H
