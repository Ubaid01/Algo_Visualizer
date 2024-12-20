#ifndef BST_H
#define BST_H
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include<QLineEdit>
#include<QVector>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QEventLoop>
#include "types.h"
#include <QTimer>
#include <QResizeEvent>
#include<QQueue>
#include<QIntValidator>

class BST : public QWidget{
    Q_OBJECT

    int width , orgWidth;
    int height , orgHeight;
    int levels;
    int time_delay;
    TreeNode *root;
    TreeNode *in_process;
    QLabel *insert_label;
    QLineEdit *delete_input;
    QLabel *delete_label;
    QLineEdit *insert_input;
    TreeNode *empty_node;
    TreeNode *successor;
    QLabel *messagelabel;
    QMap<int,QPushButton*> buttons;
public:
    BST(QWidget *parent,int width,int height);
    void paintEvent(QPaintEvent *event) override;
    void print() ;
    void inorder(TreeNode* node, QQueue<QPair<TreeNode*, QPoint>>& queue, QVector<QPoint>& positions,
                 QPainterPath& highlightPath, int& x, int y);
    void animateMove(TreeNode* node, QPoint targetPosition);
    void reset();
    void resizeButtons(int w,int h);
    void insertion();
    void deletion();
    void set_tree();
    int get_height(TreeNode *node);
    void delete_Node(TreeNode* node,TreeNode *parent, int key, bool& found);
    void resizeEvent(QResizeEvent *event) override ;
    TreeNode* Find_successor(TreeNode *right);
    void move_subtree(TreeNode *from , TreeNode *to);
    void set_button(int &start_x,int start_y,int w,int h,int seperation,QString label,int button);
    void display_message(QString message,int time);
    ~BST();
signals:
    void backToAlgorithms(); // Signal to go back
protected slots:
    void handleBackButton(); // Slot for back button
};

#endif // BST_H
