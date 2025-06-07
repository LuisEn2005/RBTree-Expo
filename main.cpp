#include <iostream>
#include <assert.h>

using std::cout;
using std::endl;

typedef enum{RED, BLACK} Color;
class Node{
  public:
    int key;
    Node* parent;
    Node* children[2];
    Color color;
    Node(int);
};

Node::Node(int data){
  parent = nullptr;
  children[0] = children[1] = nullptr;
  key = data;
  color = RED;
}

typedef enum{preOrder, inOrder, postOrder} prints;
class RBTree{
  public:
    Node* root;
    int isRootBlack();
    void RBinsert(Node*);
    void leftRotate(Node*);
    void rightRotate(Node*);
    void rbPrint(int);
    void deleteSubtree(Node*);
    RBTree(int);
    ~RBTree();
  private:
    void RBinsertFixUp(Node*);
    void printPreOrder(Node*);
    void printInOrder(Node*);
    void printPostOrder(Node*);
};

RBTree::RBTree(int key){
  root = new Node(key);
  root->color = BLACK;
}

RBTree::~RBTree() {
  deleteSubtree(root);
}

void RBTree::deleteSubtree(Node* node) {
  if (!node) return;
  deleteSubtree(node->children[0]);
  deleteSubtree(node->children[1]);
  delete node;
}

int RBTree::isRootBlack(){
  if(!root) return 1;
  return root->color == BLACK;
}

void RBTree::RBinsert(Node* node){
  Node* tmp = nullptr;
  Node* current = root;  // <-- puntero temporal para recorrer el árbol
  
  while(current){
    tmp = current;
    if(node->key < current->key)
      current = current->children[0];
    else
      current = current->children[1];
  } 
  node->parent = tmp;
  if(!tmp)
    root = node;  // árbol vacío, nuevo nodo es la raíz
  else{
    if(node->key < tmp->key)
      tmp->children[0] = node;
    else
      tmp->children[1] = node;
  }
  node->color = RED; // usualmente los nuevos nodos se insertan rojos
  RBinsertFixUp(node);
}

void RBTree::RBinsertFixUp(Node* node) {
  while(node->parent && node->parent->color == RED){ // Solo entra si el padre es rojo
    Node* grandparent = node->parent->parent; // Obtener el abuelo
    if(node->parent == grandparent->children[0]){ // Caso: padre es hijo izquierdo
      Node* uncle = grandparent->children[1]; // Tío es hijo derecho
      if(uncle && uncle->color == RED) { // Caso 1: El tío es rojo
        node->parent->color = BLACK;   // Recolorear padre
        uncle->color = BLACK;         // Recolorear tío
        grandparent->color = RED;     // Recolorear abuelo
        node = grandparent;           // Subir el nodo al abuelo
      }
      else{ // Tío es negro
        if(node == node->parent->children[1]){ // Caso 2: Nodo es hijo derecho
          node = node->parent;
          leftRotate(node); // Rotar a la izquierda
        }
        // Caso 3: Nodo es hijo izquierdo
        node->parent->color = BLACK; // Recolorear padre
        grandparent->color = RED;   // Recolorear abuelo
        rightRotate(grandparent);  // Rotar a la derecha
      }
    } 
    else{ // Simétrico: padre es hijo derecho
      Node* uncle = grandparent->children[0]; // Tío es hijo izquierdo
      if(uncle && uncle->color == RED) { // Caso 1: El tío es rojo
        node->parent->color = BLACK;   // Recolorear padre
        uncle->color = BLACK;         // Recolorear tío
        grandparent->color = RED;     // Recolorear abuelo
        node = grandparent;           // Subir el nodo al abuelo
      }
      else { // Tío es negro
        if (node == node->parent->children[0]) { // Caso 2: Nodo es hijo izquierdo
          node = node->parent;
          rightRotate(node); // Rotar a la derecha
        }
        // Caso 3: Nodo es hijo derecho
        node->parent->color = BLACK; // Recolorear padre
        grandparent->color = RED;   // Recolorear abuelo
        leftRotate(grandparent);   // Rotar a la izquierda
      }
    }
  }
  root->color = BLACK; // Asegurarse de que la raíz sea negra
}

void RBTree::leftRotate(Node* x) {
  Node* y = x->children[1];
  x->children[1] = y->children[0];
  if (y->children[0]) {
      y->children[0]->parent = x;
  }
  y->parent = x->parent;
  if (!x->parent) {
      root = y;
  } else if (x == x->parent->children[0]) {
      x->parent->children[0] = y;
  } else {
      x->parent->children[1] = y;
  }
  y->children[0] = x;
  x->parent = y;
}


void RBTree::rightRotate(Node* y) {
  assert(y != nullptr);
  Node* x = y->children[0];
  assert(x != nullptr);

  Node* B = x->children[1];
  Node* z = y->parent;

  // Paso 1: x toma la posición de y
  x->parent = z;
  if (z == nullptr) {
      root = x;
  } else if (y == z->children[0]) {
      z->children[0] = x;
  } else {
      z->children[1] = x;
  }

  // Paso 2: y pasa a ser hijo derecho de x
  x->children[1] = y;
  y->parent = x;

  // Paso 3: El subárbol B pasa a ser hijo izquierdo de y
  y->children[0] = B;
  if (B != nullptr) {
      B->parent = y;
  }
}



void RBTree::printPreOrder(Node* node) {
  if (node == nullptr) return;
  cout << node->key << " "<< node->color << " ";
  printPreOrder(node->children[0]);
  printPreOrder(node->children[1]);
}

void RBTree::printInOrder(Node* node) {
  if (node == nullptr) return;
  printInOrder(node->children[0]);
  cout << node->key << " "<< node->color << " ";
  printInOrder(node->children[1]);
}

void RBTree::printPostOrder(Node* node) {
  if (node == nullptr) return;
  printPostOrder(node->children[0]);
  printPostOrder(node->children[1]);
  cout << node->key << " "<< node->color << " ";
}

void RBTree::rbPrint(int type){
  switch(type) { 
    case preOrder:
      printPreOrder(root);
      break;
    case inOrder:
      printInOrder(root);
      break;
    case postOrder:
      printPostOrder(root);
      break;
    default:
      cout << "Tipo de recorrido no soportado" << endl;  
  }
}

int main(){
  RBTree tree(20);
  tree.RBinsert(new Node(15));
  tree.RBinsert(new Node(25));
  tree.RBinsert(new Node(10));
  tree.RBinsert(new Node(18));

  cout<<"PreOrder: ";
  tree.rbPrint(preOrder);

  cout<<"InOrder: ";
  tree.rbPrint(inOrder);

  cout<<"PostOrder: ";
  tree.rbPrint(postOrder);
  cout<<endl;
}
