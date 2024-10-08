#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <random>
#include <string>
#include <vector>
using namespace std;

enum Color { red, black, doubleBlack };
enum Status { him = 0, father = 1, grandpa = 2, succes = 3 };

template <typename T> class Node {
public:
  Node<T> *left;
  Node<T> *right;
  optional<T> key;
  Color color;

  Node(T key, Color col) : left(nullptr), right(nullptr), color(col) {
    this->key = {key};
  }
  Node(T key) : Node(key, Color::red) {}

  Node() : left(nullptr), right(nullptr), color(Color::black) {
    this->key = {};
  }

  void recursiveDeleteChildren() {
    if (this->key) {
      this->left->recursiveDeleteChildren();
      this->right->recursiveDeleteChildren();
      delete this->left;
      delete this->right;
    }
  }

  bool find(T toFind) {
    if (!this->key)
      return false;
    T curr = key.value();

    if (toFind == curr)
      return true;
    else if (toFind < curr)
      return left->find(toFind);
    else
      return right->find(toFind);
  }
  bool operator==(const Node<T> &other) {

    if (!this->key && !other.key)
      return true;
    if (!this->key)
      return false;
    if (!other.key)
      return false;
    if (this->key.value() == other.key.value() && this->color == other.color)
      return ((*(this->left)) == (*(other.left))) &&
             (*(this->right)) == (*(other.right));
    return false;
  }
  pair<bool, int> isRBTreeExcludingRootRule(Node<T> *curr) {
    if (curr->color == Color::doubleBlack)
      return {false, 0};

    if (!curr->key) {
      if (curr->color != Color::black)
        return {false, 0};
      return {true, 1};
    }

    auto levi = isRBTreeExcludingRootRule(curr->left);
    auto desni = isRBTreeExcludingRootRule(curr->right);
    if (levi.first && desni.first && levi.second == desni.second) {
      if (curr->color == Color::red && curr->left->color != Color::red &&
          curr->right->color != Color::red)
        return {true, desni.second};
      else if (curr->color == Color::black)
        return {true, desni.second + 1};
    }
    return {false, 0};
  }
  bool isRBTree() {
    return this->color == Color::black && isRBTreeExcludingRootRule(this).first;
  }
  Node<T> *leftRotate() {
    Node<T> *z = this;
    Node<T> *y = z->right;

    z->right = y->left;
    y->left = z;
    return y;
  }
  Node<T> *rightRotate() {
    Node<T> *z = this;
    Node<T> *y = z->left;

    z->left = y->right;
    y->right = z;
    return y;
  }
  // TODO what if curr is null break recursion
  void exportNode(ofstream &f, Node<T> *curr, int index) {
    if (!curr->key) {
      string col = "black";

      if (curr->color == Color::doubleBlack)
        col = "green";
      f << index << "[label=\"" << "NULL" << "\"," << "color=" << col
        << ",style=filled,fontcolor=white];\n";
      return;
    }
    string nodeName = to_string(curr->key.value());

    int leftIndex = 2 * index + 1;
    int rightIndex = 2 * index + 2;

    string col = "";
    if (curr->color == Color::black)
      col = "black";
    else if (curr->color == Color::red)
      col = "red";
    else
      col = "green";

    f << index << "[label=\"" + nodeName + "\"," << "color=" << col
      << ",style=filled,fontcolor=white];\n";
    f << index << " -> " << leftIndex << ";\n";
    f << index << " -> " << rightIndex << ";\n";

    exportNode(f, curr->left, leftIndex);

    exportNode(f, curr->right, rightIndex);
  }

  void exportToFile(string filename = "graph") {
    ofstream f;
    int i = 0;
    f.open("dots/" + filename + ".dot", std::ios::out);

    f << "digraph{\n";

    exportNode(f, this, i);

    f << "}\n";
    f.flush();
    f.close();
  }

  int calcSize(Node<T> *curr) {
    if (!curr->key)
      return 0;
    return 1 + calcSize(curr->left) + calcSize(curr->right);
  }
};

template <typename T> class InsertionFamily {
public:
  Node<T> *grandpa;
  Node<T> *father;
  Node<T> *uncle;
  Node<T> *him;
  Node<T> *sibling;
  Status status;

  void findUncleAndCousin() {
    if (grandpa->left == father)
      uncle = grandpa->right;
    else
      uncle = grandpa->left;

    if (father->left == him)
      sibling = father->right;
    else
      sibling = father->left;
  }

  InsertionFamily<T> fixUp() {
    findUncleAndCousin();
    if (uncle->color == Color::red) { // uncle is red , parent is red
      uncle->color = Color::black;
      father->color = Color::black;
      grandpa->color = Color::red;
      InsertionFamily newFam;
      newFam.status = Status::him;
      newFam.him = grandpa;
      return newFam;
    } else { // uncle is black , parent is red
      // triangle case
      if (grandpa->left == father && father->right == him) {

        grandpa->left = grandpa->left->leftRotate();
        father = grandpa->left;
        him = grandpa->left->left;
        sibling = father->right;
      } else if (grandpa->right == father && father->left == him) {
        grandpa->right = grandpa->right->rightRotate();

        father = grandpa->right;
        him = grandpa->right->right;
        sibling = father->left;
      }
      // line case
      if (grandpa->right == father && father->right == him) {
        grandpa = grandpa->leftRotate();
        grandpa->color = Color::black;
        grandpa->left->color = Color::red;

        InsertionFamily newFam;
        newFam.status = Status::succes;
        return newFam;

      } else if (grandpa->left == father && father->left == him) {
        grandpa = grandpa->rightRotate();
        grandpa->color = Color::black;
        grandpa->right->color = Color::red;

        InsertionFamily<T> newFam;
        newFam.status = Status::succes;
        return newFam;
      }
    }
  }
};

template <typename T> class RBTree {
public:
  RBTree() {
    root = new Node<T>();
    root->color = Color::black;
    size = 0;
  }
  RBTree(Node<T> *root) {
    this->root = root;
    size = root->calcSize(root);
  }
  RBTree(const vector<T> &vec) {
    this->root = new Node<T>();
    int n = vec.size();
    for (int i = 0; i < n; i++)
      this->insert(vec[i]);
  }
  ~RBTree() {
    this->root->recursiveDeleteChildren();
    delete this->root;
  }
  int getSize() const { return size; }

  bool find(T toFind) const { return root->find(toFind); }
  void insert(T toInsert) {
    root = insertHelper(root, toInsert).second;
    root->color = Color::black;
  }
  bool operator==(const RBTree &other) const {
    return (*root) == (*other.root);
  }
  void erase(T toErase) {
    root = eraseHelper(root, toErase);

    root->color = Color::black;
  }

  void exportToFile(string filename = "graph") { root->exportToFile(filename); }

  bool isRBTree() { return root->isRBTree(); }

private:
  Node<T> *eraseHelper(Node<T> *curr, T toErase) {
    if (!curr->key) { // Nothing to erase size doesent change

      return curr;
    }
    if (curr->key.value() == toErase) {
      if (!curr->left->key && !curr->right->key) { // isleaf(has no children)
        Node<T> *toReturn = new Node<T>();

        if (curr->color == Color::red)
          ; // do nothing
        else
          toReturn->color = Color::doubleBlack;

        delete curr->left;
        delete curr->right;
        delete curr;
        size--;
        return toReturn;
      } else if (!curr->left->key) { // has only red right child which is red
                                     // and has null children
        auto left = curr->left;
        auto right = curr->right;

        right->color = Color::black;

        delete left;
        delete curr;
        size--;
        return right;
      } else if (!curr->right
                      ->key) { // has only red left child with null children
        auto left = curr->left;
        auto right = curr->right;

        left->color = Color::black;

        delete right;
        delete curr;
        size--;
        return left;

      } else { // has both children
        Node<T> *leftMost = getLeftMost(curr->right);

        curr->key = leftMost->key;
        curr->right = eraseHelper(curr->right, leftMost->key.value());
        if (curr->right->color == Color::doubleBlack)
          curr = eraseFixup(curr); // TODO delete fixup:

        return curr;
      }
    }

    if (toErase < curr->key.value()) {
      curr->left = eraseHelper(curr->left, toErase);
      curr = eraseFixup(curr);
      return curr;
    } else {
      curr->right = eraseHelper(curr->right, toErase);
      curr = eraseFixup(curr);
      return curr;
    }
  }

  Node<T> *eraseFixup(Node<T> *curr) {
    if (curr->left->color == Color::doubleBlack) {
      Node<T> *sibling = curr->right;

      if (sibling->color == Color::black &&
          sibling->left->color == Color::black &&
          sibling->right->color == Color::black &&
          curr->color == Color::black) { // d2
        sibling->color = Color::red;
        curr->left->color = Color::black;
        curr->color = Color::doubleBlack;
        return curr;
      }
      if (sibling->color == Color::red) { // d3
        curr = curr->leftRotate();
        curr->color = Color::black;
        curr->left->color = Color::red;
        // curr->left->left->color=doubleBlack;
        curr->left = eraseFixup(curr->left);
        return curr; // maybe needs more work
      }
      if (sibling->color == Color::black &&
          sibling->left->color == Color::black &&
          sibling->right->color == Color::black &&
          curr->color == Color::red) { // d4
        sibling->color = Color::red;
        curr->left->color = Color::black;
        curr->color = Color::black;
        return curr;
      }
      if (sibling->color == Color::black &&
          sibling->left->color == Color::red &&
          sibling->right->color == Color::black) { // d5
        curr->right = sibling->rightRotate();
        curr->right->color = Color::black;
        curr->right->right->color = Color::red;
        sibling = curr->right;
      }
      if (sibling->color == Color::black &&
          sibling->right->color == Color::red) { // d6
        curr = curr->leftRotate();
        curr->left->left->color = Color::black;
        curr->color = curr->left->color;
        curr->right->color = Color::black;
        curr->left->color = Color::black;
      }
    } else if (curr->right->color == Color::doubleBlack) {
      Node<T> *sibling = curr->left;

      if (sibling->color == Color::black &&
          sibling->left->color == Color::black &&
          sibling->right->color == Color::black &&
          curr->color == Color::black) { // d2
        sibling->color = Color::red;
        curr->right->color = Color::black;

        curr->color = Color::doubleBlack;
        return curr;
      }

      if (sibling->color == Color::red) { // d3

        curr = curr->rightRotate();
        curr->color = Color::black;
        curr->right->color = Color::red;

        curr->right = eraseFixup(curr->right);
        return curr; // maybe Needs more work
      }
      if (sibling->color == Color::black &&
          sibling->left->color == Color::black &&
          sibling->right->color == Color::black &&
          curr->color == Color::red) { // d4

        sibling->color = Color::red;
        curr->right->color = Color::black;

        curr->color = Color::black;
        return curr;
      }
      if (sibling->color == Color::black &&
          sibling->right->color == Color::red &&
          sibling->left->color == Color::black) { // d5
        curr->left = sibling->leftRotate();
        curr->left->color = Color::black;
        curr->left->left->color = Color::red;
        sibling = curr->left;
      }
      if (sibling->color == Color::black &&
          sibling->left->color == Color::red) { // d6
        curr = curr->rightRotate();
        curr->right->right->color = Color::black;
        curr->color = curr->right->color;
        curr->left->color = Color::black;
        curr->right->color = Color::black;
      }
    }
    return curr;
  }

  Node<T> *getLeftMost(Node<T> *curr) {
    if (!curr->left->key)
      return curr;

    return getLeftMost(curr->left);
  }
  pair<InsertionFamily<T>, Node<T> *> insertHelper(Node<T> *curr, T toInsert) {
    if (!curr->key) {
      size++;
      curr->key = {toInsert};
      curr->color = Color::red;
      curr->left = new Node<T>();
      curr->right = new Node<T>();
      InsertionFamily<T> f;
      f.him = curr;
      f.status = him;
      return {f, curr};
    } else {
      InsertionFamily<T> fam;

      T key = curr->key.value();
      if (key == toInsert) {
        InsertionFamily<T> f;
        f.status = succes;
        return {f, curr};
      }
      if (toInsert < key) {
        auto tmp = insertHelper(curr->left, toInsert);
        fam = tmp.first;
        curr->left = tmp.second;
      } else {
        auto tmp = insertHelper(curr->right, toInsert);
        fam = tmp.first;
        curr->right = tmp.second;
      }

      if (fam.status == succes)
        return {fam, curr};
      else if (fam.status == father) { // child is father
        fam.status = grandpa;
        fam.grandpa = curr;

        InsertionFamily newFam = fam.fixUp();
        return {newFam, fam.grandpa};

      } else { // if(fam.status == him) { // child is him
        fam.status = father;
        fam.father = curr;
        if (curr->color == Color::black) {
          InsertionFamily<T> gfam;
          gfam.status = succes;
          return {gfam, curr};
        }
        return {fam, curr};
      }
    }
  }
  Node<T> *root;
  int size;
};


