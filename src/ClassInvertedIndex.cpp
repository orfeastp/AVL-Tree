#include "classinvertedindex.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct avl_node
{
    int data;
    struct avl_node *left;
    struct avl_node *right;
    struct avl_node *neighbor;
}*root=NULL;

ClassInvertedIndex::ClassInvertedIndex()
{
    root=NULL;
}

int ClassInvertedIndex:: Edit()
{


    ifstream f,d;
    ofstream out;

    char e1[15],e2[15];
    int x,y;
    bool h=false;
    ClassInvertedIndex neos;

    f.open("commands.txt");
    if(!f)
    {
        cerr<<"Could not open the file"<<endl;
        return 0;
    }
    f>>e2>>e2;
    d.open(e2);
    if(!d)
    {
        cerr<<"Could not open the file"<<endl;
        return 0;
    }
    while(!d.eof())
    {
        d>>x>>y;
        root = neos.insert_x(x,y,root);
    }
    d.close();
    while(!h)
    {
        f>>e1;
        if(!strcmp(e1,"INSERT_LINK"))
        {
            f>>x>>y;
            neos.insert_x(x,y,root);
        }
        else
        {
            if(!strcmp(e1,"DELETE_LINK"))
            {
                f>>x>>y;
                neos.delete_x(x,y,root);
            }
            else
            {
                f>>e1;
                h=true;
                out.open(e1,ios::in);
                neos.write_index(root);
            }
        }
    }
    f.close();
}

/*
 * Insert x,y into the tree
 */

avl_node* ClassInvertedIndex::insert_x(int value1,int value2, avl_node* &root)
{
        if(root==NULL || root->data==value1)
        {
            if(root == NULL)
            {
                root=new avl_node;
                root->data=value1;
                root->left=NULL;
                root->right=NULL;
            }
            root->neighbor=insert_y(value2,root->neighbor);
            return root;
        }
        else
        {
            if(value1<root->data)
            {
                root->left=insert_x(value1,value2,root->left);
                root=balance(root);
            }
            else
            {
                if(value1>root->data)
                {
                    root->right=insert_x(value1,value2,root->right);
                    root=balance(root);
                }
            }
        }
        return root;
}

/*
 * Insert y into the tree
 */

avl_node* ClassInvertedIndex::insert_y(int value,avl_node* &root)
{
    if(root==NULL)
    {
        root=new avl_node;
        root->data=value;
        root->left=NULL;
        root->right=NULL;
        return root;
    }
    else
    {
        if(value<root->data)
        {
            root->left=insert_y(value,root->left);
            root=balance(root);
        }
         else
         {
             if(value>root->data)
             {
                root->right=insert_y(value,root->right);
                root=balance(root);
             }
         }
    }
    return root;
}

/*
    Delete
*/

void ClassInvertedIndex::delete_x(int value1, int value2, avl_node* &root)
{
    if(root->data==value1)
    {
        delete_y(value2,root->neighbor);
    }
    else
    {
        if(root->data<value1)
        {
            delete_x(value1,value2,root->right);
        }
        else
        {
            delete_x(value1,value2, root->left);
        }
    }
    return;
}

struct avl_node *ClassInvertedIndex::minValueNode(struct avl_node* &node)
{
    struct avl_node* current = node;

    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;
}


avl_node* ClassInvertedIndex::delete_y(int value, avl_node* &root)
{
    if (value<root->data)
    {
        root->left=delete_y(value,root->left);
    }
    else
    {
        if(value>root->data)
        {
            root->right=delete_y(value,root->right);
        }
        else
        {
            if((root->left==NULL) || (root->right==NULL))
            {
                struct avl_node *temp = root->left ? root->left : root->right;
                if(temp==NULL)
                {
                    temp = root;
                    root = NULL;
                }
                else
                    *root = *temp;

                delete temp;
            }
            else
            {
                struct avl_node* temp = minValueNode(root->right);
                root->data=temp->data;
                root->right=delete_y(temp->data,root->right);
            }
        }
    }
    if (root == NULL)
    {
        return root;
    }

    return root;
}

int ClassInvertedIndex::max(int x,int y)
{
    if (x>y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

int ClassInvertedIndex::height(avl_node *temp)
{
    int h = 0;
    if (temp != NULL)
    {
        int l_height = height (temp->left);
        int r_height = height (temp->right);

        int max_height = max (l_height, r_height);
        h = max_height + 1;
    }
    return h;
}

/*
 * Height Difference
 */

int ClassInvertedIndex::diff(avl_node *temp)
{
    int l_height = height (temp->left);
    int r_height = height (temp->right);
    int b_factor= l_height - r_height;

    return b_factor;
}

/*
 * Right- Right Rotation
 */

avl_node *ClassInvertedIndex::rr_rotation(avl_node *parent)
{
    avl_node *temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;

    return temp;
}
/*
 * Left- Left Rotation
 */

avl_node *ClassInvertedIndex::ll_rotation(avl_node *parent)
{
    avl_node *temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;

    return temp;
}

/*
 * Left - Right Rotation
 */

avl_node *ClassInvertedIndex::lr_rotation(avl_node *parent)
{
    avl_node *temp;
    temp = parent->left;
    parent->left = rr_rotation (temp);

    return ll_rotation (parent);
}

/*
 * Right- Left Rotation
 */

avl_node *ClassInvertedIndex::rl_rotation(avl_node *parent)
{
    avl_node *temp;
    temp = parent->right;
    parent->right = ll_rotation (temp);

    return rr_rotation (parent);
}

/*
 * Balancing AVL Tree
 */

avl_node *ClassInvertedIndex::balance(avl_node *temp)
{
    int bal_factor = diff (temp);
    if (bal_factor > 1)
    {
        if (diff (temp->left) > 0)
        {
            temp = ll_rotation (temp);
        }
        else
        {
            temp = lr_rotation (temp);
        }
    }
    else
    {
        if (bal_factor < -1)
        {
           if (diff (temp->right) > 0)
           {
               temp = rl_rotation (temp);
           }
           else
           {
              temp = rr_rotation (temp);
           }
        }
    }
    return temp;
}

/*
    Write_index
*/

void ClassInvertedIndex::write_index(avl_node *root)
{
    ofstream out;
    out.open("output.txt",ios::out);
    inorder_x(root,out);
    return;
}

/*
  Count of neighbors
*/

int ClassInvertedIndex::geitones(avl_node *root)
{
    if(root==NULL)
    {
        return 0;
    }
    return 1+geitones(root->right)+geitones(root->left);
}

/*
  Output
*/

void ClassInvertedIndex::sum(avl_node *root,ofstream &out)
{
    int i=0;
    if(root!=NULL)
    {
        i=geitones(root);
    }
    out<<"| "<<i<<" , ";
}

/*
  Inorder Traversal of AVL Tree
*/

void ClassInvertedIndex::inorder_x(avl_node* root, ofstream & out)
{
    if (root == NULL)
    {
        return;
    }
    inorder_x(root->left,out);
    out<<root->data<<" ";
    sum(root->neighbor,out);
    inorder_y(root->neighbor,out);
    out<<"\n";
    inorder_x(root->right,out);
}

void ClassInvertedIndex::inorder_y(avl_node* root,ofstream & out)
{
    if(root == NULL)
    {
        return;
    }
    inorder_y(root->left,out);
    out<<" "<<root->data<<" ";
    inorder_y(root->right,out);
}
