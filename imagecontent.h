#ifndef IMAGECONTENT_H
#define IMAGECONTENT_H

#include "tablecellcontent.h"

#include <QImage>

class ImageContent : public TableCellContent
{
public:
    ImageContent(QImage image, QMarginsF margins = QMarginsF(), QRectF cropRect = QRectF());

    void draw(QPainter *painter, QRectF rect);
private:
    QImage _image;
    QMarginsF _margins = QMarginsF();
    QRectF _cropRect = QRectF();
};

#endif // IMAGECONTENT_H
