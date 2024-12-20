// figures: class adapter pattern
// Mikhail Nesterenko
// 11/7/2022

#include <iostream>

using std::cout; using std::endl; using std::cin;

// base interface
class Square {
public:
   virtual void draw() const =0; //pure virtual
    virtual void resize(int) =0;
   virtual ~Square(){} //virtual destrcuctor
};

// adaptee/implementer  ?? what is the difference between adaptee and base/delegator
class LegacyRectangle{
public:
    LegacyRectangle(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY):
    topLeftX_(topLeftX), topLeftY_(topLeftY), bottomRightX_(bottomRightX), bottomRightY_(bottomRightY){}

    void oldDraw() const {  //initial draw
        for(int i=0; i < bottomRightY_; ++i){ //rows from origin to end(y)
            for(int j=0; j < bottomRightX_; ++j) //columns from origin to end(x)
                if(i >= topLeftY_  && j >= topLeftX_ ) //greater than origin up till x or y
                    cout << '*';
                else
                    cout << ' ';  //end of row
            cout << endl;
        }
    }
    void move(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY){
        topLeftX_ = topLeftX;
        topLeftY_ = topLeftY;
        bottomRightX_ = bottomRightX;
        bottomRightY_ = bottomRightY;
    }
    
    //return the respective corner coordinates.
    int getTopLeftX(){return topLeftX_;}
    int getTopLeftY(){return topLeftY_;}
    int getBottomRightX(){return bottomRightX_;}
    int getBottomRightY(){return bottomRightY_;}

// defining top/left and bottom/right coordinates
private:
    int topLeftX_;
    int topLeftY_;
    int bottomRightX_;
    int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter: public Square, private LegacyRectangle { //square is interface (steering) //LegacyRect is implementation (how to steer)
public:
    SquareAdapter(int size): LegacyRectangle(0,0,size,size){};
    void draw() const override { //override draw() in Square -> LegacyRectangle oldDraw();
        oldDraw();
    }
    
    //returns the current square size
    int size(){
        return getBottomRightX() - getTopLeftX();
    }


    void resize(int newSize) override{
        move(0,0,newSize, newSize);
        oldDraw();
    }
};

//coding main() as follows: ask the user for the size of a square, declare a square of the user-specified size, draw the square; then ask the user for a new size, resize the square to the new size and redraw it
int main(){
    //    Square *square = new SquareAdapter(squareSize);
    //    square->draw();
    
    cout << "Enter size of Square: ";
    int squareSize;
    cin >> squareSize;
    //with pointer
    Square* square = new SquareAdapter(squareSize);
    square->draw();
    cout << "Enter new size of Square: ";
    cin >> squareSize;
    square->resize(squareSize);
    
}
