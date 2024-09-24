//
// Created by neske99 on 8.11.23..
//
#include<set>
#include<random>
#include<algorithm>
#include <bits/stdc++.h>
using namespace std;

void test_insert(const vector<int>&vektor){
    set<int>skup;
    int n=vektor.size();
    for(int i=0;i<n;i++){
        skup.insert(vektor[i]);
    }
}
void test(){
    vector<int>vektor;
    for(int i=0;i<100;i++){
        vektor.push_back(i);
    }
    auto rng=default_random_engine {};
    shuffle(vektor.begin(),vektor.end(),rng);
    test_insert(vektor);
}
int main(){

    time_t start, end;

    /* You can call it like this : start = time(NULL);
    in both the way start contain total time in seconds
    since the Epoch. */
    time(&start);

    // unsync the I/O of C and C++.
    //ios_base::sync_with_stdio(false);

    test();
    // Recording end time.
    time(&end);

    // Calculating total time taken by the program.
    double time_taken = double(end - start);
    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(5);
    cout << " sec " << endl;








    return 0;
}
