#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QFile>
#include <QVBoxLayout>
#include <QPushButton>
#include <QClipboard>

#include <Windows.h>

#include "wordhintengine.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void inputTest(QString str);

private:
    QString currentStr;
    QString letters = "abcdefghijklmnopqrstuvwxyz";

    QList<QPushButton*> hintButtons;
    int currentBtnIndex = 0;

    WordHintEngine wordHintEngine;

    void Clear();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
