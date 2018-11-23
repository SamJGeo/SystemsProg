/*
Sam George 2261522g SP Exercise 1
This is my own work as defined in the Academic Ethics agreement 
*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"tldlist.h"
#include"date.h"
#include<ctype.h>

//create a new node
TLDNode * create_node(char * name);
//recursively deleting nodes
void del_nodes(TLDNode * node);
//insert finds the position in the tree and adds a node there
void insert(TLDNode * n, TLDList * l, TLDNode * r);
//recalculate the height
void reheight(TLDNode * node);
//recalculate the balance
void set_balance(TLDNode * node);
//rebalance the tree
void rebalance(TLDNode * n,TLDList * l);
//rotations for balancing trees
TLDNode * l_rot(TLDNode * node);
TLDNode * r_rot(TLDNode * node);
TLDNode * l_r_rot(TLDNode * node);
TLDNode * r_l_rot(TLDNode * node);
int retHeight(TLDNode * n);

//the queue for iterating over
struct queue{
   struct queueNode * head;
    struct queueNode * tail;
};

//the queue nodes to populate the queue
struct queueNode{
    TLDNode * node;
   struct queueNode * nextNode;
};



typedef struct queueNode queueNode;
typedef struct queue queue;


//declaring functions used
struct queueNode * createQN(TLDNode * n);
void popQueue(TLDNode * n, queue * q);
queue * create_queue(TLDList * l);
void del_qnode(queueNode * qn);


struct tldlist{
    int count; // total count of visits to specific page suffix
    Date * bDate;
    Date * eDate;
    TLDNode * root;
};
struct tldnode{
    int count; //the number of times the tld was called
    char * name;
    int balance;
    int height;
    TLDNode * leftChild;
    TLDNode * rightChild;
    TLDNode * parent;
};
struct tlditerator{
    queue * q;
};

/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end){

    if(begin == NULL|| end ==NULL){
        return(NULL);
    }

    Date * startCopy = date_duplicate(begin);
    Date * endCopy = date_duplicate(end);

    if(startCopy == NULL || endCopy ==NULL){
        return(NULL);
    }

    TLDList * list;
    list = malloc(sizeof(TLDList)); //allocate memory on heap for the size of a TLDList
    if(!list){
        fprintf(stderr,"Memory allocation failure");
        return(NULL);
    } else {
        list->count = 0; //initialising all the variables of a list
        list-> bDate = startCopy;
        list->eDate = endCopy;
        list->root = NULL;
        return(list);
    }
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */
void tldlist_destroy(TLDList *tld){
    if(tld != NULL){
        del_nodes(tld->root);
        date_destroy(tld->bDate);
        date_destroy(tld->eDate);
        free(tld);

    }


}


/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d){
    //check if date is valid

    if(date_compare(d,tld->bDate)>=0 && date_compare(d,tld->eDate)<=0){
        int i;
        //finding the index of the suffix
        for(i = strlen(hostname); (hostname[i]!= '.');i--);
        if(i==-1){
            fprintf(stderr, "invalid tld");
            return(0);
        }


        //making a copy of the hostname suffix and converting to lower case
        char *hnCopy = malloc(sizeof(char) * strlen(hostname));
        strcpy(hnCopy,hostname+1+i);

        for(i=0;i<strlen(hnCopy);i++){
            hnCopy[i] = tolower(hnCopy[i]);
        }


        TLDNode * n = create_node(hnCopy);
        free(hnCopy);
        if(tld->root==NULL){

            tld->root=n;
            n->count=1;
            tld->count+=1;
        } else{
            insert(n,tld,tld->root);
            tld->count+=1;
        }
        return(1);

    }
    return(0);
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld){
    return(tld->count);
}

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld){
    if(!tld){
        return(NULL);
    }
    TLDIterator * iterator = malloc(sizeof(TLDIterator));
    if(!iterator){
        fprintf(stderr,"Memory allocation failure");
    }
    iterator->q = create_queue(tld);
    return(iterator);
}


/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter){
    if(iter->q->head==NULL){
        return(NULL);
    } else{
        queueNode * hp = iter->q->head;
        iter->q->head = iter->q->head->nextNode;
        TLDNode * n = hp->node;
        free(hp);
        return(n);
    }
}



/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter){
    del_qnode(iter->q->head);
    free(iter->q);
    free(iter);
    
}

void del_qnode(queueNode * qn){
    if(qn != NULL){
        if(qn->nextNode != NULL){
            del_qnode(qn->nextNode);
        }
        free(qn);
    }

}





/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node){
    return(node->name);
}

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node){
    return(node->count);
}

//destroys the nodes in a tree and frees their allocated memory
void del_nodes(TLDNode * node){
    if(node != NULL){
        del_nodes(node->rightChild);
        del_nodes(node->leftChild);
        free(node->name);
        free(node);
    }
}

TLDNode * create_node(char * name){
    TLDNode * n = malloc(sizeof(TLDNode));
    if(n == NULL){
        printf("Memory allocation failure\n");
        return(NULL);
    }
    n->parent = NULL;
    n->count = 1;
    n->leftChild = NULL;
    n->rightChild = NULL;
    n->name = strdup(name);
    return(n);
}

void insert(TLDNode * inNode,TLDList * l,TLDNode * node){
    //getting the key for comparisons and matching
    char * inKey = inNode->name;
    int comp;
    comp = strcmp(inKey,node->name);
    if(comp < 0){
        if(node->leftChild != NULL){

            insert(inNode,l,node->leftChild);
        } else{
            node->leftChild = inNode;
            inNode->parent = node;
            inNode->height = 1;
            inNode->count = 1;
            if(l->root != node){
                rebalance(node->parent,l);
            }
        }

    } else if(comp>0){
        if(node->rightChild != NULL){
            insert(inNode,l,node->rightChild);
        } else{
            node->rightChild = inNode;
            inNode->parent = node;
            ///NOT SURE IF THIS WORKS YET
            inNode->height = 1;
            inNode->count =1;
            if(l->root != node){
                rebalance(node->parent,l);
            }
        }



    } else{
        node->count +=1;
    }
}

int retHeight(TLDNode * n){
    if(n){
        return(n->height);
    } else{
        return(0);
    }

}
//rebalance the tree
//checks if the balance is wrong and calls the appropriate functions for each case
void rebalance(TLDNode * node, TLDList * list){
    set_balance(node);
    if(node->balance==-2){
        if(retHeight(node->leftChild->leftChild)>=retHeight(node->leftChild->rightChild)){
            node = r_rot(node);
        } else{
            node = l_r_rot(node);
        }
    
    } else if(node->balance==2){
        if(retHeight(node->rightChild->rightChild)>= retHeight(node->rightChild->leftChild)){
            node= l_rot(node);
        } else{
            node = r_l_rot(node);
        }
        
    }
    if(node->parent){
        rebalance(node->parent,list);
    }else{
        list->root = node;
    }
}




//setting the balance for a node
void set_balance(TLDNode * n){
    reheight(n);
    int rcH=0;
    int lcH=0;
    if(n->rightChild){
        reheight(n->rightChild);
        rcH = n->rightChild->height;
    }
    if(n->leftChild){
        reheight(n->leftChild);
        lcH = n->leftChild->height;
    }
    n->balance = rcH - lcH;
}




//recalibrates height of a node
void reheight(TLDNode * n){
    //gets the larger height of the two subtrees and adds one for the height of the current node
    if(n){
        int lcHeight = 0;
        int rcHeight = 0;
        if(n->leftChild){
            lcHeight = n->leftChild->height;
        }
        if(n->rightChild){
            rcHeight = n->rightChild->height;
        }
        if(lcHeight > rcHeight){
            n->height = 1 + lcHeight;
        } else {
            n->height = 1 + rcHeight;
        }
    }

}


//This function will take the parent node and make that a left child of the current node
TLDNode * l_rot(TLDNode * node){
    TLDNode * b = node->rightChild;
    b->parent = node->parent;

    node->rightChild = b->leftChild;

    if(node->rightChild != NULL){
        node->rightChild->parent = node;
    }

    b->leftChild = node;
    node->parent = b;

    if(b->parent!= NULL){
        if(b->parent->rightChild == node){
            b->parent->rightChild = b;
        } else{
            b->parent->leftChild = b;
        }
    }

    set_balance(node);
    set_balance(b);

    return(b); 
}



//This function will take the parent node and make it a right child of the current node
TLDNode * r_rot(TLDNode * node){
    TLDNode * b = node->leftChild;
    b->parent = node->parent;
    node->leftChild = b->rightChild;

    if(node->leftChild != NULL){
        node->leftChild->parent = node;
    }

    b->rightChild = node;
    node->parent = b;

    if(b->parent!= NULL){
        if(b->parent->rightChild == node){
            b->parent->rightChild = b;
        } else{
            b->parent->leftChild = b;
        }
    }

    set_balance(node);
    set_balance(b);

    return(b);
}


TLDNode * l_r_rot(TLDNode * node){
    node->leftChild = l_rot(node->leftChild);
    return(r_rot(node));
}

TLDNode * r_l_rot(TLDNode * node){
    node->rightChild=r_rot(node->rightChild);
    return(l_rot(node));
}


//create a queue and call the population
queue * create_queue(TLDList * l){
    queue * q = malloc(sizeof(queue));
    if(q){
        q->head = NULL;
        q->tail = NULL;
        popQueue(l->root,q);
        return(q);
    } else{
        return(NULL);
    }




}

//create a a queue node
queueNode * createQN(TLDNode * n){
    queueNode * qn = malloc(sizeof(queueNode));
    if(qn){
        qn->node = n;
        qn->nextNode = NULL;
        return(qn);
    } else{
        return(NULL);
    }


}

//populate the queue
void popQueue(TLDNode * n,queue * q){
    queueNode * qn = createQN(n);
    if(n->leftChild!=NULL){
        popQueue(n->leftChild,q);
    }
    if(q->head == NULL){
        q->head = qn;
        q->tail = qn;
    } else {
        q->tail->nextNode = qn;
        q->tail = qn;
    }

    if(n->rightChild!=NULL){
        popQueue(n->rightChild,q);
    }
}
