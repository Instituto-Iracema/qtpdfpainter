#include "imagecontent.h"

ImageContent::ImageContent(QImage image, QMarginsF margins, QRectF cropRect)
{
    _image = image;
    _margins = margins;
    _cropRect = cropRect;
}

void ImageContent::draw(QPainter *painter, QRectF rect)
{
    QMarginsF realMargins(_margins.left() * rect.width(),
                          _margins.top() * rect.height(),
                          _margins.right() * rect.width(),
                          _margins.bottom() * rect.height());
    if (_cropRect.isEmpty())
    {
        painter->drawImage(rect.marginsRemoved(realMargins), _image);
    } else
    {
        painter->drawImage(rect.marginsRemoved(realMargins), _image, _cropRect);
    }
}
