
//  WordList.hpp
//  Lab2_word_count
//  Created by Bianca Amoako on 9/1/23.

#ifndef WordList_h
#define WordList_h
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

class WordOccurrence {
public:
    WordOccurrence(const string& word = "", int num = 0){
        word_ = word;
        num_ = num;
    }
    bool matchWord(const string &word){ // returns true if word matches stored
        return (word == word_) ? true : false;
    }
    void increment(){ // increments number of occurrences
        ++num_;
    }
    
    bool operator< (WordOccurrence &rhs){
        return num_ < rhs.num_;
    }
    
    string getWord() const{
        return word_;
    }
    int getNum() const{
        return num_;
    }

private:
    string word_;
    int num_;
};

class WordList{
public:
    WordList(){
        wordArray_ = nullptr;
        size_ = 0;
    }
    // add copy constructor
    WordList(const WordList& w1){
        size_ = w1.size_;
        wordArray_ = new WordOccurrence[size_];
        for(auto i = 0; i < size_; ++i){
            wordArray_[i] = w1.wordArray_[i];
        }
    }
    // add destructor
    ~WordList(){
        delete [] wordArray_;
    }
    // add overloaded assignment using friend swap
    friend void swap(WordList& obj1, WordList& obj2){
        swap(obj1.size_, obj2.size_);
        swap(obj1.wordArray_, obj2.wordArray_);
    }
    WordList& operator= (WordList rhs){
        swap(*this, rhs);
        return *this;
    }
    
    // implement comparison as friend
    friend bool equal(const WordList& obj1, const WordList& obj2){
        for(auto i = 0; i < obj1.size_; ++i){
            if(obj1.wordArray_[i].getWord() != obj2.wordArray_[i].getWord())
                return false;
        }
        return true;
    }

    void addWord(const string &word){
        for(auto i = 0; i < size_; ++i){
            if(wordArray_[i].matchWord(word) == true){
                wordArray_[i].increment();
                return;
            }
        }
        WordOccurrence *temp_wordArray_ = new WordOccurrence[++size_];
        for(int i = 0; i < size_-1; ++i){
            temp_wordArray_[i] = wordArray_[i];
        }
        temp_wordArray_[size_-1] = WordOccurrence(word, 1);
        delete [] wordArray_;
        wordArray_ = temp_wordArray_;
        //*wordArray_.increment();
    }
    void print(){
        
        std::sort(wordArray_, wordArray_ + size_);
        
        
        for(int i = 0; i < size_; ++i){
            cout << wordArray_[i].getWord() << ": " << wordArray_[i].getNum() << endl;
        }
    }
private:
    WordOccurrence *wordArray_; // a dynamically allocated array of WordOccurrences
                                // may or may not be sorted
    int size_;
};

#endif /* WordList_h */
