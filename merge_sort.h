#ifndef MERGE_SORT_H
#define MERGE_SORT_H
#include"sort.h"

using namespace std;

class merge_sort:public Sort
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
    vector<vector<vector<pair<int,QRect>>>> whole_layer;

    merge_sort(QWidget* parent,int n,int width,int height);
    void sorting();
    vector<pair<int,QRect>> merge( vector<pair<int,QRect>> &x1,vector<pair<int,QRect>>& x2);
    void simulation();
    void paintEvent(QPaintEvent *event);
    void reset();
};

#endif // MERGE_SORT_H
