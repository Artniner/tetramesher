#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

#include<QDebug>

/**
    网格质量参数：
    -p	四面体化一个分段线性复形(PLC)。
    -Y	保留输入曲面网格(不修改它)。
    -r	重构生成的网格。
    -q	细化网格(提高网格质量)。
    -R	网格粗化(减少网格元素)。
    -A	分配范围属性。
    -a	应用最大四面体体积约束。
    -m	应用网格大小函数。
    -i	插入一个附加点列表。
    -O	指定网格优化的级别。
    -S	指定添加点的最大数量。
    -T	设置共面测试的公差(默认为10 - 8)。
    -X	禁止使用精确的算术。
    -M	不合并共面面片或非常接近的顶点。
    -w	生成加权Delaunay(正则)三角剖分。
    -c	保留PLC的凸包。
    -d	检测PLC表面的自交点。
    -z	将所有输出项从零开始编号。
    -f 	将所有面输出到.face文件。
    -e	将所有边输出到.edge文件。
    -n	将相邻四面体输出到.neigh文件。
    -v	将Voronoi图输出到文件中。
    -g	将mesh输出到.mesh文件中供Medit查看。
    -k	输出mesh到.vtk文件，通过Paraview查看。
    -J	不丢弃输出到.node文件中未使用的顶点。
    -B	不输出边界信息。
    -N	不输出.node文件。
    -E	不输出.ele文件。
    -F	不输出.face和.edge文件。
    -I	抑制网格迭代数。
    -C	检查最终网格的一致性。
    -Q	静默:除错误外没有终端输出。
    -V	详情:详细的信息，更多的终端输出。
    -h	帮助:使用TetGen的简要说明
**/

//char TETGEN_SWITCHES[]    = "pq1.414a0.002";
QString quality = "p";

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

//-------------------------------------加载文件-----------------------------------------
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

    ui->lbl_quality->setEnabled(true);
    ui->sld_quality->setEnabled(true);
    ui->spin_quality->setEnabled(true);

    ui->btn_outFile->setEnabled(true);
}

//-------------------------------------输出文件-----------------------------------------
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
    command = command + "q" + ui->spin_quality->text();
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


//--------------------------------------工具------------------------------------------
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


//-------------------------------------信号槽------------------------------------------
// 加载按钮
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

// 保存按钮
void MainWindow::on_btn_outFile_clicked()
{
    setStatus(tr("Saving...."));

    char *parm = setCommandLine();
    tetrahedralize(parm, &input, &output);
    qDebug()<<parm;

    QString fileName = getFileName();
    outputFile(fileName);

    alert("file saved");
    setStatus("file saved");
}

// 质量滑块
void MainWindow::on_sld_quality_valueChanged(int value)
{
    double val = (double)value/100;
    ui->spin_quality->setValue(val);
}

void MainWindow::on_spin_quality_valueChanged(double value)
{
    ui->sld_quality->setValue(value*100);
}
