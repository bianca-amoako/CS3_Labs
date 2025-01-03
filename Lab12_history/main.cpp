// implements command history (multiple undos)
// merges with Memento (checkpoint and rollback)
// demonstrates command pattern
// Mikhail Nesterenko
// 04/13/2023

#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::vector; using std::string;
using std::cout; using std::cin; using std::endl;

// receiver has main options of insert and remove
class Document {
public:

   Document(const vector <string> & lines={}): lines_(lines){}

   // actions
   void insert(int line, const string &str) {
      const int index = line-1;
      if (index <= lines_.size())
     lines_.insert(lines_.begin() + index, str);
      else
     cout << "line out of range" << endl;
   }

   const string remove(int line) {
      const int index = line-1;
      string deletedLine="";
      if(index < lines_.size()){
     // deletedLine = lines_[index];
     deletedLine = *(lines_.begin()+index); // same thing but with iterator arithmetic /the index of the string in the doc
     lines_.erase(lines_.begin() + index);
      }else
     cout << "line out of range" << endl;
      return deletedLine;
   }

   void show() {
      for(int i = 0; i < lines_.size(); ++i)
          cout << i + 1 << ". " << lines_[i] << endl;
   }
    
private:
   vector<string> lines_;
};

// abstract command of execute and unexecute
class Command{
public:
   Command(Document *doc) : doc_(doc){} //set pointer doc
   virtual void execute() = 0;
   virtual void unexecute() = 0;
    virtual void show() = 0;
   virtual ~Command(){}
protected:
   Document *doc_;
};

// two concrete commands that are specific commands
class InsertCommand : public Command {
public:
   InsertCommand(Document *doc, int line, const string &str): Command(doc), line_(line),  str_(str) {} //set pointer doc, line num, and string
   void execute() override { doc_->insert(line_, str_);}
   void unexecute() override {doc_->remove(line_);}
    void show() override {
        cout << "Insert " << str_ << " at line " << line_;
    }
private:
   int line_;
   string str_;
};


class EraseCommand : public Command {
public:
   EraseCommand(Document *doc, int line): Command(doc), line_(line), str_("") {} //set pointer doc, line num, and string to empty
   void execute() override {str_ = doc_->remove(line_);}
   void unexecute() override {doc_->insert(line_, str_);}
    void show() override {
        cout << "Erase " << str_ << " at line " << line_;
    }
private:
   int line_;
   string str_;
};

// client that is what commands are implemeted on
class DocumentWithHistory{
public:
   DocumentWithHistory(const vector<string> & text={}) : doc_(text){}

   void insert(int line, string str) {
      Command *com = new InsertCommand(&doc_, line, str);
      com->execute(); //execute from insert
      doneCommands_.push(com); //pushes action onto stack
       history_.push_back(com);
   }

   void erase(int line){
      Command *com = new EraseCommand(&doc_, line);
      com->execute();
      doneCommands_.push(com);
       history_.push_back(com);

   }

   void undo() {
       if(doneCommands_.size() != 0) {
           Command *com = doneCommands_.top(); //command pointer which is a document pointer which is a vector of strings /the top is the line that was changed
           doneCommands_.pop(); //pops top off
           com->unexecute(); //unexecute in insert class command
           delete com; // don't forget to delete command
           history_.pop_back();

      }else
     cout << "no commands to undo" << endl;
       
   }
    void history(){
        for(int i = 0; i < history_.size(); ++i){
            cout << i << ". " ;
            history_[i]->show();
            cout << endl;
        }
    }
    void redo(){
        if(doneCommands_.size() != 0) {
            Command *com = doneCommands_.top(); //command pointer which is a document pointer which is a vector of strings /the top is the line that was changed
            doneCommands_.push(com); //pops top off
            com->execute();
            history_.push_back(com);
       }else
      cout << "no commands to redo" << endl;
        
    }
    void show() {doc_.show();}
    //memento class declarition INSIDE originator(document class)
    class Memento *createMemento() const;
    void rollBack(class Memento*);


private:
   Document doc_;
   std::stack<Command*> doneCommands_;
    vector<Command*> history_;
};

//memento class definition
class Memento {
public:
   Memento(const DocumentWithHistory& doc): doc_(doc) {}
   const DocumentWithHistory& getState() const {return doc_;} //is this the same as show?
private:
   const DocumentWithHistory doc_;
};

//out of line definitions for functions in document that utilize memento class
Memento *DocumentWithHistory::createMemento() const{
    return new Memento(*this); // copying the Document itself anf saving it? with this pointer
}
void DocumentWithHistory::rollBack(Memento *mem){
   *this = mem->getState();  // copying back what was saved as memento
}


// invoker
int main() {
   DocumentWithHistory doc({
     "Lorem Ipsum is simply dummy text of the printing and typesetting",
     "industry. Lorem Ipsum has been the industry's standard dummy text",
     "ever since the 1500s, when an unknown printer took a galley of",
     "type and scrambled it to make a type specimen book. It has",
     "survived five centuries."});
   char option;
   do{
       static Memento *checkpoint = nullptr;
       doc.show();
       cout << endl;

       cout << "Enter option (i)nsert line, (e)rase line, (u)ndo last command, (c)heckpoint roll(b)ack, (h)istory, (r)edo command: ";
       cin >> option;

       int line; string str;
       switch(option) {
           case 'i':
               cout << "line number to insert: ";
               cin >> line;
               cout << "line to insert: ";
               cin.get(); // removes end-of-line character so getline() works correctly
               getline(cin, str);
               doc.insert(line, str);
               break;
               
           case 'e':
               cout << "line number to remove: ";
               cin >> line;
               doc.erase(line);
               break;
               
           case 'u':
               doc.undo();
               break;
               
           case 'c':
               checkpoint = doc.createMemento();
               break;
               
           case 'b':
               if (checkpoint != nullptr){
                   doc.rollBack(checkpoint);
                   delete checkpoint;
                   checkpoint = nullptr;
               }
               else
                   cout << "No checkpoint created" << endl;
               break;
           case 'h':
               doc.history();
               cout << endl;
               break;
           case 'r':
               doc.redo();
               break;
       }
   }
    while(option == 'i' || option == 'e' || option == 'u' || option == 'c' || option == 'r' || option == 'h' || option == 'b');
}
