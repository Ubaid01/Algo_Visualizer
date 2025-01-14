#include "merge_sort.h"
#include"cmath"

merge_sort::merge_sort(QWidget* parent,int n,int width,int height):Sort(parent,n,width,height){
    connect(start, &QPushButton::clicked, this, &merge_sort::simulation);
    start->setEnabled(true);
    time_delay=15;
    levels=ceil(log2(n))*2 + 1;
    box_width=min(min(width/(n+2)-10,(2*(height-200))/(3*levels+1)),150);
    box_height=box_width;
    separation_y=box_height/2;
    start_y=separation_y;
    start_x=width/2-(n*box_width/2);
    vector<pair<int,QRect>> temp;
    start_y=separation_y;
    for(int i=0;i<n;i++){
        temp.push_back({array[i].first,{start_x,start_y,box_width,box_height}});
        start_x+=box_width;
    }
    whole_layer.push_back({temp});
    start_y=separation_y+box_height;
    speedSlider->setSliderPosition(6);
    speedSlider->setRange(1,10);
    connect(speedSlider, &QSlider::valueChanged, [this](int value) {
        time_delay = abs(value*10-105); // Update the speed variable
        speedLabel->setText(QString("Speed: %1").arg(value)); // Update the label
    });
    sorting();
    num_paint=1;
    this->show();
    update();
}

void merge_sort::sorting(){
    bool check=true;
    int i=0;
    for(i=1;i<=ceil(log2(n));i++){
        vector<vector<pair<int,QRect>>> Temp;
        for(int j=0;j<whole_layer[i-1].size();j++){
            vector<pair<int,QRect>> temp1;
            vector<pair<int,QRect>> temp2;
            if(whole_layer[i-1][j].size()==1){
                temp2.push_back(whole_layer[i-1][j][0]);
                Temp.push_back(temp2);
            }
            else{
                temp1=whole_layer[i-1][j];
                for(int k=0;k<temp1.size()/2;k++){
                    temp2.push_back(temp1[k]);
                }
                Temp.push_back(temp2);
                temp2.clear();
                for(int k=temp1.size()/2;k<temp1.size();k++){
                    temp2.push_back(temp1[k]);
                }
                Temp.push_back(temp2);
            }
        }
        whole_layer.push_back(Temp);
    }
    int prev=i-2,prev_i;
    for(;i<levels;i++){
        vector<vector<pair<int,QRect>>> Temp;
        int size=whole_layer[i-1].size(),k=0;
        prev_i=0;
        while(k+1<size){
            if(whole_layer[i-1][prev_i].size()==1 && whole_layer[prev][prev_i]==whole_layer[i-1][k]){
                Temp.push_back(whole_layer[prev][prev_i]);
                k+=1;
            }
            else{
                Temp.push_back(merge(whole_layer[i-1][k],whole_layer[i-1][k+1]));
                k+=2;
            }
            prev_i++;
        }
        if(k!=size){
            Temp.push_back(whole_layer[i-1][k]);
        }
        prev--;
        whole_layer.push_back(Temp);
    }
}

vector<pair<int,QRect>> merge_sort::merge(vector<pair<int,QRect>> &x1,vector<pair<int,QRect>>& x2){
    vector<pair<int,QRect>> arr(x1.size()+x2.size());
    int n=0,m=0,k=0;
    while (n < x1.size() && m < x2.size()){
        if (x1[n].first <= x2[m].first)
            arr[k++] = x1[n++];
        else
            arr[k++] = x2[m++];
    }
    while (n < x1.size()){
        arr[k++] = x1[n++];
    }
    while (m < x2.size()){
        arr[k++] = x2[m++];
    }
    return arr;
}

void merge_sort::simulation(){
    sizeSelector->setEnabled(false);
    is_reseted=false;
    start->setEnabled(false);
    Reset->setEnabled(false);
    Pause->setEnabled(true);
    int size;
    update();
    for (int d = 0; d < time_delay; d+=1) {
        if (is_paused) break;
        QCoreApplication::processEvents();  // Process events during the delay
        QThread::msleep(15);
    }
    for(int i=1;i<whole_layer.size();i++){
        if(is_reseted) break;
        size=whole_layer[i].size();
        separation_x=(width-n*box_width)/(2+size-1);
        start_x=separation_x;
        for(int j=0;j<size;j++){
            if(is_reseted) break;
            for(int k=0;k<whole_layer[i][j].size();k++){
                while (is_paused) {
                    QCoreApplication::processEvents();  // Allow GUI to remain responsive
                }
                if(is_reseted) break;
                whole_layer[i][j][k].second.setRect(start_x,start_y,box_width,box_height);
                start_x+=box_width;
            }
            num_paint++;
            start_x+=separation_x;
            update();
            for (int d = 0; d < time_delay; d+=1) {
                if (is_paused) break;
                QCoreApplication::processEvents();  // Process events during the delay
                QThread::msleep(15);
            }
        }
        start_y+=(separation_y+box_height);
        update();
    }
    if(is_reseted) reset();
    Reset->setEnabled(true);
    Pause->setEnabled(false);
    is_reseted=false;
    sizeSelector->setEnabled(true);
}

void merge_sort::paintEvent(QPaintEvent *event)
{
    QString numberText;
    QPainter painter(this);
    int num=0;
    painter.setFont(QFont("Arial", box_width/3));
    for(int i=0;i<whole_layer.size();i++){
        for(int j=0;j<whole_layer[i].size();j++){
            if(num==num_paint) return;
                num++;
            for(int k=0;k<whole_layer[i][j].size();k++){
                painter.setPen(QColor(Qt::black));
                painter.drawRect(whole_layer[i][j][k].second);
                if(i>ceil(log2(n))){
                    painter.fillRect(whole_layer[i][j][k].second, Qt::green);
                }
                painter.setPen(Qt::red);
                numberText=QString::number(whole_layer[i][j][k].first);
                painter.drawText(QPoint(whole_layer[i][j][k].second.x()+box_width/4,whole_layer[i][j][k].second.y()+whole_layer[i][j][k].second.height()-box_width/4),numberText);
            }
        }

        if( i != 0 ) {
            int lineY = ( separation_y + box_height ) * i + 10 ;
            // qDebug() << start_y ;
            painter.setPen( QColor(Qt::darkGray) );
            painter.drawLine(0, lineY, width, lineY);
        }
    }
}

void merge_sort::reset(){
    whole_layer.clear();
    Sort::reset();
    levels=ceil(log2(n))*2 + 1;
    box_width=min(min(width/(n+2)-10,(2*(height-200))/(3*levels+1)),150);
    box_height=box_width;
    separation_y=box_height/2;
    start_y=separation_y;
    start_x=width/2-(n*box_width/2);
    vector<pair<int,QRect>> temp;
    for(int i=0;i<n;i++){
        temp.push_back({array[i].first,{start_x,start_y,box_width,box_height}});
        start_x+=box_width;
    }
    start_y+=separation_y+box_height;
    whole_layer.push_back({temp});
    sorting();
    num_paint=1;
    this->show();
    update();
}
