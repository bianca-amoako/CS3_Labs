// drink class to be used in Coffee Shack lab
// Mikhail Nesterenko
// 11/7/2022
#include <iostream>
#include <string>
#include <vector>
using namespace std;


enum class DrinkSize {small, medium, large};

class Drink;
//OBSERVER PATTERN CUSTOMER IS THE SUBJECT
class Customer{
public:
    Customer(Drink* drink, string name): drink_(drink), name_(name){}
    void notify();
    Drink* getDrink(){return drink_;}
    
private:
    Drink* drink_;
    string name_;
};
//base class
class Drink{
public:
    Drink(const DrinkSize size = DrinkSize::small, int price = 0): size_(size), price_(price) {}
    //default size/drinktype = small, price = $0
    std::string getSize(){
        if(size_ == DrinkSize::small){
            return "small coffee";
        }
        if(size_ == DrinkSize::medium){
            return "medium coffee";
        }
        if(size_ == DrinkSize::large){
            return "large coffee";
        }
        return "no paths entered";
    }
    void setSize(DrinkSize size){
        size_ = size;
    }
    void setPrice(){
        if(size_ == DrinkSize::small){
            price_ = 5;
        }
        if(size_ == DrinkSize::medium){
            price_ = 7;
        }
        if(size_ == DrinkSize::large){
            price_ = 10;
        }
    }
    virtual int getPrice() const {return price_;};
    virtual std::string getName() const {return name_;}; //why does this need to be virtual???
private:
    int price_;
    DrinkSize size_;
    std::string name_;
};

void Customer::notify() {
    cout << name_ << " your " << drink_->getSize() << " with " << drink_->getName() << "is ready. ";
    cout << "That'll be $" << drink_->getPrice() << " please." << endl;
}

//DECORATOR CLASSES
//decorate with add-ins????
//composed of the base coffee: large ($10), medium ($7) or small ($5) and added ingredients: sugar ($1), cream ($2) and honey ($3)
class addSugar : public Drink{
public:
    addSugar(Drink *drink): drink_(drink){};
//    void setPrice() override{
//        drink_->setPrice();
//    }
    int getPrice() const override {return drink_->getPrice() + 1;}
    string getName() const override {
        if(drink_->getName().find("sugar") == string::npos)
        return drink_->getName() + "sugar ";
    else
        return drink_->getName();}
private:
    Drink* drink_;
};

class addCream : public Drink{
public:
    addCream(Drink *drink): drink_(drink){};
    int getPrice() const override {return drink_->getPrice() + 2;}
    string getName() const override { if(drink_->getName().find("cream") == string::npos)
        return drink_->getName() + "cream ";
    else
        return drink_->getName();}
private:
    Drink* drink_;
};

class addHoney : public Drink{
public:
    addHoney(Drink *drink): drink_(drink){};
    int getPrice() const override {return drink_->getPrice() + 3;}
    string getName() const override {
        if(drink_->getName().find("honey") == string::npos)
            return drink_->getName() + "honey ";
        else
            return drink_->getName();
    }
private:
    Drink* drink_;
};

//CHAIN OF RESPONSISBILTY FOR BARISTAS Junior Barista may only prepare drinks with no added ingredients. Senior Barista may handle all added ingredients except for honey. Manager may handle all ingredients. Implement your barista class using Chain of Responsibility design pattern. In your dialog with customers, make sure to print the experience level of the barista preparing the order.

class Barista{
public:
    Barista(Barista* successor = nullptr): successor_(successor){};
    virtual void drinkRequest(Drink* drink){
        if(successor_ != nullptr)
            successor_->drinkRequest(drink);
    }
    
    void takeOrder(){
        Drink* drink = new Drink();
        cout << "Welcome to Coffee Shack, can I get you [l]arge, [m]edium, or [s]mall coffee? ";
        char drinkSize;
        cin >> drinkSize;
        switch(drinkSize){
            case 's':{
                drink->setSize(DrinkSize::small);
                drink->setPrice();
                break;
            }
            case 'm':{
                drink->setSize(DrinkSize::medium);
                drink->setPrice();
                break;
            }
            case 'l':{
                drink->setSize(DrinkSize::large);
                drink->setPrice();
                break;
            }
            default:
                cout << "Not a size \n";
                
        }
        char addIns = 'n';
        do {
            cout << "Would you like to add [s]ugar, [c]ream, [h]oney, or [d]one? ";
            cin >> addIns;
            switch(addIns){
                case 's':{
                    drink = new addSugar(drink);
                    break;
                }
                case 'c':{
                    drink = new addCream(drink);
                    break;
                }
                case 'h':{
                    drink = new addHoney(drink);
                    break;
                }
                case 'd':{
                    break;
                }
                default:
                    cout << "Not an option \n";
                    break;
            }
        } while (addIns != 'd');
        
        cout << "Can I get your name? ";
        string name; cin >> name;
        line_.push_back(new Customer(drink, name));
    }
    
    void removeCustomer(Customer* customer){
        for(int i = 0; i < line_.size(); ++i){
            if(line_[i] == customer)
                line_.erase(line_.begin() + i);
        }
    }
    
    Customer* randomCustomer() {
        if (line_.empty()) return nullptr;
        return line_[rand() % line_.size()];
    }

private:
    Barista* successor_;
    vector<Customer*> line_;
};

//chain of responsibilty for JuniorBarista
class JuniorBarista : public Barista{
public:
    JuniorBarista(Barista* successor = nullptr) : Barista(successor){}
    void drinkRequest(Drink* drink) override {
        if(drink->getName() == "")
            cout << "Your drink was prepeared by a Junior Barista!" << endl;
        else
            Barista::drinkRequest(drink);
    }
};

class SeniorBarista : public Barista{
public:
    SeniorBarista(Barista* successor = nullptr) : Barista(successor){}
    void drinkRequest(Drink* drink) override {
        if(drink->getName().find("honey") == string::npos)
            cout << "Your drink was prepeared by a Senior Barista!" << endl;
        else
            Barista::drinkRequest(drink);
    }
};

class Manager : public Barista{
public:
    Manager(Barista* successor = nullptr) : Barista(successor){}
    void drinkRequest(Drink* drink) override {
        if(drink->getName().find("honey") != string::npos)
            cout << "Your drink was prepeared by a Manager!" << endl;
        //do I need an else?
    }
};

int main(){
    
    Barista* barista = new JuniorBarista(new SeniorBarista(new Manager));
    while(true){ //why necessary if always true?
        if(rand() % 2 == 0) //0 is new order made
            barista->takeOrder();
        else{
            Customer* customer = barista->randomCustomer();
            if (!customer) continue;
            
            barista->drinkRequest(customer->getDrink());
            customer->notify();
            barista->removeCustomer(customer);
        }
    }
}

