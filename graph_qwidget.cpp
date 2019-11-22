#include "graph_qwidget.h"
#include "mainwindow.h"
#include <cstdlib>

graph_qWidget::graph_qWidget(QWidget *parent) : QWidget(parent)
{
    ker = 24;
    brushColorIdx = 0;    
    initiateGame();
}
int graph_qWidget::linearSearch(int x,int y){    
    for(int i=points.size()-1;i>-1;i--){
        QPair<QPair<int,int>, int>  it = points[i];
        if(it.first.first == x && it.first.second == y)
            return it.second;
    }
    return -1;
}
bool graph_qWidget::linearSearchDest(int x,int y){
    for(int i=destination.size()-1;i>-1;i--){
        QPair<int,int>  it = destination[i];
        if(it.first == x && it.second == y)
            return true;
    }
    return false;
}
bool graph_qWidget::linearSearchTarget(int x,int y){
    for(QPair<int,int>  it : target){
        if(it.first == x && it.second == y)
            return true;
    }
    return false;
}

void graph_qWidget::generateTarget(){
    flag = !flag;
    for(QPair<int, int> it : target)
        destination.push_back(it);
    target.clear();
    for(QPair<int, int> it : shapes[(QDateTime::currentMSecsSinceEpoch())%shapes.size()]){
        target.push_back(qMakePair(it.first,9-it.second));
    }
}

void graph_qWidget::initiateGame(){
    QPair<int, int > t;
    score = 0;
    QVector<QPair<int, int> > tempShape;
    tempShape.push_back(qMakePair(0,0));
    tempShape.push_back(qMakePair(1,1));
    tempShape.push_back(qMakePair(0,1));
    tempShape.push_back(qMakePair(1,0));
    shapes.push_back(tempShape);
    tempShape.clear();
    tempShape.push_back(qMakePair(0,0));
    tempShape.push_back(qMakePair(-1,0));
    tempShape.push_back(qMakePair(1,0));
    shapes.push_back(tempShape);
    tempShape.clear();
    tempShape.push_back(qMakePair(0,0));
    tempShape.push_back(qMakePair(-1,0));
    tempShape.push_back(qMakePair(1,0));
    tempShape.push_back(qMakePair(1,1));
    shapes.push_back(tempShape);
    tempShape.clear();
    generateTarget();
}

void graph_qWidget::paintEvent(QPaintEvent *pe) {
    QStyleOption styleOption;
    styleOption.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
    int size = this->width();
    int half = size/2;
    QColor brushColor;
    QVector<QPair<int,int>>::iterator it2;
    for(it2 =  destination.begin(); it2 != destination.end() ; ++it2){
        switch (3) {
        case 0:
            brushColor = Qt::yellow;
            break;
        case 1:
            brushColor = Qt::red;
            break;
        case 2:
            brushColor = Qt::black;
            break;
        case 3:
            brushColor = Qt::blue;
            break;
        case 4:
            brushColor = Qt::white;
            break;
        }
        painter.setPen(QPen(brushColor, ker, Qt::SolidLine));
        painter.drawPoint(ker*it2->first + width()/2-ker/2,height()/2 -  ker*it2->second-ker/2);
    }
    QVector<QPair<int,int>>::iterator it3;
    for(it3 =  target.begin(); it3 != target.end() ; ++it3){
        switch (qrand()%3) {
        case 0:
            brushColor = Qt::yellow;
            break;
        case 1:
            brushColor = Qt::red;
            break;
        case 2:
            brushColor = Qt::white;
            break;
        }
        painter.setPen(QPen(brushColor, ker, Qt::SolidLine));
        painter.drawPoint(ker*it3->first + width()/2-ker/2,height()/2 -  ker*(it3->second)-ker/2);
    }
    if ( ker==1 ){
        painter.setPen(QPen(Qt::green, ker, Qt::DotLine));
        painter.drawLine(half,0,half,size);
        painter.drawLine(0,half,size,half);
    }
    else {
        for(int i = -1*half; i<=half; i+=1){
            if (abs(i)%ker!=0)
                continue;
            if(i==-ker){
                painter.setPen(QPen(Qt::red, 5, Qt::SolidLine));
                painter.drawLine(0,i+half,size,i+half);
                painter.setPen(QPen(QColor(33,46,54), 1, Qt::SolidLine));
                painter.drawLine(i+half,0,i+half,size);
            }
            else{
                painter.setPen(QPen(QColor(33,46,54), 1, Qt::SolidLine));
                painter.drawLine(i+half,0,i+half,size);
                painter.drawLine(0,i+half,size,i+half);
            }
        }
    }
}
