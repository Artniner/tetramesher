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

    void on_sld_quality_valueChanged(int value);

    void on_spin_quality_valueChanged(double value);

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
    char* setCommandLine();
    void setSaveEnable();
    void outputFile(QString fileName);

    void alert(QString message);
    void setStatus(QString status);
};

#endif // MAINWINDOW_H
