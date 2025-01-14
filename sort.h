#ifndef SORT_H
#define SORT_H
#include<vector>
#include<QtWidgets>
#include <QPushButton>
#include <QRandomGenerator>
#include <QScreen>
#include<Qslider>
#include<QSpinBox>

using namespace std;

class Sort :public QWidget{
    Q_OBJECT
public:
    int width;
    int height;
    int n;
    vector<int> sorted_part;
    //bool function_running;
    QPushButton *start;
    QPushButton *Reset;
    QPushButton *Back;
    QPushButton *Pause;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QLabel *sizeLabel;
    QSpinBox *sizeSelector;
    vector<pair<int,QRect>> array;   // array for simulation and storing value
    int time_delay;// for time change
    bool is_paused;
    bool is_reseted;

    Sort(QWidget *parent,int n,int width,int height);
    ~Sort();
    //void paintEvent(QPaintEvent *event);
    virtual void reset();
    void handle_pause_resume();
    void setting_size(int newSize);

signals:
    void backToAllSorts(); // Signal to go back

private slots:
    void handleBackButton(); // Slot for back button
private:

};
#endif // SORT_H
