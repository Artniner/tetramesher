#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

#include<QDebug>

//char TETGEN_SWITCHES[]    = "pq1.414a0.002";
QString quality = "pq1.414a0.002";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 固定窗口大小
    setStatus("Initializing");
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags()&~Qt::WindowMinimizeButtonHint);
    setFixedSize(this->width(),this->height());
    setStatus("Initialize complete");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_openfile_clicked()
{
    // 设置文件类型
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Model Files(*.stl *.off *.ply)"));
    if(!filePath.isEmpty()){
        setStatus("loading model");
        loadModel();    //加载模型文件
    }else{
        setStatus("open failed");
        alert("error: cannot open file!");
    }
}

// 加载模型
void MainWindow::loadModel(){
    QString extension = getExtension();

    // 使用tetgen加载模型
    bool fileload;
    if(extension == "stl"){
        fileload = input.load_stl(filePath.toLatin1().data());
    }else if(extension == "off"){
        fileload = input.load_off(filePath.toLatin1().data());
    }else if(extension == "mtr"){
        fileload = input.load_mtr(filePath.toLatin1().data());
    }else if(extension == "ply"){
        fileload = input.load_ply(filePath.toLatin1().data());
    }else{
        alert("error: cannot open "+extension+" file");
    }
    if(fileload){
        setSaveEnable();
        setStatus("file loaded");
    }else{
        alert("error: cannot open file: " + filePath);
    }
}

// 获取扩展名
QString MainWindow::getExtension(){
    QStringList list = filePath.split(".");
    return list.last();
}

// 使保存按钮可用
void MainWindow::setSaveEnable()
{
    ui->chk_preserve->setEnabled(true);
    ui->chk_nodes->setEnabled(true);
    ui->chk_faces->setEnabled(true);
    ui->chk_elements->setEnabled(true);
    ui->chk_poly->setEnabled(true);
    ui->chk_edges->setEnabled(true);
    ui->chk_neighbors->setEnabled(true);

    ui->btn_outFile->setEnabled(true);
}

// 保存文件
void MainWindow::on_btn_outFile_clicked()
{
    setStatus(tr("Saving...."));

    char *parm = setCommandLine();
    qDebug()<<parm;
    tetrahedralize(parm, &input, &output);

    QString fileName = getFileName();
    outputFile(fileName);

    alert("file saved");
    setStatus("file saved");
}

// 获取文件名
QString MainWindow::getFileName()
{
    QStringList list = filePath.split("/");
    QString fullName = list.last();
    list = fullName.split(".");
    return list.first();
}

// 设置四面体化参数
char* MainWindow::setCommandLine()
{
    QString command = quality;
    if(ui->chk_preserve->isChecked()) command+="Y";
    if(ui->chk_edges->isChecked()) command+="e";
    if(ui->chk_neighbors->isChecked()) command+="n";

    return strdup(command.toLatin1().data());
}

// 保存文件
void MainWindow::outputFile(QString fileName)
{
    if(ui->chk_nodes->isChecked()) output.save_nodes(fileName.toLatin1().data());
    if(ui->chk_faces->isChecked()) output.save_faces(fileName.toLatin1().data());
    if(ui->chk_elements->isChecked()) output.save_elements(fileName.toLatin1().data());
    if(ui->chk_poly->isChecked()) output.save_poly(fileName.toLatin1().data());
    if(ui->chk_edges->isChecked()) output.save_edges(fileName.toLatin1().data());
    if(ui->chk_neighbors->isChecked()) output.save_neighbors(fileName.toLatin1().data());
}


// 警告框
void MainWindow::alert(QString message)
{
    QMessageBox msgbox;
    msgbox.setText(message);
    msgbox.exec();
}

// 设置状态栏
void MainWindow::setStatus(QString status)
{
    statusBar()->showMessage(status);
}
