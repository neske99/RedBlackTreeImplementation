#include <iostream>
#include <string>
#include <vector>
#include<queue>
#include<optional>
#include<fstream>
#include<algorithm>
#include<numeric>
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
            if(curr->color==red)
                return {true,desni.second};
            else if(curr->color==black)
                return {true,desni.second+1};
            return {false,0};
        }
        return {false,0};
    }
    bool isRBTree(){
        return isRBTree(this).first;
    }
    void leftRotate(){
        Node<T>*z=this;
        Node<T>*y=z->right;
        Node<T>*x=y->right;
        z->right=y->left;
        y->left=z;
        this=y;
    }
    void rightRotate(){
        Node<T>*z=this;
        Node<T>*y=z->left;
        Node<T>*x=y->left;
        z->left=y->right;
        y->right=z;
        this=y;

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
    BRTree(Node<T>*root){
        this->root=root;
    }
    BRTree(const vector<T>&vec){
        this->root=new Node<T>();
        int n=vec.size();
        for(int i =0;i<n;i++)
            this->insert(vec[i]);
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
    bool operator==(const BRTree&other){
        return (*root)==(*other.root);
    }
    void erase(T toErase){
        if(find(toErase)) root=eraseHelper(root,toErase);
        if(root->color==doubleBlack && !root->right->key && root->left->key){
            root->left->color=red;
        }
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

    bool isRBTree(){
        return root->isRBTree();
    }
private:
Node<T>* eraseHelper(Node<T>* curr,T toErase){
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
            Node<T>*levi=curr->left;
            auto tmp=takeLeftmost(curr->right);            
            Node<T>*leftMost=tmp.first;
            Node<T>*new_right=tmp.second;
            
            leftMost->left=levi;
            leftMost->right=new_right;

            if(curr->color==red && leftMost->color==red)          leftMost->color=red;
            else if(curr->color==black && leftMost->color==black){
                if(curr->left->color==black){
                    curr->left->color=red;
                    leftMost->color=doubleBlack;
                }else{
                    Node<T>*sibling=leftMost->left;


                    leftMost->left=sibling->right;
                    sibling->right=leftMost;

                    leftMost=sibling;
                    leftMost->color=black;
                
                    leftMost->right->right->color=black;
                    leftMost->right->left->color=red;
                }
                
            }else                                                  leftMost->color=black;

            delete curr;
            return leftMost;
            return curr;
            
        }

       
    }

    if(toErase<curr->key.value()){ 
        curr->left= eraseHelper(curr->left,toErase);
        if(curr->left->color==doubleBlack){
            Node<T>*sibling=curr->right;
            if(sibling->color==black && sibling->right->color==red){
                curr->right=sibling->left;
                sibling->left=curr;

                curr=sibling;

                curr->left->left->color=black;
                if(curr->left->color==black){
                    curr->right->color=black;
                }else{//curr->left->color==red
                    curr->left->color=black;
                    curr->right->color=black;
                    curr->color=red;
                }
                
            }else if(sibling->color==black && sibling->left->color==red){
                Node<T>*r=sibling->left;
                curr->right=r->left;
                r->left=curr;

                sibling->left=r->right;
                r->right=sibling;

                curr=r;
                curr->color=black;
                curr->left->left->color=black;
            }else if(sibling->color==red){
                curr->right=sibling->left;
                sibling->left=curr;
                curr=sibling;
                curr->color=black;
                curr->left->left->color=black;
                curr->left->right->color=red;
            }else if(sibling->color==black && sibling->left->color==black && sibling->right->color==black){
                sibling->color=red;
                curr->left->color=black;
                if(curr->color==black) curr->color=doubleBlack;
                else                   curr->color=black;
            }
        }
        return curr;
    }
    else{
        curr->right=eraseHelper(curr->right,toErase);
        if(curr->right->color==doubleBlack){
            Node<T>*sibling=curr->left;
            if(sibling->color==black && sibling->left->color==red){
                curr->left=sibling->right;
                sibling->right=curr;

                curr=sibling;

                curr->right->right->color=black;
                if(curr->right->color==black){
                    curr->left->color=black;
                }else{//curr->right->color==red
                    curr->right->color=black;
                    curr->left->color=black;
                    curr->color=red;
                }
            }else if(sibling->color==black && sibling->right->color==red){
                 Node<T>*r=sibling->right;
                curr->left=r->right;
                r->right=curr;

                sibling->right=r->left;
                r->left=sibling;

                curr=r;
                curr->color=black;
                curr->right->right->color=black;
            }else if(sibling->color==red){
                curr->left=sibling->right;
                sibling->right=curr;
                curr=sibling;
                curr->color=black;
                
                curr->right->right->color=black;
                curr->right->left->color=red;
            }else if(sibling->color==black && sibling->left->color==black && sibling->right->color==black){
                sibling->color=red;
                curr->right->color=black;

                if(curr->color==black) curr->color=doubleBlack;
                else                    curr->color=black;
            }
        }
        return curr;
    }
}

pair<Node<T>*,Node<T>*>takeLeftmost(Node<T>*curr){
    if(!curr->left->key){
        delete curr->left;
        //delete curr->right;
        return {curr,curr->right};
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

void test_insert(int dim=100){
    vector<int>vektor={};
    for(int i=0;i<dim;i++)
        vektor.push_back(i+1);
    BRTree<int> tmp=BRTree<int>();
    vector<bool>amRBTree;

    for(int i=0;i<vektor.size();i++){
        tmp.insert(vektor[i]);
        amRBTree.push_back(tmp.isRBTree());
        cerr<<"test "<< vektor[i]<<":"<<amRBTree.back()<<endl;
        //tmp.exportToFile("graph"+to_string(i));
    }
    cerr<<"insert_test passed: "<<accumulate(amRBTree.begin(),amRBTree.end(),true,[](bool x,bool y){return x && y;})<<endl;
    
    
    
    return;
    //BRTree<int> skup=BRTree<int>(vektor);
    //skup.erase(toDelete);
    //cerr<<"Delete try " <<toDelete<<" :"<<skup.isRBTree()<<endl;
    //skup.exportToFile("graph"+to_string(toDelete));    
}
void test_erase(int dim,int toDelete){
    BRTree<int>skup;

    for(int i=0;i<dim;i++)
        skup.insert(i+1);
    skup.exportToFile("graph_before_delete");
    skup.erase(toDelete);
    skup.exportToFile("skup_after_delete"+to_string(toDelete));
    cerr<<"to delete : "<<toDelete<< " " << skup.isRBTree()<<endl;



}
int main()
{   
    for(int i=0;i<32;i++)
        test_erase(16,i+1);
    //for(int i=0;i<15;i++)
    //   test(i+1);

   return 0;
}
