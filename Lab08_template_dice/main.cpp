// Games, Template Method example
// Mikhail Nesterenko
// 2/4/2014

#include <ctime>
#include <cstdlib>
#include <iostream>

using std::cout; using std::cin; using std::endl;

// template for any game where players take
// turns to make moves
// and there is a winner
class Game{
public:
    Game():playersCount_(0), movesCount_(0), playerWon_(noWinner){} //initializes num of players & moves, playerWon_ = -1

   // template method
    void playGame(const int playersCount = 0) { //default num players = 0
        playersCount_ = playersCount;
        movesCount_=0;

        initializeGame(); //changes for each sub class

        for(int i=0; !endOfGame(); i = (i+1) % playersCount_ ){ //from start till end of game, choose player to make first move
            makeMove(i);
            if (i==playersCount_-1)
                ++movesCount_;
        }
        printWinner();
    }

    virtual ~Game(){} //destructor

protected:
   // primitive operations / virtual methods
    virtual void initializeGame() = 0;
    virtual void makeMove(int player) = 0;
    virtual void printWinner() = 0;
    virtual bool endOfGame() { return playerWon_ != noWinner;} // this is a hook WHY?
                   // returns true if winner is decided
    static const int noWinner = -1; //static member singleton

    int playersCount_;
    int movesCount_;
    int playerWon_; //
};

// Monopoly - a concrete game implementing primitive
// operations for the template method
class Monopoly: public Game {
public:
   // implementing concrete methods
    void initializeGame(){
        playersCount_ = rand() % numPlayers_ + 1 ; // initialize players
    }

    void makeMove(int player) { // i in main class
        if (movesCount_ > minMoves_){
            const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
            if (chance < movesCount_) playerWon_= player;
        }
    }

    void printWinner(){
        cout << "Monopoly, player " << playerWon_<< " won in " << movesCount_<< " moves." << endl;
    }

private:
    static const int numPlayers_ = 8; // max number of players
    static const int minMoves_ = 20; // nobody wins before minMoves_
    static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess: public Game {
public:
    void initializeGame(){
        playersCount_ = numPlayers_; // initalize players 2
        for(int i=0; i < numPlayers_; ++i)
            experience_[i] = rand() % maxExperience_ + 1 ;
    }

    void makeMove(int player){
        if (movesCount_ > minMoves_){
            const int chance = (rand() % maxMoves_) / experience_[player]; // EXPLANATION
            if (chance < movesCount_)
                playerWon_= player;
        }
    }

    void printWinner(){
        cout << "Chess, player " << playerWon_ << " with experience " << experience_[playerWon_] << " won in "<< movesCount_ << " moves over" << " player with experience " << experience_[playerWon_== 0 ? 1:0]
            << endl;
   }

private:
    static const int numPlayers_ = 2;
    static const int minMoves_ = 2; // nobody wins before minMoves_
    static const int maxMoves_ = 100; // somebody wins before maxMoves_
    static const int maxExperience_ = 5; // player's experience
                              // the higher, the greater probability of winning
    int experience_[numPlayers_]; //array of 2
};

class Dice: public Game {
public:
    void initializeGame(){
        playersCount_ = numPlayers_;
        score1 = 0;
        score2 = 0;
    }
    
    void makeMove(int player){
        bool rollAgain = true;
        if(movesCount_ == maxRounds){
            playerWon_ = score1 > score2 ? 0 : 1;
            return;
        }
        
        if(player == 0){
            //computer roll
            cout << "Round " << movesCount_ + 1 << ": ";
            compPass = rand() % 2 == 0;
            if(compPass){
                cout << "Computer Rolled: ";
                for(int i = 0; i < 5; ++i){ //dice numbers
                    roll[i] = rand() % 6 + 1;
                    cout << roll[i] << " ";
                    score1 += roll[i];
                }
            }
            else{
                cout << "Computer rolled: passed";
            }
            cout << ", computer's score = " << score1 << endl;
        }
       
        if(player == 1){
            //human roll
            if(!userPass){
                cout << "You rolled: ";
                for(int i = 0; i < 5; ++i){ //dice numbers
                    roll[i] = rand() % 6 + 1;
                    cout << roll[i] << " ";
                    score2 += roll[i];
                }
                cout << ", your score = " << score2 << endl;
            }
            else {
                cout << "You rolled: passed, your score = " << score2 << endl;
            }
            
            cout << "Do you want to roll again? [y/n] ";
            char response;
            cin >> response;
            userPass = response == 'n' || response == 'N';
        }
    }
    void printWinner(){
        playerWon_ == 0 ? cout << "you lost" << endl : cout << "you won" << endl;
    }
private:

    static const int numPlayers_ = 2;
    static const int maxRounds = 3;

    int score1;
    int score2;
    int roll[5];
    bool compPass;
    bool userPass;

};

int main() {
    srand(time(nullptr));

    Game* gp = nullptr; //game pointer that is null
    //  /               gets pointed to whichever subclass

   // play chess 8 times
    for (int i = 0; i < 8; ++i){
        gp = new Chess;  //game pointer is set to point to a new instance of Chess class
        gp->playGame();  //template class function, virtuals called
        delete gp;       //deallocate the pointer
    }


   // play monopoly 8 times
    for (int i = 0; i < 8; ++i){
        gp = new Monopoly; //game pointer to monopoly class
        gp->playGame();    //template playGame, invokes virtuals
        delete gp;         //deallocate the pointer
    }

    for (int i = 0; i < 1; ++i){
        gp = new Dice;
        gp->playGame();
        delete gp;
    }
}
