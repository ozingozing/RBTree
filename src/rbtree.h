#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);
//rotate
void LeftRotate(rbtree *, node_t *);
void RightRotate(rbtree *, node_t *);
//transplant
void Transplant(rbtree *, node_t *, node_t *);
//getMinimum
node_t *TreeMinimum(rbtree *, node_t *);
//inserFixUp
void InsertFixUp(rbtree *, node_t *);
//deleteFixUp
void DeleteFixUp(rbtree *, node_t *);
void rbtree_to_array2(const rbtree *t, node_t *n, key_t *arr, int *cnt);
void delete_node(rbtree *t, node_t *n);
//
node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
