#include <iostream>
#include <string>
#include <vector>
#include<queue>
#include<optional>
#include<fstream>
using namespace std;

enum Color{red,black};
enum Status{on=0,otac=1,deda=2,gotovo=3};

template<typename T>
class Node{
public:
    Node<T>*left;
    Node<T>*right;
    optional<T>key;
    int balance;
    Color color;
    
    Node(T key){
        this->left=nullptr;
        this->right=nullptr;
        this->key={key};
        this->balance=0;
        this->color=red;
        
    }
    
    Node()
    {
        this->left=nullptr;
        this->right=nullptr;
        this->key={};
        this->balance=0;
        this->color=black;
        
    }

    
    bool find(T toFind){
        if(!this->key) return false;
        T curr=key.value();
    
        if(toFind==curr)       return true;
        else if(toFind <curr ) return left->find(toFind);
        else                   return right->find(toFind);
    
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
            }else if(grandpa->right==father && father->left==him){
                grandpa->right=him;
                father->left =him->right;
                him->right= father;
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
class BRTree{
public:
    BRTree(){
        root=new Node<T>();
        root->color=black;
    }
    bool find(T toFind){
        return root->find(toFind);
    }
    void insert(T toInsert){
        if(!root->find(toInsert)){
            root=insertHelper(root,toInsert).second;
            root->color=black;
        }    

    }
    void erase(T toErase){
        if(find(toErase)) root=eraseHelper(root,toErase);
        cerr<<"erase not implemented yet"<<endl;
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
                if(curr->color==red) toPrint+="R";
                else                 toPrint+="B";

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
        ofstream f;
        int i=0;
        f.open(filename+".dot");
        f << "digraph{\n"; 
        
        exportNode(f,root,i);



        f<< "}\n";
        f.close();
    }
    void exportNode(ostream& f,Node<T>*curr,int index){
        if(!curr->key){
            f << index<<"[label=\""<<"NULL"<<"\","<<"color="<< "black" << ",style=filled,fontcolor=white];\n";
        //f<<index<<" -> "<<leftIndex<<";\n";
        //f<<index<<" -> "<<rightIndex<<";\n";
            return;
        }
        string nodeName="";
        string lName="";
        string rName="";
        if(curr->color==black) nodeName+="B";
        else                   nodeName+="R";
        if(curr->key) nodeName+=to_string(curr->key.value());
        else          nodeName+="NULL";

        if(curr->left->color==black) lName+="B";
        else                   lName+="R";
        if(curr->left->key) lName+=to_string(curr->left->key.value());
        else          lName+="NULL";

        if(curr->right->color==black) rName+="B";
        else                   rName+="R";
        if(curr->right->key) rName+=to_string(curr->right->key.value());
        else          rName+="NULL";

        int leftIndex=2*index+1;
        int rightIndex=2*index+2;
        
         
        string col="";
        if(curr->color==black) col="black";
        else                    col="red";

        f << index<<"[label=\""+nodeName+"\","<<"color="<<col << ",style=filled,fontcolor=white];\n";
        f<<index<<" -> "<<leftIndex<<";\n";
        f<<index<<" -> "<<rightIndex<<";\n";

        //if(curr->left->key){
            exportNode(f,curr->left,leftIndex);
        //}
        //if(curr->right->key){
            exportNode(f,curr->right,rightIndex);
        //}
    }
void test(){
    auto tmp=takeLeftmost(root->right);
    cerr<<tmp.first->key.value()<<endl;
}
private:
Node<T>* eraseHelper(Node<T>* curr,T toErase){
    if(curr->key.value()==toErase){
        if(!curr->left->key && !curr->right->key){//has no children
            delete curr->left;
            delete curr->right;
            delete curr;
            return new Node<T>();
        }else if(!curr->left->key){//has only right child
            auto left=curr->left;
            auto right=curr->right;
            delete left;
            delete curr;
            return right;
        }else if(!curr->right->key){//has only left child
            auto left=curr->left;
            auto right=curr->right;
            delete right;
            delete curr;
            return left;

        }else{// has both children
            Node<T>*levi=curr->left;
            auto tmp=takeLeftmost(curr->right);            
            Node<T>*leftMost=tmp.first;
            cerr<<tmp.first->key.value()<<endl;
            cerr<<levi->key.value()<<endl;
            leftMost->left=levi;
            //leftMost->right=tmp.second;
            
            return leftMost;
            return curr;
            
        }

        
    };//radi nesto

    if(toErase<curr->key.value()){ 
        curr->left= eraseHelper(curr->left,toErase);
        return curr;
    }
    else{
        curr->right=eraseHelper(curr->right,toErase);
        return curr;
    }
}

pair<Node<T>*,Node<T>*>takeLeftmost(Node<T>*curr){
    if(!curr->left->key){
        delete curr->left;
        //delete curr->right;
        return {curr,new Node<T>()};
    }
    pair<Node<T>*,Node<T>*> tmp=takeLeftmost(curr->left);
    curr->left=tmp.second;
    return {tmp.first,curr};



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
};



int main()
{
    BRTree<int> skup;
    skup.insert(1);
    skup.insert(2);
    skup.insert(3);
    skup.insert(4);
    skup.insert(5);
    skup.insert(6);

    skup.erase(5);
//    skup.test();
    skup.bfsPrint();
    skup.exportToFile();

   return 0;
}
