//
//  main.cpp
//  Lab8_logger
//
//  Created by Bianca Amoako on 10/16/23.
//

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

class Logger{
public:
    static Logger* instance(); //static method that returns pointer to instance
    void report(const string&);  //function appending log
    
    Logger(const Logger&) = delete; //prohibiting copying
    Logger& operator= (const Logger&) = delete; //prohibiting comparison
   
private:
    ofstream fout; //payload variable
    Logger();
    static Logger* onlyInstance_; //pointer to the instance of Logger
    void static cleanUp(); //cleanUp destructor
};
Logger::Logger(){  //initialization is private
    fout.open("/Users/biancaamoako/Desktop/CS3/Lab8_logger/Lab8_logger/log.txt", fstream::out | fstream::app);
    atexit(cleanUp);
}
void Logger::cleanUp (){
    onlyInstance_ -> fout.close(); //pointer to close file
    delete onlyInstance_; //deallocate pointer
}

Logger* Logger::onlyInstance_ = nullptr;

//method definitions
Logger* Logger::instance(){
    if(onlyInstance_ == nullptr)
        onlyInstance_ = new Logger;
    return onlyInstance_;
}
void Logger::report(const string& line){
    if (fout.is_open())
        fout << line;
}

int main() {
    Logger::instance() -> report("Hello World\n");
    Logger::instance() -> report("Nice to meet you\n");
}
