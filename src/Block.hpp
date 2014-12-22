#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include <stdexcept>


struct Declarator;
struct Function;

struct Block{  

  Block() {
  }

  void add_declaration(std::vector<Declarator*> fList){
    for (Declarator* it : fList){ 
      if (variables.find(it->name) == variables.end()){
	variables.insert(std::pair<std::string, Declarator*>(it->name, it));
      }
      else{
	throw std::logic_error("variable already exist");
      }
    }
  }


  void add_function(Function* fFunction){
    if (functions.find(fFunction->declarator->name) == functions.end()){
      functions.insert(std::pair<std::string, Function*>(fFunction->declarator->name, fFunction));
    }
    else{
      throw std::logic_error("function " + fFunction->declarator->name + " already exist");
    } 
  }

  std::map<std::string, Declarator*> variables;
  std::map<std::string, Function*> functions;
};


#endif
