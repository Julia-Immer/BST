#include "BST.h"
#include <iostream>

// constructor, initialize class variables and pointers here if need.
BST::BST(){
  SetRoot(shared_ptr<bst_node>(NULL));
}

//deconstructor
BST::~BST(){
}

shared_ptr<bst_node> BST::InitNode(int data){
  //create new node with smart pointer with allocated heap memory
  shared_ptr<bst_node> new_node (new bst_node);
  new_node -> data = data;
  new_node -> left = shared_ptr<bst_node>(NULL);
  new_node -> right = shared_ptr<bst_node>(NULL);

  return new_node;
}

void BST::Insert(shared_ptr<bst_node> new_node){

  if (new_node != NULL) {
  //check root and if NULL, set the root to new_node
    if (GetRoot() == NULL) {
      SetRoot(new_node);
      return;
    }

    shared_ptr<bst_node> subt = GetRoot();

    int new_val = new_node -> data;

    //We loop until we find a suitable spot for the data
    while (true) {
      if (new_val < subt -> data) {
        // check if the left node is null
          if (subt -> left == NULL) {
            subt -> left = new_node;
            break;
          }
          else {
            subt = subt -> left; //walk left if data is lower
          }
      }
      else if (new_val >= subt -> data) {
        // check if the right node is null
        if (subt -> right == NULL) {
          subt -> right = new_node;
          break;
        }
        else {
          subt = subt -> right; //walk right if data is higher
        }
      }
    }
  }
}

void BST::InsertData(int data){
  //create the node and insert it
  shared_ptr<bst_node> new_node = InitNode(data); 
  Insert(new_node);
}

void BST::Remove(int data){

  bool is_root = false;
  vector<shared_ptr<bst_node>> vec = ParSelfSucc(data);

  shared_ptr<bst_node> rem_node = vec[1];
  shared_ptr<bst_node> successor = vec[2];
  shared_ptr<bst_node> succ_parent = vec[3];

  //only continue with the function if node exists
  if (rem_node != NULL) {

    //test if remove node is top
    if(GetRoot() == rem_node) {
      //node is top
      is_root = true;
    }

    //leaf node case
    if ((rem_node -> left == NULL) && (rem_node -> right == NULL)) {
 
        if(is_root) {
          SetRoot(NULL);
        }
        else {
          if(vec[0] -> right == rem_node) {
            vec[0] -> right = shared_ptr<bst_node>(NULL);
          }
          else if (vec[0] -> left == rem_node) {
            vec[0] -> left = shared_ptr<bst_node>(NULL);
          }
        }
    } 

    else if (rem_node -> left != NULL && rem_node -> right == NULL) {
      if(vec[0] -> right == rem_node) {
        vec[0] -> right = rem_node -> left ;
      }
      else if (vec[0] -> left == rem_node) {
        vec[0] -> left = rem_node -> left ;
      }
    }

    else if (rem_node -> left == NULL && rem_node -> right != NULL) {
      if(vec[0] -> right == rem_node) {
        vec[0] -> right = rem_node -> right ;
      }
      else if (vec[0] -> left == rem_node) {
        vec[0] -> left = rem_node -> right ;
      }
    }
    else {
      if (successor != NULL) {

        rem_node -> data = successor -> data;

        if (vec[3] -> left == successor)
          vec[3]->left = shared_ptr<bst_node>(NULL);
        else
          vec[3]->right = shared_ptr<bst_node>(NULL);

      }
    }
  }
}

//Gets vector of parent, self, successor and parent of successor
vector<shared_ptr<bst_node>> BST::ParSelfSucc(int value){
  shared_ptr<bst_node> subt = GetRoot();
  shared_ptr<bst_node> par(NULL);
  shared_ptr<bst_node> succ(NULL);
  vector<shared_ptr<bst_node>> ret{NULL, NULL, NULL, NULL};

  //Get parent and get self node
  while (subt != NULL) {
    //check if we find self or if we need to traverse left and right branches
    if (value == subt -> data) {
      ret[0] = par; //add parent
      ret[1] = subt;//add self
      break; 
    }
    else if (value < subt -> data) {
      par = subt;
      subt = subt -> left;
    }
    else {
      par = subt;
      subt = subt -> right;
    }
  }

  vector<shared_ptr<bst_node>> ret2 = Successor(subt);
  ret[2] = ret2[0];
  ret[3] = ret2[1];

  return ret;
}

vector<shared_ptr<bst_node>> BST::Successor(shared_ptr<bst_node> subt) {
  shared_ptr<bst_node> curr(NULL);
  shared_ptr<bst_node> par(NULL);
  
  if (subt != NULL) {
      //turn right once
      par = subt;
      curr = subt -> right;

    if (curr != NULL) {
      //make as many left turns as you can
      while (curr -> left != NULL) {
        par = curr;
        curr = curr -> left;
      }
    }
  }
  vector<shared_ptr<bst_node>> ret{curr, par};
  return ret;
}

//Gets next smaller value: left turn followed by as many right turns as you can make
shared_ptr<bst_node> BST::Predecessor(shared_ptr<bst_node> subt) {
  shared_ptr<bst_node> curr(NULL);

  if (subt != NULL) {
      //turn left
      curr = subt -> left;

    if (curr != NULL) {

      //make as many right turns as you can
      while (curr -> right != NULL) {
        curr = curr -> right;
      }
    }
  }
  return curr;
}

bool BST::Contains(shared_ptr<bst_node> subt, int value){
  while (subt != NULL) {
    //check if we find it or if we need to traverse left and right branches
    if (value == subt -> data) {
      return true;
    }
    else if (value < subt -> data) {
      subt = subt -> left;
    }
    else {
      subt = subt -> right;
    }
  }
  return false;
}

shared_ptr<bst_node> BST::GetNode(shared_ptr<bst_node> subt, int value){
    
  while (subt != NULL) {
  //check if we find it or if we need to traverse left and right branches
    if (value == subt -> data) {
      return subt;
    }
    else if (value < subt -> data) {
      subt = subt -> left;
    }
    else {
      subt = subt -> right;
    }
  }
  return shared_ptr<bst_node>(NULL);
}

//helper function for size
void BST::InOrder(shared_ptr<bst_node> subt, int* size) {

  if (subt == NULL) {
    return;
  }
  InOrder(subt -> left, size);
  ++(*size);
  InOrder(subt -> right, size);
}

int BST::Size(shared_ptr<bst_node> subt){
  int size = 0;
  InOrder(subt, &size);
  return size;
}


void BST::ToVector(shared_ptr<bst_node> subt, vector<int>& vec){
  //walk through tree in order from least to highest value
  if (subt == NULL) {
    return;
  }
  ToVector(subt -> left, vec);
  vec.push_back(subt -> data); //add value at node to vector
  ToVector(subt -> right, vec);
}

// This function is implemented for you
// Returns the root pointer
shared_ptr<bst_node> BST::GetRoot(){
  return root_ptr_;
}

// This function is implemented for you
// sets a given pointer as the top pointer
void BST::SetRoot(shared_ptr<bst_node> root_ptr){
  root_ptr_ = root_ptr;
}