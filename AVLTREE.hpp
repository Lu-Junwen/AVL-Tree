#ifndef AVLTREE_hpp
#define AVLTREE_hpp


template <typename T>
class AVLNode {
public:
    T element;
    int bf;
    AVLNode<T>* left;
    AVLNode<T>* right;
    AVLNode<T>* parent;
public:
    AVLNode():left(nullptr),right(nullptr){}
    AVLNode(const T& t,AVLNode<T> *p=nullptr,AVLNode<T> *l=nullptr,AVLNode<T> *r=nullptr,int b=0):element(t),parent(p),left(l),right(r),bf(b){}
};

template <typename T>
class AVLTree {
private:
    AVLNode<T> *root;
    int treeSize;
private:
    AVLNode<T>* rightRotate(AVLNode<T> *r);
    AVLNode<T>* leftRotate(AVLNode<T> *r);
    AVLNode<T>* leftRightRotate(AVLNode<T> *r);
    AVLNode<T>* rightLeftRotate(AVLNode<T> *r);
    //insert only
    //correct the balance factor again when erasing
    AVLNode<T>* R_1Rotate(AVLNode<T> *r);
    AVLNode<T>* L_1Rotate(AVLNode<T> *r);
    //Correct the L-1 or R-1 imbalance
    void case3LRBalance(AVLNode<T> *rP,AVLNode<T> *r);
    int height(AVLNode<T> *r)const;
    bool isAVLTree(AVLNode<T> *r)const;
public:
    AVLTree():root(nullptr),treeSize(0){}
    ~AVLTree();
    bool empty()const{return !(bool)(treeSize);}
    int size()const{return treeSize;}
    bool find(const T& t)const;
    bool insert(const T& t);
    bool erase(const T& t);
    T maxElement()const;
    T minElement()const;
    void preOrder(void(*pft)(const T&))const{preOrderTraverse(pft, root);}
    void inOrder(void(*pft)(const T&))const{inOrderTraverse(pft, root);}
    void postOrder(void(*pft)(const T&))const{postOrderTraverse(pft, root);}
    int height()const{return height(root);}
};

template <typename T>
void preOrderTraverse(void(*pft)(const T&),AVLNode<T> *r){
    if(r){
        (*pft)(r->element);
        preOrderTraverse(pft, r->left);
        preOrderTraverse(pft, r->right);
    }
}

template <typename T>
void inOrderTraverse(void(*pft)(const T&),AVLNode<T> *r){
    if(r){
        inOrderTraverse(pft, r->left);
        (*pft)(r->element);
        inOrderTraverse(pft, r->right);
    }
}

template <typename T>
void postOrderTraverse(void(*pft)(const T&),AVLNode<T> *r){
    if(r){
        postOrderTraverse(pft, r->left);
        postOrderTraverse(pft, r->right);
        (*pft)(r->element);
    }
}

template <typename T>
AVLNode<T>* AVLTree<T>::rightRotate(AVLNode<T> *r){
    AVLNode<T> *rLeft=r->left,*rLeftRight=rLeft->right,*rParent=r->parent;
    
    r->left=rLeftRight;
    if(rLeftRight)
        rLeftRight->parent=r;
    rLeft->right=r;
    r->parent=rLeft;
    
    if(r==root)
        root=rLeft;
    else
        (r==rParent->left?rParent->left:rParent->right)=rLeft;
    rLeft->parent=rParent;
    
    return rLeft;
}
template <typename T>
AVLNode<T>* AVLTree<T>::leftRotate(AVLNode<T> *r){
    AVLNode<T> *rRight=r->right,*rRightLeft=rRight->left,*rParent=r->parent;
    
    r->right=rRightLeft;
    if(rRightLeft)
        rRightLeft->parent=r;
    rRight->left=r;
    r->parent=rRight;
    
    if(r==root)
        root=rRight;
    else
        (r==rParent->left?rParent->left:rParent->right)=rRight;
    rRight->parent=rParent;
    
    return rRight;
}

template <typename T>
AVLNode<T>* AVLTree<T>::leftRightRotate(AVLNode<T> *r){
    AVLNode<T> *rLeft=r->left,*rLeftRight=rLeft->right;
    
    leftRotate(rLeft);
    rightRotate(r);
    
    switch (rLeftRight->bf) {
        case 1:
            rLeft->bf=rLeftRight->bf=0;
            r->bf=-1;
            break;
        case -1:
            r->bf=rLeftRight->bf=0;
            rLeft->bf=1;
            break;
        default:
            r->bf=rLeft->bf=rLeftRight->bf=0;
            break;
    }
    
    return rLeft;
}

template <typename T>
AVLNode<T>* AVLTree<T>::rightLeftRotate(AVLNode<T> *r){
    AVLNode<T> *rRight=r->right,*rRightLeft=rRight->left;
    
    rightRotate(rRight);
    leftRotate(r);
    
    switch (rRightLeft->bf) {
        case 1:
            r->bf=rRightLeft->bf=0;
            rRight->bf=-1;
            break;
        case -1:
            rRightLeft->bf=rRight->bf=0;
            r->bf=1;
            break;
        default:
            r->bf=rRight->bf=rRightLeft->bf=0;
            break;
    }
    
    return rRight;
}

template <typename T>
AVLNode<T>* AVLTree<T>::R_1Rotate(AVLNode<T> *r){
    AVLNode<T> *rLeft=r->left,*rLeftRight=rLeft->right,*rParent=r->parent;
    
    r->left=rLeftRight->right;
    if(rLeftRight->right)
        rLeftRight->right->parent=r;
    rLeft->right=rLeftRight->left;
    if(rLeftRight->left)
        rLeftRight->left->parent=rLeft;
    
    rLeftRight->left=rLeft;
    rLeftRight->right=r;
    r->parent=rLeft->parent=rLeftRight;
    
    if(r==root)
        root=rLeftRight;
    else
        (r==rParent->left?rParent->left:rParent->right)=rLeftRight;
    rLeftRight->parent=rParent;
        
    switch (rLeftRight->bf){
        case 0:
            r->bf=rLeft->bf=0;
            break;
        case 1:
            r->bf=-1;
            rLeft->bf=0;
            break;
        case -1:
            r->bf=0;
            rLeft->bf=1;
            break;
        default:
            break;
    }
    rLeftRight->bf=0;
    
    return rLeftRight;
}

template <typename T>
AVLNode<T>* AVLTree<T>::L_1Rotate(AVLNode<T> *r){
    AVLNode<T> *rRight=r->right,*rRightLeft=rRight->left,*rParent=r->parent;
    
    r->right=rRightLeft->left;
    if(rRightLeft->left)
        rRightLeft->left->parent=r;
    rRight->left=rRightLeft->right;
    if(rRightLeft->right)
        rRightLeft->right->parent=rRight;
    
    rRightLeft->left=r;
    rRightLeft->right=rRight;
    r->parent=rRight->parent=rRightLeft;
    
    if(r==root)
        root=rRightLeft;
    else
        (r==rParent->left?rParent->left:rParent->right)=rRightLeft;
    rRightLeft->parent=rParent;
    
    switch (rRightLeft->bf) {
        case 0:
            r->bf=rRight->bf=0;
            break;
        case 1:
            r->bf=0;
            rRight->bf=-1;
            break;
        case -1:
            r->bf=1;
            rRight->bf=0;
            break;
        default:
            break;
    }
    rRightLeft->bf=0;

    return rRightLeft;
}

template <typename T>
void AVLTree<T>::case3LRBalance(AVLNode<T> *parentNode,AVLNode<T> *thisNode){
    while (parentNode) {
        if(thisNode==parentNode->left)
            --parentNode->bf;
        else
            ++parentNode->bf;
        
        if(parentNode->bf==1||parentNode->bf==-1)
            break;
        else if(!parentNode->bf){
            thisNode=parentNode;
            parentNode=parentNode->parent;
        }
        else{
            if(parentNode->bf==2){
                if(!parentNode->left->bf){
                    rightRotate(parentNode);
                    parentNode->bf=1;
                    parentNode->parent->bf=-1;
                    break;
                }
                else if(parentNode->left->bf==1){
                    rightRotate(parentNode);
                    parentNode->bf=parentNode->parent->bf=0;
                    thisNode=parentNode->parent;
                    parentNode=thisNode->parent;
                }
                else{
                    parentNode=R_1Rotate(parentNode);
                    thisNode=parentNode;
                    parentNode=thisNode->parent;
                }
            }
            else{
                if(!parentNode->right->bf){
                    leftRotate(parentNode);
                    parentNode->bf=-1;
                    parentNode->parent->bf=1;
                    break;
                }
                else if(parentNode->right->bf==-1){
                    leftRotate(parentNode);
                    parentNode->bf=parentNode->parent->bf=0;
                    thisNode=parentNode->parent;
                    parentNode=thisNode->parent;
                }
                else{
                    parentNode=L_1Rotate(parentNode);
                    thisNode=parentNode;
                    parentNode=thisNode->parent;
                }
            }
        }
    }
}

template <typename T>
void eraseNode(AVLNode<T>* r){
    if(r){
        eraseNode(r->left);
        eraseNode(r->right);
        delete r;
    }
}

template <typename T>
int AVLTree<T>::height(AVLNode<T> *r)const{
    return r?(1+max(height(r->left), height(r->right))):0;
}

template <typename T>
bool AVLTree<T>::isAVLTree(AVLNode<T> *r)const{
    if(!r)
        return true;
    int h1=height(r->left),h2=height(r->right);
    if(h1-h2>1||h1-h2<-1||h1-h2!=r->bf)
        return false;
    
    return isAVLTree(r->left)&&isAVLTree(r->right);
}

template <typename T>
AVLTree<T>::~AVLTree(){
    eraseNode(root);
}

template <typename T>
bool AVLTree<T>::find(const T &t)const{
    AVLNode<T> *currentNode=root;
    
    while(currentNode){
        if(currentNode->element>t)
            currentNode=currentNode->left;
        else if(currentNode->element<t)
            currentNode=currentNode->right;
        else
            return true;
    }
    
    return false;
}

template <typename T>
bool AVLTree<T>::insert(const T &t){
    AVLNode<T> *currentNode=root,*prevNode=nullptr;
    
    while(currentNode&&currentNode->element!=t){
        prevNode=currentNode;
        currentNode=currentNode->element<t?currentNode->right:currentNode->left;
    }
    
    if(currentNode)
        return false;
    
    AVLNode<T> *newNode=new AVLNode<T>(t,prevNode);
    
    if(prevNode)
        (t<prevNode->element?prevNode->left:prevNode->right)=newNode;
    else
        root=newNode;
    
    currentNode=newNode;
    
    while(prevNode){
        if(currentNode==prevNode->left)
            ++prevNode->bf;
        else
            --prevNode->bf;
        
        if(!prevNode->bf)
            break;
        else if(prevNode->bf==1||prevNode->bf==-1){
            currentNode=prevNode;
            prevNode=prevNode->parent;
        }
        else{
            switch (prevNode->bf) {
                case 2:
                    if(prevNode->left->bf==1){
                        rightRotate(prevNode);
                        prevNode->bf=prevNode->parent->bf=0;
                    }
                    else
                        leftRightRotate(prevNode);
                    break;
                case -2:
                    if(prevNode->right->bf==-1){
                        leftRotate(prevNode);
                        prevNode->bf=prevNode->parent->bf=0;
                    }
                    else
                        rightLeftRotate(prevNode);
                    break;
                default:
                    break;
            }
            break;
        }
    }
    
    ++treeSize;
    return true;
}

template <typename T>
bool AVLTree<T>::erase(const T &t){
    AVLNode<T> *currentNode=root,*prevNode=nullptr;
    
    while(currentNode&&currentNode->element!=t){
        prevNode=currentNode;
        currentNode=(t<currentNode->element?currentNode->left:currentNode->right);
    }
    
    if(!currentNode)
        return false;
        
    if((!currentNode->left)&&(!currentNode->right)){
        if(currentNode==root)
            root=nullptr;
        else{
            case3LRBalance(prevNode, currentNode);
            
            (currentNode==prevNode->left?prevNode->left:prevNode->right)=nullptr;
        }
        delete currentNode;
    }
    else if((!currentNode->left)||(!currentNode->right)){
        if(currentNode==root)
            root=currentNode->left?currentNode->left:currentNode->right;
        else{
            case3LRBalance(prevNode, currentNode);
            (currentNode==prevNode->left?prevNode->left:prevNode->right)=(currentNode->left?currentNode->left:currentNode->right);
        }
        (currentNode->left?currentNode->left->parent:currentNode->right->parent)=prevNode;
        delete currentNode;
    }
    else{
        AVLNode<T> *leftMaxNode=currentNode->left;
        
        while(leftMaxNode->right)
            leftMaxNode=leftMaxNode->right;
        
        if(leftMaxNode==currentNode->left){
            currentNode->element=leftMaxNode->element;
            currentNode->left=leftMaxNode->left;
            if(leftMaxNode->left)
                leftMaxNode->left->parent=currentNode;
            case3LRBalance(currentNode, currentNode->left);
        }
        else{
            if(leftMaxNode->left)
                leftMaxNode->left->parent=leftMaxNode->parent;
            
            case3LRBalance(leftMaxNode->parent, leftMaxNode);
            currentNode->element=leftMaxNode->element;
            leftMaxNode->parent->right=leftMaxNode->left;
        }
                
        delete leftMaxNode;
    }
    
    --treeSize;
    return true;
}

template <typename T>
T AVLTree<T>::maxElement()const{
    AVLNode<T> *currentNode=root;
    
    while(currentNode->left)
        currentNode=currentNode->left;
    
    return currentNode->element;
}

template <typename T>
T AVLTree<T>::minElement()const{
    AVLNode<T> *currentNode=root;
    
    while(currentNode->right)
        currentNode=currentNode->right;
    
    return currentNode->element;
}

#endif /* AVLTREE_hpp */
