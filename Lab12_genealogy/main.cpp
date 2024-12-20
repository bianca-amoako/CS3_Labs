// traversing genealogical tree using Composite and Visitors
// Mikhail Nesterenko
// 04/13/2023

#include <iostream>
#include <string>
#include <vector>

using std::cout; using std::endl;
using std::string;
using std::vector;

// component
class Person{
public:
   Person(string firstName, Person* spouse, Person* father, Person* mother):
      firstName_(firstName), spouse_(spouse), father_(father), mother_(mother){}
   void setFirstName(string firstName) {firstName_ = firstName;}
   string getFirstName() const {return firstName_;}
   Person* getSpouse() const {return spouse_;}
   void setSpouse(Person *spouse){spouse_=spouse;}
   Person* getFather() const {return father_;}
   Person* getMother() const {return mother_;}
   virtual void accept(class PersonVisitor*) = 0;
   virtual ~Person(){}
private:
    string firstName_;
    Person* spouse_;
    Person* father_;
    Person* mother_;
};


// leaf
// man has a last name
class Man: public Person{
public:
    Man(string lastName, string firstName, Person* spouse, Person* father, Person* mother):
      lastName_(lastName), Person(firstName, spouse, father, mother){}
    const string& getLastName() const {return lastName_;}
    void accept(class PersonVisitor* visitor) override; //accept is part of the visitor pattern
private:
   const string lastName_;
};

// composite and why not a leaf like a man is
// woman has a vector of children
class Woman: public Person{
public:
   Woman(vector<Person*> children, string firstName, Person *spouse, Person *father, Person *mother):
       children_(children), Person(firstName, spouse, father, mother){}
   const vector<Person*>& getChildren() const {return children_;} //returns a vector
//    string newFirstName(){
//        if(getMother() != nullptr){
//            string newName = this->getFirstName() + " " + (this->getMother())->getFirstName();
//            return newName;
//        }
//        else
//            return this->getFirstName();
//    }
   void setChildren(const vector<Person*>& children){ children_ = children;}
   void accept(class PersonVisitor *visitor) override; //visitor accesses woman and man
private:
   vector<Person*> children_;
};

// abstract visitor class
class PersonVisitor{
public:
   virtual void visit(Man*) = 0; //pure virtual visit for each composite
   virtual void visit(Woman*) = 0;
   virtual ~PersonVisitor(){}
};

// visitor method for the leaf out of line definition
void Man::accept(PersonVisitor* visitor) {
    visitor->visit(this); //visit one's own information with 'this'
}

// visitor method for the component: exectues visits and defines traversal
void Woman::accept(PersonVisitor* visitor){
   // children traversal through mother only
   // father's children are not traversed so as not
    
    visitor->visit(this); //visit one's own information with 'this'

   // traversing descendants wiht for loop in vector<*Person> children_
    for(auto child : children_) //for each person* in vector
        child->accept(visitor); //person->accept(woman)
}

//update the first names of a person to add a middle name, a string is appended to the first name after a space.
//man's middle name is the first name of his father
//woman's middle name is the first name of her mother.
//If a person does not have the respective parent, no middle name should be added.

class MiddleName: public PersonVisitor{
public:
    void visit(Man* m) override {
        if(m->getFather() != nullptr){
            std::string fathersFirst = (m->getFather())->getFirstName();
            m->setFirstName(m->getFirstName() + " " + fathersFirst.substr(0, fathersFirst.find(' ')));
        }
    }
    void visit(Woman* w) override {
        if(w->getMother() != nullptr) {
            std::string mothersFirst = (w->getMother())->getFirstName();
            w->setFirstName(w->getFirstName() + " " + mothersFirst.substr(0, mothersFirst.find(' ')));
        }
    }
};

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her spouse if she is married or by her father if she is not
class NamePrinter: public PersonVisitor{ //access to visitor through man and woman accept
public:
   void visit(Man *m) override {
       cout << m->getFirstName() << " " << m->getLastName() << endl;
   }
   void visit(Woman *w) override {
       cout << w->getFirstName() << " ";
       if (w->getSpouse() != nullptr) //if she IS married
           cout << static_cast<Man*> (w->getSpouse())->getLastName();
            //(woman's husband)->his last name/string, to his person pointer
       else if (w->getFather() != nullptr) //OR if she has a father
           cout << static_cast<Man *> (w->getFather())->getLastName();
       else
           cout << "Doe"; //she has no husband or father
      cout << endl;
   }
};


class ChildrenPrinter: public PersonVisitor{
public:
   void visit(Man *m) override {
      cout << m->getFirstName() << ": ";
      Woman *spouse = static_cast<Woman *>(m->getSpouse());
      if(spouse != nullptr)
          printNames(spouse->getChildren());
      cout << endl;
   }
   void visit(Woman *w) override {
      cout << w->getFirstName() << ": ";
      printNames(w->getChildren());
      cout << endl;
   }
private:
   void printNames(const vector<Person*> &children){
      for(const auto c: children)
     cout << c->getFirstName() << ", ";
   }
};

// demonstrating the operation
int main(){

   // setting up the genealogical tree
   // the tree is as follows
   //
   //
   //       James Smith  <--spouse-->   Mary
   //                                      |
   //                                     children -------------------------
   //                                      |              |                |
   //                                      |              |                |
   //       William Johnson <--spouse-> Patricia      Robert Smith       Linda
   //                                      |
   //                                     children------------
   //                                      |                 |
   //                                     |                 |
   //       Jennifer  <--spouse-->  Michael Johnson      Barbara
   //           |
   //         children
   //           |
   //          |
   //         Susan

 //order is spouse, father, mother
   // first generation
   Man *js = new Man("Smith", "James", nullptr, nullptr, nullptr);
   Woman *ms = new Woman({}, "Mary", nullptr, nullptr, nullptr); //{} empty vector
   ms->setSpouse(js); js->setSpouse(ms); //set spouses


   // second generation
   Woman *ps = new Woman({}, "Patricia",  nullptr, js, ms);
   Man *wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
   ps->setSpouse(wj); wj->setSpouse(ps);

   vector<Person*> marysKids  = {ps, new Man("Smith", "Robert", nullptr, js, ms), new Woman({}, "Linda", nullptr, js, ms)}; //each child is a person construct
   ms->setChildren(marysKids); //set children vector

   // third generation
   Man *mj = new Man("Johnson", "Michael", nullptr, wj, ps); //unmarried
   vector<Person*> patsKids   = {mj, new Woman({}, "Barbara", nullptr, wj, ps)};
   ps->setChildren(patsKids);

   Woman *jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
   vector<Person*> jensKids = {new Woman({}, "Susan", nullptr, mj ,jj)};

    jj->setSpouse(mj);
    mj->setSpouse(jj);
    jj->setChildren(jensKids);
   

   // defining two visitors
   ChildrenPrinter *cp = new ChildrenPrinter;
   NamePrinter *np = new NamePrinter;
    MiddleName *mn = new MiddleName;

   // executing the traversal with the composite and the specific visitor

   
   cout << "\nNAME LIST\n";
   ms->accept(mn); //mary accpets the name printer
    ms->accept(np);
   cout << endl << endl;

  

   cout << "CHILDREN LIST\n";
   ms->accept(cp);  //mary accpets the name printer
   

   /*
   cout << "\nJAMES' CHILDREN\n";
   js->accept(cp);
   */
}
