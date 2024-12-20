// needed for lab
// Mikhail Nesterenko
// 3/18/2022

#include <iostream>
#include <vector>
#include <cstdlib>
#include "CarFactory.hpp"

using std::vector;
using std::cout; using std::endl;

class CarLot{
public:
   CarLot();  //default
    Car *testDriveCar(){return Lot[next_];} //pointer to potential car

   // if a car is bought, requests a new one
   Car *buyCar(){ //pointer to a new car after one is bought
       Car *bought = Lot[next_];
       Lot[next_] = factories_[rand()%factories_.size()]->requestCar(); //pointer to bought car/car4sale passed test drive
      return bought;
   }
    Car *nextCar(){
        next_ = (++next_ % lotSize());
        return Lot[next_];
    }
    int lotSize(){return 8;}
             
private:
   vector<CarFactory *> factories_;
    Car* Lot[8];
    int next_ = 0;
};


CarLot::CarLot(){  //default out of line definition
   // creates 2 Ford factories and 2 Toyota factories
    factories_.push_back(new FordFactory());
    factories_.push_back(new FordFactory());
    factories_.push_back(new ToyotaFactory());
    factories_.push_back(new ToyotaFactory());
    
    for(int i = 0; i < 8; ++i){
        Lot[i] = factories_[rand() % factories_.size()] -> requestCar();
    }
}



CarLot *carLotPtr = nullptr; // global pointer instantiation

// test-drives a car
// buys it if Toyota
void toyotaLover(int id){
    if (carLotPtr == nullptr)  //new CarLot instance
        carLotPtr = new CarLot();
    
    std::string favModel = ford_models[rand() % ford_models.size()];
    for(int i = 0; i < carLotPtr->lotSize(); ++i){
        Car *toBuy = carLotPtr -> testDriveCar();
        
        cout << "Jill Toyoter " << id << endl;
        cout << "test driving " << toBuy->getMake() << " " << toBuy->getModel();
        
        if (toBuy->getMake() == "Toyota" && toBuy->getModel() == favModel){
            cout << " love it! buying it!" << endl;
            carLotPtr -> buyCar();
        }
        else{ //they don't like the car
            carLotPtr->nextCar();
            cout << " did not like it!" << endl;
        }
    }
}

// test-drives a car
// buys it if Ford
void fordLover(int id){
   if (carLotPtr == nullptr)
      carLotPtr = new CarLot();
    
    std::string favModel = ford_models[rand() % ford_models.size()];
    for(int i = 0; i < 8; ++i){
        Car *toBuy = carLotPtr -> testDriveCar();

        cout << "Jack Fordman " << id << endl;
        cout << " test driving "
        << toBuy->getMake() << " "
        << toBuy->getModel();
        
        if (toBuy->getMake() == "Ford"){
            cout << " love it! buying it!" << endl;
            carLotPtr -> buyCar();
        } else{
            carLotPtr -> nextCar();
            cout << " did not like it!" << endl;
        }
    }
}


/*
int main() {
   srand(time(nullptr));

   const int numBuyers=20;
   for(int i=0; i < numBuyers; ++i)
      if(rand()% 2 == 0)
          toyotaLover(i);
      else
          fordLover(i);
  
}
 */


//carTest

CarFactory *getLeastBusyFactory(const vector<CarFactory*> &inFactories) {

   if (inFactories.size() == 0) return nullptr;

   auto bestSoFar = inFactories[0];

   for (size_t i = 1; i != inFactories.size(); ++i) {
      if (inFactories[i]->getNumCarsInProduction() <
           bestSoFar->getNumCarsInProduction())
     bestSoFar = inFactories[i];
   }
   return bestSoFar;
}


/*
// same function with an STL algorithm
CarFactory *getLeastBusyFactory(const vector<CarFactory*> &inFactories) {

   if (inFactories.size() == 0) return nullptr;

   return accumulate(inFactories.begin()+1, inFactories.end(), *inFactories.begin(),
       [](CarFactory *a, CarFactory *b) {
          return a->getNumCarsInProduction() < b->getNumCarsInProduction() ? a: b;
        }
       );
}
 */

// client code
int main() {
   vector<CarFactory*> factories;
   
   srand(time(nullptr));
 

   factories.push_back(new FordFactory("Chicago, IL", rand()%4));
   factories.push_back(new FordFactory("Dearborn, MI", rand()%4));
   factories.push_back(new FordFactory("Louisville, KY", rand()%4));
   factories.push_back(new ToyotaFactory("Lafayette, IN", rand()%4));

   for (size_t i=0; i < 10; ++i) {
      CarFactory *currentFactory = getLeastBusyFactory(factories);
      const Car *newCar = currentFactory->requestCar();
      cout << "Ordering " << newCar->getMake()  << " " << newCar->getModel()
       << " from " << currentFactory->getLocation()
       << endl;
   }
 }
 

