#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iomanip>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <shared_mutex>
#include <string>
#include <pthread.h>
#include <QTimer>
#include <sched.h>
#include <functional>
#include <signal.h>
#include<QMenu>
#include <QProcess>
std::vector<pthread_t> arr;
std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> times;
std::vector<char> ch = {(char)0,(char)0,(char)0,(char)0,(char)0};
std::vector<int> deathcheck;
std::vector<int> tids;
typedef std::shared_mutex Lock;
pthread_rwlock_t WriteLock;
pthread_rwlock_t ReadLock;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_spinlock_t spinlock;
int done = 0;
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //pthread_rwlock_init(&rwlock, nullptr);
    DataNewerClock(FuncName);
    ec = new EnterChar(0);
     connect(ec, &EnterChar::returnChar, this, &MainWindow::getChar);
    ui->TableView->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu = new QMenu(this);
    killAct = new QAction("Kill", this);
    detachAct = new QAction("Detach", this);

    changePrio = new QMenu("Change priority");
    idleAct = new QAction("IDLE_PRIORITY_CLASS", this);
    belowAct = new QAction("BELOW_NORMAL_PRIORITY_CLASS", this);
    normalAct = new QAction("NORMAL_PRIORITY_CLASS", this);
    highAct = new QAction("HIGH_PRIORITY_CLASS", this);
    realtAct = new QAction("REAL_TIME_PRIORITY_CLASS", this);

    contextMenu->addAction(killAct);
    contextMenu->addAction(detachAct);
    contextMenu->addMenu(changePrio);
    changePrio->addAction(idleAct);
    changePrio->addAction(belowAct);
    changePrio->addAction(normalAct);
    changePrio->addAction(highAct);
    changePrio->addAction(realtAct);


    connect(ui->TableView, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(detachAct, &QAction::triggered, this, &MainWindow::detachPr);
    connect(killAct, &QAction::triggered, this, &MainWindow::killPr);
    connect(idleAct, &QAction::triggered, this, &MainWindow::idlePr);
    connect(belowAct, &QAction::triggered, this, &MainWindow::belowPr);
    connect(normalAct, &QAction::triggered, this, &MainWindow::normPr);
    connect(highAct, &QAction::triggered, this, &MainWindow::highPr);
    connect(realtAct, &QAction::triggered, this, &MainWindow::realtPr);

    pthread_rwlock_init(&WriteLock, NULL);
    pthread_spin_init(&spinlock, 0);
}
struct ThreadData{
    char a;
    int N;
    int start;
};

MainWindow::~MainWindow()
{
    for(int i = 0; i < arr.size(); i++){
        pthread_join(arr[i], NULL);
    }
    //pthread_rwlock_destroy(&rwlock);
    delete ui;
}


void *ThreadFunction_2(void* arg){
    int z = deathcheck.size();
    deathcheck.push_back(1);
    int oldType;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldType);
         tids.push_back(pthread_self());
    struct ThreadData* data = (struct ThreadData*)arg;
         char a = data->a;
    int N = data->N;
         int start = data->start;
sleep(10);
    std::ifstream fp("//home//houston//laab6//Laab6_2//lab6_2//FileForReading");
    if (!fp.is_open()) {
        std::cerr << "Can't open file!" << std::endl;
    } else {
    int count = 0;
    fp.seekg(start);
    int n = 0;

    std::string line;
    while (std::getline(fp, line)) {
        for (char c : line) {
            if(a == c){
                count++;
            }
        }
        if(n == N){break;}
        n++;
    }

    fp.close();

    std::cout << "Count of '" << a << "' symbols in range: " << count << std::endl;
    }
         deathcheck[z] = 0;
}
void *ThreadFunction_rwlock(void* arg) {
    int z = deathcheck.size();
    deathcheck.push_back(1);
    //pthread_rwlock_rdlock(&ReadLock);
    pthread_spin_lock(&spinlock);
    int oldType;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldType);
    tids.push_back(pthread_self());
      //  pthread_rwlock_rdlock(&rwlock);
     struct ThreadData* data = (struct ThreadData*)arg;
    char a = data->a;
    int N = data->N;
    int start = data->start;

      std::ifstream fp("//home//houston//laab6//Laab6_2//lab6_2//FileForReading");
    if (!fp.is_open()) {
    //pthread_rwlock_unlock(&ReadLock);
    } else {
      int count = 0;
      fp.seekg(start);
      int n = 0;

      std::string line;
      while (std::getline(fp, line)) {
        for (char c : line) {
            if(a == c){
                count++;
            }
        }
        if(n == N){break;}
        n++;
      }
 //    pthread_rwlock_wrlock(&WriteLock);
      fp.close();
  //  pthread_rwlock_unlock(&WriteLock);
      std::cout << "Count of '" << a << "' symbols in range: " << count << std::endl;
    }
     deathcheck[z] = 0;
      pthread_spin_unlock(&spinlock);
     //pthread_rwlock_unlock(&ReadLock);
     //pthread_rwlock_unlock(&rwlock);
}
void *ThreadFunction_condvar(void* arg){
     int z = deathcheck.size();
     deathcheck.push_back(1);

     pthread_mutex_lock(&m);
     int oldType;
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldType);
     tids.push_back(pthread_self());
      struct ThreadData* data = (struct ThreadData*)arg;
     char a = data->a;
     int N = data->N;
     int start = data->start;
      std::ifstream fp("//home//houston//laab6//Laab6_2//lab6_2//FileForReading");
     if (!fp.is_open()) {
      std::cerr << "Can't open file!" << std::endl;
     } else {
      int count = 0;
      fp.seekg(start);
      int n = 0;

      std::string line;
      while (std::getline(fp, line)) {
        for (char c : line) {
            if(a == c){
                count++;
            }
        }
        if(n == N){break;}
        n++;
      }

      fp.close();

      std::cout << "Count of '" << a << "' symbols in range: " << count << std::endl;
     }
     deathcheck[z] = 0;
     done = 1;
      pthread_cond_signal(&c);
     pthread_mutex_unlock(&m);
}

void *ThreadFunction(void* arg) {
    int z = deathcheck.size();
    deathcheck.push_back(1);
    int oldType;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldType);
     tids.push_back(pthread_self());
    char a = ch[0];
    sleep(10);
      std::ifstream fp("//home//houston//laab6//Laab6_2//lab6_2//FileForReading");
    if (!fp.is_open()) {
        std::cerr << "Can't open file!" << std::endl;
       std::cerr << strerror(errno);
        } else {
    int count = 0;

    char curr;
    while (fp.get(curr)) {
        if (curr == a) {
            count++;
        }
    }
    fp.close();

    std::cout << "Count of '" << a << "' symbols in file: " << count << std::endl;}
       // pthread_exit(NULL);

        deathcheck[z] = 0;
}

void MainWindow::CreateThread2(int n){
        std::ifstream fp("/home/laab6/Laab6_2/lab6_2/FileForReading.txt");
        string line;
        int lineCount = 0;
        while (std::getline(fp, line)) {
    lineCount++;
        }
        fp.close();

        int u = 0;
        switch(n){
        case 2: u = 1; break;
        case 4: u = 2; break;
        case 8: u = 3; break;
        case 16: u = 4; break;
        default: u = 0; break;
        }


        for(int i = 0; i < n; i++){
    struct ThreadData *cjh = (struct ThreadData*)malloc( sizeof(struct ThreadData) );
    cjh->a = ch[u];
    cjh->N = lineCount/n;
    cjh->start = 0+i*(lineCount/n);
        pthread_t hThread;
    if(ui->condvar->isChecked()){
        pthread_create(&hThread, nullptr, ThreadFunction_condvar, &cjh);
           // pthread_mutex_lock(&m);
           // while (done == 0)
           //     pthread_cond_wait(&c, &m);
           // pthread_mutex_unlock(&m);
           // done = 0;
    }
    if(ui->rwlock->isChecked()){
        pthread_create(&hThread, nullptr, ThreadFunction_rwlock, &cjh);
    }

    if(!ui->rwlock->isChecked() && !ui->condvar->isChecked()){
        pthread_create(&hThread, nullptr, ThreadFunction_2, cjh);
      //  pthread_kill(arr[arr.size()-1], SIGSTOP);
    }

    if(hThread){
        arr.push_back(hThread);
        times.push_back(std::chrono::high_resolution_clock::now());
    }
        }
}


void MainWindow::on_cteateThread_clicked(){

    if(ui->chb1->isChecked()){

        pthread_t hThread;
        pthread_create(&hThread, 0, ThreadFunction, nullptr);
        if(hThread){
           arr.push_back(hThread);
            times.push_back(std::chrono::high_resolution_clock::now());
            }
    }

    if(ui->chb2->isChecked()){
        CreateThread2(2);
    }

    if(ui->chb4->isChecked()){
        CreateThread2(4);
    }
    if(ui->chb8->isChecked()){
        CreateThread2(8);
    }
    if(ui->chb16->isChecked()){
        CreateThread2(16);
    }


    ui->chb16->setCheckState(Qt::Unchecked);
    ui->chb1->setCheckState(Qt::Unchecked);
    ui->chb2->setCheckState(Qt::Unchecked);
    ui->chb4->setCheckState(Qt::Unchecked);
    ui->chb8->setCheckState(Qt::Unchecked);

}

void MainWindow::DataNewer(){
    for(int i = 0; i < arr.size(); i++){
        if(i == 0){
            ui->TableView->setRowCount(0);
        }
       if (deathcheck[i] != 0) {
                addTableInfo(i);} else {
                arr.erase(arr.begin() + i);
                deathcheck.erase(deathcheck.begin() + i);
                times.erase(times.begin() + i);
                i--;
            }
        }
    }

void MainWindow::addTableInfo(int i){
    ui->TableView->insertRow(ui->TableView->rowCount());
    ui->TableView->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::to_string(tids[i]))));
      ui->TableView->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(pthread_kill(arr[i], 0) == 0?"Active":"Suspended")));

    ui->TableView->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(beautifulShow(i))));
    int policy;
    struct sched_param param;
    pthread_getschedparam(arr[i], &policy, &param);
    ui->TableView->setItem(i, 3, new QTableWidgetItem(QString::number(param.sched_priority)));

}


void MainWindow::DataNewerClock(std::function<void(void)> a){
    int duration = 1000;
    std::thread([a, duration](){
        while(true){
            a();
            auto ms = std::chrono::steady_clock::now() + std::chrono::milliseconds(duration);
            std::this_thread::sleep_until(ms);
        }
    }).detach();

}


int MainWindow::getTime(int i)
{   auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - times[i]);
    return elapsed_time.count();
}

std::string MainWindow::beautifulShow(int i){
    int elapsed_time_seconds = getTime(i) / 1000; // Convert milliseconds to seconds
    int hours = elapsed_time_seconds / 3600;
    int minutes = (elapsed_time_seconds % 3600) / 60;
    int seconds = elapsed_time_seconds % 60;
    std::stringstream af;
    af <<  std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << minutes << ":" <<
        std::setfill('0') << std::setw(2) << seconds;

    return af.str();
}

int MainWindow::findProcessByID(int r){
    QString te = ui->TableView->item(r, 0)->text();
    for(int i = 0; i < arr.size(); i++){
         if(static_cast<pthread_t>(stoi(te.toStdString())) == arr[i]){
                return i;
            }
    }
    return -1;
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    rightClickedRow = ui->TableView->indexAt(pos).row();
    ui->TableView->selectRow(rightClickedRow);// Store the right-clicked row
    contextMenu->exec(ui->TableView->mapToGlobal(pos));
}

void MainWindow::killPr()
{
      //pthread_kill(arr[rightClickedRow], SIGKILL);
    pthread_cancel(arr[rightClickedRow]);
    deathcheck[rightClickedRow] = 0;
}

void MainWindow::setThreadPriority(pthread_t thread, int priority)
{
    if (pthread_setschedprio(thread, priority) == EINVAL) {
            std::cerr << "Error setting thread priority." << std::endl;
    }
}

void MainWindow::idlePr(){
    setThreadPriority(arr[rightClickedRow], SCHED_IDLE);
}
void MainWindow::belowPr(){
    setThreadPriority(arr[rightClickedRow], SCHED_BATCH);
}
void MainWindow::normPr(){
    setThreadPriority(arr[rightClickedRow], SCHED_OTHER);
}
void MainWindow::highPr(){
    setThreadPriority(arr[rightClickedRow], SCHED_RR);
}
void MainWindow::realtPr(){
    setThreadPriority(arr[rightClickedRow], SCHED_DEADLINE);
}
void MainWindow::detachPr(){
    pthread_detach(arr[rightClickedRow]);
}
void MainWindow::on_chb1_stateChanged(int arg1)
{
    if(arg1 == 2){
        ec->getN(0);
        ec->show();
    }
}

void MainWindow::on_chb2_stateChanged(int arg1)
{
    if(arg1 == 2){
        ec->getN(1);
        ec->show();
    }
}

void MainWindow::on_chb4_stateChanged(int arg1)
{
    if(arg1 == 2){
        ec->getN(2);
        ec->show();
    }
}

void MainWindow::on_chb8_stateChanged(int arg1)
{
    if(arg1 == 2){
        ec->getN(3);
        ec->show();
    }
}

void MainWindow::on_chb16_stateChanged(int arg1)
{
    if(arg1 == 2){
        ec->getN(4);
        ec->show();
    }
}

void MainWindow::on_startAll_clicked()
{
    for(int i = 0; i < arr.size(); i++){
       pthread_kill(arr[i], SIGCONT);
    }
}

void MainWindow::getChar(char a, int N){
    ch[N] = a;
}


