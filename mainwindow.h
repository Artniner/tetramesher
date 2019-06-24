#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

private:
    void loadModel(QString fileName);
    QString getExtension(QString filePath);

    void alert(QString message);
};

#endif // MAINWINDOW_H
