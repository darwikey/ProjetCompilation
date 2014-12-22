#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include <stdexcept>
#include "Statement.hpp"

struct Declarator;
struct Function;

struct Block : public Statement{  

  Block() {
  }

  virtual std::string get_code() override{
    std::string code;
    
    // TODO variables globales

    for (auto f : functions){
      // entête de la fonction
      code += ".glob " + f.first + "\n";
      code += f.first + ":\n";

      // adresse de retour
      code += "pushl %ebp\n";
      code += "movl %esp %ebp\n\n";

      code += "\n";

      // fin de la fonction
      code += "leave\n";
      code += "ret\n\n";
    }

    return code;
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
