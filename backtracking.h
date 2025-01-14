#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include<QSlider>
#include<QBoxLayout>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QImage>
#include <QScrollArea>
#include<QDir>
#include<QMessageBox>
#include "rat_in_maze.h"
#include "n_queen.h"

class Backtracking : public QWidget
{
    Q_OBJECT

    int n;
    int width;
    int height;
    N_Queen *NQ;
    Rat_in_Maze *RM;
    QMap<int,QMap<int,QPushButton*>> buttons;
    QMap<int,QLabel*> labels;
public:
    Backtracking(QWidget *parent,int n,int width,int height);
    ~Backtracking();
    void Change_button_status(bool status);
    void set_button(int start_x,int &start_y,int w,int h,int seperation,int seperation_y,QString label,int button);
    void N_Queen_function();
    void Rat_in_Maze_function();
    void display_article(QString name);
signals:
    void backToAlgorithms(); // Signal to go back

private slots:
    void handleBackButton(); // Slot for back button
    void showVideoWindow(int buttonIndex);
};

#endif // BACKTRACKING_H
