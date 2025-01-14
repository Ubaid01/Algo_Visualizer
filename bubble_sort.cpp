#include "bubble_sort.h"

bubble_sort::bubble_sort(QWidget* parent,int n,int width,int height):Sort(parent,n,width,height) {
    connect(start, &QPushButton::clicked, this, &bubble_sort::sorting);
    this->show();
    // update();
}

void bubble_sort::sorting(){
    sizeSelector->setEnabled(false);
    is_reseted=false;
    Pause->setEnabled(true);
    start->setEnabled(false);
    Reset->setEnabled(false);
    for(int i=0;i<n;i++){
        if(is_reseted)
            break;
        for(int j=0;j<n-i-1;j++){
            if(is_reseted) break;
            if(array[j].first>array[j+1].first)
                simulation(j,j+1);

        }
        sorted_part.push_back(n-i-1);// the updated will be pushed to the sorted part
        update();
    }
    if(is_reseted)
        reset();

    Pause->setEnabled(false);
    Reset->setEnabled(true);
    is_reseted=false;
    sizeSelector->setEnabled(true);
    update();
}

void bubble_sort::simulation(int s1,int s2){
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
        // QEventLoop loop;
        // QTimer::singleShot(time_delay, &loop, SLOT(quit()));
        // loop.exec();
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
        // QEventLoop loop;
        // QTimer::singleShot(time_delay, &loop, SLOT(quit()));
        // loop.exec();
        for (int d = 0; d < time_delay; d+=2) {
            if (is_paused) break;
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
        // QEventLoop loop;
        // QTimer::singleShot(time_delay, &loop, SLOT(quit()));
        // loop.exec();
        for (int d = 0; d < time_delay; d+=2) {
            if (is_paused || is_reseted) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::usleep(1);
        }
    }
    pair<int,QRect> temp=array[s1];// f1
    array[s1]=array[s2];
    array[s2]=temp;
    // QEventLoop loop;
    // QTimer::singleShot(time_delay, &loop, SLOT(quit()));
    // loop.exec();
    for (int d = 0; d < time_delay; d+=2) {
        if (is_paused || is_reseted) break;
        QCoreApplication::processEvents();  // Process events during the delay
        QThread::usleep(1);
    }
}

void bubble_sort::paintEvent(QPaintEvent *event)
{
    int box_width=array[0].second.width();
    QString numberText;
    QPainter painter(this);
    painter.setFont(QFont("Arial", box_width/3));
    for(int i=0;i<n;i++){
        painter.setPen(Qt::red);
        numberText=QString::number(array[i].first);
        painter.drawText(QPoint(array[i].second.x()+box_width/4,array[i].second.y()+array[i].second.height()-box_width/4),numberText);
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
