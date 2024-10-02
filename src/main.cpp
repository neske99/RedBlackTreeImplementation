//
// Created by neske99 on 12.11.23..
//
#include<iostream>
#include<string>
#include "../lib/RBTree.cpp"

using namespace std;

void interactive(){
    RBTree<int>skup;
    string tmp;
    int i=1;
    while(true){
        cout<<"action"<<i<<" : ";
        cout.flush();
        cin>>tmp;
        if(tmp[0]=='i'){
            cin>>tmp;
            skup.insert(stoi(tmp));
        }else if(tmp[0]=='e'){
            cin>>tmp;
            skup.erase(stoi(tmp));
        }else if(tmp[0]=='q'){
            break;
        }else{
            cerr<<"Unknown command: "<<tmp<< endl;
        }
        skup.exportToFile("graph"+ to_string(i));

        i++;

    }
}

int main(){
    interactive();

    return 0;
}