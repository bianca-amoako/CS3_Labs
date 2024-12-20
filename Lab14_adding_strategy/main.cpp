// using sorting algorihtms to demonstrate the Strategy Design Pattern
// Mikhail Nesterenko
// 4/27/2022

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

using std::cout; using std::endl;
using std::vector;

typedef vector<int>::iterator It;  // shorthand for a vector iterator

// abstract strategy
// strategy interface, push method
class Sorter{
public:
   virtual void sort(It, It) const = 0;
};

// context using the strategies
class OrderedCollection{
public:
   void setSorter(Sorter *sorter) {sorter_ = sorter;}
   void setCollection(const vector<int> &v){v_ = v;}
   const vector<int>& getCollection() const{return v_;}
   void sort() {sorter_->sort(v_.begin(), v_.end());}
private:
   vector<int> v_;
   Sorter *sorter_;
};

// concrete strategy using insertion sort
class InsertionSorter: public Sorter{
public:
    void sort(It begin, It end) const override {
        for(It one = next(begin, 1); one != end; ++one ){
            for(It subNum = begin; subNum != one; ++subNum){
                if(*one < *subNum){
                    It movedOne = one;
                    while(movedOne != subNum && movedOne-1 >= begin){
                        std::swap(*movedOne, *(movedOne-1));
                        --movedOne;
                    }
                    break;
                }
            }
        }
    }
};

// concrete strategy using bubble sort
class BubbleSorter: public Sorter {
public:
   void sort(It begin, It end) const override {
      bool swapped;
      do{
     swapped = false;
     for(It one=begin, two = next(one, 1); two != end; ++one, ++two)
        if(*one > *two){
           std::swap(*one, *two);
           swapped=true;
        }
      }while(swapped);
   }
};


// concrete strategy using STL sorting alg
class StdSorter: public Sorter{
public:
   void sort(It begin, It end) const override {
      std::sort(begin, end);
   }
};



int main() {
   vector<int> v={10555,9,55,2,103,7}, vc=v;

   OrderedCollection sortedVector;
   StdSorter ss;
   BubbleSorter bs;
    InsertionSorter is;

   //
   // sorting short vectors to demo correctness
   //

   // STL sorter
   sortedVector.setSorter(&ss);
   sortedVector.setCollection(v);
   for(auto e: v) cout << e << " "; cout << endl;

   sortedVector.sort();
   v = sortedVector.getCollection();
   cout << "STL sorted" << endl;
   for(auto e: v) cout << e << " "; cout << endl << endl;

   
   // BubbleSort sorter
   sortedVector.setSorter(&bs);
   sortedVector.setCollection(vc);
   for(auto e: vc) cout << e << " "; cout << endl;

   sortedVector.sort();
   vc = sortedVector.getCollection();
   cout << "bubble sorted" << endl;
   for(auto e: vc) cout << e << " "; cout << endl << endl;

   
   
   //
   // sorting large vectors to demo performance
   //
   v.clear();
   srand(time(nullptr));

   std::generate_n(back_inserter(v), 50000, []{return rand()%100;});
   vc=v;

   std::chrono::time_point<std::chrono::high_resolution_clock>
      startTime, endTime; // chrono time points
   std::chrono::duration<double> duration;
                          // chrono time interval
    std::ofstream myfile;
    myfile.open ("/Users/biancaamoako/Desktop/CS3/Lab14_addingStrategy/Lab14_addingStrategy/timing.txt");
    
    
   // STL sorter
   sortedVector.setSorter(&ss);
   sortedVector.setCollection(v);

   startTime = std::chrono::high_resolution_clock::now();
   sortedVector.sort();
   endTime = std::chrono::high_resolution_clock::now();
   duration = endTime - startTime;
   myfile << "standard sort ran for " << duration.count() << " seconds" << endl;
   v = sortedVector.getCollection();
   
   // bubble sort
   sortedVector.setSorter(&bs);
   sortedVector.setCollection(vc);

   startTime = std::chrono::high_resolution_clock::now();
   sortedVector.sort();
   endTime = std::chrono::high_resolution_clock::now();
   duration = endTime - startTime;
   vc = sortedVector.getCollection();

   myfile << "bubble sort ran for " << duration.count() << " seconds" << endl;
    
    // insertion sort
    sortedVector.setSorter(&is);
    sortedVector.setCollection(vc);
    
    startTime = std::chrono::high_resolution_clock::now();
    sortedVector.sort();
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    vc = sortedVector.getCollection();
    
    myfile << "insertion sort ran for " << duration.count() << " seconds" << endl;
    myfile.close();
   
   // confirming sorted vectors are the same
   if(v == vc)
      cout << "sorted vectors are the same" << endl;
   else
      cout << "sorted vectors are different" << endl;
   
}
