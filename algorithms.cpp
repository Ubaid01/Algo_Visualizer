#include "algorithms.h"
#include "types.h"
#include<QMessageBox>

Algorithms::Algorithms(QWidget *parent, int N, int Width, int Height)
    : QMainWindow(parent), n(N), width(Width), height(Height), ST(nullptr), BT(nullptr) , TR(nullptr) {
    this->setWindowIcon( QIcon(":/images/images/icon.ico") ) ;
    QLabel* backgroundLabel = new QLabel(this);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setPixmap(QPixmap(":/images/images/background.png"));
    // ui -> label -> setPixmap( pixmap.scaled( ui -> label -> size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    backgroundLabel->setGeometry(0, 0, width, height);
    backgroundLabel->lower();// Bottom of widget stack

    connect(this, &Algorithms::resized, this, [backgroundLabel, this]() {
        backgroundLabel->setGeometry(0, 0, width, height);  // Update geometry dynamically on resize
    });

    int num_buttons = 3, separation = 150, button_w = 350;
    int start_x = ( width - ( ( button_w * num_buttons ) / 1.75 + separation * ( num_buttons - 1 ) ) ) / 2;
    int button_h = std::min((height - 500) / num_buttons, 50);
    int start_y = ( height - separation ) / 2;

    set_button(start_x, start_y, button_w, button_h, separation, "Sorts", SORTING, ":/images/images/sorting_algos.png", 952,444);
    set_button(start_x + button_w + separation, start_y, button_w, button_h, separation, "Backtracking", BACKTRACKING, ":/images/images/backtrack_algos.png", 1179, 576);
    set_button( ( width - start_x - separation ) / 2 , start_y + 1.85 * separation , button_w, button_h, separation, "Trees", TREES , ":/images/images/tree_algos.png" , 975, 551);

    connect(buttons[SORTING], &QPushButton::clicked, this, &Algorithms::Sort);
    connect(buttons[BACKTRACKING], &QPushButton::clicked, this, &Algorithms::Backtrack);
    connect(buttons[TREES], &QPushButton::clicked, this, &Algorithms::Trees_call);
}


void Algorithms::set_button(int start_x, int start_y, int w, int h, int separation, QString label, int button, QString image_path , int img_width , int img_height ) {
    QLabel* image_label = new QLabel(this);
    buttons[button] = new QPushButton(label, this);
    buttons[button]->setGeometry(start_x, start_y + 10, w, h);
    buttons[button]->setStyleSheet("background-color: lightblue; font-size: 25px; padding: 5px; border-radius: 5px;");

    QPixmap pixmap(image_path);
    if ( pixmap.isNull() )
        qWarning() << "Failed to load image from path:" << image_path;

    int scaled_width = w - 25 ;
    int scaled_height = (img_height * scaled_width) / img_width;

    QFrame* image_frame = new QFrame(this);
    image_frame->setFrameShape(QFrame::Box);
    image_frame->setLineWidth(3);
    image_frame->setStyleSheet("border: 2px solid black;");

    // Padding for image.
    image_frame->setGeometry(start_x + w / 2 - scaled_width / 2 - 5, start_y - h - scaled_height / 2 - 55 , scaled_width + 10, scaled_height + 10);
    image_label->setGeometry(start_x + w / 2 - scaled_width / 2, start_y - h - scaled_height / 2 - 55, scaled_width, scaled_height);  // Position the image inside the frame

    image_label->setPixmap(pixmap.scaled(scaled_width, scaled_height,Qt::KeepAspectRatio));
    image_label->setGeometry(start_x + w / 2 - scaled_width / 2, start_y - h - scaled_height / 2 - 50, scaled_width, scaled_height);  // Position the image inside the frame
    imageWidgets[button] = {image_label , image_frame} ;
}

void Algorithms::Sort() {
    Change_button_status(false);
    if (ST == nullptr) {
        ST = new All_Sorts(this, n, width, height);
        connect(ST, &All_Sorts::backToAlgorithms, this, [this]() {
            this->show();  // Show the All_Sorts window
            this->Change_button_status(true); // Show the sorting buttons again
        });
    }
    // if( sorts -> exec() == QDialog::Rejected )
    //     show() ;
    ST->show();
    ST->raise(); // To prioritize on stack of windows
}

void Algorithms::Backtrack() {
    Change_button_status(false);
    if (BT == nullptr) {
        BT = new Backtracking(this, n, width, height);
        connect(BT, &Backtracking::backToAlgorithms, this, [this]() {
            this->show();  // Show the All_Sorts window
            this->Change_button_status(true); // Show the sorting buttons again
        });
    }
    BT->show();
    BT->raise();
}

void Algorithms::Trees_call() {
    Change_button_status(false);

    if( TR == nullptr ) {
        TR = new Trees( this , n , width , height ) ;
        connect( TR , &Trees::backToAlgorithms , this, [this]() {
            this->show() ;
            this->Change_button_status(true) ;
        } ) ;
    }
    TR->show() ; TR->raise() ;
}


void Algorithms::Change_button_status(bool status) {
    if (status) {
        imageWidgets[SORTING].first->show() ;
        imageWidgets[SORTING].second->show() ;
        imageWidgets[BACKTRACKING].first->show();
        imageWidgets[BACKTRACKING].second->show();
        imageWidgets[TREES].first->show();
        imageWidgets[TREES].second->show();
        buttons[SORTING]->show();
        buttons[BACKTRACKING]->show();
        buttons[TREES] -> show() ;
    }
    else {
        imageWidgets[SORTING].first->hide();
        imageWidgets[SORTING].second->hide();
        imageWidgets[BACKTRACKING].first->hide();
        imageWidgets[BACKTRACKING].second->hide();
        imageWidgets[TREES].first->hide();
        imageWidgets[TREES].second->hide();
        buttons[SORTING]->hide();
        buttons[BACKTRACKING]->hide();
        buttons[TREES]->hide() ;
    }
}

void Algorithms::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event); // Call the base class implementation
    emit resized(); // Emit the resized signal
}

Algorithms::~Algorithms() {
    if (ST != nullptr) {
        delete ST;
        ST = nullptr;
    }

    if (BT != nullptr) {
        delete BT;
        BT = nullptr;
    }

    if( TR != nullptr ) {
        delete TR ; TR = nullptr ;
    }

    disconnect(buttons[SORTING], &QPushButton::clicked, this, &Algorithms::Sort);
    disconnect(buttons[BACKTRACKING], &QPushButton::clicked, this, &Algorithms::Backtrack);
    disconnect(buttons[TREES], &QPushButton::clicked, this, &Algorithms::Trees_call);
    qDeleteAll(buttons);  // Delete by parent-child object ownership model of QT
    for (auto& widgetPair : imageWidgets) {
        delete widgetPair.first;
        delete widgetPair.second;
    }
    imageWidgets.clear();
    buttons.clear();
}
