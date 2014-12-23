#include "Block.hpp"
#include <stdexcept>
#include "Function.hpp"
#include "Declarator.hpp"
#include "Statement.hpp"


std::string Block::get_code(std::vector<Block*> fParent_blocks, Function* fFunction){
  std::string code;
  fParent_blocks.push_back(this);
  
  // on ajoute les variables locales sur la pile
  int position = -4;
  for (auto it : variables){
    Declarator* var = it.second;

    if (! var->is_function) {
      var->stack_position = position;
      
      if (var->structure == Declarator_structure::VARIABLE && var->type == Declarator_type::INT){
	code += "pushl $0\n";
      }
      else {
	throw std::logic_error("local variable (" + Declarator::get_string(var->structure) + " " + Declarator::get_string(var->type) + ") are not supported");
      }
      
    }
  }

  for (Statement* s : statements){
    code += s->get_code(fParent_blocks, fFunction) + "\n";
  }

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

