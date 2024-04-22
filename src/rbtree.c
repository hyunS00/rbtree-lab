#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  nil->left = nil;
  nil->right = nil;
  nil->parent = nil;

  p->nil = nil;
  p->root = nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root == t->nil){
    free(t->nil);
    free(t);
    return;
  }
  
  node_t *left = t->root->left;
  node_t *right = t->root->right;
  node_t *root = t->root;
  if (left != t->nil){
    t->root = left;
    delete_rbtree(t);
  }
  
  if(right != t->nil){
    t->root = right;
    delete_rbtree(t);
  }

  free(t->nil);
  free(root);
  free(t);
}
void rbtree_leftrotate(rbtree *t, node_t *x){
  node_t *y = x->right; // x의 오른쪽 서브트리 y 결국 y를 끌어 올리고 x내려야 함
  x->right = y->left; // y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 옮김

  // 옮겨진 y의 왼쪽서브트리의 루트가 nil이 아니라면 y의 부모를 x로 설정
  if (y->left != t->nil)
    y->left->parent = x;
  y->parent = x->parent; // y의 부모를 x의 부모로 설정

  // x의 부모가 nil이라는 것은 x가 root였던것
  if(x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->left) //x가 부모의 왼쪽 서브트리의 루트였다면
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y; // x가 부모의 오른쪽 서브트리의 루트였다면
  }
  // 서브트리와 부모노드의 관계를 설정후 y의 왼쪽 서브트리에 완전히 x를 옮김
  y->left = x;
  x->parent = y;
}

void rbtree_rightrotate(rbtree *t, node_t *x){
  node_t *y = x->left; // x의 왼쪽 서브트리 y 결국 y를 끌어 올리고 x내려야 함
  x->left = y->right; // y의 오른쪽 서브트리를 x의 왼쪽 서브트리로 옮김

  // 옮겨진 y의 오른쪽서브트리의 루트가 nil이 아니라면 y의 부모를 x로 설정
  if (y->right != t->nil)
    y->right->parent = x;
  y->parent = x->parent; // y의 부모를 x의 부모로 설정

  // x의 부모가 nil이라는 것은 x가 root였던것
  if(x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->right) //x가 부모의 오른쪽 서브트리의 루트였다면
  {
    x->parent->right = y;
  }
  else
  {
    x->parent->left = y; // x가 부모의 왼쪽 서브트리의 루트였다면
  }
  // 서브트리와 부모노드의 관계를 설정후 y의 오른쪽 서브트리에 완전히 x를 옮김
  y->right = x;
  x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
  // 삽입한 노드z는 red이기 때문에 z의 부모노드는 red가 되면 안됨
  // 부모 노드가 black이 될때 까지 재조정
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left) // z의 부모 노드가 조부모노드의 왼족인 경우
    {
      node_t *y = z->parent->parent->right; // z의 삼촌 노드
      if (y->color == RBTREE_RED){ // case1: 부모가 red이고 삼촌이 red이면 두 노드와 조부모 노드의 색을 switching
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
      }
      else 
      { 
        if (z == z->parent) // case2: 부모가 red이고 삼촌이 black일때 z가 오른쪽 자식인 경우 case3으로 만듦
        {
          z = z->parent; // z의 부모를 기준으로 왼쪽으로 회전
          rbtree_leftrotate(t,z);
        }
        // case3: 부모가 red이고 삼촌이 black일때 z가 왼쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent = RBTREE_RED;
        rbtree_rightrotate(t,z);
      }
    }
    else // z의 부모 노드가 조부모노드의 오른쪽인 경우
    {
      node_t *y = z->parent->parent->left; // z의 삼촌 노드
      if (y->color == RBTREE_RED){ // case1: 부모가 red이고 삼촌이 red이면 두 노드와 조부모 노드의 색을 switching
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
      }
      else 
      { 
        if (z == z->parent) // case2: 부모가 red이고 삼촌이 black일때 z가 왼쪽 자식인 경우
        {
          z = z->parent; // z의 부모를 기준으로 오른쪽으로 회전
          rbtree_rightrotate(t,z);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent = RBTREE_RED;
        rbtree_leftrotate(t,z);
      }
    }
  }
  // 루트가 빨간색인 경우 블랙으로 칠해줌
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *x = t->nil; // 삽입할 노드의 위치를 탐색하는 노드
  node_t *y = t->root; // 삼입할 노드의 부모 노드

  // 삽입할 노드 z 생성 및 초기화
  node_t *z = (node_t*) calloc(1, sizeof(node_t));
  z->color = RBTREE_RED;
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->parent = t->nil;

  // z의 key 값에 따라 알맞은 자리로 이동
  while (x != t->nil)
  {
    y = x;
    // 이미 중복되는 key가 있다면 종료
    if (z->key == x->key)
      return t->root;
    
    // 삽입할 노드의 key값이 x의 key값보다 작다면 x의 왼쪽 자식 탐색
    if (z->key < x->key)
    {
      x = x->left;
    }
    // 삽입할 노드의 key값이 x의 key값보다 작다면 x의 오른쪽 자식 탐색
    else {
      x = x->right;
    }
  }

  // 삽일할 적절한 위치를 탐색후 z의 부모를 y로 설정
  z->parent = y;
  // 처음 삽입되는 노드라면
  if (y == t->nil)
  {
    t->root = z;
  }
  else if (z->key < y->key) // 탐색노드 x가 nil까지 내려가 왼쪽 자식이었는지 오른쪽 자식인지 판단을 못하기 떄문에 y의 어느 자식인지 판단
  {
    y->left = z;
  }
  else{
    y->right = z;
  }
  
  // RBtree 규칙 위반여부 판단 후 조정
  rbtree_insert_fixup(t,z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root; // 위치를 탐색하는 노드

  // key 값에 따라 알맞은 자리로 이동
  while (x != t->nil)
  {
    // key가 있다면 x return
    if (key == x->key)
      return x;
    // key값이 x의 key값보다 작다면 x의 왼쪽 자식 탐색
    if (key < x->key)
    {
      x = x->left;
    }
    // key값이 x의 key값보다 작다면 x의 오른쪽 자식 탐색
    else {
      x = x->right;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
