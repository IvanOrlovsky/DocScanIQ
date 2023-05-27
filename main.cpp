#include "mainwindow.h"

#include <QApplication>

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPushButton>

class ImageDialog : public QDialog
{
public:
    ImageDialog(const QPixmap& image, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        // Создание метки для отображения картинки
        QLabel* imageLabel = new QLabel(this);
        imageLabel->setPixmap(image);
        imageLabel->setAlignment(Qt::AlignCenter);

        // Создание кнопки для закрытия окна
        QPushButton* closeButton = new QPushButton("Понятно", this);

        // Создание шрифта Montserrat
        QFont font("Montserrat", 12, QFont::Bold);

        // Установка шрифта для кнопки
        closeButton->setFont(font);

        // Задний фон кнопки
        QString styleSheet = "QPushButton {"
                             "background-color: #2F2F2F;"
                             "border-style: outset;"
                             "border-width: 2px;"
                             "border-radius: 5px;"
                             "border-color: #1F1F1F;"
                             "color: white;"
                             "padding: 6px;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #3F3F3F;"
                             "}"
                             "QPushButton:pressed {"
                             "background-color: #1F1F1F;"
                             "}";

        closeButton->setStyleSheet(styleSheet);

        connect(closeButton, &QPushButton::clicked, this, &ImageDialog::close);

        // Расположение метки и кнопки в окне
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(imageLabel);
        layout->addWidget(closeButton);

        // Установка расположения в окне
        setLayout(layout);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QPixmap image("C://test//start.png");
    // Создание всплывающего окна с картинкой
    ImageDialog imageDialog(image, &w);
    imageDialog.exec();

    w.show();
    return a.exec();
}


