//Red-black tree(Main functions for operations and balancing)
//In understanding and working on the project helped me a lot:
//http://www.mkurnosov.net/teaching/uploads/DSA/dsa-fall-lecture4.pdf
//https://en.wikipedia.org/wiki/Red–black_tree
//http://algolist.ru/ds/rbtree.php
//http://www.codenet.ru/np-includes/upload/2004/04/05/127988.txt
//https://habr.com/ru/post/330644/
//https://www.ibm.com/developerworks/ru/library/l-data_structures_09/index.html
//https://www.youtube.com/watch?v=QjsQpgzuO-c&list=PL55ElYIEKI0AIeQxsp7iW3WmUPdu7FTh-&index=29&t=0s    :)
//Cormen T.H., Leizerson Ch.I., Rivest R.L., Stein K.
//Algorithms: building and analysis. – 2-nd pub. – M.:
//Williams, 2005. – 1296 p. (P. 336-356)


#include <cstdlib>
#include <iostream>

using namespace std;
class tree
{

                                     //typedef - 'alias', used to simplify code;
public:
       void rotateLeft(tree_element *x);
       void rotateRight(tree_element *x);
       void insertFix(tree_element *x);
       tree_element* create_tree_element(value data2);
       tree_element *insert_tree_element(value data);
       void deleteFix(tree_element *x);
       void deleteElement(tree_element *z);
       tree_element *find_element(value data);
       void delete_tree (tree_element* root);
private:
       tree_element leaf = {0, NIL, NIL, 0, BLACK};   //Tree leaves
       tree_element *root = NIL;
};


enum typecolor { BLACK, RED };            //Enter the enumeration type to indicate the color of the element

typedef int value;
#define compLT(a,b) (a < b)              //Let's introduce comparison operations
#define compEQ(a,b) (a == b)

typedef struct tree_element {            //All the information about the item
  value data;
  struct tree_element* left;
  struct tree_element* right;
  struct tree_element* parent;
  typecolor color;
} tree_element;

#define NIL & leaf
tree_element leaf = {0, NIL, NIL, 0, BLACK};   //Tree leaves
tree_element *root = NIL;                      //Root element


tree_element* create_tree_element(value data2) {       //Create element
    tree_element* elem = new tree_element;
    elem ->data = data2;
    elem ->left = NIL;
    elem ->right = NIL;
    elem ->parent = root;
    elem ->color = BLACK;
    return elem;
}

void rotateLeft(tree_element *x) {                     //Left turn
    tree_element *y = x->right;
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;
    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        root = y;
    }
    y->left = x;
    if (x != NIL) x->parent = y;
}

void rotateRight(tree_element *x) {                    //Right turn
    tree_element *y = x->left;
    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;
    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    } else {
        root = y;
    }
    y->right = x;
    if (x != NIL) x->parent = y;
}

void insertFix(tree_element *x) {                      //Balancing of the insertion element
    while (x != root && x->parent->color == RED) {
        //If the tree needs balancing
        if (x->parent == x->parent->parent->left) {
            tree_element *y = x->parent->parent->right;
            if (y->color == RED) {
                // If "uncle" is red:
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {

                //If "uncle" is black:
                if (x == x->parent->right) {
                    x = x->parent;
                    rotateLeft(x);
                }

                //Change color and a turn
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateRight(x->parent->parent);
            }
        } else {

            tree_element *y = x->parent->parent->left;
            if (y->color == RED) {

                //If "uncle" is red
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {

                //If "uncle" is black
                if (x == x->parent->left) {
                    x = x->parent;
                    rotateRight(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateLeft(x->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

tree_element *insert_tree_element(value data) {       //Insert an item
    tree_element *current, *parent, *x ;
    current = root;
    parent = 0;
    while (current != NIL) {
        if (compEQ(data, current->data)) return (current);
        parent = current;
        current = compLT(data, current->data) ?       // ? Branch operator
            current->left : current->right;
    }
    if ((x = (tree_element*)malloc (sizeof(*x))) == 0) {
        printf ("insufficient memory");
        exit(1);
    }
    x->data = data;                                    //Changing an item
    x->parent = parent;
    x->left = NIL;
    x->right = NIL;
    x->color = RED;
    if(parent) {                                       //Insert itself
        if(compLT(data, parent->data))
            parent->left = x;
        else
            parent->right = x;
    } else {
        root = x;
    }

    insertFix(x);
    return(x);
}

void deleteFix(tree_element *x) {                     //The balancing of deletion an item
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            tree_element *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft (x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight (w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft (x->parent);
                x = root;
            }
        } else {
            tree_element *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight (x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft (w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight (x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void deleteElement(tree_element *z) {                 //Deletion an item
    tree_element *x, *y;

    if (!z || z == NIL) return;


    if (z->left == NIL || z->right == NIL) {
        y = z;
    } else {
        y = z->right;
        while (y->left != NIL) y = y->left;
    }

    if (y->left != NIL)
        x = y->left;
    else
        x = y->right;

    x->parent = y->parent;
    if (y->parent)
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    else
        root = x;

    if (y != z) z->data = y->data;


    if (y->color == BLACK)
        deleteFix(x);

    delete(y);                                   //Free of an item
}

tree_element *find_element(value data) {              //Search of an item

    tree_element *current = root;
    while(current != NIL)
        if(compEQ(data, current->data))
            return (current);
        else
            current = compLT (data, current->data) ?
                current->left : current->right;
    return(0);
}

void delete_tree (tree_element* root) {         //Delete all tree
    if (root -> right != NIL)
    {
        delete_tree(root -> right);
    }
    if (root -> left != NIL)
    {
        delete_tree(root -> left);
    }
    delete(root);
}




int main()
{
    insert_tree_element(42);
    create_tree_element(42);
    cout << find_element(42)->color << " " << find_element(42)->data << endl;
    delete_tree(root);
    return 0;
}
