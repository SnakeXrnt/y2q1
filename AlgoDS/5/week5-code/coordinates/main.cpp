#include <iostream>

int main() {
    /* TODO:
        Write a program that inputs a string consisting of the characters "U", "D", "L", and "R",
        representing movements Up, Down, Left, and Right on a 2D grid, starting from the origin (0, 0).

        The program must then compute and print the top three of *the most visited unique coordinates*
        that are visited during the sequence of movements, including the starting point (0, 0).

        Ties should be broken by the order in which the coordinates were first visited.

        For example, given the input string "UUDLURD", the program must print `(0, 1), (0, 2), (0, 0)`,
        because the visited coordinates are (0, 0), (0, 1), (0, 2), (0, 1), (-1, 1), (-1, 2), (0, 2), and (0, 1).

        The time complexity of your solution must be O(n + 3 log(n)), which can be simplified to O(n + log(n))
        where n is the length of the input string.

        To efficiently store and check for unique coordinates, you should use a hash table (e.g.
        `std::unordered_set` or `std::unordered_map` in C++). Note that you will have to define a custom hash
        function for the coordinate pairs, as well as an equality operator. Also, you must copy the coordinates
        into a vector or similar structure to be able to sort them by their visit counts.

        As a final note, you are not allowed to use the nth_element algorithm from the standard library.
    */
    return 0;
}
