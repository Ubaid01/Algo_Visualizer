#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include<QLabel>
#include "all_sorts.h"
#include "Backtracking.h"
#include "trees.h"

using namespace std;

class Algorithms : public QMainWindow{
    Q_OBJECT
public:
    int n;
    int width;
    int height;
    QMap<int,QPushButton*> buttons;
    QMap<int,QPair<QLabel*,QFrame*>> imageWidgets ;
    All_Sorts *ST;
    Backtracking *BT;
    Trees* TR ;

    Algorithms(QWidget *parent,int n,int width,int height);
    ~Algorithms();
    void set_button(int start_x,int start_y,int w,int h,int separation,QString label,int button, QString image_path = "" ,int img_width = 0 , int img_height = 0 );
    void Sort();
    void Backtrack();
    void Trees_call() ;
    void Change_button_status(bool status);
    void resizeEvent(QResizeEvent* event) override ;
signals:
    void resized(); // Custom signal to handle resizing
};

#endif // ALGORITHMS_H
