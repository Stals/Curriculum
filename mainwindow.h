#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "curriculum.h"

#include <QMainWindow>
#include <QTreeWidget>
#include <string>

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
    //converts QString to std::wstring
    std::wstring qs2ws(const QString& str);
    // Очишает поля на форме
    void clearAllEdits();

    void on_pushButton_clicked();
    void addSubjectsToTree(int year);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    bool firstTime;
    Curriculum *curriculum;
    std::vector<Subject *> subjects;
};

#endif // MAINWINDOW_H
