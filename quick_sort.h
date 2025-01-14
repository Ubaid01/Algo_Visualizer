#ifndef QUICK_SORT_H
#define QUICK_SORT_H
#include"sort.h"

using namespace std;

class quick_sort:public Sort
{
public:
    int levels;
    int separation_y;
    int separation_x;
    int start_y;
    int start_x;
    int box_width;
    int box_height;
    int num_paint;
    int time_delay_internal;

    vector<vector<pair<vector<pair<int,QRect>>,int>>> whole_layer;
    quick_sort(QWidget* parent,int n,int width,int height);
    int partition(vector<pair<int,QRect>> &arr);
    void partitioning_with_animation(vector<pair<int,QRect>> &arr,int I,int J);
    void animation(int s1,int s2,int i,int j);
    void sorting();
    vector<pair<int,QRect>> merge( vector<pair<int,QRect>> &x1,vector<pair<int,QRect>>& x2);
    void simulation();
    void paintEvent(QPaintEvent *event);
    void reset();
};

#endif // QUICK_SORT_H
