#include "Block.hpp"
#include <stdexcept>
#include "Function.hpp"
#include "Declarator.hpp"


std::string Block::get_code(){
  std::string code;
  

  return code;
}

void Block::add_declaration(std::vector<Declarator*> fList){
  for (Declarator* it : fList){ 
    if (variables.find(it->name) == variables.end()){
      variables.insert(std::pair<std::string, Declarator*>(it->name, it));
      }
    else{
      throw std::logic_error("variable already exist");
    }
  }
}

