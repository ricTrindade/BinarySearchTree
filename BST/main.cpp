#include "BST.h"

int *RamdomArrayGenerator(int size, int max) {
    
    static int array[100];
    
    //give the array some ramdom initial values
    for (int i=0; i<10; i++) {
        
        *(array+i) = rand();
        
        //Do not allow any random value to be higher than max
        while (*(array+i) > max) {
            
            *(array+i) = rand();
        }
        std::cout << "Index " << i << " has been assigned a value: " << *(array+i) << ".\n";
    }
    
    return array;
}


int main() {

    int size = 10;
    int max = 10;

    // Declaring an array thaat hold 10 elements of the int data type
    int *nums = RamdomArrayGenerator(size, max);
    
    cout << '\n';
    
    //display the ramdom numbers values assigned to each element of the array (original array)
    cout << "Original array is:\n  ";
    for (int t=0; t<size; t++) cout << "nums[" << t << "]=" << nums[t] << " | ";
    cout << '\n';
    cout << '\n';

    // Create Binary Search Tree
    BST<int> tree;

    // Add elements to the tree
    for (int i=0; i<size; i++) tree.Insert(nums[i]);

    // Sort Array based on the BST
    int *sortedArr;
    sortedArr = tree.ConvertToArray();

    // Print Sorted Array
    for(int i = 0; i < tree.getNumOfElements(); i++)
        std::cout << *(sortedArr+i) << " ";
    
    std::cout << '\n';

    // Print Tree
    tree.PrintBST();

    return 0;
}
