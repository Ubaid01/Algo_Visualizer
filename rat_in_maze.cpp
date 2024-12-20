#include "rat_in_maze.h"
#include "types.h"
#include <qpainter.h>
#include <QEventloop >
#include<QTimer>

Rat_in_Maze::Rat_in_Maze(QWidget *parent,int N,int Width,int Height)
    :QWidget(parent),width(Width),height(Height),time_delay(100),row(N),col(N),messagelabel(nullptr){
    QVector<QPair<QRect,int>> temp(row);
    int x,y,x_start;
    x=y=std::min((height-200)/(row+2),(width-100)/(col+2));
    x_start=(width-y*col)/2-x;
    colors[Empty] = {255,255,255} ;
    colors[obstacle] = Qt::gray ;
    colors[in_path] = {200,230,255} ;
    for(int i=1;i<=row;i++){
        for(int j=1;j<=col;j++){
            temp[j-1]={QRect(x_start+x*j+j-1,y*i+i-1,x,y),Empty};
        }
        maze.push_back(temp);
    }
    int separation=40,button_w=150,start_x=(width-5*button_w-4*separation)/2,button_h=50,start_y=Height-200;
    set_button(start_x,start_y,button_w,button_h,separation,"Back",BACK);
    set_button(start_x,start_y,button_w,button_h,separation,"Start",START);
    set_button(start_x,start_y,button_w,button_h,separation,"Pause",PAUSE);
    set_button(start_x,start_y,button_w,button_h,separation,"Reset",RESET);
    set_button(start_x,start_y,button_w,button_h,separation,"Set Maze",SET_MAZE);
    connect(buttons[PAUSE], &QPushButton::clicked, this, &Rat_in_Maze::handle_pause_resume);
    connect(buttons[RESET], &QPushButton::clicked, this, &Rat_in_Maze::reset);
    connect(buttons[BACK], &QPushButton::clicked, this, &Rat_in_Maze::handleBackButton);
    connect(buttons[START], &QPushButton::clicked, this, &Rat_in_Maze::Find_path);
    connect(buttons[SET_MAZE], &QPushButton::clicked, this, &Rat_in_Maze::set_maze);

    start_x=(width-300-70-70-100-80)/2;
    start_y=height-100;
    speedSlider=new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(1, 20);
    speedSlider->setValue(10);
    speedLabel = new QLabel("Speed: 10", this);
    connect(speedSlider, &QSlider::valueChanged, [this](int value) {
        time_delay = abs(value*10-205);
        speedLabel->setText(QString("Speed: %1").arg(value));
    });
    speedLabel->setGeometry( start_x, start_y, 70, 50);
    speedSlider->setGeometry(start_x+90,start_y+10, 300, 30);
    start_x+=90+300+30;
    sizeLabel.first = new QLabel("Rows :", this);
    sizeLabel.first->setGeometry(start_x , start_y, 70, 50);
    sizeSelector.first = new QSpinBox(this);
    sizeSelector.first->setRange(2, 9);
    sizeSelector.first->setValue(row);
    sizeSelector.first->setGeometry(start_x+80, start_y, 100, 50);
    connect(sizeSelector.first, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](){ row=sizeSelector.first->value(); reset();});

    start_x+=80+30+100;

    sizeLabel.second = new QLabel("Columns :", this);
    sizeLabel.second->setGeometry(start_x , start_y, 70, 50);
    sizeSelector.second = new QSpinBox(this);
    sizeSelector.second->setRange(2, 9);
    sizeSelector.second->setValue(col);
    sizeSelector.second->setGeometry(start_x+80, start_y, 100, 50);
    connect(sizeSelector.second, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](){ col=sizeSelector.second->value(); reset();});

    messagelabel = new QLabel(this);
    messagelabel->setGeometry(0,(height-200)/2,width,200);
    messagelabel->setVisible(false);
    rat_position={-1,-1};
    is_paused=false;
    is_reseted=false;
    this->setFixedSize(width, height);
    this->show();
    // update();
    buttons[PAUSE]->setEnabled(false);
}

void Rat_in_Maze::set_maze(){
    if(buttons[SET_MAZE]->text()=="Lock Maze"){
        buttons[SET_MAZE]->setText("Set Maze");
        return;
    }
    reset();
    buttons[SET_MAZE]->setText("Lock Maze");
    buttons[START]->hide();
    while(buttons[SET_MAZE]->text()=="Lock Maze"){
        QCoreApplication::processEvents();
    }
    buttons[START]->show();
}

void Rat_in_Maze::mousePressEvent(QMouseEvent *event){
    QPoint clickPos = event->pos();
    if(buttons[SET_MAZE]->text()=="Set Maze") return;
    int adjustedX = clickPos.x() - maze[0][0].first.left();
    int adjustedY = clickPos.y() - maze[0][0].first.top();

    // Map to grid cell
    int clickedRow = adjustedY / maze[0][0].first.height();
    int clickedCol = adjustedX / maze[0][0].first.width();

    // Check bounds to avoid invalid cell clicks
    if (clickedRow >= 0 && clickedRow < row && clickedCol >= 0 && clickedCol < col) {
        if (maze[clickedRow][clickedCol].second == Empty) {
            maze[clickedRow][clickedCol].second = obstacle; // Example action: Turn it into an obstacle
        }
        else
            maze[clickedRow][clickedCol].second = Empty; // Toggle back to empty
        update(); // Refresh the widget to show the change
    }
}

void Rat_in_Maze::set_button(int &start_x,int start_y,int w,int h,int separation,QString label,int button){
    buttons[button]= new QPushButton(label, this);
    buttons[button]->setGeometry(start_x,start_y,w,h);
    buttons[button]->setStyleSheet(" background-color: lightblue; font-size: 18px;");
    start_x+=w+separation;
}

void Rat_in_Maze::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap QueenImage(":/images/images/Rat_image.png");
    painter.setPen(QColor(Qt::black));
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            painter.drawRect(maze[i][j].first);
            painter.fillRect(maze[i][j].first, colors[ maze[i][j].second ] );
        }
    }

    QPen pathPen ; // Was working but giving error with constructor. QPen pathPen(QColor(Qt::red))
    pathPen.setColor( QColor( Qt::red ) ) ;
    pathPen.setWidth(2);
    painter.setPen(pathPen);

    for (int i = 1; i < path.size(); i++) {
        QPoint p1 = path[i - 1];
        QPoint p2 = path[i];
        QRect rect1 = maze[p1.x()][p1.y()].first;
        QRect rect2 = maze[p2.x()][p2.y()].first;

        QPoint center1 = rect1.center();
        QPoint center2 = rect2.center();
        painter.drawLine(center1, center2);  // Draw line b/w centers of consecutive points
    }

    // Draw rat position if it's valid
    if (rat_position.first != -1)
        painter.drawPixmap(maze[rat_position.first][rat_position.second].first, QueenImage.scaled(maze[rat_position.first][rat_position.second].first.size(), Qt::KeepAspectRatio));

}

void Rat_in_Maze::Find_path()
{
    is_paused=false;
    is_reseted=false;
    sizeSelector.first->setEnabled(false);
    sizeSelector.second->setEnabled(false);
    buttons[SET_MAZE]->setEnabled(false);
    buttons[PAUSE]->setEnabled(true);
    buttons[RESET]->setEnabled(false);
    buttons[START]->setEnabled(false);
    bool res=false;
    rat_position={0,0};
    if(maze[0][0].second==Empty)
        solve(0,0,res);
    if(is_reseted) reset();
    else{
        if(res){
            rat_position={row-1,col-1};
            messagelabel->setText("Path Found");
        }
        else{
            messagelabel->setText("Path Not Found");
        }
        messagelabel->setStyleSheet("color: black; font-size: 80px; font-family: 'Times New Roman'; font-style: italic; background-color: rgba(210, 210, 210, 0.7); padding: 10px;");
        messagelabel->setAlignment(Qt::AlignCenter);
        messagelabel->setVisible(true);
        QEventLoop loop;
        QTimer::singleShot(2000, &loop, SLOT(quit()));
        loop.exec();
        messagelabel->setVisible(false);
    }
    is_paused=false;
    is_reseted=false;
    sizeSelector.first->setEnabled(true);
    sizeSelector.second->setEnabled(true);
    buttons[PAUSE]->setEnabled(false);
    buttons[RESET]->setEnabled(true);
    buttons[START]->setEnabled(true);
    buttons[SET_MAZE]->setEnabled(true);
}

void Rat_in_Maze::solve(int x,int y,bool &res){
    while (is_paused) {
        QCoreApplication::processEvents();  // Allow GUI to remain responsive
    }
    if(is_reseted || res) return;
    path.append(QPoint(x, y));
    rat_position={x,y};
    if(x==row-1 && y==col-1){
        res=true;
        maze[row-1][col-1].second=in_path;
        update();
        return;
    }
    if(is_reseted) return;

    int temp=maze[x][y].second;
    maze[x][y].second = in_path;

    update();

    QEventLoop loop;
    QTimer::singleShot(time_delay, &loop, SLOT(quit()));
    loop.exec();

    if (x + 1 != row && maze[x + 1][y].second == Empty)
    {
        update();
        solve( x +1, y,res);
    }
    if(res || is_reseted) return;
    rat_position={x,y};
    update();
    QTimer::singleShot(time_delay, &loop, SLOT(quit()));
    loop.exec();
    while (is_paused) {
        QCoreApplication::processEvents();  // Allow GUI to remain responsive
    }
    if (y + 1 != col && maze[x][y + 1].second == Empty)
    {
        solve( x, y+1,res);
    }
    if(res || is_reseted) return;
    rat_position={x,y};
    update();
    QTimer::singleShot(time_delay, &loop, SLOT(quit()));
    loop.exec();
    while (is_paused) {
        QCoreApplication::processEvents();  // Allow GUI to remain responsive
    }
    if (x - 1 != -1 && maze[x - 1][y].second == Empty)
    {
        solve( x - 1, y,res);
    }
    if(res || is_reseted) return;
    rat_position={x,y};
    update();
    QTimer::singleShot(time_delay, &loop, SLOT(quit()));
    loop.exec();
    while (is_paused) {
        QCoreApplication::processEvents();  // Allow GUI to remain responsive
    }
    if (y - 1 != -1 && maze[x][y - 1].second == Empty)
    {
        solve( x, y-1,res);
    }

    if(is_reseted) return;
    if(!res){
        maze[x][y].second = temp;
        rat_position={x,y};
        path.removeLast() ; // Backtrack.
        update();  // Force the UI to update

        // Block the recursion for 1 second
        QEventLoop loop;
        QTimer::singleShot(time_delay, &loop, SLOT(quit()));
        loop.exec();
    }
}

void Rat_in_Maze::reset(){
    maze.clear();
    path.clear();
    is_paused=false;
    is_reseted=true;
    rat_position={-1,-1};
    QVector<QPair<QRect,int>> temp(col);
    int x,y,x_start;
    x=y=std::min((height-200)/(row+2),(width-100)/(col+2));
    x_start=(width-y*col)/2-x;
    for(int i=1;i<=row;i++){
        for(int j=1;j<=col;j++){
            temp[j-1]={QRect(x_start+x*j+j-1,y*i+i-1,x,y),Empty};
        }
        maze.push_back(temp);
    }
    update();
}

void Rat_in_Maze::handle_pause_resume(){
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

void Rat_in_Maze::handleBackButton() {
    // Emit signal to tell the main window to go back to All_Sorts
    emit backToBacktracking();
    this->close();
}

Rat_in_Maze::~Rat_in_Maze() {
    speedSlider->disconnect() ;
    disconnect(buttons[PAUSE], &QPushButton::clicked, this, &Rat_in_Maze::handle_pause_resume);
    disconnect(buttons[RESET], &QPushButton::clicked, this, &Rat_in_Maze::reset);
    disconnect(buttons[BACK], &QPushButton::clicked, this, &Rat_in_Maze::handleBackButton);
    disconnect(buttons[START], &QPushButton::clicked, this, &Rat_in_Maze::Find_path);
    disconnect(buttons[SET_MAZE], &QPushButton::clicked, this, &Rat_in_Maze::set_maze);
    delete speedSlider;
    speedSlider = nullptr ;
    delete speedLabel;
    speedLabel = nullptr ;
    delete sizeLabel.first ;
    sizeLabel.first = nullptr ;
    delete sizeLabel.second ;
    sizeLabel.second = nullptr ;
    delete sizeSelector.first ;
    sizeSelector.first = nullptr ;
    delete sizeSelector.second ;
    sizeSelector.second = nullptr ;
    delete messagelabel;
    messagelabel = nullptr ;
    buttons.clear() ;
}
