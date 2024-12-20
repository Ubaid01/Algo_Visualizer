#ifndef AVL_H
#define AVL_H

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
#include <QThread>
#include <QResizeEvent>
#include<QQueue>
#include<QIntValidator>

class AVL : public QWidget
{
    Q_OBJECT

    int width, orgWidth;
    int height, orgHeight;
    int levels;
    int time_delay;
    bool formEdges;
    bool isPaused;
    TreeNode *root;
    TreeNode *in_process;
    QLabel *insert_label;
    QLineEdit *delete_input;
    QLabel *delete_label;
    QLineEdit *insert_input;
    TreeNode *empty_node;
    TreeNode *predecessor;
    QLabel *messagelabel;
    QMap<int,QPushButton*> buttons;
public:
    AVL(QWidget* parent, int Width, int Height) ;
    void paintEvent(QPaintEvent *event) override;
    void changeButtonsStatus(bool status) ;
    void pauseResume() ;
    void reset();
    void resizeButtons(int w,int h);
    void insertion();
    void deletion();
    void set_tree();
    void setHeight( TreeNode* node ) ;
    int getMaxLevel( TreeNode* node ) ;
    int get_height(TreeNode* node);
    int getBalanceFactor(TreeNode* node) ;
    void delete_Node(TreeNode* node,TreeNode *parent, int key, bool& found);
    void resizeEvent(QResizeEvent *event) override ;
    TreeNode* find_predecessor(TreeNode *node);
    TreeNode* leftRotation( TreeNode* node ) ;
    TreeNode* rightRotation( TreeNode* node ) ;
    void balanceNode( TreeNode* node ) ;
    void move_subtree(TreeNode *from , TreeNode *to);
    void rotate_subtree( TreeNode *from , TreeNode *to ) ;
    void set_button(int &start_x,int start_y,int w,int h,int seperation,QString label,int button);
    void display_message(QString message,int time);
    ~AVL();
signals:
    void backToAlgorithms(); // Signal to go back
protected slots:
    void handleBackButton(); // Slot for back button
};

#endif // AVL_H
