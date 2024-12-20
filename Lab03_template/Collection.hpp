// node class tempate used to contruct lists
// Mikhail Nesterenko
// 5/01/00

#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_
#include <iostream>
using namespace std;
// the pointer part of every list is the same
// lists differ by data part

// forward class declaration
template<typename T>
class Collection;


// forward function declaration
template<typename T>
bool equal(const Collection<T>&, const Collection<T>&);

template <typename T>
class node{
public:
    //default node
    node(): next_(nullptr) {}
    
    // functions can be inlined
    T getData() const {return data_;}
    void setData(const T& data){data_=data;}

  // or can be defined outside
    node<T>* getNext() const;
    void setNext(node<T> *const);

private:
    
    T data_;
    node<T> *next_;
};

//
// member functions for node
//
template <typename T>
node<T>* node<T>::getNext() const {
    return next_;
}

template<typename T>
void node<T>::setNext(node<T> *const next){
    next_=next;
}

//-----------------------------------------

template <typename T>
class Collection{
public:
    Collection(){
        head_ = nullptr;
    }
    void add(T data_);
    void remove(T data_);//remove(): takes an item as argument, removes all instances from the collection, removed nodes deallocated
    Collection<T> last();
    void print();
    friend bool equal<T>(const Collection&, const Collection&);
private:
    node<T> *head_;
};

template <typename T>
void Collection<T>::add(T data){
    node<T>* new_node = new node<T>;
    new_node->setNext(head_); //-> is the same as . but only for pointers
    new_node->setData(data);
    head_ = new_node;
}

template <typename T>
void Collection<T>::remove(T data){
    node<T>* temp_node = head_;
    if(head_ == nullptr){ //if list is empty
        return;
    }
    while(head_->getData() == data){
        head_ = head_->getNext();
        delete temp_node;
        temp_node = head_;
    }
    node<T>* prev_node = head_;
    temp_node = head_->getNext();
    while(temp_node != nullptr){
        if(temp_node->getData() == data){
            prev_node->setNext(temp_node->getNext());
            delete temp_node;
            temp_node = prev_node->getNext();
        }
        else{
            prev_node = temp_node;
            temp_node = temp_node->getNext();
        }
    }
}

template <typename T>
Collection<T> Collection<T>::last(){
    return head_->getData();
}

template<typename T>
void Collection<T>::print(){
    node<T>* temp_node = head_;
    while(temp_node != nullptr){
        cout << temp_node->getData() << endl;
        temp_node = temp_node->getNext();
    }
}

template<typename T>
bool equal(const Collection<T>& one, const Collection<T>& two){
    node<T>* temp_one = one.head_;
    node<T>* temp_two = two.head_;
    while(temp_one != nullptr && temp_two != nullptr){
        if(temp_one->getData() != temp_two->getData())
            return false;
        temp_one = temp_one->getNext();
        temp_two = temp_two->getNext();
    }
    if (temp_one != nullptr || temp_two != nullptr)
        return false;
    return true;
}








#endif // COLLECTION_HPP_

