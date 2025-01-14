#include "selection_sort.h"

selection_sort::selection_sort(QWidget* parent,int n,int width,int height):Sort(parent,n,width,height) {
    connect(start, &QPushButton::clicked, this, &selection_sort::sorting);
    this->show();
    // update();
}

void selection_sort::sorting(){
    sizeSelector->setEnabled(false);
    is_reseted=false;
    Pause->setEnabled(true);
    start->setEnabled(false);
    Reset->setEnabled(false);
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (array[j].first < array[minIndex].first)
                minIndex = j;
        }
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        if (minIndex != i) {
            simulation(i,minIndex);
            sorted_part.push_back(i);
            update();
        }
        else{
            sorted_part.push_back(i);
            update();
        }
    }
    if(is_reseted){
        sorted_part.clear();
        reset();
    }
    else
        sorted_part.push_back(n-1);
    is_reseted=false;
    update();
    Pause->setEnabled(false);
    Reset->setEnabled(true);
    sizeSelector->setEnabled(true);
}

void selection_sort::simulation(int s1,int s2){
    int x=array[s1].second.width(),y;
    y=x;
    if(s1>s2) swap(s1,s2);
    for(int i=0;i<y+10;i+=8){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        array[s1].second.moveTop(array[s1].second.top() - 8);   // Move the first rectangle up
        array[s2].second.moveTop(array[s2].second.top() + 8);   // Move the second rectangle down
        update();
        for (int d = 0; d < time_delay; d+=2) {
            if (is_paused) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::usleep(1);
        }
    }
    for(int i=0;i<(s2-s1)*x;i+=8){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        array[s1].second.moveLeft(array[s1].second.left() + 8);  // Move the first rectangle right
        array[s2].second.moveLeft(array[s2].second.left() - 8);  // Move the second rectangle left
        update();
        for (int d = 0; d < time_delay; d+=2) {
            if (is_paused ) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::usleep(1);
        }
    }
    int diff=(s2-s1)*x;
    if(diff%8!=0){
        diff=8-(diff%8);
        array[s1].second.moveLeft(array[s1].second.left()- diff);   // Move the first rectangle up
        array[s2].second.moveLeft(array[s2].second.left() + diff);   // Move the second rectangle down
        update();
    }
    for(int i=0;i<y+10;i+=8){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        array[s1].second.moveTop(array[s1].second.top() + 8);   // Move the first rectangle down
        array[s2].second.moveTop(array[s2].second.top() - 8);   // Move the second rectangle up
        update();
        for (int d = 0; d < time_delay; d+=2) {
            if (is_paused ) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::usleep(1);
        }
    }
    pair<int,QRect> temp=array[s1];
    array[s1]=array[s2];
    array[s2]=temp;
    for (int d = 0; d < time_delay; d+=2) {
        if (is_paused ) break;
        QCoreApplication::processEvents();  // Process events during the delay
        QThread::usleep(1);
    }
}

void selection_sort::paintEvent(QPaintEvent *event)
{
    int box_width=array[0].second.width();
    QString numberText;
    QPainter painter(this);
    painter.setFont(QFont("Arial", box_width/3));
    for(int i=0;i<n;i++){
        painter.setPen(Qt::red);
        numberText=QString::number(array[i].first);
        painter.drawText(QPoint(array[i].second.x()+35,array[i].second.y()+array[i].second.height()-35),numberText);
        painter.setPen(QColor(Qt::black));
        painter.drawRect(array[i].second);
    }
    for(int i=0;i<sorted_part.size();i++){
        painter.fillRect(array[sorted_part[i]].second, Qt::green);
        painter.setPen(Qt::red);
        numberText=QString::number(array[sorted_part[i]].first);
        painter.drawText(QPoint(array[sorted_part[i]].second.x()+box_width/4,array[sorted_part[i]].second.y()+array[sorted_part[i]].second.height()-box_width/4),numberText);
        painter.setPen(QColor(Qt::black));
        painter.drawRect(array[i].second);
    }
}
