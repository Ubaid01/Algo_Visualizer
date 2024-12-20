#ifndef RAT_IN_MAZE_H
#define RAT_IN_MAZE_H
#include <QWidget>
#include<QVector>
#include<QSet>
#include<QMap>
#include<QPushButton>
#include<QSlider>
#include<QSpinBox>
#include<QCoreApplication>
#include<QMouseEvent>
#include<QLabel>

class Rat_in_Maze : public QWidget
{
    Q_OBJECT
public:
    int width;
    int height;
    int time_delay;
    bool is_paused;
    bool is_reseted;
    int row;
    int col;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QPair<QLabel*,QLabel*> sizeLabel;
    QPair<QSpinBox*,QSpinBox*> sizeSelector;
    QPair<int,int> rat_position;
    QLabel *messagelabel;
    QMap<int,QColor> colors;
    QVector<QVector<QPair<QRect,int>>> maze;
    QMap<int,QPushButton*> buttons;
    QVector<QPoint> path;

    Rat_in_Maze(QWidget *parent,int n,int width,int height);
    ~Rat_in_Maze();
    void solve(int x,int y,bool &res);
    void Find_path();
    void reset();
    void paintEvent(QPaintEvent *event) override ;
    void set_button(int &start_x,int start_y,int w,int h,int seperation,QString label,int button);
    void mousePressEvent(QMouseEvent *event) override;
    void set_maze();
    void handle_pause_resume();

signals:
    void backToBacktracking(); // Signal to go back

private slots:
    void handleBackButton(); // Slot for back button
};

#endif // RAT_IN_MAZE_H
