#include "quick_sort.h"

quick_sort::quick_sort(QWidget* parent,int n,int width,int height):Sort(parent,n,width,height),time_delay_internal(6){
    connect(start, &QPushButton::clicked, this, &quick_sort::simulation);
    time_delay=100;
    sorting();
    levels=whole_layer.size();
    box_width=min(min(width/(n+2)-10,(2*(height-200))/(3*levels+1)),150);
    box_height=box_width;
    separation_y=box_height/2;
    start_y=separation_y;
    start_x=width/2-(n*box_width/2);
    for(int i=0;i<n;i++){
        whole_layer[0][0].first[i].second={start_x,start_y,box_width,box_height};
        start_x+=box_width;
    }
    start_y+=separation_y+box_height;
    num_paint=1;
    this->show();
    update();
    speedSlider->setRange(1,10);
    speedSlider->setSliderPosition(5);
    connect(speedSlider, &QSlider::valueChanged, [this](int value) {
        time_delay_internal = abs(value-11); // Update the speed variable
        time_delay=abs(value*100-1100);
        speedLabel->setText(QString("Speed: %1").arg(value)); // Update the label
    });
}

void quick_sort::animation(int s1,int s2,int i,int j){
    if(is_reseted) return;
    int x=whole_layer[i][j].first[s1].second.width(),y;
    y=x;
    if(s1>s2) swap(s1,s2);
    for(int k=0;k<y+10;k+=5){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) return;
        whole_layer[i][j].first[s1].second.moveTop(whole_layer[i][j].first[s1].second.top() + 5);   // Move the first rectangle down
        whole_layer[i][j].first[s2].second.moveTop(whole_layer[i][j].first[s2].second.top() + 10);   // Move the second rectangle down by double
        update();
        for (int d = 0; d < time_delay_internal; d+=1) {
            if (is_paused) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::msleep(1);
        }
    }
    int diff=(s2-s1)*x;
    for(int k=0;k<(s2-s1)*x;k+=8){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) return;
        whole_layer[i][j].first[s1].second.moveLeft(whole_layer[i][j].first[s1].second.left() + 8);  // Move the first rectangle right
        whole_layer[i][j].first[s2].second.moveLeft(whole_layer[i][j].first[s2].second.left() - 8);  // Move the second rectangle left
        update();
        for (int d = 0; d < time_delay_internal; d+=1) {
            if (is_paused) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::msleep(1);
        }
    }
    if(diff%8!=0){
        diff=8-(diff%8);
        whole_layer[i][j].first[s1].second.moveLeft(whole_layer[i][j].first[s1].second.left() - diff);
        whole_layer[i][j].first[s2].second.moveLeft(whole_layer[i][j].first[s2].second.left() + diff);
        update();
    }

    for(int k=0;k<y+10;k+=5){
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) return;
        whole_layer[i][j].first[s1].second.moveTop(whole_layer[i][j].first[s1].second.top() - 5);   // Move the first rectangle up
        whole_layer[i][j].first[s2].second.moveTop(whole_layer[i][j].first[s2].second.top() - 10);   // Move the second rectangle up by double
        update();
        for (int d = 0; d < time_delay_internal; d+=1) {
            if (is_paused) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::msleep(1);
        }
    }
}

int quick_sort::partition(vector<pair<int,QRect>>&arr){
    int pivot = arr.back().first, x = -1;
    for (int i = 0;i < arr.size() - 1;i++){
        if (arr[i].first <= pivot){
            swap(arr[++x], arr[i]);
        }
    }
    swap(arr[++x], arr[arr.size() - 1]);
    return x;
}

void quick_sort::partitioning_with_animation(vector<pair<int,QRect>>&arr,int I,int J){
    int pivot = arr.back().first, x = -1;
    for (int i = 0;i < arr.size() - 1;i++){
        if(is_reseted) return;
        if (arr[i].first <= pivot){
            while (is_paused) {
                QCoreApplication::processEvents();  // Allow GUI to remain responsive
            }
            if(is_reseted) return;
            if(i!=x+1)
                animation(x+1,i,I,J);
            swap(arr[++x], arr[i]);
        }
    }
    if( !is_reseted && x+1!=arr.size() - 1 ){
        animation(x+1,arr.size() - 1,I,J);
    }
    return;
}

void quick_sort::sorting(){
    bool check = true;
    int i = 1;
    if(n==1){
        whole_layer.push_back({{array,1}});
        return;
    }
    whole_layer.push_back({{array,0}});
    while (check){
        vector<pair<vector<pair<int,QRect>>,int>> Temp;
        check = false;
        for (int j = 0;j < whole_layer[i - 1].size();j++){
            vector<pair<int,QRect>> temp1, temp2;
            if (whole_layer[i - 1][j].first.size() == 1)
                Temp.push_back(whole_layer[i - 1][j]);
            else{
                temp1 = whole_layer[i - 1][j].first;
                int pivot = partition(temp1);
                if (pivot > 0){
                    for (int k = 0;k < pivot;k++){
                        temp2.push_back(temp1[k]);
                    }
                    Temp.push_back({temp2,0});
                    if (temp2.size() > 1) check = true;
                    else Temp[Temp.size()-1].second=1;
                    temp2.clear();
                }
                Temp.push_back({ {temp1[pivot] },1});
                if (pivot < temp1.size() - 1){
                    for (int k = pivot + 1;k < temp1.size();k++){
                        temp2.push_back(temp1[k]);
                    }
                    Temp.push_back({temp2,0});
                    if (temp2.size() > 1) check = true;
                    else Temp[Temp.size()-1].second=1;
                }
            }
        }
        whole_layer.push_back(Temp);
        i++;
    }
    for(int i=0;i<array.size();i++){
        for(int j=0;j<array.size()-i-1;j++){
            if(array[j].first>array[j+1].first){
                swap(array[j],array[j+1]);
            }
        }
    }
    whole_layer.push_back({{array,1}});
}

void quick_sort::simulation(){
    sizeSelector->setEnabled(false);
    is_reseted=false;
    Pause->setEnabled(true);
    start->setEnabled(false);
    Reset->setEnabled(false);
    int size,pivot;
    for(int i=1;i<whole_layer.size();i++){
        if(is_reseted) break;
        size=whole_layer[i].size();
        separation_x=(width-n*box_width)/(2+size-1);
        start_x=separation_x;
        for(int j=0;j<whole_layer[i-1].size();j++){
            partitioning_with_animation(whole_layer[i-1][j].first,i-1,j);
            if(is_reseted) break;
        }
        for(int j=0;j<size;j++){
            for(int k=0;k<whole_layer[i][j].first.size();k++){
                while (is_paused) {
                    QCoreApplication::processEvents();  // Allow GUI to remain responsive
                }
                if(is_reseted) break;
                whole_layer[i][j].first[k].second.setRect(start_x,start_y,box_width,box_height);
                start_x+=box_width;
            }
            if(is_reseted) break;
            num_paint++;
            start_x+=separation_x;
            update();
            for (int d = 0; d < time_delay; d+=2) {
                if (is_paused) break;
                QCoreApplication::processEvents();  // Process events during the delay
                QThread::msleep(1);
            }
        }
        if(is_reseted) break;
        start_y+=(separation_y+box_height);
        update();
    }
    if(is_reseted) {
        is_reseted=false;
        reset();
    }
    Pause->setEnabled(false);
    Reset->setEnabled(true);
    sizeSelector->setEnabled(true);
}

void quick_sort::paintEvent(QPaintEvent *event)
{
    QString numberText;
    QPainter painter(this);
    int num=0;
    painter.setFont(QFont("Arial", box_width/3));
    for(int i=0;i<whole_layer.size();i++){
        for(int j=0;j<whole_layer[i].size();j++){
            if(num==num_paint) return;
            num++;
            for(int k=0;k<whole_layer[i][j].first.size();k++){
                painter.setPen(QColor(Qt::black));
                painter.drawRect(whole_layer[i][j].first[k].second);
                if(whole_layer[i][j].second){
                     painter.fillRect(whole_layer[i][j].first[k].second, Qt::green);
                }
                painter.setPen(Qt::red);
                numberText=QString::number(whole_layer[i][j].first[k].first);
                painter.drawText(QPoint(whole_layer[i][j].first[k].second.x()+box_width/4,whole_layer[i][j].first[k].second.y()+whole_layer[i][j].first[k].second.height()-box_width/4),numberText);
            }
        }
    }
}

void quick_sort::reset(){
    Sort::reset();
    whole_layer.clear();
    sorting();
    levels=whole_layer.size();
    box_width=min(min(width/(n+2)-10,(2*(height-200))/(3*levels+1)),150);
    box_height=box_width;
    separation_y=box_height/2;
    start_y=separation_y;
    start_x=width/2-(n*box_width/2);
    for(int i=0;i<n;i++){
        whole_layer[0][0].first[i].second={start_x,start_y,box_width,box_height};
        start_x+=box_width;
    }
    start_y+=separation_y+box_height;
    num_paint=1;
    this->show();
    update();
}
