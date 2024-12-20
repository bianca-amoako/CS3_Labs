// filled/hollow figures demonstrates Bridge Design Pattern,
// square is either hollow or square and is painted with a particular character,
// it is bridged over Figure --> Fill abstract body/handle
// Mikhail Nesterenko
// 2/12/2014

#include <iostream>

using std::cout; using std::endl; using std::cin;

// abstract body
class Fill{
public:
   Fill(char fillChar, char borderChar): fillChar_(fillChar), borderChar_(borderChar){}
   virtual char getBorder() = 0; //p virtual
   virtual char getInternal() = 0;
   virtual ~Fill() {}
protected:
   char fillChar_;
   char borderChar_;   //?????????
};

// concrete body
class Hollow: public Fill{
public:
   Hollow(char borderChar): Fill(' ', borderChar){} //changed parameter of hollow to borderChar and called Fill with empty space and borderChar
   char getBorder() override {return borderChar_;} //changed from fillChar to borderChar ????
   char getInternal() override {return ' ';}
   ~Hollow() override {}
};

// another concrete body
class Filled: public Fill {
public:
   Filled(char fillChar):Fill(fillChar, fillChar){}
   Filled(char fillChar, char borderChar):Fill(fillChar, borderChar){}
   char getBorder() override {return borderChar_;}
   char getInternal() override {return fillChar_;}
   ~Filled() override {}
};

// other concrete bodies
class FullyFilled: public Filled{
public:
    FullyFilled(char fillChar, char borderChar): Filled(fillChar, borderChar){}
    char getBorder() override {return borderChar_;} //okay for both to be override methods becuase they are derived from Filled
    char getInternal() override {return fillChar_;}
    ~FullyFilled() override {}
};

class RandomFilled: public Filled{
public:
    RandomFilled(char fillChar, char borderChar): Filled(fillChar, borderChar){}
    char getBorder() override {
        int choiceChar = rand() % 2;
        if(choiceChar == 0) return fillChar_;
        return borderChar_;
        
    } //okay for both to be override methods becuase they are derived from Filled
    char getInternal() override {
        return getBorder();
    }
    ~RandomFilled() override {}
};


// abstract handle
class Figure {
public:
   Figure(int size, Fill *fill): size_(size), fill_(fill){}
   virtual void draw() = 0;  //p virtual
   virtual ~Figure(){}
protected:
   int size_; //size of figure
   Fill *fill_; //pointer to Fill object
};

// concrete handle
class Square: public Figure{
public:
   Square(int size, Fill *fill): Figure(size, fill){}
   void draw() override;
};

void Square::draw(){  //out of line definition for draw() override
   for(int i = 0; i < size_; ++i){  //table where i is columns/y and j is rows/x
       for(int j = 0; j < size_; ++j)
           if(i == 0 || j == 0 || i == size_-1 || j == size_-1 ) //if at the origin to the right bottom end, use border character
               cout << fill_ -> getBorder();
           else
               cout << fill_ -> getInternal(); //use fill character
       cout << endl;
   }
}

int main(){

   /*
   Fill* hollowPaintJ = new Hollow('%');
   Fill* filledPaintStar = new Filled('@');

   
   Figure *smallBox = new Square(5, hollowPaintJ);
   Figure *bigBox = new Square(15, filledPaintStar);

   smallBox->draw();
   cout << endl;
   bigBox -> draw();
   */
   
   /*
   Figure *userBox = new Square(size, ifFilled == 'f'?
           static_cast<Fill *>(new Filled(fchar)):
           static_cast<Fill *>(new Hollow(fchar))
           );
   */
   
   /*
   Figure *userBox = new Square(size,
              ifFilled == 'f'?
                new Filled(fchar):
                new Hollow(fchar)
               );
   */
    char fchar = '*';
    char bchar = '|';
   
    //Figure *userBox = ifFilled == 'f'? new Square(size, new Filled(fchar, bchar)): new Square(size,new Hollow(bchar));
    cout << "Filled Square: " << endl;
    Figure *filledSquare = new Square(7, new Filled(fchar, bchar));
    filledSquare-> draw();
    cout << endl;
    
    cout << "Hollow Square: " << endl;
    Figure *hollowSquare = new Square(7, new Hollow (bchar));
    hollowSquare-> draw();
    cout << endl;
    
    cout << "Fully Filled Square: " << endl;
    Figure *fullyFilledSquare = new Square(7, new FullyFilled(fchar, bchar));
    fullyFilledSquare-> draw();
    cout << endl;
    
    cout << "Random Filled Square: " << endl;
    Figure *RandomFilledSquare = new Square(7, new RandomFilled(fchar, bchar));
    RandomFilledSquare-> draw();
    cout << endl;
   
}
