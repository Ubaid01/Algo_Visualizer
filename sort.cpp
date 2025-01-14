#include "sort.h"
#include <QPainter>

Sort::Sort(QWidget *parent,int n,int width,int height)
    : QWidget(parent),n(n),width(width),height(height),array(n),time_delay(10),speedSlider(nullptr),speedLabel(nullptr)
{
    is_reseted=false;
    int x=min(width/(n+2),150),y=x,start_x=(width-n*x)/2,start_y=(height)/2-y;
    for(int i=0;i<n;i++){
        array[i]={ QRandomGenerator::global()->bounded(100) , QRect{start_x+i*x,start_y,x,y} };
    }
    start_x=(width-5*100-50*4)/2;
    int separation_x=50;
    Pause=new QPushButton("Pause",this);
    Pause->setGeometry(start_x,height-150,100,50);
    connect(Pause, &QPushButton::clicked, this,&Sort::handle_pause_resume);
    start_x+=(100+separation_x);
    start=new QPushButton("Start",this);
    start->setGeometry(start_x,height-150,100,50);
    start_x+=(100+separation_x);
    Reset=new QPushButton("Reset",this);
    Reset->setGeometry(start_x,height-150,100,50);
    connect(Reset, &QPushButton::clicked, this, &Sort::reset);
    start_x+=(100+separation_x);
    Back=new QPushButton("Back",this);
    Back->setGeometry(start_x,height-150,100,50);
    connect(Back, &QPushButton::clicked, this, &Sort::handleBackButton);

    start_x+=(100+separation_x);
    sizeLabel = new QLabel("Array Size:", this);
    sizeLabel->setGeometry(start_x , height - 150, 70, 50); // Adjust position
    start_x+=70;
    sizeSelector = new QSpinBox(this);
    sizeSelector->setRange(3, 10); // Set the range for the size
    sizeSelector->setValue(n);      // Default value
    sizeSelector->setGeometry(start_x, height - 150, 100, 50);
    connect(sizeSelector, QOverload<int>::of(&QSpinBox::valueChanged), this, &Sort::setting_size);


    speedSlider=new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(1, 20); // Speed range: 1 (slow) to 10 (fast)
    speedSlider->setValue(10);    // Default speed is 5 (medium)
    speedLabel = new QLabel("Speed: 10", this);
    connect(speedSlider, &QSlider::valueChanged, [&](int value) {
        time_delay = abs(value-21); // Update the speed variable
        speedLabel->setText(QString("Speed: %1").arg(value)); // Update the label
    });
    speedLabel->setGeometry(width/2-150-80, height - 80, 80, 30); // Place above the slider
    speedSlider->setGeometry(width/2-100, height - 80, 300, 30); // Slider width of 300px


    sorted_part.clear();
    this->setFixedSize(width, height); // Set the size of the widget
    this->show(); // Ensure the widget is visible
    start->setEnabled(true);
    Reset->setEnabled(true);
    is_paused=false;
    Pause->setText("Pause");
    Pause->setEnabled(false);
}
void Sort::setting_size(int newSize){
    n = newSize;
    reset();
}
void Sort::reset(){
    is_reseted=true;
    is_paused=false;
    Pause->setText("Pause");
    Pause->setEnabled(false);
    start->setEnabled(true);
    Reset->setEnabled(true);
    int x=min(width/(n+2),150),y=x,start_x=(width-n*x)/2,start_y=(height)/2-y;
    array.resize(n);
    for(int i=0;i<n;i++){
        array[i]={ QRandomGenerator::global()->bounded(100) , QRect{start_x+i*x,start_y,x,y} };
    }
    sorted_part.clear();
    update();
}

void Sort::handleBackButton() {
    // Emit signal to tell the main window to go back to All_Sorts
    emit backToAllSorts();

    // Close the current sorting widget
    this->close();
}

void Sort::handle_pause_resume(){
    is_paused=!is_paused;
    if(is_paused){
        Pause->setText("Resume");
        Reset->setEnabled(true);
    }
    else{
       Pause->setText("Pause");
        Reset->setEnabled(false);
    }
}

Sort::~Sort() {
    disconnect(Pause, &QPushButton::clicked, this,&Sort::handle_pause_resume);
    disconnect(Reset, &QPushButton::clicked, this, &Sort::reset);
    disconnect(Back, &QPushButton::clicked, this, &Sort::handleBackButton);
    disconnect(sizeSelector, QOverload<int>::of(&QSpinBox::valueChanged), this, &Sort::setting_size);
    disconnect(speedSlider, &QSlider::valueChanged, this , nullptr ) ;
    delete start ;
    start = nullptr ;
    delete Reset ;
    Reset = nullptr ;
    delete Back ;
    Back = nullptr ;
    delete Pause ;
    Pause = nullptr ;
    delete speedSlider ;
    speedSlider = nullptr ;
    delete sizeLabel ;
    sizeLabel = nullptr ;
    delete sizeSelector ;
    sizeSelector = nullptr ;
}
