#ifndef ALL_SORTS_H
#define ALL_SORTS_H
#include <QPushButton>
#include <QMap>
#include<QMainWindow>
#include<QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QImage>
#include <QScrollArea>
#include"bubble_sort.h"
#include"selection_sort.h"
#include"insertion_sort.h"
#include"merge_sort.h"
#include"quick_sort.h"

using namespace std;

class All_Sorts : public QWidget{
    Q_OBJECT
public:
    int n;
    int width;
    int height;
    QMap<int,QMap<int,QPushButton*>> buttons;
    QMap<int,QLabel*> labels;
    QVector<QString> sortingAlgos ;
    bubble_sort *BS;
    selection_sort *SS;
    insertion_sort *IS;
    merge_sort *MS;
    quick_sort *QS;

    All_Sorts(QWidget *parent,int n,int width,int height);
    ~All_Sorts() ;
    void sorting_function(int algoritham);
    void Change_button_status(bool status);
    void set_button(int start_x,int &start_y,int w,int h,int separation,QString label,int button);
    void display_article(QString name);

signals:
    void backToAlgorithms(); // Signal to go back

private slots:
    void handleBackButton(); // Slot for back button
    void showVideoWindow(int buttonIndex);
};

#endif // ALL_SORTS_H
