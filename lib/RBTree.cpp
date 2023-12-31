#include <iostream>
#include <string>
#include <vector>
#include<queue>
#include<optional>
#include<fstream>
#include<algorithm>
#include<numeric>
#include<functional>
#include<random>
#include <bits/stdc++.h>
using namespace std;

enum Color{red,black,doubleBlack};
enum Status{on=0,otac=1,deda=2,gotovo=3};

template<typename T>
class Node{
public:
    Node<T>*left;
    Node<T>*right;
    optional<T>key;
    Color color;
    
    Node(T key,Color col):left(nullptr),right(nullptr),color(col){
        this->key={key};
    }
    Node(T key):Node(key,red){}
    
    Node():left(nullptr),right(nullptr),color(black)
    {
        this->key={};
    }
    void recursiveDeleteChildren(){
        if(this->key){
            this->left->recursiveDeleteChildren();
            this->right->recursiveDeleteChildren();
        }
        delete this->left;
        delete this->right;
    }
    

    bool find(T toFind){
        if(!this->key) return false;
        T curr=key.value();
    
        if(toFind==curr)       return true;
        else if(toFind <curr ) return left->find(toFind);
        else                   return right->find(toFind);
    
    }
    bool operator==(const Node<T>& other){
        
        if(!this->key && !other.key)
            return true;
        if(!this->key )
            return false;
        if(!other.key)
            return false;
        if(this->key.value() ==other.key.value() && this->color==other.color)
            return  ((*(this->left)) ==(*(other.left))) && (*(this->right))==(*(other.right));
        return false;
    }
    pair<bool,int> isRBTree(Node<T>*curr){
        if(!curr->key){
            return {true,1};
        }
        auto levi=isRBTree(curr->left);
        auto desni=isRBTree(curr->right);
        if(levi.first && desni.first && levi.second==desni.second){
            if(curr->color==red && curr->left->color==black && curr->right->color==black)
                return {true,desni.second};
            else if(curr->color==black)
                return {true,desni.second+1};
            return {false,0};
        }
        return {false,0};
    }
    bool isRBTree(){
        return isRBTree(this).first && this->color==black;
    }
    Node<T>* leftRotate(){
        Node<T>*z=this;
        Node<T>*y=z->right;
        //Node<T>*x=y->right;
        z->right=y->left;
        y->left=z;
        return y;
    }
    Node<T>* rightRotate(){
        Node<T>*z=this;
        Node<T>*y=z->left;
        //Node<T>*x=y->left;
        z->left=y->right;
        y->right=z;
        return y;

    } 
    void exportNode(ostream& f,Node<T>*curr,int index){
        if(!curr->key){
            string col="black";
            
            if(curr->color==doubleBlack) col="green";
            f << index<<"[label=\""<<"NULL"<<"\","<<"color="<< col << ",style=filled,fontcolor=white];\n";
            return;
        }
        string nodeName="";
        string lName="";
        string rName="";

        if(curr->key) nodeName+=to_string(curr->key.value());
        else          nodeName+="NULL";        
        
        if(curr->left->key) lName+=to_string(curr->left->key.value());
        else          lName+="NULL";
        
        if(curr->right->key) rName+=to_string(curr->right->key.value());
        else          rName+="NULL";

        int leftIndex=2*index+1;
        int rightIndex=2*index+2;
        
        string col="";
        if(curr->color==black)    col="black";
        else if(curr->color==red) col="red";
        else                      col="green";

        f << index<<"[label=\""+nodeName+"\","<<"color="<<col << ",style=filled,fontcolor=white];\n";
        f<<index<<" -> "<<leftIndex<<";\n";
        f<<index<<" -> "<<rightIndex<<";\n";

        
        exportNode(f,curr->left,leftIndex);
        
        
        exportNode(f,curr->right,rightIndex);
    }

    void exportToFile(string filename="graph"){
        ofstream f;
        int i=0;
        f.open("dots/"+filename+".dot");
        f << "digraph{\n"; 
        
        exportNode(f,this,i);

        f<< "}\n";
        f.close();
    }
    int calcSize(Node<T>*curr){
        if(!curr->key) return 0;        
        return 1+calcSize(curr->left)+calcSize(curr->right);

    }

};

template<typename T>
class Family{
public:
    Node<T>*grandpa;
    Node<T>*father;
    Node<T>*uncle;
    Node<T>*him;
    Node<T>*cousin;
    Status status;

    void findUncleAndCousin(){
        if(grandpa->left==father) uncle=grandpa->right;
        else                    uncle=grandpa->left;

        if(father->left==him) cousin=father->right;
        else                  cousin=father->left;

    }

    Family<T> fixUp(){
        findUncleAndCousin();
        if(uncle->color==red){//uncle is red , parent is red
            uncle->color=black;
            father->color=black;
            grandpa->color=red;
            Family newFam;
            newFam.status=on;
            newFam.him=grandpa;
            return newFam;
        }else{//uncle is black , parent is red
            //triangle case
            if(grandpa->left==father && father->right==him){
                grandpa->left=him;
                father->right=him->left;
                him->left=father;

                father=grandpa->left;
                him=grandpa->left->left;
                cousin=father->right;
            }else if(grandpa->right==father && father->left==him){
                grandpa->right=him;
                father->left =him->right;
                him->right= father;

                father=grandpa->right;
                him=grandpa->right->right;
                cousin=father->left;
            }
            //line case
            if(grandpa->right==father && father->right==him){
                grandpa->right=cousin;
                father->left=grandpa;
                grandpa->color=red;
                father->color=black;

                grandpa=father;
                Family newFam;
                newFam.status=gotovo;
                return newFam;

            }else if(grandpa->left==father && father->left==him){
                grandpa->left=cousin;
                father->right=grandpa;
                grandpa->color=red;
                father->color=black;

                grandpa=father;
                Family<T> newFam;
                newFam.status=gotovo;
                return newFam;
            }
        }
    }
};

template<typename T>
class RBTree{
public:
    RBTree(){
        root=new Node<T>();
        root->color=black;
        size=0;
    }
    RBTree(Node<T>*root){
        this->root=root;
        size=root->calcSize(root);

    }
    RBTree(const vector<T>&vec){
        this->root=new Node<T>();
        int n=vec.size();
        for(int i =0;i<n;i++)
            this->insert(vec[i]);
    }
    ~RBTree(){
        this->root->recursiveDeleteChildren();
        delete this->root;
    }
    int getSize() const{
        return size;
    }

    bool find(T toFind)const{
        return root->find(toFind);
    }
    void insert(T toInsert){
        size++;
        root=insertHelper(root,toInsert).second;
        root->color=black;
    }
    bool operator==(const RBTree&other)const{
        return (*root)==(*other.root);
    }
    void erase(T toErase){
        root=eraseHelper(root,toErase);
        --size;
        root->color=black;
       
    }

    void bfsPrint(){
        queue<Node<T>*>redd;
        redd.push(root);
        while(!redd.empty()){
            int n=redd.size();
            for(int i=0;i<n;i++){
                Node<T>*curr=redd.front();
                redd.pop();
                string toPrint="";
                if(curr->color==red)        toPrint+="R";
                else if(curr->color==black) toPrint+="B";
                else                        toPrint+="DB";
                if(curr->key){
                    toPrint+=to_string(curr->key.value());
                    redd.push(curr->left);
                    redd.push(curr->right);
                }
                else          toPrint+="NULL";
                cout<<toPrint<<"    ";

            }
            cout<<endl;
        }
    }

    void exportToFile(string filename="graph"){
        root->exportToFile(filename);
    }
    

    bool isRBTree(){
        return root->isRBTree();
    }
private:
Node<T>* eraseHelper(Node<T>* curr,T toErase){
    if(!curr->key){
        size++;
        return curr;
    }
    if(curr->key.value()==toErase){
        if(!curr->left->key && !curr->right->key){//has no children
            Node<T>*toReturn=new Node<T>();

            if(curr->color==red) ;//do nothing
            else                 toReturn->color=doubleBlack;//do something TODO 
        
            delete curr->left;
            delete curr->right;
            delete curr;
            return toReturn;
        }else if(!curr->left->key){//has only right child
            auto left=curr->left;
            auto right=curr->right;
            
            //if(curr->color==red && right->color==red) ;cant happen
            if(curr->color==red || right->color==red) right->color=black;
            else                                      right->color=doubleBlack;

            delete left;
            delete curr;
            return right;
        }else if(!curr->right->key){//has only left child
            auto left=curr->left;
            auto right=curr->right;

            if(curr->color==red || left->color==red ) left->color=black; 
            else                                      left->color=doubleBlack;

            delete right;
            delete curr;
            return left;

        }else{// has both children
            
                       
            Node<T>*leftMost=getLeftMost(curr->right);
            
            
            
            curr->key=leftMost->key;
            curr->right=eraseHelper(curr->right,leftMost->key.value());
            if(curr->right->color==doubleBlack) curr=eraseFixup(curr);//TODO delete fixup:
            
           
            return curr;

            
        }

       
    }

    if(toErase<curr->key.value()){ 
        curr->left=eraseHelper(curr->left,toErase);
        curr=eraseFixup(curr);
        return curr;
    }
    else{
        curr->right=eraseHelper(curr->right,toErase);
        curr=eraseFixup(curr);
        return curr;
    }
}

Node<T>* eraseFixup(Node<T>*curr){
    if(curr->left->color==doubleBlack){
            Node<T>*sibling=curr->right;

            if(sibling->color==black && sibling->left->color==black && sibling->right->color==black && curr->color==black ){//d2
                sibling->color=red;
                curr->left->color=black;
                curr->color=doubleBlack;
                return curr;
            }
             if(sibling->color==red){//d3
                curr=curr->leftRotate();
                curr->color=black;
                curr->left->color=red; 
                //curr->left->left->color=doubleBlack;
                curr->left=eraseFixup(curr->left);
                return curr;//maybe needs more work
            }
            if(sibling->color==black && sibling->left->color==black && sibling->right->color==black && curr->color==red ){//d4
                sibling->color=red;
                curr->left->color=black;
                curr->color=black;
                return curr;
            }
            if(sibling->color==black && sibling->left->color==red && sibling->right->color==black){//d5
                curr->right=sibling->rightRotate();
                curr->right->color=black;
                curr->right->right->color=red;
                sibling=curr->right;

            }
            if(sibling->color==black && sibling->right->color==red){//d6
                curr=curr->leftRotate();
                curr->left->left->color=black;
                curr->color=curr->left->color;
                curr->right->color=black;
                curr->left->color=black;
            }
    }else if(curr->right->color==doubleBlack){
            Node<T>*sibling=curr->left;

            if(sibling->color==black && sibling->left->color==black && sibling->right->color==black && curr->color==black){//d2
                sibling->color=red;
                curr->right->color=black;

                 curr->color=doubleBlack;  
                 return curr;   
            }

            if(sibling->color==red){//d3
                
                curr=curr->rightRotate();
                curr->color=black;
                curr->right->color=red;
                //curr->right->right->color=doubleBlack;
                curr->right=eraseFixup(curr->right);
                return curr;//maybe Needs more work
            }
            if(sibling->color==black && sibling->left->color==black && sibling->right->color==black && curr->color==red){//d4
                
                sibling->color=red;
                curr->right->color=black;
                  
                curr->color=black;
                return curr;
            }
            if(sibling->color==black && sibling->right->color==red && sibling->left->color==black){//d5
                curr->left=sibling->leftRotate();
                curr->left->color=black;
                curr->left->left->color=red;
                sibling=curr->left;
            }
            if(sibling->color==black && sibling->left->color==red){//d6
                curr=curr->rightRotate();
                curr->right->right->color=black;
                curr->color=curr->right->color;
                curr->left->color=black;
                curr->right->color=black;
            }
    }
        return curr;

}

Node<T>* getLeftMost(Node<T>*curr){
    if(!curr->left->key)
        return curr;
    
    return getLeftMost(curr->left);
}
pair<Family<T>,Node<T>*> insertHelper(Node<T>*curr,T toInsert){
    if(!curr->key){
        curr->key={toInsert};
        curr->color=red;
        curr->left=new Node<T>();
        curr->right=new Node<T>();
        Family<T> f;
        f.him=curr;
        f.status=on;
        return {f,curr};
    }else{
        Family<T> fam;
        
        T key=curr->key.value();
        if(key==toInsert){
            Family<T>f;
            f.status=gotovo;
            --size;
            return {f,curr};
        }
        if(toInsert<key){
            auto tmp=insertHelper(curr->left,toInsert);
            fam=tmp.first;
            curr->left=tmp.second;
        }else{
            auto tmp=insertHelper(curr->right,toInsert);
            fam=tmp.first;
            curr->right=tmp.second;
        }

        if(fam.status==gotovo)
            return {fam,curr};
        else if(fam.status==otac){//dete je otac
            fam.status=deda;
            fam.grandpa=curr;
            
            Family newFam=fam.fixUp();
            return {newFam,fam.grandpa};
            
        }else if(fam.status==on){//dete je on
            fam.status=otac;
            fam.father=curr;
            if(curr->color==black){
                Family<T> gfam;
                gfam.status=gotovo;
                return {gfam,curr};
            }
            return {fam,curr};
        }
    }
}
    Node<T>*root;
    int size;
};

/*
bool test_erase(const vector<int>&vektor,int toDelete){
    RBTree<int>skup;

    for(int i=0;i<vektor.size();i++)
        skup.insert(vektor[i]);
    skup.exportToFile("before");
    skup.erase(toDelete);
    skup.exportToFile("after");
    return skup.isRBTree();


}
void multiple_erase_test(int dim){
    vector<int>vektor;
    vector<bool>tests;
    for(int i=0;i<dim;i++){
        vektor.push_back(i+1);
    }
    for(int i=0;i<dim;i++) {
        auto rng=default_random_engine {};
        shuffle(vektor.begin(),vektor.end(),rng);
        tests.push_back(test_erase(vektor, i + 1));
        if(!tests.back()){
            cout<<"VEKTOR INSERTION ORDER"  <<endl;
            for(int j=0;j<vektor.size();j++){
                cout<<vektor[j]<<"  ";
            }
            cout<<endl;
            cerr<<"toDelete: "<<i+1<<endl;
            break;
        }
    }
       cout<<"ALL DELETION TESTS PASSED=="<<accumulate(tests.begin(),tests.end(),true,[](bool x,bool y){return x && y;})<<endl;
}
void test_insert(const vector<int>&vektor){
    RBTree<int>skup;
    int n=vektor.size();
    for(int i=0;i<n;i++){
        skup.insert(vektor[i]);
    }
}
void test(){
    vector<int>vektor;
    for(int i=0;i<10000000;i++){
        vektor.push_back(i);
    }
    auto rng=default_random_engine {};
    shuffle(vektor.begin(),vektor.end(),rng);
    test_insert(vektor);
}

int main()
{
    RBTree<int>myTree;
    myTree.insert(1);
    myTree.insert(2);
    myTree.insert(3);
    myTree.exportToFile("123");
    return 0;
}
*/