#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "enterchar.h"
#include <vector>
#include <fstream>
#include <thread>
#include <QMenu>
#include <pthread.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void getChar(char a, int N);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::function<void(void)> FuncName = [this](){DataNewer();};
private slots:
    void CreateThread2(int n);
    void DataNewerClock(std::function<void(void)> a);
    void addTableInfo(int i);
    void DataNewer();
     int getTime(int i);
   std::string beautifulShow(int i);
    void killPr();
    void idlePr();
    void belowPr();
    void normPr();
    void highPr();
    void realtPr();
    void detachPr();
    int findProcessByID(int Row);
    void on_startAll_clicked();
    void on_chb1_stateChanged(int arg1);
    void on_chb2_stateChanged(int arg1);
    void on_chb4_stateChanged(int arg1);
    void on_chb8_stateChanged(int arg1);
    void on_chb16_stateChanged(int arg1);
    void on_cteateThread_clicked();

private:
    Ui::MainWindow *ui;
     EnterChar* ec;
    QMenu *contextMenu;
    QAction *killAct;
    QAction *detachAct;
    QMenu *changePrio;
    QAction *idleAct;
    QAction *normalAct;
    QAction *belowAct;
    QAction *realtAct;
    QAction *highAct;
    int rightClickedRow;
    void showContextMenu(const QPoint &pos);
    void setThreadPriority(pthread_t thread, int priority);
signals:
    void returnN(int n);
};
#endif // MAINWINDOW_H
