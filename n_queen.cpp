#include "n_queen.h"
#include "types.h"
#include <qpainter.h>
#include <QEventloop >
#include<QTimer>

N_Queen::N_Queen(QWidget *parent,int N,int Width,int Height)
    :QWidget(parent),n(N),width(Width),height(Height),time_delay(100){


    QVector<QPair<QRect,int>> temp(n);
    int y=(height-180)/(n+2),x,x_start;
    x=y;
    x_start=(width-y*n)/2-x;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            temp[j-1]={QRect(x_start+x*j+j-1,y*i+i-1,x,y),0};
        }
        grid.push_back(temp);
    }
    int seperation=40,button_w=150,start_x=(width-4*button_w-3*seperation)/2,button_h=50,start_y=Height-200;
    set_button(start_x,start_y,button_w,button_h,seperation,"Back",BACK);
    set_button(start_x,start_y,button_w,button_h,seperation,"Start",START);
    set_button(start_x,start_y,button_w,button_h,seperation,"Pause",PAUSE);
    set_button(start_x,start_y,button_w,button_h,seperation,"Reset",RESET);
    connect(buttons[PAUSE], &QPushButton::clicked, this, &N_Queen::handle_pause_resume);
    connect(buttons[RESET], &QPushButton::clicked, this, &N_Queen::reset);
    connect(buttons[BACK], &QPushButton::clicked, this, &N_Queen::handleBackButton);
    connect(buttons[START], &QPushButton::clicked, this, &N_Queen::solveNQueens);

    start_x=(width-300-70-70-100-50)/2+90;
    start_y=height-100;
    speedSlider=new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(1, 20);
    speedSlider->setValue(10);
    speedLabel = new QLabel("Speed: 10", this);
    connect(speedSlider, &QSlider::valueChanged, [this](int value) {
        time_delay = abs(value*10-205); // Update the speed variable
        speedLabel->setText(QString("Speed: %1").arg(value)); // Update the label
    });
    speedSlider->setGeometry(start_x,start_y+10, 300, 30);
    speedLabel->setGeometry( start_x-90, start_y, 80, 50);
    start_x+=90+300+30;
    sizeLabel = new QLabel("Grid Size:", this);
    sizeLabel->setGeometry(start_x , start_y, 70, 50);
    start_x+=80;
    sizeSelector = new QSpinBox(this);
    sizeSelector->setRange(4, 9); // Set the range for the size
    sizeSelector->setValue(n);
    sizeSelector->setGeometry(start_x, start_y, 100, 50);
    connect(sizeSelector, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](){ n=sizeSelector->value(); reset();});

    is_paused=false;
    is_reseted=false;
    this->setFixedSize(width, height);
    this->show();
    // update();
}

void N_Queen::set_button(int &start_x,int start_y,int w,int h,int separation,QString label,int button){
    buttons[button]= new QPushButton(label, this);
    buttons[button]->setGeometry(start_x,start_y,w,h);
    buttons[button]->setStyleSheet(" background-color: lightblue; font-size: 18px;");
    start_x+=w+separation;
}

void N_Queen::change_speed(bool fast){
    if(!fast){
        if(time_delay<300) time_delay+=20;
    }
    else{
        if(time_delay>20) time_delay-=20;
    }
}

void N_Queen::solve(int idx,bool &res){
    if (idx == n || res) {
        res = true;
        return;
    }
    // if(is_reseted) return;

    for (int i = 0; i < n; i++) {
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) return;
        if (col.find(i) == col.end() &&
            diagonal1.find(n - idx + i) == diagonal1.end() &&
            diagonal2.find(n + n - i - idx - 1) == diagonal2.end()) {

            col.insert(i);
            diagonal1.insert(n - idx + i);
            diagonal2.insert(n + n - i - idx - 1);
            grid[idx][i].second = 1;
            update();  // Force the UI to update

            // Block the recursion for 1 second
            for (int d = 0; d < time_delay; d+=1) {
                if (is_paused) break;
                QCoreApplication::processEvents();  // Process events during the delay
                QThread::msleep(1);
            }

            solve(idx + 1,res);

            while (is_paused) {
                QCoreApplication::processEvents();  // Allow GUI to remain responsive
            }
            if(is_reseted) return;
            if(!res){
                grid[idx][i].second = 0;
                update();  // Force the UI to update

                // Block the recursion for 1 second
                for (int d = 0; d < time_delay; d+=1) {
                    if (is_paused) break;
                    QCoreApplication::processEvents();  // Process events during the delay
                    QThread::msleep(1);
                }
            }
            else break;
            col.erase(col.find(i));
            diagonal1.erase(diagonal1.find(n - idx + i));
            diagonal2.erase(diagonal2.find(n + n - i - idx - 1));
        }
    }
}

void N_Queen::solveNQueens() {
    sizeSelector->setEnabled(false);
    reset();
    is_reseted=false;
    is_paused=false;
    buttons[RESET]->setEnabled(false);
    buttons[START]->setEnabled(false);
    buttons[PAUSE]->setEnabled(true);
    bool res=false;
    for (int i = 0; i < n; i++) {
        if(is_reseted) break;
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        col.insert(i);
        diagonal1.insert(n + i);
        diagonal2.insert(n + n - i - 1);
        grid[0][i].second = 1;

        update();  // Force the UI to update

        // Block the recursion for 1 second
        for (int d = 0; d < time_delay; d+=1) {
            if (is_paused) break;
            QCoreApplication::processEvents();  // Process events during the delay
            QThread::msleep(1);
        }

        solve(1,res);
        while (is_paused) {
            QCoreApplication::processEvents();  // Allow GUI to remain responsive
        }
        if(is_reseted) break;
        if(!res){
            grid[0][i].second = 0;
            update();  // Force the UI to update

            // Block the recursion for 1 second
            for (int d = 0; d < time_delay; d+=1) {
                if (is_paused) break;
                QCoreApplication::processEvents();  // Process events during the delay
                QThread::msleep(1);
            }
        }
        else break;
        col.erase(col.find(i));
        diagonal1.erase(diagonal1.find(n - 0 + i));
        diagonal2.erase(diagonal2.find(n + n - i - 0 - 1));
    }
    if(is_reseted) reset();
    is_reseted=false;
    buttons[RESET]->setEnabled(true);
    buttons[START]->setEnabled(true);
    buttons[PAUSE]->setEnabled(false);
    res=false;
    sizeSelector->setEnabled(true);
}

void N_Queen::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap QueenImage(":/images/images/Queen_image.png");
    painter.setPen(QColor(Qt::black));
    int x;
    for(int i=0;i<n;i++){
        if(i%2)x=0;
        else x=1;
        for(int j=0;j<n;j++){
            painter.drawRect(grid[i][j].first);
            if(grid[i][j].second==1){
                painter.drawPixmap(grid[i][j].first, QueenImage.scaled(grid[i][j].first.size(), Qt::KeepAspectRatio));
            }
            else if(x%2){
                painter.fillRect(grid[i][j].first, QColor(125,148,93));
            }
            else{
                painter.fillRect(grid[i][j].first, QColor(238,238,213));
            }
            x++;
        }
    }
}

void N_Queen::reset(){
    grid.clear();
    is_paused=false;
    is_reseted=true;
    col.clear();
    diagonal1.clear();
    diagonal2.clear();
    QVector<QPair<QRect,int>> temp(n);
    int y=(height-180)/(n+2),x,x_start;
    x=y;
    x_start=(width-y*n)/2-x;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            temp[j-1]={QRect(x_start+x*j+j-1,y*i+i-1,x,y),0};
        }
        grid.push_back(temp);
    }
    update();
}

void N_Queen::handle_pause_resume(){
    is_paused=!is_paused;
    if(is_paused){
        buttons[PAUSE]->setText("Resume");
        buttons[RESET]->setEnabled(true);
    }
    else{
        buttons[PAUSE]->setText("Pause");
        buttons[RESET]->setEnabled(false);
    }
}

void N_Queen::handleBackButton() {
    // Emit signal to tell the main window to go back to All_Sorts
    emit backToBacktracking();

    this->close();
}

N_Queen::~N_Queen() {
    speedSlider->disconnect() ;
    disconnect(buttons[PAUSE], &QPushButton::clicked, this, &N_Queen::handle_pause_resume);
    disconnect(buttons[RESET], &QPushButton::clicked, this, &N_Queen::reset);
    disconnect(buttons[BACK], &QPushButton::clicked, this, &N_Queen::handleBackButton);
    disconnect(buttons[START], &QPushButton::clicked, this, &N_Queen::solveNQueens);
    delete speedSlider;
    speedSlider = nullptr ;
    delete speedLabel;
    speedLabel = nullptr ;
    delete sizeLabel;
    sizeLabel = nullptr ;
    delete sizeSelector;
    sizeSelector = nullptr ;
    buttons.clear() ;
}
