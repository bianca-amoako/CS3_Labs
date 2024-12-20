// implementation of carFactory
// Mikhail Nesterenko
// 7/16/2014

#include <array>
#include <cstdlib>
#include "CarFactory.hpp"

// concrete product methods
Ford::Ford(){  //out of line definition
   make_ = "Ford";
   model_ = ford_models[rand() % ford_models.size()];  //models[i] for random model
}

Toyota::Toyota(){
   make_ = "Toyota";
   model_ = toyota_models[rand() % toyota_models.size()];
}


// factory methods
Car* CarFactory::requestCar() {
    ++numCarsInProduction_;
    return makeCar();
}

Car* FordFactory::makeCar() {  //was override in hpp
    return new Ford(); //retuns new instance for Car through ford concrete class
}

Car* ToyotaFactory::makeCar() {
    return new Toyota(); //returns new instance for Car through toyota concrete class
}
