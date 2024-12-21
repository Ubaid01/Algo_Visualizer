#ifndef TYPES_H
#define TYPES_H

#include <QRect>

enum ALGOS {
    SORTING = 1,
    BACKTRACKING,
    TREES
};

enum SortingsAlgos {
    Bubble = 1,
    Selection,
    Insertion,
    Merge,
    Quick,
    Back_to_main,
    visuals,
    sorts,
    articles,
    video,
    header
};

enum BacktrackAlgos {
    RAT_IN_MAZE = 1,
    N_QUEEN,
};

enum Colors{
    Empty=1,
    obstacle,
    in_path
};

enum TreeAlgos {
    avl ,
    bst
};

enum Operations{
    INSERT=1,
    DELETE,
    START,
    BACK,
    PAUSE,
    RESET,
    SET_MAZE,
    PRINT
};

struct TreeNode{
    int data , height ;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
    QRect rect;
    TreeNode(int d):data(d),left(nullptr),right(nullptr),parent(nullptr),height(1){}
    TreeNode(int d,TreeNode *l,TreeNode *r,TreeNode *p):data(d),left(l),right(r),parent(p),height(1){}
    TreeNode(TreeNode *copy):data(copy->data),left(copy->left),right(copy->right),parent(copy->parent),rect(copy->rect),height(copy->height){}
};

#endif // TYPES_H
