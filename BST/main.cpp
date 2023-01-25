#include "BST.h"

int main() {

    int size; // number of emelements to sort
    int max; // Maximum value that the elements of the array can have
    
    //Ramdom number generator
    std::cout << "Please Enter the size of the Array: ";
    std::cin >> size;
    
    std::cout << "Maximum value that the elements of the array can have: ";
    std::cin >> max;

    int nums[size]; //Declaring an array thaat hold 10 elements of the int data type
    
    //give the array some ramdom initial values
    for (int t=0; t<size; t++) {
        
        nums[t] = rand();
        
        //Do not allow any random value to be higher than max
        while (nums[t] > max) {
            
            nums[t] = rand();
        }
        std::cout << "Index " << t << " has been assigned a value\n.";
    }
    
    cout << '\n';
    
    //display the ramdom numbers values assigned to each element of the array (original array)
    cout << "Original array is:\n  ";
    for (int t=0; t<size; t++) cout << "nums[" << t << "]=" << nums[t] << " | ";
    cout << '\n';
    cout << '\n';

    // Create Binary Search Tree
    BST<int> tree;

    // Add elements to the tree
    for (auto i : nums) {
        tree.Insert(i);
    }

    int *sortedArr;

    sortedArr = tree.ConvertToArray();

    // Print Sorted Array
    for(int i = 0; i < tree.getNumOfElements(); i++) {
        
        std::cout << *(sortedArr+i) << " ";
    }
    std::cout << '\n';

    tree.Dump();

    return 0;
}
