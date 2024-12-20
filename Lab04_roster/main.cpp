// vector and list algorithms
// Mikhail Nesterenko
// 3/11/2014

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; using std::size_t;
using std::sort;

// reading a list from a fileName
void readRoster(vector<list<string>>& roster, string fileName);

// printing a list out
void printRoster(const vector<list<string>>& roster);

int main(int argc, char* argv[]){
    if (argc <= 1){
        cout << "usage: " << argv[0]
        << " list of courses, dropouts last"
        << endl;
        exit(1);
    }
        
   // vector of courses of students
    vector<list<string>> courseStudents;

    for(int i=1; i < argc-1; ++i){
        readRoster(courseStudents, argv[i]);
   }
    //printRoster(courseStudents);
    
    //dropouts in 

  
   // reading in dropouts
    vector<list<string>> dropouts; //list of droupouts
    readRoster(dropouts, argv[argc-1]);
    cout << endl;
    //printRoster(dropouts);
    for(int i = 0; i < dropouts.size(); ++i){
        string dropout_name = dropouts[i].front();
        //cout << "dropouts[i].front() = " << dropout_name << endl;
        for(int i = 0; i < courseStudents.size(); ++i){
            string roster_name = courseStudents[i].front();
            if(dropout_name == roster_name){
                //cout << "erasing student from roster" << endl;
                courseStudents.erase(courseStudents.begin() + i);
            }
        }
    }
    cout << "dropouts removed \n NOT SORTED \n";
    printRoster(courseStudents);
    
    cout << "dropouts removed \n  SORTED \n";
    std::sort(courseStudents.begin(), courseStudents.end());
    printRoster(courseStudents);
   
}

// reading in a file of names into a list of strings
void readRoster(vector<list<string>>& roster, string fileName){
    ifstream course(fileName);
    
    string first, last;
    auto slash = fileName.rfind('/') + 1;
    string courseName = fileName.substr(slash, fileName.size() - slash - 4);
    //size_t found_first = first.find(first);
    while(course >> first >> last){
        string name = first + ' ' + last + ':';
        //loop through the vector
        bool found = false;
        for(int i = 0; i < roster.size(); ++i){
            if(roster[i].front() == name) {
                found = true;
                roster[i].push_back(courseName);
                break;
            }
            
        }
        if(!found){
            list<string> names;
            names.push_back(name);
            names.push_back(courseName);
            roster.push_back(names);
        }
    }
    course.close();
}

// printing a list out
void printRoster(const vector<list<string>>& roster){
    cout << "All Students, dropouts removed and sorted \nfirst name last name: courses enrolled" << endl;
    //semicolon
    for(const auto& lst : roster) {
        for(const auto& str : lst)
            cout << str << ' ';
        cout << endl;
    }
}
