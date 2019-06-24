#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tetgen.h"
#include <QMessageBox>
#include <QFileDialog>

#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_openfile_clicked()
{
    // 设置文件类型
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Model Files(*.stl *.off *.ply)"));
    if(!path.isEmpty()){
        loadModel(path);    //加载模型文件
    }else{
        alert("error: cannot open file!");
    }
}

// 加载模型
void MainWindow::loadModel(QString fileName){
    QString extension = getExtension(fileName);

    // 使用tetgen加载模型
    tetgenio input;
    bool fileload;
    if(extension == "stl"){
        fileload = input.load_stl(fileName.toLatin1().data());
        qDebug()<<"stl";
    }else if(extension == "off"){
        fileload = input.load_off(fileName.toLatin1().data());
        qDebug()<<"off";
    }else if(extension == "mtr"){
        fileload = input.load_mtr(fileName.toLatin1().data());
        qDebug()<<"mtr";
    }else if(extension == "ply"){
        fileload = input.load_ply(fileName.toLatin1().data());
        qDebug()<<"ply";
    }else{
        alert("error: cannot open "+extension+" file");
    }
    if(fileload){
        qDebug()<<"file loaded";
    }else{
        alert("error: cannot open file: " + fileName);
    }
}

// 获取扩展名
QString MainWindow::getExtension(QString filePath){
    QStringList list = filePath.split(".");
    return list.last();
}

// 警告框
void MainWindow::alert(QString message)
{
    QMessageBox msgbox;
    msgbox.setText(message);
    msgbox.exec();
}


