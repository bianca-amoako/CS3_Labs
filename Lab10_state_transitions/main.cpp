//
//  3 state change model
//  Lab10_stateTransitions
//
//  Created by Bianca Amoako on 10/30/23.
//

#include <iostream>
#include <string>
using namespace std;

//abstract forward class declaration
class State;

//base/context
class Process{
public:
    Process();
    
    //actions/behaviors
    void dispatch();
    void suspend();
    void block();
    void unblock();
    string report();
    
    //part of implementation of state pattern
    void changeState(State* state){state_ = state;}
    
private:
    State* state_;
};

//abstract forward class definition
class State{
public:
    //default virtual behaviors
    virtual void dispatch(Process*){} //virtual behavior with context pointer
    virtual void suspend(Process*){}
    virtual void block(Process*){}
    virtual void unblock(Process*){}
    virtual string report(Process*) = 0; //pure virtual because HAVE to use the function in the implementation of the concrete class
    void changeState(Process* p, State* s){
        p->changeState(s);
    }
};

//concrete states 1/3 READY
class Ready: public State{
public:
    static State* instance(){ // SINGLETON implementation
        static State* onlyInstance = new Ready; //static variable pointer to the only instance of Ready State
        return onlyInstance;
    }
    //behaviors override
    void dispatch(Process*) override;
    string report(Process*) override {return "READY ";}
    
};

//concrete states 2/3 READY
class Running: public State{
public:
    //SINGLETON IMPLEMENTATION
    static State* instance(){
        static State* onlyInstance = new Running; //static pointer to only instance of running
        return onlyInstance;
    }
    //behaviors that RUNNING can implement (suspend & block)
    void suspend(Process*) override;
    void block(Process*) override;
    string report(Process*) override {return "RUNNING ";}
};

//concrete states 3/3 BLOCKED
class Blocked: public State{
public:
    //SINGLETON iomplementation of concrete state
    static State* instance(){
        static State* onlyInstance = new Blocked;
        return onlyInstance;
    }
    //behaviors that BL0CKED can implement (only unblock)
    void unblock(Process*) override;
    string report(Process*) override {return "BLOCKED ";}
};

//out of line definition for singleton state READY
//READY can only dispatch
void Ready::dispatch(Process * p) {
    changeState(p, Running::instance());
}

//out of line definition for singleton state RUNNING
//RUNNING can suspend
void Running::suspend(Process* p){
    changeState(p, Ready::instance());
}
//RUNNING can block
void Running::block(Process* p){
    changeState(p, Blocked::instance());
}

//out of line definiton for singleton state BLOCKED
void Blocked::unblock(Process* p){
    changeState(p, Ready::instance());
}

//PROCESS BASE CLASS MEMBER FUNCTION DEFINITONS
Process::Process(){state_ = Ready::instance();}
//behavior PROCESS definitions
void Process::dispatch(){state_ = Running::instance();}
void Process::suspend(){state_ = Ready::instance();}
void Process::block(){state_ = Blocked::instance();}
void Process::unblock(){state_ = Ready::instance();}
string Process::report(){return state_->report(this);}

int main(int argc, const char * argv[]) {
    Process process;
    cout << "process 1 is " << process.report() << endl;
    process.dispatch();
    cout << "process 1 is " << process.report() << endl;
    process.dispatch();
    cout << "process 1 is " << process.report() << endl;
    process.suspend();
    cout << "process 1 is " << process.report() << endl;
    process.dispatch(); process.block();
    cout << "process 1 is " << process.report() << endl;
    process.unblock();
    cout << "process 1 is " << process.report() << endl;


}
