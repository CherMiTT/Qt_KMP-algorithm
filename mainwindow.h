#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void findSubstring();
    void fromFile();

private:
    Ui::MainWindow *ui;

    std::vector<int> *createPrefixFunction(QString substring);
};
#endif // MAINWINDOW_H
