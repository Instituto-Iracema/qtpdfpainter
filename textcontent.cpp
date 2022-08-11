#include "textcontent.h"
#include <QDebug>
#include <QLabel>

TextContent::TextContent(QString text, Qt::Alignment alignmentFlags, QMarginsF margins, bool multiLine, QColor textColor, QFont font) : TableCellContent()
{
    _text = text;
    _font = font;
    _margins = margins;
    _textColor = textColor;
    _textOption = QTextOption(alignmentFlags);
    _multiLine = multiLine;
}

TextContent::TextContent(QString text, QTextOption textOption, QMarginsF margins, bool multiLine, QColor textColor, QFont font) : TableCellContent()
{
    _text = text;
    _font = font;
    _margins = margins;
    _textColor = textColor;
    _textOption = textOption;
    _multiLine = multiLine;
}

void TextContent::draw(QPainter *painter, QRectF rect)
{
    QMarginsF realMargins(_margins.left() * rect.width(),
                          _margins.top() * rect.height(),
                          _margins.right() * rect.width(),
                          _margins.bottom() * rect.height());
    QRectF finalRect = rect.marginsRemoved(realMargins);

    if(!_multiLine){
        _font.setPixelSize(finalRect.height());
    } else {
        QRectF boundingRect = painter->boundingRect(rect, Qt::TextWrapAnywhere, _text);
        rect = boundingRect;
    }

    painter->setFont(_font);
    painter->setBrush(QBrush(_textColor));
    painter->drawText(finalRect, _text, _textOption);
}
