#include "insertion_sort.h"

insertion_sort::insertion_sort(QWidget* parent,int n,int width,int height):Sort(parent,n,width,height) {
    line=new QLine(0,0,0,0);
    // start=new QPushButton("Start",this);
    // start->setGeometry(width/2-120,height-200,100,50);
    connect(start, &QPushButton::clicked, this, &insertion_sort::sorting);
    // Debugging info
    // qDebug() << "Insertion sort created!";
    // qDebug() << "Widget dimensions: " << width << "x" << height;
    // qDebug() << "Parent widget: " << parent;
    this->show();
    // update();
}

void insertion_sort::sorting(){
    sizeSelector->setEnabled(false);
    is_reseted=false;
    Pause->setEnabled(true);
    start->setEnabled(false);
    Reset->setEnabled(false);
    line->setLine(array[0].second.right(),array[0].second.top()-15,array[0].second.right(),array[0].second.bottom()+15);
    for (int i = 1; i < n; i++) {
        update();
        int key = array[i].first;
        int j = i-1;
        // Move elements of arr[0..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= 0 && array[j].first > key) {
            j--;
        }
        if(is_reseted) break;
        if(j!=i-1){
            simulation(j+1,i);
        }
        else{
            for (int d = 0; d < 25; d+=5) {
                if (is_paused || is_reseted) break;
                QCoreApplication::processEvents();  // Process events during the delay
                QThread::msleep(5);
            }
        }
        line->setP1(QPoint(line->x1() + array[i].second.width(), line->y1()));
        line->setP2(QPoint(line->x2() + array[i].second.width(), line->y2()));
        for (int d = 0; d < time_delay; d+=5) {
            if (is_paused || is_reseted) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::msleep(5);
        }
    }
    line->setLine(0,0,0,0);
    if(is_reseted) reset();
    update();
    Pause->setEnabled(false);
    Reset->setEnabled(true);
    is_reseted=false;
    sizeSelector->setEnabled(true);
}

void insertion_sort::simulation(int s1,int s2){
    if(s2<s1) swap(s1,s2);
    int x=array[s1].second.width(),y;
    y=x;
    for(int i=0;i<y+10;i+=8){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        array[s2].second.moveTop(array[s2].second.top() - 8);   // Move the first rectangle up
        update();
        for (int d = 0; d < time_delay; d+=2) {
            if (is_paused) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::msleep(1);
        }
    }
    for(int i=s2-1;i>=s1;i--){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        for(int j=0;j<x;j+=8){
            while (is_paused) {
                QCoreApplication::processEvents();  // Allow GUI to remain responsive
            }
            array[i].second.moveLeft(array[i].second.left() + 8);  // Move the ith rectangle left
            update();
            for (int d = 0; d < time_delay; d+=2) {
                if (is_paused) break;
                QCoreApplication::processEvents();  // Process events during the delay
                QThread::msleep(1);
            }
        }
        int diff=x;
        if(diff%8!=0){
            diff=8-(diff%8);
            array[i].second.moveLeft(array[i].second.left()- diff);   // Move the first rectangle up  // Move the second rectangle down
            update();
        }
    }
    for(int i=0;i<(s2-s1)*x;i+=8){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        array[s2].second.moveLeft(array[s2].second.left() - 8);  // Move the first rectangle right
        update();
        for (int d = 0; d < time_delay; d+=2) {
            if (is_paused) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::msleep(1);
        }
    }
    int diff=(s2-s1)*x;
    if(diff%8!=0){
        diff=8-(diff%8);
        array[s2].second.moveLeft(array[s2].second.left() + diff);   // Move the first rectangle up  // Move the second rectangle down
        update();
    }
    for(int i=0;i<y+10;i+=8){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        array[s2].second.moveTop(array[s2].second.top() + 8);   // Move the first rectangle up
        update();
        for (int d = 0; d < time_delay; d+=2) {
            if (is_paused) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::msleep(1);
        }
    }
    pair<int,QRect> temp=array[s2];
    for(int i=s2-1;i>=s1;i--){
        array[i+1]=array[i];
    }
    array[s1]=temp;
}

void insertion_sort::paintEvent(QPaintEvent *event)
{
    int box_width=array[0].second.width();
    QString numberText;
    QPainter painter(this);
    painter.setFont(QFont("Arial", box_width/3));
    for(int i=0;i<n;i++){
        painter.setPen(QColor(Qt::black));
        painter.drawRect(array[i].second);
        painter.setPen(Qt::red);
        numberText=QString::number(array[i].first);
        painter.drawText(QPoint(array[i].second.x()+35,array[i].second.y()+array[i].second.height()-35),numberText);
    }
    for(int i=0;i<sorted_part.size();i++){
        painter.fillRect(array[sorted_part[i]].second, Qt::green);
        painter.setPen(Qt::red);
        numberText=QString::number(array[sorted_part[i]].first);
        painter.drawText(QPoint(array[sorted_part[i]].second.x()+box_width/4,array[sorted_part[i]].second.y()+array[sorted_part[i]].second.height()-box_width/4),numberText);
        painter.setPen(QColor(Qt::black));
        painter.drawRect(array[i].second);
    }
    painter.setRenderHint(QPainter::Antialiasing);  // Optional: For smooth lines
    QPen pen(Qt::black);  // Set the color of the line
    pen.setWidth(3);  // Set the width of the line
    painter.setPen(pen);
    // Draw a line from point (50, 50) to point (200, 200)
    painter.drawLine(*line);
}

insertion_sort::~insertion_sort() {
    disconnect(start, &QPushButton::clicked, this, &insertion_sort::sorting);
    delete line ;
    line = nullptr ;
}

