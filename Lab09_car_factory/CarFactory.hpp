// interface for abstract and concrete car factories
// Mikhail Nesterenko
// 7/16/2014

#include <string>
#include <array>
static const std::array<std::string, 4> ford_models = {"Focus", "Mustang", "Explorer", "F-150"}; //static const array of models
static const std::array<std::string, 5> toyota_models = {"Corolla", "Camry", "Prius", "4Runner", "Yaris"};
// abstract product of CAR
class Car{
 public:
   std::string getMake() const {return make_;}
   std::string getModel() const {return model_;}
 protected:
   std::string make_; // this car's make
   std::string model_; // this car's model
};

// two concrete products of CARS
class Ford : public Car{
 public:
   Ford(); //needs out of line definition??
};

class Toyota : public Car{
 public:
   Toyota();
};

// absract car factory
class CarFactory {
public:
   CarFactory(std::string location, int numCarsInProduction):  //initializer list
      location_(location),
      numCarsInProduction_(numCarsInProduction){}

   Car* requestCar();
   int getNumCarsInProduction() const {return numCarsInProduction_;}
   std::string getLocation() const {return location_;}
   virtual ~CarFactory(){} //virtual destructor
protected:
   virtual Car* makeCar() =0;  //protected method set to default zero

private:
   int numCarsInProduction_;
   std::string location_;
};

// two concrete car factories
class FordFactory : public CarFactory {
public:
   FordFactory(std::string location="", int numCarsInProduction=0): //initialization list
       CarFactory(location, numCarsInProduction){}
protected:
   Car* makeCar() override;  //explain override??
};

class ToyotaFactory : public CarFactory {
public:
   ToyotaFactory (std::string location="",
          int numCarsInProduction=0):
       CarFactory(location, numCarsInProduction){}
protected:
   Car* makeCar() override; //explain override?
};
