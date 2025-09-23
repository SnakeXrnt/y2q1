#include <iostream>
#include <vector>
#include "utils.h"

int main() {
    // TODO: write a program that receives two arguments over stdin, separated by spaces:
    // a vector of integers, followed by an integer to search for
    //
    // The program MUST use binary search to find the first occurrence of the integer
    // and return its index. If the integer is not found, it should return the index
    // at which the value should be inserted to maintain sorted order.

    // See the utils.h file for writing / reading vectors
    std::vector<int> vec;
    int value;

    std::cout << "gimme the vector and the integer seperated with space ~~ " << std::endl;
    std::cin >> vec >> value;

    int left = 0;
    int right = vec.size();

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (vec[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    
    
    std::cout << left << std::endl;

}
