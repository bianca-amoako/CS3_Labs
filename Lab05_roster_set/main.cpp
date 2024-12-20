// vector and list algorithms with objects in containers
// Mikhail Nesterenko
// 9/10/2018

#include <fstream>
#include <iostream>
#include <set>
#include <list>
#include <string>
#include <cstdlib>


using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; using std::set;


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
      return left.lastName_ == right.lastName_ &&
         left.firstName_ == right.firstName_;
   }

   // needed for sort()
   friend bool operator< (Student left, Student right){
      return left.lastName_ < right.lastName_ ||
         (left.lastName_ == right.lastName_ &&
          left.firstName_ < right.firstName_);
   }
private:
   string firstName_;
   string lastName_;
};




// reading a list from a fileName
void readRoster(set<Student>& roster, string fileName);
// printing a list out
void printRoster(const set<Student>& roster);

int main(int argc, char* argv[]){

    if (argc <= 1){
        cout << "usage: " << argv[0] << " list of courses, dropouts last" << endl; exit(1);
    }
    set<Student> roster;

    for(int i=1; i < argc-1; ++i){
        readRoster(roster, argv[i]);
    }
    printRoster(roster);
    ///dropouts
    set<Student> dropouts;
    
    readRoster(dropouts, argv[argc-1]);
    for(const auto& students : dropouts){
        roster.erase(roster.find(students));
    }
    cout << endl << "Currently Enrolled Students " << endl;
    printRoster(roster);


}


void readRoster(set<Student>& roster, string fileName){
    ifstream course(fileName);
    string first, last;
    while(course >> first >> last)
        roster.insert(Student(first, last));
    course.close();
}

// printing a list out
void printRoster(const set<Student>& roster){
    for(const auto& element : roster)
        cout << element.print() << endl;
}
