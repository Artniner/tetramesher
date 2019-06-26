#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tetgen.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_openfile_clicked();

    void on_btn_outFile_clicked();

private:
    Ui::MainWindow *ui;


private:
    QString filePath;
    tetgenio input;
    tetgenio output;
private:
    void loadModel();
    QString getExtension();
    QString getFileName();
    void setSaveEnable();

    void alert(QString message);
    void setStatus(QString status);
};

#endif // MAINWINDOW_H
