//
// Created by neske99 on 9.11.23..
//
#define CATCH_CONFIG_MAIN
#include<iostream>
#include <string>
#include "../lib/RBTree.cpp"
#include "../lib/catch.cpp"

using namespace std;

TEST_CASE("Testing constructors for our Node<T> "){
    SECTION("Node<T>() should always give a black node"){
        Node<int>node=Node<int>();
        REQUIRE(node.color==black);
    }

    SECTION("Node<T>(T value) should always giva a red node"){
        Node<int>node=Node<int>(2);
        REQUIRE(node.color==red);
    }
}


TEST_CASE("Testing our isRBTRee functionality of the node",""){

    SECTION("Root can not be red"){
        Node<int>* root=new Node<int>();
        root->color=red;
        root->key={2};
        root->left=new Node<int>();
        root->right=new Node<int>();
        RBTree<int>skup=RBTree<int>(root);
        REQUIRE(!skup.isRBTree());

    }

    SECTION("Red node can not have a red child"){
        Node<int>*root=new Node<int>();
        root->color=black;
        root->key={2};
        root->left=new Node<int>(1);
        root->right=new Node<int>(3);

        root->left->left=new Node<int>();
        root->left->right=new Node<int>();
        root->right->left=new Node<int>();
        root->right->right=new Node<int>();

        root->left->left->color=red;
        RBTree<int>skup=RBTree<int>(root);
        REQUIRE(!skup.isRBTree());

    }

    SECTION("Left subtree cannot have different red depth from  the right subtree"){

        Node<int>*root=new Node<int>();
        root->color=black;
        root->key={2};
        root->left=new Node<int>(1);
        root->right=new Node<int>(3);

        root->left->left=new Node<int>(-1);
        root->left->left->color=black;
        root->left->right=new Node<int>();
        root->right->left=new Node<int>();
        root->right->right=new Node<int>();

        root->left->left->left=new Node<int>();
        root->left->left->right=new Node<int>();

        RBTree<int>skup=RBTree<int>(root);
        REQUIRE(!skup.isRBTree());

    }
}




