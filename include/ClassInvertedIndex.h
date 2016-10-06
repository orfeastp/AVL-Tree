#ifndef CLASSINVERTEDINDEX_H
#define CLASSINVERTEDINDEX_H
#include <fstream>

using namespace std;

/*
 * Class Declaration
*/

class ClassInvertedIndex
{
    public:
        ClassInvertedIndex();
        int Edit();

        struct avl_node* insert_x(int,int,struct avl_node*&);
        struct avl_node* insert_y(int ,avl_node*&);

        void delete_x(int,int,struct avl_node*&);
        struct avl_node* delete_y(int,struct avl_node*&);
        struct avl_node* minValueNode(struct avl_node*&);

        int max(int , int );
        int height(avl_node* );
        int diff(avl_node* );
        avl_node* balance(avl_node * );
        avl_node* ll_rotation(avl_node *);
        avl_node* rr_rotation(avl_node * );
        avl_node* rl_rotation(avl_node * );
        avl_node* lr_rotation(avl_node * );

        void sum(avl_node* root,ofstream&);
        int geitones(avl_node* root);

        void write_index(avl_node* );
        void inorder_x(avl_node*,ofstream& );
        void inorder_y(avl_node*,ofstream&);
};

#endif // CLASSINVERTEDINDEX_H
