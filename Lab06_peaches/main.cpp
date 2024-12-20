// jamming peaches
// non STL-algorithm code to be replaced by algorthms
// Mikhail Nesterenko
// 9/30/2021


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <numeric>
#include <functional>

using std::cin; using std::cout; using std::endl;
using std::string; using namespace std::placeholders;
using std::vector; using std::deque;

struct Peaches{
   double weight; // oz
   bool ripe;  // ripe or not
   void print() const { cout << (ripe?"ripe":"green") << ", " <<  weight << endl; }
};



int main(){
    srand(time(nullptr));
    const double minWeight = 3.;
    const double maxWeight = 8.;

    cout << "Input basket size: ";
    int size;
    cin >> size;
    
    vector <Peaches> basket(size);

   // assign random weight and ripeness peaches in the basket
   // replace with generate()
    std::generate(basket.begin(), basket.end(), [&](){
        Peaches p_;
        p_.weight = minWeight + static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
        p_.ripe = rand() % 2;
        return p_;
    });
    /*
   for(auto it = basket.begin(); it != basket.end(); ++it){
      it->weight = minWeight +
               static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
      it->ripe = rand() % 2;
   }
     */
   
   // for_each() possibly
    cout << "all peaches"<< endl;
    for_each(basket.begin(), basket.end(), [](Peaches p_) {return p_.print();});
 
    
   // moving all the ripe peaches from basket to peck
   // remove_copy_if() with back_inserter()/front_inserter() or equivalents
    deque<Peaches> peck;
    std::remove_copy_if(basket.begin(), basket.end(), std::back_inserter(peck), [](Peaches p_){return !p_.ripe;});
//    auto position = basket.begin();
//    while(position != basket.end()){
//        std::find_if(position, basket.end(), [](Peaches p_){return !p_.ripe;});
//        basket.erase(position);
//        ++position;
//    }
    /*
   for(auto it=basket.begin(); it != basket.end();)
      if(it->ripe){
     peck.push_front(std::move(*it));
     it=basket.erase(it);
      }else
     ++it;
     */
   // for_each() possibly
    cout << "\npeaches remaining in the basket"<< endl;
    std::for_each(basket.begin(), basket.end(), [](Peaches p_){return p_.print();});

//    for(const auto &e: basket) {
//        e.print();
//    }
//    cout << endl;
//
//   // for_each() possibly
    cout << "\npeaches moved to the peck"<< endl;
    std::for_each(peck.begin(), peck.end(), [](Peaches p_){return p_.print();});
//    for(const auto &e: peck) {
//        e.print();
//    }
//
//
   // prints every "space" peach in the peck
    const int space=3;
    cout << "\nevery " << space << "\'d peach in the peck"<< endl;
//
   // replace with advance()/next()/distance()
    
   // no explicit iterator arithmetic
   auto it=peck.cbegin(); int i = 1;
   while(it != peck.cend()){
      if(i == space){
          it->print();
          i=0;
      }
       ++i;
       it = std::next(it);
   }


   // putting all small ripe peaches in a jam
   // use a binder to create a functor with configurable max weight
   // accumulate() or count_if() then remove_if()
    const double weightToJam = 10.0;
    double jamWeight = 0;
    auto isJam = std::bind(std::less<double>(), _1, weightToJam);
    count_if(peck.begin(), peck.end(), [&](Peaches p_){
        if(isJam(p_.weight)){
            jamWeight += p_.weight;
        }
        return isJam(p_.weight);
    });
    
    accumulate(peck.begin(), peck.end(), 0, [&] (double sum, Peaches &p) {
        return isJam(p.weight) ? sum + p.weight : sum;
    });
   cout << "Weight of jam is: " << jamWeight << endl;

}
