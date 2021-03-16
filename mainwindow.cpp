#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->findSubstringButton, &QPushButton::clicked, this, &MainWindow::findSubstring);
    connect(ui->textFromFileButton, &QPushButton::clicked, this, &MainWindow::fromFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::findSubstring()
{
    //Валидация вхожных строк
    if(ui->textEdit->toPlainText().isEmpty())
    {
        ui->resultTextBrowser->setText("Не введён текст!");
        return;
    }

    if(ui->substringTextEdit->toPlainText().isEmpty())
    {
        ui->resultTextBrowser->setText("Не введена подстрока!");
        return;
    }

    QString text = ui->textEdit->toPlainText(); // считываем текст
    QString substring = ui->substringTextEdit->toPlainText(); // считываем подстроку
    int M = substring.length(); //M - длина подстроки

    std::vector<int> *prefixFunction = createPrefixFunction(substring); //Строим префикс-функцию по подстроке

    int charactersViewed = 0;
    bool found, foundAtLeastOnce = false;
    while(!text.isEmpty())
    {
        found = true;
        for(int i = 0; i < M; i++)
        {
            if(text[i] != substring[i])
            {
                text.remove(0, (*prefixFunction).at(i)); //Смещаемся по тексту на значение префикс-функции
                charactersViewed += (*prefixFunction).at(i);
                found = false;
                break;
            }
        }
        if(found)
        {
            foundAtLeastOnce = true;
            ui->resultTextBrowser->append(QString("Подстрока содержится в строке начиная с позиции %1.").arg(charactersViewed));
            text.remove(0, M);
        }
    }
    if(!foundAtLeastOnce)
    {
        ui->resultTextBrowser->append("Подстрока не найдена в строке.");
    }
}

std::vector<int> *MainWindow::createPrefixFunction(QString substring)
{
    int M = substring.length(); //M - длина подстроки

    static std::vector<int> prefixFunction; //создаём массив для префикс-функции
    prefixFunction.reserve(M);
    prefixFunction.push_back(1); //если не совпал первый элемент, то сдвинуться на 1

    QString resultString = "Префиксная функция: ";

    for(int i = 1; i < M; i++)
    {
        if(substring[i] != substring[0]) //если не совпадает с первой буквой, то сдвинуть подстроку так, чтобы первая буква подстроки встала на место несовпавшей
        {
            prefixFunction.push_back(i);
            resultString.append(QString::number(i));
        }
        else //если совпадает с первой буквой, то смещаем подстроку за несовпавшую букву
        {
            prefixFunction.push_back(i + 1);
            resultString.append(QString::number(i + 1));
        }
        (i != M - 1) ? resultString.append(", ") : resultString.append(".");
        ui->resultTextBrowser->setText(resultString);
    }
    return &prefixFunction;
}

void MainWindow::fromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть текстовый файл", "", "Text files (*.txt)");
    QFile textFile(fileName);
    textFile.open(QIODevice::ReadOnly);
    QByteArray a = textFile.readAll();
    ui->textEdit->setText(QString(a));
    textFile.close();
}
