#include "avl.h"

AVL::AVL(QWidget *parent,int Width,int Height)
    :QWidget(parent),width(Width),height(Height),orgWidth(Width),orgHeight(Height),root(nullptr),in_process(nullptr),formEdges(true),isPaused(false),time_delay(400),levels(0), empty_node(nullptr),predecessor(nullptr){
    int num_buttons = 8 , w = 100 , h = 50 ;
    int separation_x=(width-num_buttons*w)/(num_buttons-3);
    int start_x=separation_x, start_y=height-h-60;

    set_button(start_x,start_y,w,h,separation_x,"Back",BACK);
    connect(buttons[BACK], &QPushButton::clicked, this, &AVL::handleBackButton);

    insert_label = new QLabel("Insert a new Node : ", this);
    insert_label->setGeometry(start_x, start_y, 100, 50);
    start_x += 110 ;
    // Create QLineEdit for the input
    insert_input = new QLineEdit(this);
    insert_input->setGeometry(start_x, start_y, 100, 40);
    insert_input->setValidator(new QIntValidator());
    start_x += 110 ;
    set_button(start_x,start_y,w,h,separation_x,"Insert",INSERT);
    connect(insert_input, &QLineEdit::returnPressed, this, &AVL::insertion);
    connect(buttons[INSERT], &QPushButton::clicked, this, &AVL::insertion);

    delete_label = new QLabel("Delete a Node : ", this);
    delete_label->setGeometry(start_x, start_y, 100, 50);
    start_x += 110 ;
    // Create QLineEdit for the input
    delete_input = new QLineEdit(this);
    delete_input->setGeometry(start_x, start_y, 100, 40);
    delete_input->setValidator(new QIntValidator());
    start_x += 110;
    set_button(start_x,start_y,w,h,separation_x,"Delete",DELETE);
    connect(delete_input, &QLineEdit::returnPressed, this, &AVL::deletion);
    connect(buttons[DELETE], &QPushButton::clicked, this, &AVL::deletion);
    set_button(start_x,start_y,w,h,separation_x,"Pause",PAUSE);
    connect(buttons[PAUSE], &QPushButton::clicked, this, &AVL::pauseResume);
    buttons[PAUSE]->setEnabled(false) ;

    messagelabel = new QLabel(this);
    messagelabel->setGeometry(0,(height-200)/2,width,200);
    messagelabel->setVisible(false) ;
    messagelabel->setStyleSheet("color: black; font-size: 40px; font-family: 'Times New Roman'; font-style: italic; background-color: rgba(210, 210, 210, 0.7); padding: 10px;");
    messagelabel->setAlignment(Qt::AlignCenter);

    this->setGeometry(0,0,width, height);
    this->show();
}

void AVL::set_button(int &start_x,int start_y,int w,int h,int separation,QString label,int button){
    buttons[button]= new QPushButton(label, this);
    buttons[button]->setGeometry(start_x,start_y,w,h);
    buttons[button]->setStyleSheet(" background-color: lightblue; font-size: 18px;");
    start_x+=w+separation;
}

void AVL::changeButtonsStatus(bool status) {
    buttons[INSERT]->setEnabled(status);
    buttons[DELETE]->setEnabled(status);
    buttons[BACK]->setEnabled(status);
    buttons[PAUSE]->setEnabled(!status);
    insert_input->setEnabled(status) ;
    delete_input->setEnabled(status);
}

void AVL::pauseResume() {
    isPaused = !isPaused;
    buttons[PAUSE]->setText(isPaused ? "Resume" : "Pause");
    buttons[PAUSE] -> setEnabled(( buttons[INSERT] ->isEnabled() ) ? false : true) ;
}

void AVL::reset(){
    in_process=nullptr;
    empty_node=nullptr;
    predecessor=nullptr;
    formEdges = true ;
    resizeButtons(orgWidth,orgHeight);
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

void AVL::insertion() {
    QString input_text = insert_input->text().trimmed();
    bool valid = false ;
    int number = input_text.toInt( &valid );
    if ( !valid ) {
        QMessageBox::warning(nullptr, "Invalid Input","Input must be in the integer bounds.\n");
        return;
    }

    changeButtonsStatus(false);
    TreeNode *new_node=new TreeNode(number);
    int curHeight = 0 ;
    TreeNode *temp=root;
    if(!root)
        root=new_node;
    else{
        in_process=root;
        while( true ){
            while ( isPaused ) {
                QCoreApplication::processEvents();
            }
            update();
            QEventLoop loop;
            QTimer::singleShot(time_delay, &loop, SLOT(quit()));
            loop.exec();
            curHeight++ ;
            if( curHeight >= 10 ) {
                QMessageBox::warning(nullptr, "Message" , "Only less than 10 levels are supported for now!") ;
                delete new_node ; new_node = nullptr;
                temp = nullptr;
                break;
            }
            if(number>temp->data){
                if(!temp->right){
                    temp->right=new_node;
                    new_node->parent=temp;
                    temp = temp->right ;
                    break;
                }
                temp=temp->right;
            }
            else if(number<temp->data){
                if(!temp->left){
                    temp->left=new_node;
                    new_node->parent=temp;
                    temp=temp->left ;
                    break;
                }
                temp=temp->left;
            }
            else{
                display_message("Duplicate_Entry!",time_delay) ;
                delete new_node; new_node=nullptr;
                temp=nullptr;
                break;
            }
            in_process=temp;
        }
    }

    while( temp ) {
        while ( isPaused ) {
            QCoreApplication::processEvents();
        }
        set_tree() ;
        in_process = temp;
        balanceNode(temp);
        QEventLoop loop;
        QTimer::singleShot(time_delay, &loop, SLOT(quit()));
        loop.exec();
        update();
        temp = temp -> parent ;
    }

    in_process=nullptr;
    set_tree();
    changeButtonsStatus(true) ;
    insert_input->clear();
    insert_input->setFocus() ;
}

TreeNode* AVL::find_predecessor(TreeNode* node){
    while(node -> right)
        node = node->right ;

    return node ;
}

TreeNode* AVL::leftRotation( TreeNode* node ) {
    TreeNode* temp = node -> right;
    if( temp ) {
        formEdges = false;
        rotate_subtree(temp,node) ;
    }

    node -> right = temp -> left;
    if (temp->left)
        temp->left->parent = node;

    temp -> parent = node -> parent ;
    if (!node->parent)
        root = temp;
    else if (node == node->parent->left)
        node->parent->left = temp;
    else
        node->parent->right = temp;

    temp->left = node;
    node->parent = temp;

    setHeight(node);
    setHeight(temp);
    return temp;
}

TreeNode* AVL::rightRotation(TreeNode* node) {
    TreeNode* temp = node->left;
    if( temp ) {
        formEdges = false;
        rotate_subtree(temp,node) ;
    }

    node->left = temp->right;
    if (temp->right)
        temp->right->parent = node;

    // Update the parent ptr of newRoot also.
    temp->parent = node->parent;

    // Update parent child-links also.
    if (!node->parent)
        root = temp;
    else if (node == node->parent->right)
        node->parent->right = temp;
    else
        node->parent->left = temp;

    // At-final, move the node to the right child of temp
    temp->right = node;
    node->parent = temp;
    setHeight(node);
    setHeight(temp);
    return temp;
}

void AVL::setHeight( TreeNode* node ) {
    if( node )
        node->height = std::max( get_height(node->left) , get_height(node->right) ) + 1 ;
}

int AVL::get_height(TreeNode *node){
    return (node) ? node->height : 0 ;
}

int AVL::getBalanceFactor(TreeNode* node){
    return (node) ? get_height(node->left) - get_height(node->right) : 0 ;
}

void AVL::balanceNode( TreeNode* node ) {
    setHeight( node ) ;
    int BF = getBalanceFactor( node ) ;
    bool single = true ;

    if( BF > 1 ) {
        if( getBalanceFactor( node -> left ) < 0 ) {
            single = false;
            display_message(QString("Node %1 is left heavy with its child %2 right heavy.So apply LR-Rotation.").arg(node->data).arg(node->left->data), 1750);
            node -> left = leftRotation( node -> left ) ;
            formEdges = true;
            set_tree();
            update();
            while ( isPaused ) {
                QCoreApplication::processEvents();
            }
        }
        if( !single )
            display_message(QString("Now Performing right rotation at Node %1").arg(node->data), 1750);
        else
            display_message(QString("Node %1 is left heavy.So apply Right Rotation").arg(node->data), 1750);

        node = rightRotation( node ) ; // Must do =.
        formEdges = true;
        set_tree() ;
        update() ;
        while ( isPaused ) {
            QCoreApplication::processEvents();
        }
    }

    if( BF < -1 ) {
        if( getBalanceFactor( node -> right ) > 0 ) {
            single = false;
            display_message(QString("Node %1 is left heavy with its child %2 right heavy.So apply RL-Rotation.").arg(node->data).arg(node->right->data), 1750);
            node -> right = rightRotation( node -> right ) ;
            formEdges = true;
            set_tree();
            update();
            while ( isPaused ) {
                QCoreApplication::processEvents();
            }
        }
        if( !single )
            display_message(QString("Now Pefroming Left Rotation at Node %1").arg(node->data), 1750);
        else
            display_message(QString("Node %1 is left heavy.So apply Left Rotation").arg(node->data), 1750);

        node = leftRotation( node ) ;
        formEdges = true;
        set_tree() ;
        update() ;
        while ( isPaused ) {
            QCoreApplication::processEvents();
        }
    }
}

void AVL::display_message(QString message,int time){
    messagelabel->setVisible(true);
    messagelabel->setText(message);
    QEventLoop loop;
    QTimer::singleShot(time, &loop, SLOT(quit()));
    loop.exec();
    while ( isPaused ) {
        QCoreApplication::processEvents();
        QThread::usleep(1) ;
    }
    messagelabel->setVisible(false);
}

void AVL::deletion() {
    QString delete_text = delete_input->text().trimmed();
    bool valid = false;
    int number = delete_text.toInt(&valid);
    if ( !valid ) {
        QMessageBox::warning(nullptr, "Invalid Input", "Input must be within the integer bounds.\n");
        return;
    }

    if ( !root ) { display_message("Empty AVL Found!", 750); delete_input->clear(); return; }

    changeButtonsStatus(false);
    TreeNode* temp = root;
    while (temp) {
        while ( isPaused ) {
            QCoreApplication::processEvents();
        }

        in_process = temp;
        update();
        QEventLoop loop;
        QTimer::singleShot(time_delay, &loop, SLOT(quit()));
        loop.exec();
        if (number < temp->data)
            temp = temp->left;
        else if (number > temp->data)
            temp = temp->right;
        else
            break;
    }

    if (!temp) {
        display_message("Key Not Found!", 750);
        in_process=nullptr;
        changeButtonsStatus(true);
        delete_input->clear();
        delete_input->setFocus() ;
        return;
    }

    TreeNode* delParent = temp -> parent ;
    if ( temp ->left && temp -> right ) {
        TreeNode* nodeToDelete = find_predecessor( temp -> left ) ;
        empty_node = nodeToDelete ;
        predecessor= new TreeNode(nodeToDelete) ;
        predecessor->parent=predecessor->right=predecessor->left=nullptr;
        display_message("Replacing with Inorder Predecessor!", 1500);
        move_subtree(predecessor,temp);
        temp -> data = predecessor->data;
        temp = nodeToDelete ;
        delParent = temp -> parent ;
        update() ;
        QEventLoop loop;
        QTimer::singleShot(time_delay, &loop, SLOT(quit()));
        loop.exec();
        delete predecessor ; predecessor=nullptr ; empty_node=nullptr;
    }

    // Child for predecessor OR node.
    TreeNode* child = (temp->left) ? temp -> left : temp -> right;
    empty_node = temp ;
    if (!delParent) // New-Root.
        root = child;
    else if ( delParent->left == temp ) {
        delParent->left = child;
        if( child ) {  // Only assign & move if there is child.
            child -> parent = delParent ;
            move_subtree(delParent->left,child);
        }
    }
    else {
        delParent->right = child;
        if( child ) {
            child -> parent = delParent ;
            move_subtree(delParent->right,child) ;
        }
    }

    set_tree() ;
    update();
    delete temp; temp = nullptr;
    empty_node = nullptr;
    while (delParent) {
        while ( isPaused ) {
            QCoreApplication::processEvents();
        }
        set_tree() ;
        in_process = delParent;
        balanceNode( delParent );
        QEventLoop loop;
        QTimer::singleShot(time_delay, &loop, SLOT(quit()));
        loop.exec();
        update();
        delParent = delParent -> parent;
    }

    in_process = nullptr;
    set_tree();
    changeButtonsStatus(true);
    delete_input->clear();
    delete_input->setFocus();
}

void AVL::move_subtree(TreeNode* from, TreeNode* to){
    int current_dx,current_dy,steps=100,step_x,step_y;
    QQueue<TreeNode*> q,sub_nodes;
    q.enqueue(from);
    while(!q.empty()){
        int size=q.size();
        while( size-- ){
            while ( isPaused ) {
                QCoreApplication::processEvents();
            }
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
        while ( isPaused ) {
            QCoreApplication::processEvents();
        }
        // Dynamically calculate step size for this iteration
        current_dx = from->rect.left() - to->rect.left();
        current_dy = from->rect.top() - to->rect.top();
        step_x = std::ceil(current_dx / double(steps - i));
        step_y = std::ceil(current_dy / double(steps - i));

        for (int j = 0; j < sub_nodes.size(); j++) {
            while ( isPaused ) {
                QCoreApplication::processEvents();
            }
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
        while ( isPaused ) {
            QCoreApplication::processEvents();
        }
        sub_nodes.front()->rect.moveLeft(sub_nodes.front()->rect.left() - final_dx);
        sub_nodes.front()->rect.moveTop(sub_nodes.front()->rect.top() - final_dy);
        sub_nodes.enqueue(sub_nodes.front());
        sub_nodes.dequeue();
    }
    update();
}

void AVL::rotate_subtree(TreeNode* from, TreeNode* to) {
    int dx = from->rect.left() - to->rect.left();
    int dy = from->rect.top() - to->rect.top();
    int steps = 15; // Smooth steps
    int step_x = dx / steps;
    int step_y = dy / steps;

    while( steps-- ) {
        QQueue<TreeNode*> queue;
        queue.enqueue(from);
        while (!queue.isEmpty()) {
            while ( isPaused ) {
                QCoreApplication::processEvents();
            }
            TreeNode* node = queue.dequeue();
            node->rect.moveLeft(node->rect.left() - step_x);
            node->rect.moveTop(node->rect.top() - step_y);

            if (node->left) queue.enqueue(node->left);
            if (node->right) queue.enqueue(node->right);
        }

        update();
        QEventLoop loop;
        QTimer::singleShot(15, &loop, SLOT(quit()));
        loop.exec();
    }

    // Final correction for precise alignment
    dx = from->rect.left() - to->rect.left();
    dy = from->rect.top() - to->rect.top();
    QQueue<TreeNode*> queue;
    queue.enqueue(from);

    while (!queue.isEmpty()) {
        while ( isPaused ) {
            QCoreApplication::processEvents();
        }
        TreeNode* node = queue.dequeue();
        node->rect.moveLeft(node->rect.left() - dx);
        node->rect.moveTop(node->rect.top() - dy);

        if (node->left) queue.enqueue(node->left);
        if (node->right) queue.enqueue(node->right);
    }
    update();
}

int AVL::getMaxLevel( TreeNode* node ) {
    if( !node )
        return 0 ;

    int l = 0 ;
    QQueue<TreeNode*> q ;
    q.push_back(root) ;
    while( !q.empty() ) {
        int size = q.size() ;
        l++ ;
        while( size-- ) {
            TreeNode* cur = q.front() ;
            q.pop_front() ;
            if( cur->left ) q.push_back(cur->left) ;
            if( cur -> right ) q.push_back( cur -> right ) ;
        }
    }
    return l ;
}

void AVL::set_tree() {
    // levels = get_height(root); // Delayed-visualization due to bottom-up change.
    levels = getMaxLevel(root) ;
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

void AVL::resizeButtons(int w,int h){
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
    buttons[PAUSE]->move(start_x, start_y);
    messagelabel->resize(width , 200) ;
    messagelabel->move(0,(height-200)/2);
}
void AVL::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // Get the new width and height of the widget from the resize event
    int newWidth = event->size().width();
    int newHeight = event->size().height();
    resizeButtons(newWidth,newHeight);
}

void AVL::paintEvent(QPaintEvent *event){
    if(!root) return; // Don't pause here else program broke OR if used so nothing will update.
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
            QString balanceFactor = QString("%1").arg(getBalanceFactor(curr));
            QPoint textPos = {curr->rect.right() + 10, curr->rect.center().y()};
            painter.setFont(QFont("Arial", max_font_size - 5));
            painter.drawText(textPos, balanceFactor);
            painter.setFont(QFont("Arial", font_size));
            QFontMetrics new_metrics(painter.font());
            sep_y=(curr->rect.height()-new_metrics.height())/2+new_metrics.ascent();
            sep_x=(curr->rect.width()-new_metrics.horizontalAdvance(numberText))/2;
            if(empty_node!=curr)
                painter.drawText(QPoint(curr->rect.left()+sep_x,curr->rect.top()+sep_y),numberText);// node with two child is currently in process of deletion

            if ( formEdges && curr->parent ) {
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
    if(predecessor){
        painter.setPen(Qt::red);
        numberText=QString::number(predecessor->data);
        painter.setFont(QFont("Arial", max_font_size));
        painter.drawText(QPoint(predecessor->rect.left()+sep_x,predecessor->rect.top()+sep_y),numberText);
    }
}


void AVL::handleBackButton() {
    this->reset() ;
    emit backToAlgorithms();
    this->close();
}

AVL::~AVL() {
    delete root; root = nullptr ;
    delete in_process; in_process = nullptr;
    delete predecessor; predecessor = nullptr ;
    delete insert_label; insert_label=nullptr;
    delete delete_input; delete_input = nullptr;
    delete delete_label; delete_label=nullptr;
    delete insert_input; insert_input = nullptr;
    delete empty_node; empty_node=nullptr;
    delete messagelabel; messagelabel = nullptr ;
    qDeleteAll(buttons) ;
}
