//
//  3 state change model
//  Lab10_stateTransitions
//
//  Created by Bianca Amoako on 10/30/23.
//

#include <iostream>
#include <string>
#include <queue>
using namespace std;

//abstract forward class declaration
class State;

//base/context
class Process{
public:
    Process(); //default constructor
    
    //clone constructor
    Process(Process* p){
        state_ = p->state_;
        processID = p->nextPID++;
    }
    
    static int nextPID;
    int getID() {return processID;}
    //actions/behaviors
    void dispatch();
    void suspend();
    void block();
    void unblock();
    Process* clone(){return new Process(this);}
    string report();
    
    //part of implementation of state pattern
    void changeState(State* state){state_ = state;}
    
private:
    State* state_;
    int processID;
    
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


int Process::nextPID = 0;
//PROCESS BASE CLASS MEMBER FUNCTION DEFINITONS
Process::Process(){
    processID = ++nextPID;
    state_ = Ready::instance();}

//PROCESS CLONE CONSTRUCTOR OUT OF LINE DEF
//Process::Process(Process* p){
//    state_ = p->state_;
//    processID = p->nextPID++;
//}

//behavior PROCESS definitions
void Process::dispatch(){state_ = Running::instance();}
void Process::suspend(){state_ = Ready::instance();}
void Process::block(){state_ = Blocked::instance();}
void Process::unblock(){state_ = Ready::instance();}
string Process::report(){return state_->report(this);}

int main(int argc, const char * argv[]) {
    //The first process in the queue is in running state, the rest are in ready.
    Process one;
    
    //Use STL queue of processes
    queue<Process*> readyQ;
        
    queue<Process*> blockedQ;
    //CLONE OBJECTS
    for(auto i = 1; i < 5; ++i){
        readyQ.push(one.clone());
        cout << "PID " << i << " created" << endl << "PID " << i << " state: " << readyQ.front()->report() << endl;
    }
    
    //FIRST PROCESS IS RUNNING
    readyQ.front()->dispatch();

    //Randomly (with probability of 33% for each case), the running process may either exit, be suspended, or be blocked
    cout << "------------- scheduler running -------------" << endl;
    
    //ready queue processes
    while(!readyQ.empty() || !blockedQ.empty()){
        if(!readyQ.empty()){
//            readyQ.front().dispatch(); //first is in running state
            cout << "dispatching.... " << endl;
            cout << readyQ.front()->getID() << " state: " << readyQ.front()->report() << endl;
            int route = rand() % 3;
            switch(route){
                case 0:     //In case of exiting, the process is destroyed.
                    cout << "exiting... " << endl;
                    cout << readyQ.front()->getID() << " state: exited" << endl;
                    readyQ.pop();
                    break;
                case 1:     //In case of suspension, the process becomes ready and moves to the tail of the ready queue.
                    cout << "suspending... " << endl;
                    readyQ.front()->suspend();
                    cout << readyQ.front()->getID() << " state: " << readyQ.front()->report() << endl;
                    readyQ.push(readyQ.front());
                    readyQ.pop();
                    break;
                case 2:    //In case of blocking, the process moves to the tail of blocked queue and becomes blocked.
                    cout << "blocking... " << endl;
                    readyQ.front()->block();
                    cout << readyQ.front()->getID() << " state: " << readyQ.front()->report() << endl;
                    blockedQ.push(readyQ.front());
                    readyQ.pop();
                    break;
            }
        }
        if(readyQ.empty())
            cout << "---- the ready queue is empty ----" << endl;
        
        if(!blockedQ.empty()){
            cout << blockedQ.front()->report();
            int route = rand() % 2; //still blocked or ready
            switch(route){
                case 0: //still blocked
                    cout << "still blocking... " << endl;
                    blockedQ.push(blockedQ.front());
                    blockedQ.pop();
                    break;
                case 1: //ready through unblocking
                    cout << "unblocking... " << endl;
                    blockedQ.front()->unblock();
                    cout << blockedQ.front()->getID() << " state: " << blockedQ.front()->report() << endl;
                    readyQ.push(blockedQ.front());
                    blockedQ.pop();
                    break;
            }
        }
    }
}
