#ifndef N_QUEEN_H
#define N_QUEEN_H

#include <QWidget>
#include<QVector>
#include<QSet>
#include<QMap>
#include<QPushButton>
#include<QCoreApplication>
#include<QSlider>
#include<QSpinBox>
#include<QLabel>
#include<QThread>

class N_Queen : public QWidget
{
    Q_OBJECT
public:

    int n;
    int width;
    int height;
    int time_delay;
    bool is_paused;
    bool is_reseted;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QLabel *sizeLabel;
    QSpinBox *sizeSelector;
    QSet<int> diagonal1, diagonal2, col;
    QVector<QVector<QPair<QRect,int>>> grid;
    QMap<int,QPushButton*> buttons;

    N_Queen(QWidget *parent,int n,int width,int height);
    ~N_Queen();
    void solve(int idx,bool &res);
    void solveNQueens();
    void reset();
    void paintEvent(QPaintEvent *event);
    void set_button(int &start_x,int start_y,int w,int h,int seperation,QString label,int button);
    void change_speed(bool fast);
    void handle_pause_resume();

signals:
    void backToBacktracking(); // Signal to go back

private slots:
    void handleBackButton(); // Slot for back button
};

#endif // N_QUEEN_H
