#include <stdio.h>
#include <malloc.h>
#include <iostream>

using namespace std;

struct node 
	
{
   int red;
   int data;
   struct node *link [2];
};

struct tree 
{ 
  struct node *root;
};

int is_red (struct node *root)
{
       return root != NULL && root->red == 1;
}



struct node *rot_single (struct node *root, int dir)
{         struct node *save = root->link [!dir];

         root->link [!dir] = save->link [dir];
         save->link [dir] = root;

         root->red = 1;
         save->red = 0;

        return save;
 }

 struct node *rot_double (struct node *root, int dir)
 {
        root->link [!dir] = rot_single (root->link [!dir], !dir);
        return rot_single (root, dir);
 }


 int rb_assert (struct node *root)
 {        int lh, rh;

        if (root == NULL)
               return 1;
       else {
               struct node *ln = root->link [0];
               struct node *rn = root->link [1];

              
              if (is_red (root)) {
                     if (is_red (ln) || is_red (rn)) {
                            puts ("Red violation");
                            return 0;
                     }
              }

              lh = rb_assert (ln);
              rh = rb_assert (rn);

              
              if ((ln != NULL && ln->data >= root->data)
                     || (rn != NULL && rn->data <= root->data))
              {
                     puts ("Binary tree violation");
                     return 0;
              }

              
              if (lh != 0 && rh != 0 && lh != rh) {
                     puts ("Black violation");
                     return 0;
              }

              
              if (lh != 0 && rh != 0)
                     return is_red (root) ? lh : lh + 1;
              else
                     return 0;
       }
 }


 struct node *make_node (int data)
 {
       struct node *rn = (node*)malloc (sizeof *rn);
 
       if (rn != NULL) {
             rn->data = data;
             rn->red = 1; 
             rn->link [0] = NULL;
             rn->link [1] = NULL;
       }
 
       return rn;
 }



 
//Вставка сверху вниз
 
 int insert (struct tree *tree, int data)
 {
       if (tree->root == NULL) {
             
             tree->root = make_node (data);
             if (tree->root == NULL)
                   return 0;
       }
       else {
             struct node head = { 0 }; 

             struct node *g, *t;     
             struct node *p, *q;    
             int dir = 0, last;

             
             t = &head;
             g = p = NULL;
             q = t->link [1] = tree->root;

           
             for ( ; ; ) {
                   if (q == NULL) {
                        
                         p->link [dir] = q = make_node (data);
                         if (q == NULL)
                               return 0;
                   }
                   else if (is_red (q-> link [0]) && is_red (q->link [1])) {
                          
                         q->red = 1;
                         q->link [0]->red = 0;
                         q->link [1]->red = 0;
                   }

                   
                   if (is_red (q) && is_red (p)) {
                         int dir2 = t->link [1] == g;

                         if (q == p->link [last])
                               t->link [dir2] = rot_single (g, !last);
                         else
                               t->link [dir2] = rot_double (g, !last);
                   }

                   
                   if (q->data == data)
                         break;

                   last = dir;
                   dir = q->data < data;

                   
                   if (g != NULL)
                         t = g;
                   g = p, p = q;
                   q = q->link [dir];
             }

              
             tree->root = head.link [1];
       }

        
       tree->root->red = 0;

       return 1;
} 







//Удаление сверху вниз

 int remove (struct tree *tree, int data)
 {
       if (tree->root != NULL) 
	   {
             struct node head = { 0 }; 
             struct node *q, *p, *g; 
             struct node *f = NULL;  
             int dir = 1;

              
             q = &head;
             g = p = NULL;
             q->link [1] = tree->root;

             
             while (q->link [dir] != NULL) {
                   int last = dir;

                   
                   g = p, p = q;
                   q = q->link [dir];
                   dir = q->data < data;

                    
                   if (q->data == data)
                         f = q;

                   
                   if (!is_red (q) && !is_red (q->link [dir])) {
                         if (is_red (q->link [!dir]))
                               p = p->link [last] = rot_single (q, dir);
                         else if (!is_red (q->link [!dir])) {
                               struct node *s = p->link [!last];

                               if (s != NULL) {
                                     if (!is_red (s->link [!last]) && !is_red (s->link [last])) {
                                           
                                           p->red = 0;
                                           s->red = 1;
                                           q->red = 1;
                                     }
                                     else {
                                           int dir2 = g->link [1] == p;

                                           if (is_red (s->link [last]))
                                                 g->link [dir2] = rot_double (p, last);
                                           else if (is_red (s->link [!last]))
                                                 g->link [dir2] = rot_single (p, last);

                                          
                                           q->red = g->link [dir2]->red = 1;
                                           g->link [dir2]->link [0]->red = 0;
                                           g->link [dir2]->link [1]->red = 0;
                                     }
                               }
                         }
                   }
             }

             
             if (f != NULL) {
                   f->data = q->data;
                   p->link [p->link [1] == q] =
                         q->link [q->link [0] == NULL];
                   free (q);
             }

            
             tree->root = head.link [1];
             if (tree->root != NULL)
                   tree->root->red = 0;
       }

       return 1;
}


node *findnode(struct tree *tree ,int data) 
{


    node *current = tree->root;
    while(current != NULL)
        if(data==current->data)
            return (current);
        else
            current = (data < current->data) ?
			current->link[0] : current->link[1];
    return(0);
}


int main()
{
int vibor;

struct tree mytree;
mytree.root=NULL;
while(1)
{	
	cout<<("\n");
	cout<<("1 - add element\n");
	cout<<("2 - delete element\n");
	cout<<("3 - find element\n");
	cout<<("0 - Exit\n\n");
	cin>>vibor;

	if (vibor == 0)
	{			
	break;
	}
	else if (vibor == 1)
	{
		cout << "Vvedite celoe chislo: ";	
		int numb;
		cin >> numb;
		insert(&mytree,numb);
		cout << "\n";
	} 
	else if(vibor == 2)
	{
		cout << "kakoi element udalit' ? ";	
		int numb;
		cin >> numb;
		node* pnode=findnode(&mytree,numb);
		if(pnode) remove (&mytree,numb);
		else cout << "takogo elementa net";
		cout << "\n";
	}
	else if(vibor == 3)
	{
		cout << "Vvedite kluch elementa: ";	
		int numb;
		cin >> numb;
		node* pnode=findnode(&mytree,numb);
		if(pnode)cout << "Element nahoditsya po adresu " << pnode;
		else cout << "element ne naiden";
		cout << "\n";
	}

}

return 0;
}



