#include "Main_block.hpp"
#include <stdexcept>
#include "Function.hpp"
#include "Declarator.hpp"


std::string Main_block::get_code(std::vector<Block*> fParent_blocks, Function* fFunction){
  std::string code;
  fParent_blocks.push_back(static_cast<Block*>(this));
  
  // TODO variables globales
  
  // code pour toutes les fonctions
  for (auto f : functions){
    
    code += f.second->get_code(fParent_blocks);
  }


  return code;
}


void Main_block::add_function(Function* fFunction){
  if (functions.find(fFunction->declarator->name) == functions.end()){
    functions.insert(std::pair<std::string, Function*>(fFunction->declarator->name, fFunction));
  }
  else{
    throw std::logic_error("function " + fFunction->declarator->name + " already exist");
  } 
}
