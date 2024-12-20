// vector and list algorithms with objects in containers
// Mikhail Nesterenko
// 9/10/2018

#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <map>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; using std::map;


class Student{
public:
    Student(string firstName, string lastName):
    firstName_(firstName), lastName_(lastName) {}
 
   // move constructor, not really needed, generated automatically
   Student(Student && org) noexcept:
      firstName_(std::move(org.firstName_)),
      lastName_(std::move(org.lastName_))
   {}
  
   // force generation of default copy constructor
    Student(const Student & org) = default;
    string print() const {return firstName_ + ' ' + lastName_;}

    // needed for unique() and for remove()
    friend bool operator== (Student left, Student right){
        return left.lastName_ == right.lastName_ && left.firstName_ == right.firstName_;
   }

   // needed for sort()
    friend bool operator< (Student left, Student right){
        return left.lastName_ < right.lastName_ || (left.lastName_ == right.lastName_ && left.firstName_ < right.firstName_);
    }
  
private:
    string firstName_;
    string lastName_;
};




// reading a list from a fileName
void readRoster(map<Student, list<string>>& roster, string fileName);
// printing a list out
void printRoster(map<Student, list<string>>& roster);

int main(int argc, char* argv[]){
    if (argc <= 1){
        cout << "usage: " << argv[0] << " list of courses, dropouts last" << endl;
        exit(1);
    }
    map<Student, list<string>> roster;
    for(int i=1; i < argc-1; ++i){
        readRoster(roster, argv[i]);
        cout << "\n\n" << argv[i] << "\n";
        printRoster(roster);
   }
    map<Student, list<string>> dropouts;
    readRoster(dropouts, argv[argc-1]);
    cout << "\n\n dropouts \n"; printRoster(dropouts);
    for(const auto& element : dropouts){
        roster.erase(roster.find(element.first));

    }
    cout << "\n\n dropouts removed from roster\n";
    printRoster(roster);


}


void readRoster(map<Student, list<string>>& roster, string fileName){
    ifstream course(fileName);
    string first, last;
    auto slash = fileName.rfind('/') + 1;
    string courseName = fileName.substr(slash, fileName.size() - slash - 4);
    while(course >> first >> last){
        Student one(first, last);
        roster[one].push_back(courseName);
    }
    course.close();
}

// printing a list out
void printRoster( map<Student, list<string>>& roster){
    for(const auto& element : roster) {
        cout << element.first.print() << " ";
        for(auto it : element.second){
            cout << it + ' ';
        }
        cout << endl;
    }
}
