#ifndef TREES_H
#define TREES_H

#include <QPushButton>
#include <QMap>
#include<QMainWindow>
#include<QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QImage>
#include<QVideoWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSlider>
#include <QDir>
#include "bst.h"
#include "avl.h"

using namespace std;

class Trees : public QWidget
{
    Q_OBJECT
private:
    int n;
    int width;
    int height;
    BST* BS_TREE ;
    QScrollArea *BST_Scroll ;
    AVL* AVL_TREE ;
    QScrollArea *AVL_Scroll ;
    QMap<int,QMap<int,QPushButton*>> buttons;
    QMap<int,QLabel*> labels;
public:
    Trees(QWidget *parent,int n,int width,int height);
    ~Trees();
    void Change_button_status(bool status);
    void set_button(int start_x,int &start_y,int w,int h,int seperation,int seperation_y,QString label,int button);
    void BST_connect();
    void AVL_connect();
    void display_article(QString name);
signals:
    void backToAlgorithms(); // Signal to go back
private slots:
    void handleBackButton(); // Slot for back button
    void showvideosWindow(int buttonIndex);
};

#endif // TREES_H
