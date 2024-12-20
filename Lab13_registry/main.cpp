// observer pattern using a signup registry
// Mikhail Nesterenko
// 4/22/2014

#include <set>
#include <map>
#include <iostream>
#include <string>
#include <algorithm>

using std::cout; using std::endl;
using std::map; using std::set;
using std::string;

class Observer;
class Subject;

class EventRegistry{
public:
   static void registerObserver(Subject*, Observer*);
   static void deregisterObserver(Subject*, Observer*);
    static void registerObserver(string, Observer*); //string flyweight
    static void deregisterObserver(string, Observer*); //string flyweight
   static void handleMessage(string);
private:
   static map<string, set<Observer*>> observerMap_;
};

// initialize the static map
map<string, set<Observer*>> EventRegistry::observerMap_;


class Observer{
public:
   Observer(const string &name): name_(name) {}
   string getName() const {return name_;}
   void subscribe(string s) {EventRegistry::registerObserver(s, this);}
   void unsubscribe(string s){EventRegistry::deregisterObserver(s, this);}
   void handleMessage(string s);
private:
   string name_;
};

class Subject{
public:
   Subject(const string &name): name_(name) {}
   string getName() const {return name_;}
   void generateMessage(){
      cout << name_ << " sent a message" << endl;
      EventRegistry::handleMessage(name_);}
private:
   string name_;
};

//void EventRegistry::registerObserver(Subject *s, Observer* o){
//   observerMap_[s].insert(o);
//   cout << o->getName() << " subscribed to " << s->getName() << endl;
//}
//
//void EventRegistry::deregisterObserver(Subject *s, Observer* o){
//   observerMap_[s].erase(o);
//   cout << o->getName() << " unsubscribed from " << s->getName() << endl;
//}

//out of line definitions for string flyweights
void EventRegistry::registerObserver(string s, Observer* o){
    observerMap_[s].insert(o);
    cout << o->getName() << " subscribed to " << s << endl;
    
}

void EventRegistry::deregisterObserver(string s, Observer* o){
    observerMap_[s].erase(o);
    cout << o->getName() << " unsubscribed to " << s << endl;
    
}

//out of line observer definitions
void EventRegistry::handleMessage(string s){
   for (auto e: observerMap_[s])
      e->handleMessage(s);
}

void Observer::handleMessage(string s) {
   cout << name_ << " received message from " << s << endl;
}


int main() {
   Subject  sue("Subject Sue"),  steve("Subject Steve");
   Observer olaf("Observer Olaf"), olga("Observer Olga");


   olaf.subscribe("Subject Sue"); olaf.subscribe("Subject Steve");
   olga.subscribe("Subject Sue");

   cout << endl;

   

   sue.generateMessage();
   steve.generateMessage();
   cout << endl;

   
   olga.unsubscribe("Subject Sue");
   sue.generateMessage();
   
}
