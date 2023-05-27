#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <QPixmap>

class ImageHandler
{
public:
    ImageHandler();

    static QPixmap getDocInfo(const QString path);
private:

    static QPixmap MatToPixmap(cv::Mat src);
    static cv::Mat QPixmapToMat(const QPixmap& pixmap);
};

#endif // IMAGEHANDLER_H
