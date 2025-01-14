#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H
#include"sort.h"

class bubble_sort:public Sort
{
public:
    bubble_sort(QWidget* parent,int n,int width,int height);
    void sorting();
    void simulation(int s1,int s2);
    void paintEvent(QPaintEvent *event);
};

#endif // BUBBLE_SORT_H
