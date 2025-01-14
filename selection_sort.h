#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H
#include"sort.h"

class selection_sort:public Sort
{
public:
    selection_sort(QWidget* parent,int n,int width,int height);
    void sorting();
    void simulation(int s1,int s2);
    void paintEvent(QPaintEvent *event);
};

#endif // SELECTION_SORT_H
