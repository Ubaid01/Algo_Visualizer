#include "bst.h"
#include <QPainterPath>

BST::BST(QWidget *parent,int Width,int Height)
    :QWidget(parent),width(Width),height(Height),orgWidth(Width),orgHeight(Height),root(nullptr),in_process(nullptr),time_delay(300),levels(0),empty_node(nullptr),successor(nullptr){
    int num_buttons=8,w=100,h=50;
    int separation_x=(width-num_buttons*w)/(num_buttons-3);
    int start_x=separation_x, start_y=height-h-60;

    set_button(start_x,start_y,w,h,separation_x,"Back",BACK);
    connect(buttons[BACK], &QPushButton::clicked, this, &BST::handleBackButton);

    insert_label = new QLabel("Insert a new Node : ", this);
    insert_label->setGeometry(start_x, start_y, 100, 50);
    start_x += 110 ;
    // Create QLineEdit for the input
    insert_input = new QLineEdit(this);
    insert_input->setGeometry(start_x, start_y, 100, 40);
    insert_input->setValidator(new QIntValidator());
    start_x += 110 ;
    set_button(start_x,start_y,w,h,separation_x,"Insert",INSERT);
    connect(insert_input, &QLineEdit::returnPressed, this, &BST::insertion);
    connect(buttons[INSERT], &QPushButton::clicked, this, &BST::insertion);

    delete_label = new QLabel("Delete a Node : ", this);
    delete_label->setGeometry(start_x, start_y, 100, 50);
    start_x += 110 ;
    // Create QLineEdit for the input
    delete_input = new QLineEdit(this);
    delete_input->setGeometry(start_x, start_y, 100, 40);
    delete_input->setValidator(new QIntValidator());
    start_x += 110;
    set_button(start_x,start_y,w,h,separation_x,"Delete",DELETE);
    connect(delete_input, &QLineEdit::returnPressed, this, &BST::deletion);
    connect(buttons[DELETE], &QPushButton::clicked, this, &BST::deletion);
    set_button(start_x,start_y,w,h,separation_x,"Print",PRINT);
    connect(buttons[PRINT], &QPushButton::clicked, this, &BST::print);

    messagelabel = new QLabel(this);
    messagelabel->setGeometry(0,(height-200)/2,width,200);
    messagelabel->setVisible(false);
    messagelabel->setStyleSheet("color: black; font-size: 40px; font-family: 'Times New Roman'; font-style: italic; background-color: rgba(210, 210, 210, 0.7); padding: 10px;");
    messagelabel->setAlignment(Qt::AlignCenter);

    this->setGeometry(0,0,width, height);
    this->show();
}

void BST::set_button(int &start_x,int start_y,int w,int h,int separation,QString label,int button){
    buttons[button]= new QPushButton(label, this);
    buttons[button]->setGeometry(start_x,start_y,w,h);
    buttons[button]->setStyleSheet(" background-color: lightblue; font-size: 18px;");
    start_x+=w+separation;
}

void BST::resizeButtons(int w,int h){
    width=w ; height=h;
    int diff_x = (width - 800) / 5 ;
    int start_x = diff_x, start_y = height - 110;

    buttons[BACK]->move(start_x, start_y);
    start_x += diff_x + 100 ;
    insert_label->move(start_x, start_y);
    start_x += 110;
    insert_input->move(start_x, start_y);
    start_x += 110;
    buttons[INSERT]->move(start_x, start_y);
    start_x += diff_x + 100 ;
    delete_label->move(start_x, start_y);
    start_x += 110;
    delete_input->move(start_x, start_y);
    start_x += 110;
    buttons[DELETE]->move(start_x, start_y);
    start_x += diff_x + 100 ;
    buttons[PRINT]->move(start_x, start_y);
    messagelabel->resize(width , 200) ;
    messagelabel->move(0,(height-200)/2);
}

void BST::reset(){
    in_process=nullptr;
    empty_node=nullptr;
    successor=nullptr;
    resizeButtons(orgWidth,orgHeight) ;
    levels=0;
    if(!root) return;
    QQueue<TreeNode*> q;
    q.enqueue(root);
    while(!q.isEmpty()){
        TreeNode *curr=q.front();
        q.dequeue();
        if(curr->left) q.enqueue(curr->left);
        if(curr->right) q.enqueue(curr->right);
        delete curr;
    }
    root=nullptr;
    insert_input->clear() ; delete_input->clear() ;
    update();
}

void BST::print() {
    if (!root) {
        display_message("Empty Tree!", 750);
        return;
    }

    buttons[DELETE]->setEnabled(false);
    buttons[PRINT]->setEnabled(false);
    buttons[BACK]->setEnabled(false);
    buttons[INSERT]->setEnabled(false);
    QVector<QPoint> positions;
    int start_x = 25;
    int start_y = height - 190;

    QPainterPath highlight_path;
    QQueue<QPair<TreeNode*, QPoint>> q; // Queue for smooth transitions

    inorder(root, q, positions, highlight_path, start_x, start_y);
    QQueue<QLabel*> displayedLabels;
    while (!q.isEmpty()) {
        QPair<TreeNode*, QPoint> pair = q.dequeue();
        TreeNode* curNode = pair.first;
        QPoint targetPos = pair.second;
        in_process = curNode;
        update();
        QEventLoop loop;
        QTimer::singleShot(500, &loop, SLOT(quit()));
        loop.exec();

        animateMove(curNode, targetPos);
        update();
        QTimer::singleShot(500, &loop, SLOT(quit()));
        loop.exec();

        QString valueText = QString::number(curNode->data);
        int aw = static_cast<int>( valueText.length() ) * 7 + 20 ; // Dynamic width based on num.
        // qDebug() << valueText.length() << " " << aw ;
        QLabel* value_label = new QLabel(QString::number(curNode -> data), this);
        value_label->setGeometry(targetPos.x(), targetPos.y(), aw, 40);
        value_label->setAlignment(Qt::AlignCenter);
        value_label->setStyleSheet("font-size: 16px; background-color: lightgray;");
        value_label->show();
        displayedLabels.enqueue(value_label);
    }

    in_process = nullptr;
    QTimer::singleShot(2000, this, [displayedLabels]() mutable { // As & is not asserted as mutuable in QT so was giving error.
        while (!displayedLabels.isEmpty()) {
            QLabel* label = displayedLabels.dequeue();
            delete label;
        }
    });
    update() ;
    buttons[DELETE]->setEnabled(true);
    buttons[PRINT]->setEnabled(true);
    buttons[INSERT]->setEnabled(true);
    buttons[BACK]->setEnabled(true);
}

void BST::inorder(TreeNode* node, QQueue<QPair<TreeNode*, QPoint>>& queue, QVector<QPoint>& positions,
                           QPainterPath& highlightPath, int& x, int y) {
    if (!node) return;
    inorder(node->left, queue, positions, highlightPath, x, y);
    queue.enqueue({node, QPoint(x, y)});
    positions.push_back(QPoint(x, y));
    int dx = QString::number( node -> data ).length() * 7 + 20 ;
    x += dx + 15 ;
    inorder(node->right, queue, positions, highlightPath, x, y);
}

void BST::animateMove(TreeNode* node, QPoint targetPosition) {
    QPoint start = node->rect.center();
    QPoint end = targetPosition;
    int steps = 20 ;
    int dx = (end.x() - start.x()) / steps;
    int dy = (end.y() - start.y()) / steps;

    QLabel* tempLabel = new QLabel(QString::number(node->data), this);
    tempLabel->setGeometry(start.x() - 25, start.y() - 25, 50, 50);
    tempLabel->setAlignment(Qt::AlignCenter);
    tempLabel->setStyleSheet("font-size: 16px; background-color: lightblue;");
    tempLabel->show();

    for (int i = 0; i <= steps; i++) {
        tempLabel->move(start.x() + dx * i - 25, start.y() + dy * i - 25);
        QEventLoop loop;
        QTimer::singleShot(20, &loop, SLOT(quit()));
        loop.exec();
    }

    delete tempLabel;
}

void BST::insertion(){
    QString input_text = insert_input->text().trimmed();
    bool valid = false ;
    int number = input_text.toInt( &valid );
    if ( !valid ) {
        QMessageBox::warning(nullptr, "Invalid Input","Input must be in the integer bounds.\n");
        return;
    }

    buttons[DELETE]->setEnabled(false);
    buttons[PRINT]->setEnabled(false);
    buttons[INSERT]->setEnabled(false);
    buttons[BACK]->setEnabled(false);
    TreeNode *new_node=new TreeNode(number);
    int curHeight = 0 ;
    if(!root)
        root=new_node;
    else {
        in_process=root;
        TreeNode *temp=root;
        while( true ){
            update();
            QEventLoop loop;
            QTimer::singleShot(time_delay, &loop, SLOT(quit()));
            loop.exec();
            curHeight++ ;
            if( curHeight >= 6 ) {
                QMessageBox::warning(nullptr, "Message" , "Only less than 6 levels are supported for now!") ;
                delete new_node ; new_node = nullptr;
                in_process = nullptr ;
                buttons[INSERT]->setEnabled(true);
                buttons[DELETE]->setEnabled(true);
                buttons[PRINT]->setEnabled(true);
                return ;
            }
            if(number>temp->data){
                if(!temp->right){
                    temp->right=new_node;
                    new_node->parent=temp;
                    break;
                }
                temp=temp->right;
            }
            else if(number<temp->data){
                if(!temp->left){
                    temp->left=new_node;
                    new_node->parent=temp;
                    break;
                }
                temp=temp->left;
            }
            else{
                display_message("Duplicate_Entry!",time_delay) ;
                delete new_node;
                break;
            }
            in_process=temp;
        }
    }
    in_process=nullptr;
    set_tree();
    buttons[INSERT]->setEnabled(true);
    buttons[DELETE]->setEnabled(true);
    buttons[PRINT]->setEnabled(true);
    buttons[BACK]->setEnabled(true);
    insert_input->clear();
    insert_input->setFocus() ;
}

TreeNode* BST::Find_successor(TreeNode* right){
    while(right->left)
        right=right->left;

    return right;
}

void BST::display_message(QString message,int time){
    messagelabel->setVisible(true);
    messagelabel->setText(message);
    QEventLoop loop;
    QTimer::singleShot(time, &loop, SLOT(quit()));
    loop.exec();
    messagelabel->setVisible(false);
}

void BST::delete_Node(TreeNode* node,TreeNode *parent, int key, bool& found){
    if( !node )
        return ;
    QEventLoop loop;
    QTimer::singleShot(time_delay, &loop, SLOT(quit()));
    loop.exec();
    in_process=node;
    TreeNode *temp=node;
    update() ;
    if(key>node->data){
        if(node->right){
            delete_Node(node->right,node,key,found);
        }
    }
    else if(key<node->data){
        if(node->left){
            delete_Node(node->left,node,key,found);
        }
    }
    else{
        found = true ;
        if(node->left && node->right){
            temp=Find_successor(node->right);
            successor=new TreeNode(temp) ;
            successor->parent=successor->right=successor->left=nullptr;
            empty_node=node;// marking as deleted
            // temp->parent=nullptr;// marking as null so the parent child arow not drawn
            display_message("Replacing root value with Inorder Successor's value",1500);
            move_subtree(successor,node);
            node->data=successor->data;
            delete successor;
            successor=nullptr;
            empty_node=nullptr;
            display_message("Deleting Inorder Successor",1500);
            delete_Node(node->right,node,temp->data,found);
            update();
            QEventLoop loop;
            QTimer::singleShot(400, &loop, SLOT(quit()));
            loop.exec();
            if(node->left) node->left->parent=node;
            if(node->right) node->right->parent=node;
            if(node) node->parent=parent;
            return;
        }
        else if(node->left){
            if(parent && parent->left==node) parent->left=node->left;
            if(parent && parent->right==node) parent->right=node->left;
            node->left->parent=nullptr;
            empty_node=node->left;// marking as deleted
            move_subtree(node->left,node);
            node= node->left;
            node->parent=parent;
            update();
            empty_node=nullptr;
            QEventLoop loop;
            QTimer::singleShot(400, &loop, SLOT(quit()));
            loop.exec();
            if(parent==nullptr){
                root=node;
            }
            delete temp;
        }
        else if(node->right){
            if(parent && parent->left==node) parent->left=node->right;
            if(parent && parent->right==node) parent->right=node->right;
            node->right->parent=nullptr;
            empty_node=node->right;// marking as deleted
            move_subtree(node->right,node);
            node= node->right;
            node->parent=parent;
            update();
            empty_node=nullptr;
            QEventLoop loop;
            QTimer::singleShot(400, &loop, SLOT(quit()));
            loop.exec();
            if(parent==nullptr){
                root=node;
            }
            delete temp;
        }
        else {
            if(parent && parent->left==node) parent->left=nullptr;
            if(parent && parent->right==node) parent->right=nullptr;
            if(!parent) root=nullptr;
            node=nullptr;
            delete temp;
        }
    }

    if( node )
        node->parent=parent;
    else    return ;
    if(node->left) node->left->parent=node;
    if(node->right) node->right->parent=node;
}

void BST::move_subtree(TreeNode* from, TreeNode* to){
    int current_dx,current_dy,steps=100,step_x,step_y;
    // qDebug()<<" dx "<<dx<<" dy "<<dy<<" stepx "<<step_x<<" stepy "<<step_y;
    QQueue<TreeNode*> q,sub_nodes;
    q.enqueue(from);
    while(!q.empty()){
        int size=q.size();
        while( size-- ){
            if(q.front()->left){
                q.enqueue(q.front()->left);
            }
            if(q.front()->right){
                q.enqueue(q.front()->right);
            }
            sub_nodes.enqueue(q.front());
            q.dequeue();
        }
    }

    for (int i = 0; i < steps; i++) {
        // Dynamically calculate step size for this iteration
        current_dx = from->rect.left() - to->rect.left();
        current_dy = from->rect.top() - to->rect.top();
        step_x = std::ceil(current_dx / double(steps - i));
        step_y = std::ceil(current_dy / double(steps - i));

        for (int j = 0; j < sub_nodes.size(); j++) {
            sub_nodes.front()->rect.moveLeft(sub_nodes.front()->rect.left() - step_x);
            sub_nodes.front()->rect.moveTop(sub_nodes.front()->rect.top() - step_y);
            sub_nodes.enqueue(sub_nodes.front());
            sub_nodes.dequeue();
        }
        update();
        QEventLoop loop;
        QTimer::singleShot(10, &loop, SLOT(quit()));
        loop.exec();
    }

    // Final adjustment to ensure exact alignment
    int final_dx = from->rect.left() - to->rect.left();
    int final_dy = from->rect.top() - to->rect.top();
    for (int j = 0; j < sub_nodes.size(); j++) {
        sub_nodes.front()->rect.moveLeft(sub_nodes.front()->rect.left() - final_dx);
        sub_nodes.front()->rect.moveTop(sub_nodes.front()->rect.top() - final_dy);
        sub_nodes.enqueue(sub_nodes.front());
        sub_nodes.dequeue();
    }
    update();
}

void BST::deletion(){
    QString delete_text = delete_input->text().trimmed();
    bool valid = false ;
    int number = delete_text.toInt( &valid );
    if ( !valid ) {
        QMessageBox::warning(nullptr, "Invalid Input", "BST dose not contain values out of integer bound.\n");
        return;
    }

    if(!root) { display_message("Empty BST Found!",750) ; delete_input->clear(); return; }
    buttons[INSERT]->setEnabled(false);
    buttons[DELETE]->setEnabled(false);
    buttons[PRINT]->setEnabled(false);
    bool found = false ;
    delete_Node(root,nullptr,number, found);
    in_process=nullptr;
    set_tree();
    if( !found )
        display_message("Key Not Found!",750) ;
    buttons[INSERT]->setEnabled(true);
    buttons[DELETE]->setEnabled(true);
    buttons[PRINT]->setEnabled(true);
    delete_input->clear();
    delete_input->setFocus() ;
}

int BST::get_height(TreeNode *node){
    if(!node) return 0;

    return std::max(get_height(node->left),get_height(node->right))+1;
}

// void BST::set_tree(){
//     levels=get_height(root);
//     if(levels==0){
//         update();
//         return;
//     }
//     root->parent=nullptr;
//     int start_x,start_y,w,h,separation_x,separation_y;
//     w = width / ( std::pow(2,levels) * 2.5 );
//     int r=std::min(std::max(w,35),70);
//     separation_y=std::min(100,(height-250-r*levels)/levels);
//     start_y=separation_y;
//     QQueue<QPair<TreeNode*,int>> q;
//     q.enqueue( {root,0} );

//     for(int i=0;i<levels;i++){
//         int row_size = q.size(); // Same as std::pow(2,i);
//         separation_x =( width - row_size * r ) / row_size ;
//         start_x = separation_x / 2 ;

//         for(int j=0;j<row_size;j++){
//             TreeNode *curr = q.front().first ;
//             int change = std::min(std::max(q.front().second, -65), 65);

//             q.dequeue();
//             if(curr){
//                 if( i > 4 ) // start_x = 0.
//                     change = ( curr -> parent ? ( curr->parent->left==curr ? curr->parent->rect.x() - 30 : curr->parent->rect.x() + 10 ) : change ) ;
//                 else if( i == 4 && j != 0 ) {
//                     if( j == 1 )
//                         change = 55;
//                     else if( j == 14 )
//                         change = -55 ;
//                     else if( j == 6 || j == 10 || j == 12 )
//                         change = -10 ;
//                     else if( j == 7 || j == 11 || j == 13 || j == 15 )
//                         change = change + 10 ;
//                     else
//                         change = 25 ;
//                     // qDebug() << start_x + change ;
//                 }
//                 curr->rect={ start_x + change,start_y,r,r};
//                 qDebug() << "For val = " << curr->data << " x,y = " << curr->rect.x() << " " << curr->rect.y() << " Change = " << change;
//                 if(curr->left)  q.enqueue( {curr->left,change + 25} );
//                 else q.enqueue({nullptr,0});
//                 if(curr->right)
//                     q.enqueue( {curr->right, change - 20} );
//                 else q.enqueue({nullptr,0});
//             }
//             else{
//                 q.enqueue( {nullptr,0} );
//                 q.enqueue( {nullptr,0} );
//             }

//             start_x += separation_x + r;
//         }
//         start_y += separation_y + r;
//     }
//     update() ;
// }

void BST::set_tree() {
    levels = get_height(root);
    if (levels == 0) {
        update();
        return;
    }

    root->parent = nullptr;
    int nodeSize = std::max(40, 70 - (levels - 1) * 4);
    const int separation_x = 20;
    const int separation_y = 50;
    int curWidth = (nodeSize + separation_x) * std::pow(2, levels) - separation_x ;
    int curHeight = (nodeSize + separation_y) * levels;

    this->setMinimumSize(curWidth, curHeight);
    int start_x, start_y;
    start_y = separation_y ;
    QQueue<QPair<TreeNode*, int>> q;
    q.enqueue({root, curWidth / 2 - 50 });

    for (int i = 0; i < levels; i++) {
        int row_size = q.size();
        int horizontalOffset = std::pow(2, levels - i - 1) * (nodeSize + separation_x) / 2;

        for (int j = 0; j < row_size; j++) {
            TreeNode* curr = q.front().first;
            int parent_x = q.front().second;
            q.dequeue();

            if (curr) {
                curr->rect = {parent_x + nodeSize / 2, start_y, nodeSize, nodeSize};
                if (curr->left)
                    q.enqueue({curr->left, parent_x - horizontalOffset}); // Enqueue with offset.
                if (curr->right)
                    q.enqueue({curr->right, parent_x + horizontalOffset});
            }
        }
        start_y += nodeSize + separation_y;
    }
    update();
}

void BST::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    // Get the new width and height of the widget from the resize event
    int newWidth = event->size().width();
    int newHeight = event->size().height();
    resizeButtons(newWidth,newHeight) ;
}

void BST::paintEvent(QPaintEvent *event){
    if(!root) return;
    QString numberText;
    QPainter painter(this);
    int max_font_size=root->rect.height()*0.25,sep_x,sep_y;
    QQueue<TreeNode*> q;
    q.enqueue(root);
    while(!q.isEmpty()){
        int size=q.size();
        for(int j=0;j<size;j++){
            TreeNode *curr=q.front();
            q.dequeue();
            if(curr->left) q.enqueue(curr->left);
            if(curr->right) q.enqueue(curr->right);
            // if(curr->rect.width()==0) continue;// marked as deleted
            painter.setPen(QColor(Qt::black));
            if(curr==in_process)
                painter.setBrush(Qt::gray);
            if( curr == empty_node )
                painter.setPen( Qt::green ) ;

            painter.drawEllipse(curr->rect);
            painter.setBrush(Qt::NoBrush);
            painter.setFont(QFont("Arial", max_font_size));

            QFontMetrics metrics(painter.font());
            painter.setPen(Qt::red);
            numberText=QString::number(curr->data);
            int font_size=max_font_size;
            if(metrics.horizontalAdvance(numberText)>curr->rect.width()){
                font_size*=(curr->rect.width()/(metrics.horizontalAdvance(numberText)*1.0));
            }
            painter.setFont(QFont("Arial", font_size));
            QFontMetrics new_metrics(painter.font());
            sep_y=(curr->rect.height()-new_metrics.height())/2+new_metrics.ascent();
            sep_x=(curr->rect.width()-new_metrics.horizontalAdvance(numberText))/2;
            if(empty_node!=curr)
                painter.drawText(QPoint(curr->rect.left()+sep_x,curr->rect.top()+sep_y),numberText);// node with two child is currently in process of deletion

            if (curr->parent) {
                // Calculate points for parent and child
                int change = ( curr->parent->right == curr ) ? 20 : -20 ;
                QPoint parent_p = {curr->parent->rect.left() + curr->parent->rect.width() / 2, curr->parent->rect.bottom() + 2};
                QPoint child_p = {curr->rect.left() + curr->rect.width() / 2 - change, curr->rect.top() + 3};

                QPen pen(Qt::blue, 2, Qt::SolidLine);
                painter.setPen(pen);
                painter.drawLine(parent_p, child_p);

                painter.setPen(QPen(Qt::black));
                QLineF line(parent_p, child_p);
                double angle = std::atan2(-line.dy(), line.dx()); // Angle of the line in radians by float-point.

                QPointF arrowP1 = child_p - QPointF(sin(angle + M_PI / 6) * 10, cos(angle + M_PI / 6) * 10); // Left side
                QPointF arrowP2 = line.p2() - QPointF(sin(angle + M_PI - M_PI / 6) * 10, cos(angle + M_PI - M_PI / 6) * 10); // Right side

                QPolygonF arrowHead;
                arrowHead.clear() ;
                arrowHead << child_p << arrowP1 << arrowP2 ;
                painter.setBrush(Qt::black);
                painter.drawPolygon(arrowHead);
                painter.setBrush(Qt::NoBrush) ; // To avoid coloring whole next-child.
            }
        }
    }
    if(successor){
        painter.setPen(Qt::red);
        numberText=QString::number(successor->data);
        painter.setFont(QFont("Arial", max_font_size));
        painter.drawText(QPoint(successor->rect.left()+sep_x,successor->rect.top()+sep_y),numberText);
    }
}


void BST::handleBackButton() {
    this->reset() ;
    emit backToAlgorithms();
    this->close();
}

BST::~BST() {
    delete root; root = nullptr ;
    delete in_process; in_process = nullptr;
    delete successor; successor = nullptr ;
    delete insert_label; insert_label=nullptr;
    delete delete_input; delete_input = nullptr;
    delete delete_label; delete_label=nullptr;
    delete insert_input; insert_input = nullptr;
    delete empty_node; empty_node=nullptr;
    delete messagelabel; messagelabel = nullptr ;
    qDeleteAll(buttons) ;
}
