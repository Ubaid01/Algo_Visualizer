#include "trees.h"

Trees::Trees(QWidget *parent,int N,int Width,int Height)
    :QWidget(parent),n(N),width(Width),height(Height),BS_TREE(nullptr),BST_Scroll(nullptr), AVL_TREE(nullptr) , AVL_Scroll(nullptr) {
    int num_rows=4;
    int separation=50,w=(width-250)/4,start_x=50,h=(height-450)/num_rows,start_y=50,separation_y=100;

    QString headers[] = {"Algorithms", "Visualization", "Video Explanation", "Article"};
    for (int i = 0; i < 4; i++) {
        labels[header + i] = new QLabel(headers[i], this);
        labels[header + i]->setGeometry(start_x + i * (separation + w), start_y, w, h);
        labels[header + i]->setStyleSheet("QLabel { font: bold 20px; background-color: rgb(140,208,232); border: 2px solid black; "
                                            "border-radius: 10px; padding: 10px; }");
        labels[header + i]->setAlignment(Qt::AlignCenter);
    }

    start_y += separation_y + h ;
    set_button(start_x,start_y,w,h,separation,separation_y,"Binary_Search_Tree",bst);
    set_button(start_x,start_y,w,h,separation,separation_y,"AVL_Tree",avl);
    buttons[visuals][bst]->setIcon(QIcon(":/images/images/bst_icon.png"));
    buttons[visuals][bst]->setIconSize(QSize(96, 96));
    buttons[visuals][avl]->setIcon(QIcon(":/images/images/avl_icon.png"));
    buttons[visuals][avl]->setIconSize(QSize(160, 160));

    buttons[visuals][Back_to_main]=new QPushButton("Back to Main menu",this);
    buttons[visuals][Back_to_main]->setGeometry((width-w)/2, height-100-h, w, h);
    buttons[visuals][Back_to_main]->setStyleSheet("color:white; background-color: gray; font-size: 20px; border: 2px solid black; border-radius: 10px ;");
    connect(buttons[visuals][bst], &QPushButton::clicked, this, &Trees::BST_connect);
    connect(buttons[visuals][avl], &QPushButton::clicked, this, &Trees::AVL_connect);
    connect(buttons[visuals][Back_to_main], &QPushButton::clicked, this, &Trees::handleBackButton);

    this->setGeometry(0,0,width, height);
    this->show();
}

void Trees::BST_connect(){
    Change_button_status(false);

    if ( !BS_TREE || !BST_Scroll ) {
        BST_Scroll = new QScrollArea(this);
        BST_Scroll->setWidgetResizable(true);
        BST_Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        BST_Scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        BS_TREE = new BST(BST_Scroll, width, height); // Set ScrollArea as parent.
        BST_Scroll->setWidget(BS_TREE);
        BST_Scroll->resize(width, height);

        BST_Scroll->setStyleSheet("QScrollArea { background: transparent; border: none; }");
        BS_TREE->setStyleSheet("QWidget { background: transparent; }");

        // Connect back button signal to return to the parent widget
        connect(BS_TREE, &BST::backToAlgorithms, this, [this]() {
            BST_Scroll->hide();
            this->show();
            this->Change_button_status(true);
        });
    }
    BS_TREE->show();
    // BS_TREE->raise();
    BST_Scroll->show();
    BST_Scroll->raise();
}

void Trees::AVL_connect(){
    Change_button_status(false);

    if ( !AVL_TREE || !AVL_Scroll ) {
        AVL_Scroll = new QScrollArea(this);
        AVL_Scroll->setWidgetResizable(true);
        AVL_Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        AVL_Scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        AVL_TREE = new AVL(AVL_Scroll, width, height);
        AVL_Scroll->setWidget(AVL_TREE);
        AVL_Scroll->resize(width, height);
        AVL_Scroll->setStyleSheet("QScrollArea { background: transparent; border: none; }");
        AVL_TREE->setStyleSheet("QWidget { background: transparent; }");

        connect(AVL_TREE, &AVL::backToAlgorithms, this, [this]() {
            AVL_Scroll->hide();
            this->show();
            this->Change_button_status(true);
        });
    }

    AVL_TREE->show();
    AVL_Scroll->show();
    AVL_Scroll->raise();
}

void Trees::display_article(QString name) {
    QWidget *articleWindow = new QWidget();
    articleWindow->setWindowTitle("Algorithm Article");
    articleWindow->setFixedSize( width / 1.5 , height / 1.25 );

    QVBoxLayout *layout = new QVBoxLayout(articleWindow);
    // Create a scroll area to contain the images
    QScrollArea *scrollArea = new QScrollArea(articleWindow);
    QWidget *scrollContainer = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContainer);

    QLabel *image = new QLabel(scrollContainer);
    image->setPixmap(QPixmap(":/images/images/" + name + ".png"));
    image->setAlignment(Qt::AlignCenter);
    scrollLayout->addWidget(image);

    // Set the scroll container layout and set it in the scroll area
    scrollContainer->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContainer);

    layout->addWidget(scrollArea);
    articleWindow->setLayout(layout);
    articleWindow->show();
}

void Trees::set_button(int start_x,int &start_y,int w,int h,int separation,int separation_y,QString label,int button){
    QStringList toolTips = {
        "Visualize how the " + label + " works.",
        "Watch a videos explanation of " + label + ".",
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
            connect(btn, &QPushButton::clicked, this, [=]() { showvideosWindow(button); });
        }
        else {
            buttons[articles][button] = btn;
            connect(btn, &QPushButton::clicked, this, [=]() { display_article(label); });
        }
    }

    start_y += h + separation_y;
}

void Trees::showvideosWindow(int buttonIndex) {
    QWidget *videosWindow = new QWidget();
    videosWindow->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    videosWindow->setWindowTitle("Algorithm videos");
    videosWindow->resize( width / 1.25 , height / 1.25 );

    QVBoxLayout *layout = new QVBoxLayout(videosWindow);

    // videos player setup
    QMediaPlayer *mediaPlayer = new QMediaPlayer(videosWindow);
    QVideoWidget *videosWidget = new QVideoWidget(videosWindow);
    mediaPlayer->setVideoOutput(videosWidget);

    // Audio output setup
    QAudioOutput *audioOutput = new QAudioOutput(videosWindow);
    mediaPlayer->setAudioOutput(audioOutput);
    layout->addWidget(videosWidget);

    // Add videos controls
    QSlider *positionSlider = new QSlider(Qt::Horizontal, videosWindow);
    positionSlider->setRange(0, 0);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, positionSlider, &QSlider::setValue);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, positionSlider, &QSlider::setMaximum);
    connect(positionSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    // Control buttons
    QHBoxLayout *controlLayout = new QHBoxLayout();
    QPushButton *playButton = new QPushButton("Play", videosWindow);
    QPushButton *pauseButton = new QPushButton("Pause", videosWindow);
    QPushButton *stopButton = new QPushButton("Stop", videosWindow);

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
        QMessageBox::critical(this, "Error", "videoss directory not found: " + videoDirectory);
        return;
    }

    QString videosName;
    switch (buttonIndex) {
    case 0:
        videosName = "AVL.mp4";
        break;
    case 1:
        videosName = "BST.mp4";
        break;
    default:
        break;
    }

    QString videosFile = videoDirectory + "/" + videosName;
    if (!QFile::exists(videosFile)) {
        QMessageBox::critical(this, "Error", "videos file not found: " + videosFile);
        return;
    }

    QUrl videosUrl = QUrl::fromLocalFile(videosFile);
    mediaPlayer->setSource(videosUrl);

    mediaPlayer->play();
    videosWindow->setLayout(layout);
    videosWindow->show();
}

void Trees::Change_button_status(bool status){
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

void Trees::handleBackButton() {
    emit backToAlgorithms();
    this->close();
}

Trees::~Trees() {
    if( buttons[visuals][Back_to_main] != nullptr )
        disconnect(buttons[visuals][Back_to_main], &QPushButton::clicked, this, &Trees::handleBackButton);

    if (buttons[visuals][bst] != nullptr)
        disconnect(buttons[visuals][bst], &QPushButton::clicked, this, &Trees::BST_connect);

    if (buttons[visuals][avl] != nullptr)
        disconnect(buttons[visuals][avl], &QPushButton::clicked, this, &Trees::AVL_connect);

    if (BS_TREE != nullptr) {
        delete BS_TREE;
        BS_TREE = nullptr;
    }
    if( BST_Scroll != nullptr ) { delete BST_Scroll ; BST_Scroll=nullptr ; }

    if( AVL_TREE != nullptr) {
        delete AVL_TREE;
        AVL_TREE = nullptr;
    }
    if( AVL_Scroll != nullptr ) { delete AVL_Scroll ; AVL_Scroll=nullptr ; }

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
