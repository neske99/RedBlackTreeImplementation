#include "../lib/RBTree.cpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>
using namespace std;

int main() {
  vector<int> nums;
  const int numNumbers = 5000000;

  for (int i = 0; i < numNumbers; i++) {
    nums.push_back(i);
  }

  set<int> stdSet;
  RBTree<int>mySet;
  std::shuffle(nums.begin(), nums.end(), mt19937());
//insertion
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < numNumbers; i++) {
    stdSet.insert(nums[i]);
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Elapsed time insertion stdSet: " << elapsed.count() << " ms\n";

  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < numNumbers; i++) {
    mySet.insert(nums[i]);
  }

  end = std::chrono::high_resolution_clock::now();
  elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Elapsed time insertion mySet: " << elapsed.count() << " ms\n";

  std::shuffle(nums.begin(), nums.end(), mt19937());
//deletion
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < numNumbers; i++) {
    stdSet.erase(nums[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Elapsed time deletion stdSet: " << elapsed.count() << " ms\n";

  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < numNumbers; i++) {
    mySet.erase(nums[i]);
  }

  end = std::chrono::high_resolution_clock::now();
  elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Elapsed time deletion mySet: " << elapsed.count() << " ms\n";
  return 0;
}
