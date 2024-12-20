// testing the implementation of class WordList
// Mikhail Nesterenko
// 9/04/2015

#include <iostream>
#include <string>
#include "WordList.hpp" // class definitions
#include <fstream>


using std::cout; using std::endl;

void testfunc(WordList); // function to test pass-by-value for WordList class


int main(int argc, char* argv[]){
    if(argc < 1){
        cout << "Pass an argument to the program: ";
        return 0;
    }
    //"/Users/biancaamoako/Desktop/CS3/Lab2_wordCount/Lab2_wordCount/words.txt"
    WordList list1;
    ifstream file1(argv[1]);
    string word;
    string newword_ = "";
    while(file1 >> word){
        for (int i = 0; i < word.size(); ++i) {
            if(isalnum(word[i])){
                newword_ += word[i];
            }
            else if(newword_.size() > 0){
                list1.addWord(newword_);
                newword_ = "";
            }
        }
        if(newword_.size() > 0){
            list1.addWord(newword_);
            newword_ = "";
        }
    }
    list1.print();
}

// tests pass-by-value for object of class varArray
void testfunc(WordList myList){ // copy constructor is invoked on "myList"
    cout << "word list in testfunc: " << endl;
    myList.print();
} // destructor is invoked when "myList" goes out of scope
