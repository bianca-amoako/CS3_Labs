// vector and list algorithms with objects in containers
// Mikhail Nesterenko
// 9/10/2018

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;


class Student{
public:
    Student(string firstName, string lastName):
    firstName_(firstName), lastName_(lastName) {}
 
   // move constructor, not really needed, generated automatically
    Student(Student && org) noexcept:
    firstName_(std::move(org.firstName_)),
    lastName_(std::move(org.lastName_)),
    courses_(std::move(org.courses_))
    {}
  
   // force generation of default copy constructor
    Student(const Student& org) = default;
    
    string print() const {
        string courses = "";
        for(auto it : courses_){
            courses += it + ' ';
        }
        return firstName_ + ' ' + lastName_ + ": " + courses;
    }

    void add_course(string name){
        courses_.push_back(name);
    }
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
    list<string> courses_;
};




// reading a list from a fileName
void readRoster(list<Student>& roster, string fileName);

// printing a list out
void printRoster(const list<Student>& roster);

int main(int argc, char* argv[]){

   if (argc <= 1){ cout << "usage: " << argv[0]
      << " list of courses, dropouts last" << endl; exit(1);}

   // vector of courses of students
   list<Student> courseStudents;

   for(int i=1; i < argc-1; ++i){
      readRoster(courseStudents, argv[i]);
   }


   // reading in dropouts
   list<Student> dropouts;
   readRoster(dropouts, argv[argc-1]);
   
   for (const auto& str : dropouts)  // removing individual dropouts
      courseStudents.remove(str);
    
    
    courseStudents.sort();
    cout << "\n\n all students, dropouts removed \n"; printRoster(courseStudents);
 
}


void readRoster(list<Student>& roster, string fileName){
    ifstream course(fileName);
    string first, last;
    auto slash = fileName.rfind('/') + 1;
    string courseName = fileName.substr(slash, fileName.size() - slash - 4);
    while(course >> first >> last){
        Student one(first, last);
        bool found = false;
        for(auto it = roster.begin(); it != roster.end(); ++it){
            if(*it == one){
                found = true;
                it -> add_course(courseName);
                break;
            }
        }
        if(!found){
            one.add_course(courseName);
            roster.push_back(one);
        }
    }
   course.close();
}

// printing a list out
void printRoster(const list<Student>& roster){
   for(const auto& student : roster)
      cout << student.print() << endl;
}
