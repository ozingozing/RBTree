#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;

  return p;
}

void delete_rbtree(rbtree *t)
{
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}
//////////////ADD
void delete_node(rbtree *t, node_t *n)
{
  if(n == t->nil) return;
  delete_node(t, n->left);
  delete_node(t, n->right);
  free(n);
}
///////////////
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode->key = key;

  newNode->color = RBTREE_RED;
  node_t *curNodeParent = t->nil;
  node_t *curNode = t->root;

  while (curNode != t->nil)
  {
    curNodeParent = curNode;
    if (curNode->key > key)
      curNode = curNode->left;
    else
      curNode = curNode->right;
  }

  newNode->parent = curNodeParent;

  if (curNodeParent == t->nil)
    t->root = newNode;
  else if (curNodeParent->key > key)
    curNodeParent->left = newNode;
  else
    curNodeParent->right = newNode;

  newNode->left = t->nil;
  newNode->right = t->nil;
  InsertFixUp(t, newNode);
  return newNode;
}

// ADD//////////////////////////////
void InsertFixUp(rbtree *t, node_t *node)
{
  node_t *temp = NULL;
  while (node->parent->color == RBTREE_RED)
  {
    if (node->parent == node->parent->parent->left)
    {
      temp = node->parent->parent->right;
      if (temp->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        temp->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else
      {
        if (node == node->parent->right)
        {
          node = node->parent;
          LeftRotate(t, node);
        }

        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        RightRotate(t, node->parent->parent);
      }
    }
    else
    {
      temp = node->parent->parent->left;
      if (temp->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        temp->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else
      {
        if (node == node->parent->left)
        {
          node = node->parent;
          RightRotate(t, node);
        }

        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        LeftRotate(t, node->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

void LeftRotate(rbtree *t, node_t *n)
{
  node_t *temp = n->right;

  n->right = temp->left;
  if (temp->left != t->nil)
    temp->left->parent = n;

  temp->parent = n->parent;
  if (n->parent == t->nil)
    t->root = temp;
  else if (n == n->parent->left)
    n->parent->left = temp;
  else
    n->parent->right = temp;

  temp->left = n;
  n->parent = temp;
}

void RightRotate(rbtree *t, node_t *n)
{
  node_t *temp = n->left;
  n->left = temp->right;

  if (temp->right != t->nil)
    temp->right->parent = n;

  temp->parent = n->parent;

  if (n->parent == t->nil)
    t->root = temp;
  else if (n == n->parent->left)
    n->parent->left = temp;
  else
    n->parent->right = temp;
  temp->right = n;
  n->parent = temp;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *temp = t->root;

  while (temp != t->nil)
  {
    if (temp->key < key)
      temp = temp->right;
    else if (temp->key > key)
      temp = temp->left;
    else
      return temp;
  }

  return NULL;
}

void Transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

node_t *TreeMinimum(rbtree *t, node_t *node)
{
  if (node == t->nil)
    return t->nil;
  while (node->left != t->nil)
    node = node->left;
  return node;
}

void DeleteFixUp(rbtree *t, node_t *x)
{
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        LeftRotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          RightRotate(t, w);
          w = x->parent->right;
        }
        
        w->color = (x->parent->color == RBTREE_RED) ? RBTREE_RED : RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        LeftRotate(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        RightRotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          LeftRotate(t, w);
          w = x->parent->left;
        }

        w->color = (x->parent->color == RBTREE_RED) ? RBTREE_RED : RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        RightRotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

////////////////////////////////
node_t *rbtree_min(const rbtree *t)
{
  if (t == NULL)
    return NULL;

  node_t *temp = t->root;
  while (temp->left != t->nil)
  {
    temp = temp->left;
  }

  return temp;
}

node_t *rbtree_max(const rbtree *t)
{
  if (t == NULL)
    return NULL;

  node_t *temp = t->root;
  while (temp->right != t->nil)
  {
    temp = temp->right;
  }

  return temp;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *y = p;
  color_t yOriginalColor = y->color;
  node_t *x = NULL;

  if (p->left == t->nil)
  {
    x = p->right;
    Transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    Transplant(t, p, p->left);
  }
  else
  {
    y = TreeMinimum(t, p->right);
    yOriginalColor = y->color;
    x = y->right;
    if (y->parent == p)
      x->parent = y;
    else
    {
      Transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    Transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (yOriginalColor == RBTREE_BLACK)
  {
    DeleteFixUp(t, x);
  }
  free(p);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  node_t *temp = t->root;
  int cnt = 0;
  rbtree_to_array2(t, temp, arr, &cnt);
  return 0;
}
///////////ADD
void rbtree_to_array2(const rbtree *t, node_t *n, key_t *arr, int *cnt)
{
  if(t->nil == n) return;
  rbtree_to_array2(t, n->left, arr, cnt);
  arr[*cnt] = n->key;
  *cnt+=1;
  rbtree_to_array2(t, n->right, arr ,cnt);
}
////////////