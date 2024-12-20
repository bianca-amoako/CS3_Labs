//
//  Lab1_Diff
//  Created by Bianca Amoako on 8/23/23


// C++ program to illustrate command line arguments
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
 
int main(int argc, char* argv[]){  ///# of arguments, pointer to array of arguments
    if(argc < 2){
        cout << "Pass an argument to the program: ";
        return 0;
    }
    
    ifstream file1(argv[1]);
    vector<string> vctr1;
    string text1;
    if(file1.is_open()){
        while(!file1.eof()){
            getline(file1, text1);
            vctr1.push_back(text1);
        }
    }
    for(auto i = vctr1.begin(); i != vctr1.end(); ++i){
        cout << *i << endl;
    }
    ifstream file2(argv[2]);
    vector<string> vctr2;
    string text2;
    if(file2.is_open()){
        while(!file2.eof()){
            getline(file2, text2);
            vctr2.push_back(text2);
        }
    }
    for(auto i = vctr2.begin(); i != vctr2.end(); ++i){
        cout << *i << endl;
    }
    vector<string> shorter;
    vector<string> longer;
    
    
    cout << endl << vctr1[0].size() << endl;
    
    if(vctr1.size() >= vctr2.size()){
        shorter = vctr2;
        longer = vctr1;
    }
    else {
        shorter = vctr1;
        longer = vctr2;
    }
        for(auto i = 0; i < longer.size(); ++i){
            // our criteria for there being a difference between the two lines is: (has one of the files run out and is there a non-blank line in the other file) or (are the lines that exist not equal)
            if((i >= shorter.size() && longer[i].size() > 0) || vctr2[i] != vctr1[i]){  // is there a difference between these two files at line number i?
                
                // if i gte shorter.size() {if file1.txt is longer print out a line from it else blank; if file2.txt is longer print out a line from it else blank; print the caret out at the beginning of the line. else, do the other stuff below}
                if(i >= shorter.size()){
                    if(vctr1.size() == longer.size()){
                        vctr2.push_back("");
                    }
                    else{
                        vctr1.push_back("");
                    }
                }
                
                cout << "file1.txt: " << i+1 << ": " << vctr1[i] << endl;
                cout << "file2.txt: " << i+1 << ": " << vctr2[i] << endl;
                
                int numSpace = 0;
                if(i < 9)
                    numSpace = 1;
                else if(i < 100)
                    numSpace = 2;
                string spaceString(13 + numSpace, ' ');
                
                size_t shortLine;
                if(vctr2[i].size() < vctr1[i].size())
                    shortLine = vctr2[i].size();
                else
                    shortLine = vctr1[i].size();
                
                bool diffFound = false;
                for(auto j = 0; j < shortLine; ++j){
                    if(vctr2[i][j] != vctr1[i][j]){
                        spaceString += '^';
                        diffFound = true;
                        break;
                    }
                    else
                        spaceString += ' ';
                }
                if(diffFound == false){
                    spaceString += '^';
                }
                cout << spaceString << endl;
            }
            
            cout << endl;
        }
}



