#include "rbtree.h"

#include <stdlib.h>


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  //node_t 사이즈만큼의 배열을 1개생성과 동시에 초기화해준다.
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));

  //nil 노드의 색깔은 BLACK으로 설정
  nil -> color = RBTREE_BLACK;

  //rbtree의 nil 노드와 root는 nil처리해준다.
  p -> nil = p -> root = nil;

  
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory

  // node 변수에 root를 가져온다.
  node_t *node = t -> root;
  
  // node가 nil이 아니라면 delete_traverse함수호출
  if(node != t-> nil){
    delete_traverse(t,node);
  }
  // nil노드 메모리할당해제
  free(t->nil);
  
  // tree 메모리할당해제
  free(t);
  }

  // 부모노드인지 자식노드인지 확인하고 자식노드가 nil노드면 메모리할당해제
  void delete_traverse(rbtree *t, node_t *node){
    // node의 left가 nil이 아니면 재귀호출
    if(node->left != t->nil){
      delete_traverse(t,node->left);
    }
    // node의 right가 nil이 아니면 재귀호출
    if(node->right != t->nil){
      delete_traverse(t,node->right);
    }
    // nil이면 해당 node 메모리할당해제
    free(node);
  }
  

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  //새 노드 생성
  node_t *new_node = (node_t *)calloc(1,sizeof(node_t));
  //key값을 넣어줌
  new_node ->key = key;
  //노드의 색은 레드로 먼저 지정
  new_node ->color = RBTREE_RED;
  //노드의 left, right에 nil 노드 만들어줌
  new_node -> left = new_node -> right = t-> nil;

  // current는 root노드
  node_t *current = t->root;
  //current가 nil노드가 아닐때
  while(current != t->nil)
  { 
    //key값이 current의 key보다 작으면 왼쪽
    if(key < current->key){
      //current 의 left값이 nil이면 current에 new_node입력
      if(current -> left == t->nil){
        current->left = new_node;
        break;
      }
      //nil이 아니라면 current를 current left로 바꿔줌.
      current = current->left;
    }
    //key값이 current의 key보다 크면 오른쪽
    else{
      //current의 right가 nil이면 current에 new_node입력
      if(current -> right ==t->nil){
        current->right = new_node;
        break;
      }
      //nil이 아니라면 current를 current right로 바꿔줌
      current = current->right;
    }
  }
    //new_node의 부모는 current
    new_node -> parent = current;

    //current(root)가 nil이면 새 노드를 트리의 루트로 지정
    if(current == t->nil){
      t->root = new_node;
    }
    //불균형 복구함수 
    rbtree_insert_fixup(t,new_node);

    return new_node;
}

void rbtree_insert_fixup(rbtree *t, node_t *node){
  node_t *parent = node -> parent;
  node_t *grand_parent = parent -> parent;
  node_t *uncle;
  
  //node가 부모의 left이면 is_left라고 설정
  int(is_left) = node == parent -> left;
  //부모가 왼쪽 자식인지 확인
  int(parent_is_left);

  //node가 root일경우 Black으로 바꿔준다.
  if(node == t->root){
    node -> color =RBTREE_BLACK;
    return;
  }

  // 부모의 color가 black일경우 변경없음
  if(parent -> color==RBTREE_BLACK){
    return;
  }
  
  parent_is_left = grand_parent -> left == parent;
  uncle = (parent_is_left) ? grand_parent -> right : grand_parent ->left;

  //CASE 1: 삼촌 노드가 Red일경우
  if(uncle -> color == RBTREE_RED){
    uncle -> color = RBTREE_BLACK;
    parent -> color = RBTREE_BLACK;
    grand_parent -> color = RBTREE_RED;
    rbtree_insert_fixup(t,grand_parent);
    return;
  }

  if(parent_is_left){
    if(is_left){
      // CASE 3:부모가 왼쪽 자식이고 부모의 형제가 Black, 현재 노드가 왼쪽 자식인경우
      parent -> color = RBTREE_BLACK;
      grand_parent ->color =RBTREE_RED;
      right_rotate(t,parent);
      return;
    }

    // CASE 2: 부모가 왼쪽 자식이고 부모의 형제가 Black, 현재 노드가 오른쪽 자식인경우
    left_rotate(t,node);
    node ->color =RBTREE_BLACK;
    grand_parent -> color = RBTREE_RED;
    right_rotate(t,node);
    return;
  }

  // CASE 2: 부모가 오른쪽 자식이고 부모의 형제가 Black, 현재 노드가 왼쪽 자식인경우
  if(is_left){
    right_rotate(t,node);
    node -> color = RBTREE_BLACK;
    grand_parent ->color =RBTREE_RED;
    left_rotate(t,node);
    return;
  }

  // CASE 3 : 부모가 오른쪽 자식이고 부모의 형제가 Black, 현재 노드가 오른쪽 자식인경우
  parent->color = RBTREE_BLACK;
  grand_parent ->color = RBTREE_RED;
  left_rotate(t,parent);
  return;

}
//오른쪽으로 회전하는 함수
void right_rotate(rbtree *t, node_t* node){
  node_t *parent = node ->parent;
  node_t *grand_parent = parent -> parent;
  node_t *right_node = node -> right;

  //부모가 root일경우 node를 root로 설정
  if(parent == t->root){
    t->root = node;
  }
  else{
    // grand_parent의 왼쪽 자식이 parent일 경우
    if(grand_parent -> left == parent){
      grand_parent -> left =node;
    }
    else{
      grand_parent -> right =node;
    }
  }
  node->parent = grand_parent;
  parent ->parent = node;
  node->right=parent;
  right_node->parent =parent;
  parent->left=right_node;
}
//왼쪽으로 회전하는 함수
void left_rotate(rbtree *t, node_t* node){
  node_t *parent = node ->parent;
  node_t *grand_parent = parent -> parent;
  node_t *left_node = node -> left;

  //부모가 root일경우 node를 root로 설정
  if(parent == t->root){
    t->root = node;
  }
  else{
    // grand_parent의 왼쪽 자식이 parent일 경우
    if(grand_parent -> left == parent){
      grand_parent -> left =node;
    }
    else{
      grand_parent -> right =node;
    }
  }
  node->parent =grand_parent;
  parent->parent = node;
  node->left = parent;
  parent->right = left_node;
  left_node -> parent = parent;
}



node_t *rbtree_find(const rbtree *t, const key_t key) {
  //root를 현재 노드 current로 표시/
  node_t *current = t->root;

  //current 노드가 nil이 아닐때까지 반복
  while(current!= t->nil){
    //찾고자하는 key값이 current의 key값이랑 같으면 current출력
    if(key==current->key){
      return current;
    }
    else{
      //찾고자하는 key가 current의 key보다 작으면 left로 이동.
      if(key<current->key){
        current = current->left;
      }
      //찾고자하는 key가 current의 key보다 작으면 right로 이동.
      else{
        current = current->right;
      }
    }
  }
  //못찾으면 null 반환.
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  
  //root를 current 노드로 설정.
  node_t *current = t->root;

  while(current->left!=t->nil){
    current = current->left;
  }
  return current;
}

node_t *rbtree_max(const rbtree *t) {

  //root를 current 노드로 설정.
  node_t *current = t->root;

  while(current->right!=t->nil){
    current = current->right;
  }
  return current;
}
node_t *get_successor_node(rbtree *t, node_t *remove){

   while(remove->left != t->nil){
      remove = remove->left;
   } 
   return remove;
}
int rbtree_erase(rbtree *t, node_t *remove) {
  // TODO: implement erase
  node_t *successor;
  node_t *successor_parent, *replace_node;
  int is_successor_black, is_successor_left;

  //remove 노드의 자식이 둘일경우
  if(remove->left!=t->nil && remove->right!=t->nil){
    //successor노드 찾기
    successor = get_successor_node(t,remove->right);
    //대체노드는 successor노드의 오른쪽
    replace_node=  successor->right;
    //successor_node key값을 remove의 key값에 저장 
    remove->key = successor->key;  
  }
  //remove 노드의 자식이 하나이거나 없을때
  else{
    successor = remove;
    //대체할 노드, 자식이 있으면 자식노드로 대체 없으면 nil로 대체
    replace_node = (successor-> right !=t->nil) ? successor->right :successor->left;
  }
  successor_parent = successor->parent;

  //삭제 노드가 root일경우
  if(successor==t->root){
      t->root = replace_node;
      t->root->color = RBTREE_BLACK;
      free(successor);
      return 0;
  }
  //successor노드 지워지기전에 색 저장
  is_successor_black = successor->color;
  is_successor_left = successor_parent ->left ==successor;

  //succeessor이 왼쪽자식이라면 대체노드 왼쪽에연결
  if(is_successor_left){
    successor_parent->left = replace_node;
  }
  //succeessor이 오른쪽자식이라면 대체노드 오른쪽에연결
  else{
    successor_parent->right = replace_node;
  }
  //대체 노드의 부모 양뱡향 연결
  replace_node ->parent = successor_parent;
  free(successor);

  //successor 노드가 검정노드인 경우 불균형 복구 함수 호출
  if(is_successor_black){
    rbtree_erase_fixup(t,successor_parent,is_successor_left);
    return 0;
  }

  return 0;
}

void rbtree_erase_fixup(rbtree *t, node_t *parent, int is_left){

  //extra_black 노드가 RED일경우 BLACK으로 변경
  node_t *extra_black = is_left?parent->left:parent->right;
  if(extra_black->color==RBTREE_RED)
  {
    extra_black->color=RBTREE_BLACK;
    return;
  }
  //형제 노드는 extra_black의 반대
  node_t *sibling = is_left?parent->right:parent->left;
  node_t *sibling_left = sibling->left;
  node_t *sibling_right = sibling->right;

  //CASE1: 형제가 RED일경우
  if(sibling->color==RBTREE_RED){
    //is_left이면 부모와 형제의 색바꾸고 left_rotate
    if(is_left){
      sibling->color=RBTREE_BLACK;
      sibling->parent->color=RBTREE_RED;
      left_rotate(t,sibling);
    }
    //is_left가 아니면 부모와 형제의 색바꾸고 right_rotate
    else{
      sibling->color=RBTREE_BLACK;
      sibling->parent->color=RBTREE_RED;
      right_rotate(t,sibling);
    }
    rbtree_erase_fixup(t,parent,is_left);
    return;
  }
  //형제의 자식중 extra_black과 가까운경우
  node_t *near = is_left ? sibling_left:sibling_right;
  //형제의 자식중  extrablack과 먼경우
  node_t *distant = is_left ?sibling_right:sibling_left;
  //CASE3: 형제가 BLACK이고 꺾인자식이 RED일떄
  if(is_left&&near->color==RBTREE_RED && distant->color==RBTREE_BLACK){
    near->color = RBTREE_BLACK;
    sibling->color = RBTREE_RED;
    right_rotate(t,near);
    rbtree_erase_fixup(t,parent,is_left);
    return;
  }
  //CASE4: 형제가 BLACK이고 펴진자식이 RED일떄
  if(is_left&&distant->color==RBTREE_RED){
    if(sibling->parent->color==RBTREE_BLACK){
      sibling->color = RBTREE_BLACK;
      sibling->parent->color=RBTREE_BLACK;
    }
    else{
      sibling->color = RBTREE_RED;
      sibling->parent->color=RBTREE_BLACK;
    }
    left_rotate(t,sibling);
    distant->color =RBTREE_BLACK;
    return;
  }
  // 형제가 BLACK이고 형제의 가까운자식이 RED, 펴진 자식이 BLACK일때
  if(near->color == RBTREE_RED && distant->color==RBTREE_BLACK){
    near->color =RBTREE_BLACK;
    sibling->color=RBTREE_RED;
    left_rotate(t,near);
    rbtree_erase_fixup(t,parent,is_left);
    return;
  }
  //형제가 BLACK이고 형제의 펴진 자식이 RED일때
  if(distant->color==RBTREE_RED){
    if(sibling->parent->color==RBTREE_BLACK){
      sibling->color = RBTREE_BLACK;
      sibling->parent->color=RBTREE_BLACK;
    }
    else{
      sibling->color = RBTREE_RED;
      sibling->parent->color=RBTREE_BLACK;
    }
    right_rotate(t,sibling);
    distant->color=RBTREE_BLACK;
    return;
  }

  //CASE2: 형제가 BLACK이고 자식들이 BLACK일때
  sibling->color = RBTREE_RED;
  if(parent!=t->root){
    rbtree_erase_fixup(t,parent->parent,parent->parent->left==parent);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *node = t-> root;
  int idx=0;
  array_inorder(t,node,arr,&idx);
  return 0;
}
void array_inorder(const rbtree *t,node_t *node,key_t *arr, int *idx){
  if(node==t->nil){
    return;
  }
  if(node->left != t->nil){
  array_inorder(t,node->left,arr,idx);
  }
  arr[*idx]=node->key;
  (*idx)++;
  if(node->right != t->nil){
  array_inorder(t,node->right,arr,idx);
  }
}
