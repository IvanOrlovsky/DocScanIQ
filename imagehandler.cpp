#include "imagehandler.h"

ImageHandler::ImageHandler()
{

}


QPixmap ImageHandler::getDocInfo(const QString path)
{
    cv::Mat inputImage = cv::imread(path.toStdString(), 1);
        cv::Mat resizedImage;
        cv::resize(inputImage, resizedImage, cv::Size(MatToPixmap(inputImage).width() * 0.8, MatToPixmap(inputImage).height() * 0.8), 0, 0, cv::INTER_CUBIC);

        cv::Mat sharpened;
        cv::GaussianBlur(resizedImage, sharpened, cv::Size(0, 0), 3);
        cv::Mat sharpened2;
        cv::addWeighted(resizedImage, 2.5, sharpened, -0.5, 0, sharpened2);

        cv::Mat image = sharpened2.clone(); // Создание копии исходного изображения

        cv::Mat grayImage;
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

        double alpha = 1.5; // Множитель контрастности
        int beta = 0; // Сдвиг яркости

        cv::Mat adjustedImage;
        grayImage.convertTo(adjustedImage, -1, alpha, beta);

        cv::Mat binaryImage;
        cv::threshold(adjustedImage, binaryImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        cv::Mat edges;
        cv::Canny(binaryImage, edges, 50, 150);

        cv::Mat closedEdges;
        cv::morphologyEx(edges, closedEdges, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)));


        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(closedEdges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Проход по контурам и выбор наибольшего контура
        double maxArea = 0.0;
        std::vector<cv::Point> largestContour;
        for (const auto& contour : contours)
        {
            double area = cv::contourArea(contour);
            if (area > maxArea)
            {
                maxArea = area;
                largestContour = contour;
            }
        }

        std::vector<cv::Point> approxContour;
        cv::approxPolyDP(largestContour, approxContour, 10, true);

        cv::Rect boundingRect = cv::boundingRect(largestContour);

        cv::Mat croppedImage = image(boundingRect);
        std::string outputPath = "C://test//prework.jpg";
        cv::imwrite(outputPath, binaryImage);
        //return MatToPixmap(croppedImage);

        int x = boundingRect.x;
        int y = boundingRect.y;
        int width = boundingRect.width;
        int height = boundingRect.height;

        cv::Rect regionAboveContour(x, 0, width, y + height);
        cv::Mat regionImage = image(regionAboveContour);
        return MatToPixmap(regionImage);
}


QPixmap ImageHandler::MatToPixmap(cv::Mat src)
{
    QImage::Format format=QImage::Format_Grayscale8;
    int bpp=src.channels();
    if(bpp==3)format=QImage::Format_RGB888;
    QImage img(src.cols,src.rows,format);
    uchar *sptr,*dptr;
    int linesize=src.cols*bpp;
    for(int y=0;y<src.rows;y++)
    {
        sptr=src.ptr(y);
        dptr=img.scanLine(y);
        memcpy(dptr,sptr,linesize);
    }
    if(bpp==3)return QPixmap::fromImage(img.rgbSwapped());
    return QPixmap::fromImage(img);
}

cv::Mat ImageHandler::QPixmapToMat(const QPixmap &pixmap)
{
    QImage image = pixmap.toImage();

    cv::Mat mat;
    if (image.format() == QImage::Format_RGB888)
    {
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine());
    }
    else if (image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_ARGB32_Premultiplied)
    {
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, image.bits(), image.bytesPerLine());
    }
    else
    {
        QImage converted = image.convertToFormat(QImage::Format_ARGB32);
        mat = cv::Mat(converted.height(), converted.width(), CV_8UC4, converted.bits(), converted.bytesPerLine());
    }

    cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGR);

    return mat;
}
