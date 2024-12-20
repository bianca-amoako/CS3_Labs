// mediator-based air traffic control
// Mikhail Nesterenko
// 11/30/2022

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <algorithm>

// for timed sleep
#include <thread>
#include <chrono>


using std::cout; using std::cin; using std::endl;
using std::string;

//Modify the code for the ATC example to use flyweight design pattern as follows. Every flight has an intrinsic flyweight portion in the form of the airline logo. The logo is a text of the airline name in CAPITAL LETTERS. Note that the logo may be shared by several flights of the same airline. Modify the ATC example such that after getting clearance and acknowledging takeoff/landing, there is an extra printout (with the logo) of the aircraft actually doing the takeoff/landing.

// Enum for all compaines
enum class logo {airfrance, KLM, aeroflot, lufthansa, delta, united, jetblue};

// Abstract flyweight
class Logo{
public:
    virtual string image() = 0;
};

// corresponding concrete flyweights for each company
class AirFrance: public Logo{
public:
    string image() override {return "_AIRFRANCE_";}
};

class KLM: public Logo{
public:
    string image() override {return "_KLM_";}
};

class Aeroflot: public Logo{
public:
    string image() override {return "_AEROFLOT_";}
};

class Lufthansa: public Logo{
public:
    string image() override {return "_LUFTHANSA_";}
};

class Delta: public Logo{
public:
    string image() override {return "_DELTA_";}
};

class United: public Logo{
public:
    string image() override {return "_UNITED_";}
};

class JetBlue: public Logo{
public:
    string image() override {return "_JETBLUE_";}
};

// a factory to store the logos and make them
class LogoFactory{
public:
    static Logo *giveLogo(logo l){ //a static instastance of Logo pointer with enum logo class passed
        if(logos_.find(l) == logos_.end()) //if the logo is not found in map
            switch(l){ //make a new Logo
                case logo::airfrance:
                    logos_[l] = new AirFrance;
                    break;
                case logo::KLM:
                    logos_[l] = new KLM;
                    break;
                case logo::aeroflot:
                    logos_[l] = new Aeroflot;
                    break;
                case logo::lufthansa:
                    logos_[l] = new Lufthansa;
                    break;
                case logo::delta:
                    logos_[l] = new Delta;
                    break;
                case logo::united:
                    logos_[l] = new United;
                    break;
                case logo::jetblue:
                    logos_[l] = new JetBlue;
                    break;
            }
        return logos_[l]; //else the logo has already been used
    }
private:
    static std::map<logo, Logo*> logos_; //map of enum, Logo* which gives back and image
};

std::map<logo, Logo*> LogoFactory::logos_; //initialization of static variable

// abstract mediator
class Controller{
public:
   virtual void join(class Flight*)=0;
   virtual void leave(class Flight*)=0;
   virtual void broadcast() = 0;
   virtual void observe() = 0;
};


// concrete mediator
// tower, runway clearance
class Tower: public Controller{
public:
   void join(class Flight*) override;
   void leave(class Flight*) override;
   void broadcast() override;
   void observe() override;
private:
   std::set<class Flight*> waiting_;
};


// abstract colleague
class Flight {
public:
   Flight(Controller *controller):controller_(controller), status_(Status::waiting){
      task_ = rand() % 2 ? Task::taxiing : Task::approaching;
      controller_->join(this);
   }
   void receive(const string &msg){
      if (msg.find(flightNo_) != string::npos || msg.find("all") != string::npos){
     if(msg.find("clear") != string::npos){
        cout << flightNo_ << " roger that, ";
        cout << (task_ == Task::taxiing ? "taking off" : "landing") << endl;
        status_ = Status::cleared;
     } else if(msg.find("status") != string::npos) {
        cout << flightNo_
         << (status_ == Status::waiting ? " waiting to " : " cleared to ")
             << (task_ == Task::taxiing ? "take off" : "land") << endl;
     } else
        cout << "Tower, this is " << flightNo_ << " please repeat." << endl;
      }
   }
   bool isCleared() const {return status_ == Status::cleared;}
   void proceed(){
      std::this_thread::sleep_for(std::chrono::seconds(rand()%3+1)); // wait a while
      cout << ".... " << logo_->image() << FlightNum_
       << (task_ == Task::taxiing ? " took off ...." : " landed...")
       << ". Have a good day!" << endl;
      controller_->leave(this);
   }
    
protected:
    string flightNo_;
    int FlightNum_;
    Controller *controller_;
    Logo* logo_;
    enum class Task {taxiing, approaching};  Task task_;
    enum class Status {waiting, cleared};    Status status_;
};

// concrete colleagues
class Airbus: public Flight{
public:
   Airbus(Tower *tower): Flight(tower){ static const std::vector<string> companies = {"AirFrance", "KLM", "Aeroflot", "Lufthansa"};
       FlightNum_ = rand() % 1000;
       flightNo_ = companies[rand() % companies.size()] + std::to_string(FlightNum_);
       
       // assign logo_ from givelogo in the Logo Factory
       if(flightNo_.find("France") != string::npos)
           logo_ = LogoFactory::giveLogo(logo::airfrance);
       if(flightNo_.find("KLM") != string::npos)
           logo_ = LogoFactory::giveLogo(logo::KLM);
       if(flightNo_.find("Aero") != string::npos)
           logo_ = LogoFactory::giveLogo(logo::aeroflot);
       if(flightNo_.find("Lufthansa") != string::npos)
           logo_ = LogoFactory::giveLogo(logo::lufthansa);
       
       cout << flightNo_ << " requesting " << (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
   }
};

class Boeing: public Flight{
public:
   Boeing(Tower *tower): Flight(tower){ static const std::vector<string> companies = {"Delta", "United", "JetBlue"};
       FlightNum_ = rand() % 1000;
       flightNo_ = companies[rand() % companies.size()] + std::to_string(FlightNum_);
       
       // assign logo_ from givelogo in the Logo Factory
       if(flightNo_.find("Delta") != string::npos)
           logo_ = LogoFactory::giveLogo(logo::delta);
       if(flightNo_.find("United") != string::npos)
           logo_ = LogoFactory::giveLogo(logo::united);
       if(flightNo_.find("Jet") != string::npos)
           logo_ = LogoFactory::giveLogo(logo::jetblue);
       
       cout << flightNo_ << " requesting "
       << (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
   }
};


// member functions for Tower
void Tower::broadcast() {
   cout << "Tower: ";
   string msg;
   getline(cin,msg);
   if(!msg.empty())
      for(auto f: waiting_) f->receive(msg);
}


void Tower::observe() {
   auto findCleared = [](Flight *f){return f->isCleared();};
   
   auto toProceed = std::find_if(waiting_.begin(),waiting_.end(), findCleared);
   
   while (toProceed != waiting_.end()){ // found a cleared flight
      (*toProceed) -> proceed();
      toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);
      if(toProceed != waiting_.end())
     cout << "MAYDAY! MAYDAY! MAYDAY! " << endl; // more than a single flight cleared
   }
}


void Tower::join(Flight *f) {
   waiting_.insert(f);
}


void Tower::leave(Flight *f) {
   waiting_.erase(f);
   delete f;
}

int main(){
   srand(time(nullptr));
   Tower jfk;

   new Boeing(&jfk);
   new Airbus(&jfk);
   new Boeing(&jfk);
   new Airbus(&jfk);

   while(true){
      jfk.broadcast();
      jfk.observe();
      if(rand() % 2){
      if(rand() % 2)
        new Boeing(&jfk);
     else
        new Airbus(&jfk);
      }
   }
}
