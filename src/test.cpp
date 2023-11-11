//
// Created by neske99 on 9.11.23..
//
#include<iostream>
#include "../lib/RBTree.cpp"


using namespace std;

int main(){
    RBTree<int>skup;
    skup.insert(2);
    skup.insert(3);
    skup.insert(7);
    skup.exportToFile("237");

    return 0;
}