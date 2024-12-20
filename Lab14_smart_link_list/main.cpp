#include <iostream>
#include <memory>

using namespace std;
// the pointer part of every list is the same
// lists differ by data part
template <typename T>
class node{
public:

  node(): next_(nullptr), prev_(nullptr){}
    node(T data): data_(data) {}
  // functions can be inlined or out of line defined
  T getData() const {return data_;}
  void setData(const T& data){data_=data;}
  //next nodes
  std::shared_ptr<node<T>> getNext() const{return next_;}
  void setNext(std::shared_ptr<node> next){next_ = next;} //why const ????
  //prev nodes
  std::weak_ptr<node<T>> getPrev() const{return prev_;}
  void setPrev(std::weak_ptr<node> prev){prev_ = prev;}

private:
  T data_;
  std::shared_ptr<node<T>> next_;  //std::make_shared<node<T>> next_ (new node());
  std::weak_ptr<node<T>> prev_;
};


int main(){
   //node<int> *head, *currentPtr; //

   std::shared_ptr<node<int>> sp = std::make_shared<node<int>>(1);
    auto sp1 = std::make_shared<node<int>>(2);

    sp->setNext(sp1); // list contains two nodes
    sp1->setPrev(sp);
    
    for(sp1 = sp; sp1 != nullptr; sp1 = sp1->getNext()){
        cout << sp1->getData() << " "; // printing head node
    }
    cout << endl;
    //cout << sp->getData() << " "; // printing head node


   //
   // character list manipulation
   //
   
//    sp.reset(new node<char>('A'));
//    sp1.reset(sp);
    
    std::shared_ptr<node<char>> sp2 = std::make_shared<node<char>>('A');
    auto sp3 = sp2;
    cout << "Shared Ptr's reference count before creating list is: " << sp3.use_count() << endl;
    
//   // creating list
   for(char letter = 'B'; letter <= 'Z'; ++letter){
       auto spNew = std::make_shared<node<char>>(letter);  //dynamic allocating
        if(sp3->getPrev().lock() == nullptr){ //if at head of list, set next pointer to current ptr
            sp3->setNext(spNew); //set head next ptr to newNode
            spNew->setPrev(sp2); //set newNode prev to head
        }
       if(sp3->getPrev().lock() != nullptr){
           spNew->setPrev(sp3);
           sp3->setNext(spNew);
       }
//       cout << "CharTail's next is " << charTail->getNext() << endl;
//       cout << "CharTail's prev is " << charTail->getPrev() << endl << endl;

        //charTail->setNext(newNode);
       sp3 = spNew;
    }
    cout << "Shared Ptr's reference count after creating list is: " << sp3.use_count() << endl;
    //cout << sp3->getPrev().use_count() << endl;
    
    if(sp2->getPrev().lock() == nullptr){cout << "HEAD PREV IS NULL" << endl;}
    if(sp3->getNext() == nullptr){cout << "TAIL NEXT IS NULL" << endl;}

   for(sp3 = sp2;  sp3->getNext() != nullptr; sp3=sp3->getNext()) //set current back to head; until end of list; increment current by moving it to get next pointer
     cout << sp3->getData();
   cout << endl;
}
