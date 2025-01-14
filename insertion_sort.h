#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H
#include"sort.h"

class insertion_sort:public Sort
{
private:
    QLine *line ;
public:
    insertion_sort(QWidget* parent,int n,int width,int height);
    ~insertion_sort() ;
    void sorting();
    void simulation(int s1,int s2);
    void paintEvent(QPaintEvent *event);
};

#endif // INSERTION_SORT_H
