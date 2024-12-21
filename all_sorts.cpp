#include "all_sorts.h"
#include "types.h"

All_Sorts::All_Sorts(QWidget *parent,int N,int Width,int Height)
    :QWidget(parent),n(N),width(Width),height(Height),BS(nullptr),SS(nullptr),IS(nullptr),MS(nullptr),QS(nullptr){
    int num_rows=7;
    int separation = 12, w = (width - 250) / 4, start_x = 50, h = (height - 160 - (separation * num_rows) ) / num_rows, start_y = 50;
    // qDebug() << QDir::currentPath() ;
    QString headers[] = {"Algorithms", "Visualization", "Video Explanation", "Article"};
    for (int i = 0; i < 4; i++) {
        labels[header + i] = new QLabel(headers[i], this);
        labels[header + i]->setGeometry(start_x + i * (separation + w), start_y, w, h);
        labels[header + i]->setStyleSheet("QLabel { font: bold 20px; background-color: rgb(140,208,232); border: 2px solid black; "
                                           "border-radius: 10px; padding: 5px; }");
        labels[header + i]->setAlignment(Qt::AlignCenter);
    }
    start_y += separation + h + 20 ;

    QString buttonNames[] = {"Bubble Sort", "Selection Sort", "Insertion Sort" , "Merge Sort", "Quick Sort"};
    int buttonIndex[] = {Bubble, Selection, Insertion , Merge, Quick};
    for (int i = 0; i < 5; i++) {
        set_button(start_x, start_y, w, h, separation, buttonNames[i], buttonIndex[i]);
        start_y += separation ;
    }
    sortingAlgos = { "Bubble Sort.mp4" , "Selection Sort.mp4" , "Insertion Sort.mp4" , "Merge Sort.mp4" , "Quick Sort.mp4"} ;
    // Back button
    buttons[visuals][Back_to_main] = new QPushButton("Back to Main menu", this);
    buttons[visuals][Back_to_main]->setGeometry((width - w) / 2, start_y, w, ( h + 20 ) / 2);
    buttons[visuals][Back_to_main]->setStyleSheet("color:white; background-color: gray; font-size: 20px; border: 2px solid black; border-radius: 10px ;");
    connect(buttons[visuals][Back_to_main], &QPushButton::clicked, this, &All_Sorts::handleBackButton);

    this -> setGeometry( 0 , 0 , width , height ) ;
    this->show();
}

void All_Sorts::display_article(QString name) {
    QWidget *articleWindow = new QWidget();
    articleWindow->setWindowTitle("Algorithm Article");
    articleWindow->setFixedSize( width / 1.5 , height / 1.25 );

    QVBoxLayout *layout = new QVBoxLayout(articleWindow);
    QScrollArea *scrollArea = new QScrollArea(articleWindow);
    QWidget *scrollContainer = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContainer);

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

void All_Sorts::set_button(int start_x,int &start_y,int w,int h,int separation,QString label,int button){
    QString buttonLabels[] = {"Visualization", "Video Explanation", "Article"};
    QString buttonIcons[] = {"visuallization_icon.png", "video_icon.png", "Article_icon.png"};
    QSize buttonIconSize(75, 75);

    QString tooltips[] = {
        "Visualize yourself how the " + label + " works.",
        "An explanation video of how " + label + " works.",
        "View the pseudocode for the " + label + " algorithm."
    };

    // Set up the label
    labels[button] = new QLabel(label, this);
    labels[button]->setGeometry(start_x  + (w - 250) / 2, start_y + (h - 50) / 2, 250, 50);
    labels[button]->setStyleSheet("QLabel { font: bold 20px; background-color: rgb(140,208,232); border: 2px solid black; padding: 5px; border-radius: 10px; }");
    labels[button]->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < 3; i++) {
        buttons[i][button] = new QPushButton(this);
        buttons[i][button]->setGeometry(start_x + (i + 1) * (separation + w) + (w - 80) / 2, start_y + (h - 75) / 2, 75, 75);
        buttons[i][button]->setStyleSheet( "QPushButton { border: 2px solid black; border-radius: 12px; }" );
        buttons[i][button]->setIcon(QIcon( ":/images/images/" + buttonIcons[i]));
        buttons[i][button]->setIconSize(buttonIconSize);
        buttons[i][button]->setToolTip(tooltips[i]);

        // Connect buttons to their respective functions
        if (i == 0)
            connect(buttons[i][button], &QPushButton::clicked, this, [=]() { sorting_function(button); }); // By-value.
        else if (i == 1)
            connect(buttons[i][button], &QPushButton::clicked, this, [=]() { showVideoWindow(button); });
        else if (i == 2)
            connect(buttons[i][button], &QPushButton::clicked, this, [=]() { display_article(label); });
    }

    start_y += h + separation; // Update the y position for the next set of buttons
}


void All_Sorts::sorting_function(int algorithm){
    Change_button_status(false);
    if(algorithm==Bubble){
        if(BS==nullptr){
            BS = new bubble_sort(this,n,width,height);
            BS->hide();
            connect(BS, &Sort::backToAllSorts, this, [this]() {
                this->show();
                this->Change_button_status(true); // Show the sorting buttons again
            });
        }
        BS->reset();
        BS->show();
        BS->raise();
    }
    else if(algorithm==Selection){
        if(SS==nullptr){
            SS = new selection_sort(this,n,width,height);
            SS->hide();
            connect(SS, &Sort::backToAllSorts, this, [this]() {
                this->show();
                this->Change_button_status(true); // Show the sorting buttons again
            });
        }
        SS->reset();
        SS->show();
        SS->raise();
    }
    else if(algorithm==Insertion){
        if(IS==nullptr){
            IS = new insertion_sort(this,n,width,height);
            IS->hide();  // Hide initially until sorting starts
            connect(IS, &Sort::backToAllSorts, this, [this]() {
                this->show();        // Show the All_Sorts window
                this->Change_button_status(true); // Show the sorting buttons again
            });
        }
        IS->reset();
        IS->show();
        IS->raise();
    }
    else if(algorithm==Merge){
        if(MS==nullptr){
            MS = new merge_sort(this,n,width,height);
            MS->hide();  // Hide initially until sorting starts
            connect(MS, &Sort::backToAllSorts, this, [this]() {
                this->show();        // Show the All_Sorts window
                this->Change_button_status(true); // Show the sorting buttons again
            });
        }
        MS->reset();
        MS->show();
        MS->raise();
    }
    else{
        if(QS==nullptr){
            QS = new quick_sort(this,n,width,height);
            QS->hide();  // Hide initially until sorting starts
            connect(QS, &Sort::backToAllSorts, this, [this]() {
                this->show();        // Show the All_Sorts window
                this->Change_button_status(true); // Show the sorting buttons again
            });
        }
        QS->reset();
        QS->show();
        QS->raise();
    }
}


void All_Sorts::Change_button_status(bool status){
    if(status){
        for(auto& x:labels){
            x->show();
        }
        for(auto& x:buttons){
            for(auto& y:x){
                y->show();
            }
        }
    }
    else{
        for(auto& x:labels){
            x->hide();
        }
        for(auto& x:buttons){
            for(auto& y:x){
                y->hide();
            }
        }
    }
}

void All_Sorts::showVideoWindow(int buttonIndex) {
    QString videoDirectory = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("../../videos"); // From parent directory of build.
    if (!QDir(videoDirectory).exists()) {
        QMessageBox::critical(this, "Error", "Videos directory not found: " + videoDirectory);
        return;
    }

    QString videoFile = videoDirectory + "/" + sortingAlgos[buttonIndex - 1];
    if (!QFile::exists(videoFile)) {
        QMessageBox::critical(this, "Error", "Video file not found: " + videoFile);
        return;
    }

    QWidget *videoWindow = new QWidget();
    videoWindow->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    videoWindow->setWindowTitle("Algorithm Video");
    videoWindow->resize(width / 1.25, height / 1.25);

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

    // Set video source
    mediaPlayer->setSource(QUrl::fromLocalFile(videoFile));
    mediaPlayer->play();

    // Set the layout and show the window
    videoWindow->setLayout(layout);
    videoWindow->show();
}


void All_Sorts::handleBackButton() {
    // Emit signal to tell the main window to go back to All_Sorts
    emit backToAlgorithms();

    // Close the current sorting widget
    this->close();
}

All_Sorts::~All_Sorts() {
    if ( buttons[visuals][Back_to_main] )
        disconnect(buttons[visuals][Back_to_main], &QPushButton::clicked, this, &All_Sorts::handleBackButton);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            if ( buttons[j][i] != nullptr ) {
                if (j == 0)
                    disconnect(buttons[j][i], &QPushButton::clicked, this, &All_Sorts::sorting_function);
                else if (j == 1)
                    disconnect(buttons[j][i], &QPushButton::clicked, this, &All_Sorts::showVideoWindow);
                else if (j == 2)
                    buttons[j][i]->disconnect() ;
            }
        }
    }

    if ( BS ) {
        disconnect(BS, &Sort::backToAllSorts, this, &All_Sorts::show);
        delete BS;
        BS = nullptr;
    }
    if (SS) {
        disconnect(SS, &Sort::backToAllSorts, this, &All_Sorts::show);
        delete SS;
        SS = nullptr;
    }
    if (IS) {
        disconnect(IS, &Sort::backToAllSorts, this, &All_Sorts::show);
        delete IS;
        IS = nullptr;
    }
    if (MS) {
        disconnect(MS, &Sort::backToAllSorts, this, &All_Sorts::show);
        delete MS;
        MS = nullptr;
    }
    if (QS) {
        disconnect(QS, &Sort::backToAllSorts, this, &All_Sorts::show);
        delete QS;
        QS = nullptr;
    }

    buttons.clear();
    labels.clear();
}
