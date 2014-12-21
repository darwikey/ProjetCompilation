#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <ostream>
#include <vector>
#include <map>

struct Declarator;

struct Block{  

  Block()
  {
  }

  void add_declaration(std::vector<Declarator*> fList){
    for (Declarator* it : fList){ 
      if (variables.find(it->name) != variables.end()){
	variables.insert(std::pair<std::string, Declarator*>(it->name, it));
      }
      else{
	//todo error
      }
    }
  }

  std::map<std::string, Declarator*> variables;
};


#endif
