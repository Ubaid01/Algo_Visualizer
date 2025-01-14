#include "backtracking.h"
#include "types.h"

Backtracking::Backtracking(QWidget *parent,int N,int Width,int Height)
    :QWidget(parent),n(N),width(Width),height(Height),NQ(nullptr),RM(nullptr){
    int num_rows=4;
    int separation=50,w=(width-50*5)/4,start_x=50,h=(height-200-50*5)/num_rows,start_y=50,separation_y=100;

    QString headers[] = {"Algorithms", "Visualization", "Video Explanation", "Article"};
    for (int i = 0; i < 4; i++) {
        labels[header + i] = new QLabel(headers[i], this);
        labels[header + i]->setGeometry(start_x + i * (separation + w), start_y, w, h);
        labels[header + i]->setStyleSheet("QLabel { font: bold 20px; background-color: rgb(140,208,232); border: 2px solid black; "
                                           "border-radius: 10px; padding: 10px; }");
        labels[header + i]->setAlignment(Qt::AlignCenter);
    }

    start_y+= separation_y + h ;
    set_button(start_x,start_y,w,h,separation,separation_y,"Rat In Maze",RAT_IN_MAZE);
    set_button(start_x,start_y,w,h,separation,separation_y,"N Queens",N_QUEEN);
    buttons[visuals][RAT_IN_MAZE]->setIcon(QIcon(":/images/images/maze_icon.png"));
    buttons[visuals][N_QUEEN]->setIcon(QIcon(":/images/images/chess_icon.png"));
    buttons[visuals][N_QUEEN]->setIconSize(QSize(96, 96));
    buttons[visuals][RAT_IN_MAZE]->setIconSize(QSize(96, 96));

    buttons[visuals][Back_to_main]=new QPushButton("Back to Main menu",this);
    buttons[visuals][Back_to_main]->setGeometry((width-w)/2, height-100-h, w, h);
    buttons[visuals][Back_to_main]->setStyleSheet("color:white; background-color: gray; font-size: 20px; border: 2px solid black; border-radius: 10px ;");
    connect(buttons[visuals][N_QUEEN], &QPushButton::clicked, this, &Backtracking::N_Queen_function);
    connect(buttons[visuals][RAT_IN_MAZE], &QPushButton::clicked, this, &Backtracking::Rat_in_Maze_function);
    connect(buttons[visuals][Back_to_main], &QPushButton::clicked, this, &Backtracking::handleBackButton);

    this->setGeometry(0,0,width, height);
    this->show();
}

void Backtracking::N_Queen_function(){
    Change_button_status(false);
    if(NQ==nullptr){
        NQ=new N_Queen(this,n,width,height);
        NQ->hide();  // Hide initially until sorting starts
        connect(NQ, &N_Queen::backToBacktracking, this, [this]() {
            this->show();        // Show the All_Sorts window
            this->Change_button_status(true); // Show the sorting buttons again
        });
    }

    NQ->show();
    NQ->raise();
}

void Backtracking::Rat_in_Maze_function(){
    Change_button_status(false);
    if(RM==nullptr){
        RM=new Rat_in_Maze(this,n,width,height);
        RM->hide();  // Hide initially until sorting starts
        connect(RM, &Rat_in_Maze::backToBacktracking, this, [this]() {
            this->show();        // Show the All_Sorts window
            this->Change_button_status(true); // Show the sorting buttons again
        });
    }
    RM->show();
    RM->raise();
}

void Backtracking::display_article(QString name) {
    // Create a new window to display the article and images
    QWidget *articleWindow = new QWidget();
    articleWindow->setWindowTitle("Algorithm Article");
    articleWindow->setFixedSize( width / 1.5 , height / 1.25 );

    // Layout to arrange the article and images
    QVBoxLayout *layout = new QVBoxLayout(articleWindow);

    // Create a scroll area to contain the images
    QScrollArea *scrollArea = new QScrollArea(articleWindow);
    QWidget *scrollContainer = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContainer);

    // Add the images for the selected algorithm (each algorithm has three images)
    QLabel *image1 = new QLabel(scrollContainer);
    image1->setPixmap(QPixmap(":/images/images/" + name + ".png"));
    image1->setAlignment(Qt::AlignCenter);
    scrollLayout->addWidget(image1);

    // Set the scroll container layout and set it in the scroll area
    scrollContainer->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContainer);

    // Add the scroll area to the main layout
    layout->addWidget(scrollArea);

    // Set the layout and show the window
    articleWindow->setLayout(layout);
    articleWindow->show();
}

void Backtracking::set_button(int start_x,int &start_y,int w,int h,int separation,int separation_y,QString label,int button){
    QStringList toolTips = {
        "Visualize how the " + label + " works.",
        "Watch a video explanation of " + label + ".",
        "View the pseudocode for " + label + "."
    };

    QLabel *buttonLabel = new QLabel(label, this);
    buttonLabel->setGeometry(start_x, start_y, 250, 50);
    buttonLabel->setStyleSheet( "font: bold 20px; background-color: rgb(140,208,232); border: 2px solid black; padding: 5px; border-radius: 10px;" );
    buttonLabel->setAlignment(Qt::AlignCenter);
    labels[button] = buttonLabel;

    QString icons[] = { "", "video_icon.png", "Article_icon.png" };

    for (int i = 0; i < 3; i++) {
        QPushButton *btn = new QPushButton(this);
        btn->setGeometry(start_x + (separation + w) * (i + 1) + separation * 1.5 , start_y - 25, 100, 100);
        btn->setStyleSheet( "border: 2px solid black; border-radius: 12px;" );
        btn->setIcon(QIcon( ":/images/images/" + icons[i]));
        btn->setIconSize(QSize(94, 94));
        btn->setToolTip(toolTips[i]);

        if (i == 0)
            buttons[visuals][button] = btn;
        else if (i == 1) {
            buttons[video][button] = btn;
            connect(btn, &QPushButton::clicked, this, [=]() { showVideoWindow(button); });
        }
        else {
            buttons[articles][button] = btn;
            connect(btn, &QPushButton::clicked, this, [=]() { display_article(label); });
        }
    }

    start_y += h + separation_y;
}

void Backtracking::showVideoWindow(int buttonIndex) {
    // Create a new window for video playback
    QWidget *videoWindow = new QWidget();
    videoWindow->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    videoWindow->setWindowTitle("Algorithm Video");
    videoWindow->resize( width / 1.25 , height / 1.25 );

    QVBoxLayout *layout = new QVBoxLayout(videoWindow);

    // Video player setup
    QMediaPlayer *mediaPlayer = new QMediaPlayer(videoWindow);
    QVideoWidget *videoWidget = new QVideoWidget(videoWindow);
    mediaPlayer->setVideoOutput(videoWidget);

    // Audio output setup
    QAudioOutput *audioOutput = new QAudioOutput(videoWindow);
    mediaPlayer->setAudioOutput(audioOutput);

    // Add video widget to the layout
    layout->addWidget(videoWidget);

    // Add video controls
    QSlider *positionSlider = new QSlider(Qt::Horizontal, videoWindow);
    positionSlider->setRange(0, 0);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, positionSlider, &QSlider::setValue);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, positionSlider, &QSlider::setMaximum);
    connect(positionSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    // Control buttons
    QHBoxLayout *controlLayout = new QHBoxLayout();
    QPushButton *playButton = new QPushButton("Play", videoWindow);
    QPushButton *pauseButton = new QPushButton("Pause", videoWindow);
    QPushButton *stopButton = new QPushButton("Stop", videoWindow);

    controlLayout->addWidget(playButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(stopButton);

    connect(playButton, &QPushButton::clicked, mediaPlayer, &QMediaPlayer::play);
    connect(pauseButton, &QPushButton::clicked, mediaPlayer, &QMediaPlayer::pause);
    connect(stopButton, &QPushButton::clicked, mediaPlayer, &QMediaPlayer::stop);

    layout->addLayout(controlLayout);
    layout->addWidget(positionSlider);

    QString videoDirectory = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("../../videos");
    if (!QDir(videoDirectory).exists()) {
        QMessageBox::critical(this, "Error", "Videos directory not found: " + videoDirectory);
        return;
    }

    QString videoName;
    switch (buttonIndex) {
    case 1:
        videoName = "Rat_in_Maze.mp4";
        break;
    case 2:
        videoName = "N_Queen.mp4";
        break;
    default:
        break;
    }

    QString videoFile = videoDirectory + "/" + videoName;
    if (!QFile::exists(videoFile)) {
        QMessageBox::critical(this, "Error", "Video file not found: " + videoFile);
        return;
    }

    QUrl videoUrl = QUrl::fromLocalFile(videoFile);
    mediaPlayer->setSource(videoUrl);

    mediaPlayer->play();
    // Set the layout and show the window
    videoWindow->setLayout(layout);
    videoWindow->show();
}

void Backtracking::Change_button_status(bool status){
    if(status){
        for(auto& x:labels)
            x->show();

        for(auto& x:buttons){
            for(auto& y:x)
                y->show();
        }
    }
    else{
        for(auto& x:labels)
            x->hide();

        for(auto& x:buttons){
            for(auto& y:x)
                y->hide();
        }
    }
}

void Backtracking::handleBackButton() {
    // Emit signal to tell the main window to go back to All_Sorts
    emit backToAlgorithms();

    // Close the current sorting widget
    this->close();
}

Backtracking::~Backtracking() {
    if( buttons[visuals][Back_to_main] != nullptr )
        disconnect(buttons[visuals][Back_to_main], &QPushButton::clicked, this, &Backtracking::handleBackButton);

    if (buttons[visuals][RAT_IN_MAZE] != nullptr)
        disconnect(buttons[visuals][RAT_IN_MAZE], &QPushButton::clicked, this, &Backtracking::Rat_in_Maze_function);

    if (buttons[visuals][N_QUEEN] != nullptr)
        disconnect(buttons[visuals][N_QUEEN], &QPushButton::clicked, this, &Backtracking::N_Queen_function);

    if (NQ != nullptr) {
        delete NQ;
        NQ = nullptr;
    }

    if (RM != nullptr) {
        delete RM;
        RM = nullptr;
    }

    for (auto &category : buttons) {
        for (auto &btn : category) {
            if (btn != nullptr) {
                btn->disconnect();
                delete btn ;
            }
        }
    }
    buttons.clear();

    for (auto &lbl : labels) {
        if( lbl ) {
            lbl->disconnect() ;
            delete lbl;
        }
    }
    labels.clear();
}
