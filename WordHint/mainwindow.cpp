#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "wordhintengine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);

    this->wordHintEngine.UploadDictionary();

    for(int i = 0; i < 5; i++){
        QPushButton *btn = new QPushButton("", this);
        btn->setStyleSheet("border: 1px solid black");
        this->ui->horizontalLayout->addWidget(btn);
        btn->hide();
        this->hintButtons.append(btn);
    }

    this->hintButtons[this->currentBtnIndex]->setStyleSheet("border: 1px solid green;");
}

void MainWindow::inputTest(QString str)
{
    if(str == "[backspace]"){
        this->currentStr.chop(1);
    }
    else if(str == "[control]"){
        int activeBtnLength = 0;
        for(int i = 0; i < this->hintButtons.length(); i++){
            if(!this->hintButtons[i]->isHidden()) activeBtnLength++;
        }

        this->currentBtnIndex = this->currentBtnIndex >= activeBtnLength-1 ? 0 : this->currentBtnIndex + 1;

        for(int i = 0; i < this->hintButtons.length(); i++){
            if(this->currentBtnIndex == i){
                this->hintButtons[i]->setStyleSheet("border: 1px solid green");
                continue;
            }

            this->hintButtons[i]->setStyleSheet("border: 1px solid black");
        }

        return;
    }
    else if(str == "[alt_control]"){
        QString hintWord = this->hintButtons[this->currentBtnIndex]->text();
        QString newWord = hintWord.right(hintWord.length() - this->currentStr.length());

        QClipboard *clipBoard = QGuiApplication::clipboard();
        clipBoard->setText(newWord);

        this->Clear();

        return;
    }
    else if(this->letters.contains(str.toLower())){
        this->currentStr.append(str);
    }
    else{
        this->currentStr.clear();
    }

    qDebug() << this->currentStr;

    this->ui->userInputLabel->setText(this->currentStr);

    QList<QString> hintWords = this->wordHintEngine.getHintWords(this->currentStr);

    for(int i = 0; i < this->hintButtons.length(); i++){
        if(i < hintWords.length()){
            this->hintButtons[i]->show();
            this->hintButtons[i]->setText(hintWords[i]);
        }else{
            this->hintButtons[i]->hide();
        }
    }
}

void MainWindow::Clear(){
    this->currentBtnIndex = 0;
    for(int i = 0; i < this->hintButtons.length(); i++){
        this->hintButtons[i]->hide();
    }
    this->ui->userInputLabel->setText("");
    this->currentStr = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}


