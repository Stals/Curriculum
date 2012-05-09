#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "curriculum.h"

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
    void on_pushButton_clicked();
    void addSubjectsToTree(int year);

private:
    Ui::MainWindow *ui;
    std::vector<Subject *> subjects;
};

#endif // MAINWINDOW_H
