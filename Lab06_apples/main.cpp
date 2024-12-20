// sorting apples
// non STL-algorithm code to be replaced by algorthms
// Mikhail Nesterenko
// 09/26/2022


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>

using std::cin; using std::cout; using std::endl;
using std::string; using std::transform;
using std::vector; using std::deque;

struct Apples{
   double weight; // oz
   string color;  // red or green
   void print() const { cout << color << ", " <<  weight << endl; }
};



int main(){
    srand(time(nullptr));
    const double minWeight = 3.;
    const double maxWeight = 8.;
    
    cout << "Input crate size: ";
    int size;
    cin >> size;
    
    vector <Apples> crate(size);
    
    // assign random weight and color to apples in the crate
    // replace with generate()
    std::generate(crate.begin(), crate.end(), [&](){
        Apples apple_;
        apple_.weight = minWeight + static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
        apple_.color = rand() % 2 == 1 ? "green" : "red";
        return apple_;
    });
    
    // for_each() possibly
    cout << "all apples"<< endl;
    for_each(crate.begin(), crate.end(), [](Apples a) {a.print();});
    /*
     for(const auto &e: crate) {
     e.print();
     }
     */
    
    cout << "Enter weight to find: ";
    double toFind;
    cin >> toFind;
    
    // count_if()
    /*
     int cnt = 0;
     for(auto it = crate.cbegin(); it != crate.cend(); ++it)
     if(it->weight > toFind) ++cnt;
     */
    int cnt = count_if(crate.begin(), crate.end(), [&toFind](Apples a_) {return toFind < a_.weight;});
    cout << "There are " << cnt << " apples heavier than " << toFind << " oz" << endl;
    // find_if()
    
    cout << "at positions ";
    auto position = crate.begin();
    while(position != crate.end()){
        position = find_if(position, crate.end(), [&toFind](Apples a_){return toFind < a_.weight;});
        cout << std::distance(crate.begin(), position) << ", ";
        ++position;
    }
    /*
     for(int i=0; i < size; ++i)
     if(crate[i].weight > toFind)
     cout << i << ", ";
     cout << endl;
     */
    
    // max_element()
    double heaviest = crate[0].weight;
    /*
     for(int i=1; i < size; ++i)
     if(crate[i].weight > heaviest) heaviest = crate[i].weight;
     */
    auto it = max_element(crate.begin(), crate.end(), [](Apples a_, Apples b_){return a_.weight < b_.weight;});
    cout << "Heaviest apple weighs: " << it->weight << " oz" << endl;
    
    
    // for_each() or accumulate()
    double sum = std::accumulate(crate.begin(), crate.end(), 0.0, [](double sum, Apples a_) {return sum + a_.weight;});
    /*
     double sum = 0;
     for(int i=0; i < size; ++i)
     sum += crate[i].weight;
     */
    cout << "Total apple weight is: " << sum << " oz" << endl;
    
    
    // transform();
    cout << "How much should they grow: ";
    double toGrow;
    cin >> toGrow;
    transform(crate.begin(), crate.end(), crate.begin(), [&toGrow](Apples& a_){a_.weight += toGrow; return a_;});
    /*
     for(int i=0; i < crate.size(); ++i){
     //crate[i].weight += toGrow;
     cout << crate[i].weight;
     }
     */
    // remove_if()
    cout << "Input minimum acceptable weight: ";
    double minAccept;
    cin >> minAccept;
    
    
    // removing small apples
    // nested loops, replace with a single loop modification idiom
    auto toRemove = std::remove_if(crate.begin(), crate.end(), [&minAccept](Apples a_){return a_.weight < minAccept;});
    crate.erase(toRemove, crate.end());
    cout << "removed " << size - crate.size() << " elements" << endl;
    
    
    
    // bubble sort, replace with sort()
    std::sort(crate.begin(), crate.end(), [](Apples a_, Apples b_){return a_.weight < b_.weight;});
    std::for_each(crate.begin(), crate.end(), [](Apples a_){cout << a_.color << ", " << a_.weight << endl;});
}

