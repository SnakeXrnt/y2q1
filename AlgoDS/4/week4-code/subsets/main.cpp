#include <iostream>
#include <string>
#include "bintree.h" // for binary_tree_node
#include "node_ptr.h"

int main() {
    /* TODO:
        Write a program that reads two binary search trees from standard input, which represent sets of
        integers (no duplicates within each tree), and checks if the first one is a subset of the second one.

        The program must output "true" if the first tree is a subset of the second one, and "false" otherwise.

        The time complexity of your program must be O(n + m), where n and m are the sizes of the two trees.
    */

    sax::binary_tree_node<int> * one = nullptr;
    sax::binary_tree_node<int> * two = nullptr;

    std::cin >> one >> two;

    sax::node_ptr<int> it(one);

    return 0;
}
