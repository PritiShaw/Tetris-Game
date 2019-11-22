#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graph_qwidget.h"
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graph->resize(480,480);
    result = 1;
    timerId = startTimer(500);

    on_transRotBtn_clicked(180);
    srand(time(NULL));
    switch (rand()%3) {
        case 0:
            on_transRotBtn_clicked(90);
            break;
        case 1:
            on_transRotBtn_clicked(180);
            break;
        case 2:
            on_transRotBtn_clicked(-90);
            break;
    }
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;
}
void MainWindow::timerEvent(QTimerEvent *event)
{
    on_transTranslateBtn_clicked(0,-1);
    checkParity();
    lineCheck();
}
void MainWindow::on_transShearBtn_clicked(float hX, float hY)
{
    QVector<QPair<int,int>>::iterator it;
    for(it =  ui->graph->target.begin(); it != ui->graph->target.end() ; ++it){
        int x = it->first;
        int y = it->second;
        it->first = x + hX*y;
        it->second = y + hY*x;
    }
    ui->graph->repaint();
}

void MainWindow::on_transScaleBtn_clicked(float sX, float sY)
{
    QVector<QPair<int,int>>::iterator it;
    for(it =  ui->graph->target.begin(); it != ui->graph->target.end() ; ++it){
        it->first *= sX;
        it->second *= sY;
    }
    ui->graph->repaint();
}

void MainWindow::on_transTranslateBtn_clicked(float tX,float tY)
{
    QVector<QPair<int,int>>::iterator it;
    for(it =  ui->graph->target.begin(); it != ui->graph->target.end() ; ++it){
        it->first += tX;
        it->second += tY;
    }
    ui->graph->repaint();
}

void MainWindow::onTimerSignal(){
//    on_transTranslateBtn_clicked(0,-1);
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(result==1){
        switch (QString::number(ev->key()).toInt()) {
            case 16777236:
                //Right arrow
                on_transTranslateBtn_clicked(1,0);
                break;
            case 16777234:
                //Left arrow
                on_transTranslateBtn_clicked(-1,0);
                break;
            case 16777237:
                //Left arrow
                on_transTranslateBtn_clicked(0,-1);
                break;
            case 32:
                //space bar
                on_transRotBtn_clicked(90);
                break;
            }
        checkParity();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
//    qDebug()<<"You Release Key " + QString::number(ev->key());
}

void MainWindow::checkParity(){
    QVector<QPair<int,int>>::iterator it;
    for(it =  ui->graph->target.begin(); it != ui->graph->target.end() ; ++it){
        int x = it->first;
        int y = it->second;
        if(ui->graph->linearSearchDest(x,y-1) || y==-10){
            ui->graph->flag = !ui->graph->flag;
            ui->graph->score += ui->graph->target.size();
            ui->graph->generateTarget();
            break;
        }
    }
    for(it = ui->graph->destination.begin();it != ui->graph->destination.end(); ++it){
        if(it->second==0){
                QMessageBox msgBox;
                msgBox.setWindowTitle("Game Over");
                msgBox.setText(QString("You Scored %1").arg(ui->graph->score));
                msgBox.exec();
                exit(0);
        }
    }
}

void MainWindow::lineCheck(){
    return;
    QMap<int , int > checker;
    QVector<QPair<int,int>>::iterator it;
    for(it =  ui->graph->destination.begin(); it != ui->graph->destination.end() ; ++it)
        checker[it->second]++;
    int remCount = 0;
    int terminateCount = ui->graph->destination.size();
    for(int i = 0; i <  terminateCount; i++){
        if(checker[ui->graph->destination[i].second] == 20 ){
            ui->graph->destination.push_back(ui->graph->destination[i]);
            ui->graph->destination.erase(ui->graph->destination.begin() + i);
            remCount++;
        }
    }
    for(int i = 0; i < remCount; i++){
        ui->graph->destination.pop_back();
    }
}
void MainWindow::on_transRotBtn_clicked(int deg)
{
    float c = 0;
    float s = 1;
    int cx,cy;
    cx=cy=0;
    QVector<QPair<int,int>>::iterator it;
    for(it =  ui->graph->target.begin(); it != ui->graph->target.end() ; ++it){
        cx += it->first;
        cy += it->second;
    }
    cx /= ui->graph->target.size();
    cy /= ui->graph->target.size();
    for(it =  ui->graph->target.begin(); it != ui->graph->target.end() ; ++it){
        int x = it->first-cx;
        int y = it->second-cy;
        it->first = x*c - y*s + cx;
        it->second = x*s + y*c + cy;
    }
    ui->graph->repaint();
}

