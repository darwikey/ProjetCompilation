#include "Main_block.hpp"
#include <stdexcept>
#include "Function.hpp"
#include "Declarator.hpp"


std::string Main_block::get_code(){
  std::string code;
  
  // TODO variables globales
  
  for (auto f : functions){

    code += f.second->get_code();
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
