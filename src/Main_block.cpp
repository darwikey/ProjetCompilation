#include "Main_block.hpp"
#include <stdexcept>
#include <sstream>
#include "Function.hpp"
#include "Declarator.hpp"


Main_block::Main_block(){
  // enregistrement des prototypes de printint() et printfloat()
  Declarator* decl = new Declarator("printint", Declarator_structure::VARIABLE);
  decl->is_function = true;
  Declarator* param = new Declarator("value", Declarator_structure::VARIABLE);
  param->type = Declarator_type::INT;
  decl->parameter_list.push_back(param);
  variables.insert(std::pair<std::string, Declarator*>(decl->name, decl));

  decl = new Declarator("printfloat", Declarator_structure::VARIABLE);
  decl->is_function = true;
  param = new Declarator("value", Declarator_structure::VARIABLE);
  param->type = Declarator_type::FLOAT;
  decl->parameter_list.push_back(param);
  variables.insert(std::pair<std::string, Declarator*>(decl->name, decl));
}


std::string Main_block::get_code(std::vector<Block*> fParent_blocks, Function* fFunction){
  std::string code = get_header();

  fParent_blocks.push_back(static_cast<Block*>(this));
  
  // variables globales
  for (auto it : variables){
    Declarator* var = it.second;

    if (! var->is_function) {
      int var_size = 4;
      if (var->structure == Declarator_structure::ARRAY && var->type == Declarator_type::INT){
	
	var_size = 4 * var->array_size;
      }

      std::ostringstream str;
      str << ".comm " << var->name << ", " << var_size << "\n";
      code += str.str();
    }
  }

  code += "\n";
  
  // code pour toutes les fonctions
  for (auto f : functions){
    
    code += f.second->get_code(fParent_blocks);
  }


  return code;
}


std::string Main_block::get_code_load_variable(std::string fIdentifier, std::string fRegister){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    std::ostringstream str;
    str << "movl " << fIdentifier << ", %" << fRegister << "\n";
    return str.str();
  }

  throw std::logic_error("global variable " + fIdentifier + " undeclared");
  return "";
}


std::string Main_block::get_code_load_array(std::string fIdentifier, std::string fRegister_index, std::string fRegister_dest){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    if (it->second->structure == Declarator_structure::ARRAY || it->second->structure == Declarator_structure::POINTER){
      std::ostringstream str;

      str << "movl "<< fIdentifier << "(, %" << fRegister_index << ", 4), %" << fRegister_dest << "\n";

      return str.str();

    }
  }

  throw std::logic_error(fIdentifier + " undeclared or is not an array or a pointer");
  return "";
}


std::string Main_block::get_code_store_variable(std::string fIdentifier, std::string fRegister){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    std::ostringstream str;
    str << "movl %" << fRegister << ", " << fIdentifier << "\n";
    return str.str();
  }

  throw std::logic_error("global variable " + fIdentifier + " undeclared");
  return "";
}


std::string Main_block::get_code_store_array(std::string fIdentifier, std::string fRegister_index, std::string fRegister_dest){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    if (it->second->structure == Declarator_structure::ARRAY || it->second->structure == Declarator_structure::POINTER){
      std::ostringstream str;

      str << "movl %" << fRegister_dest << ", " << fIdentifier << "(, %" << fRegister_index << ", 4)\n";
      return str.str();
    }
  }

  throw std::logic_error(fIdentifier + " undeclared or is not an array or a pointer");
  return "";
}


std::string Main_block::get_header(){
  std::string code = ".text \n\
__STR_PRINT_INT: \n\
   .ascii \"%d\\12\\0\" \n\
__STR_PRINT_FLOAT: \n\
   .ascii \"%f\\12\\0\" \n\
\n\
.globl printint \n\
printint: \n\
   pushl   %ebp \n\
   movl    %esp, %ebp \n\
   \n\
   pushl   8(%ebp) \n\
   pushl   $__STR_PRINT_INT \n\
   call    printf \n\
   addl    $8, %esp \n\
   \n\
   leave \n\
   ret \n\
\n\
.globl printfloat \n\
printfloat: \n\
   pushl   %ebp \n\
   movl    %esp, %ebp \n\
   \n\
   pushl   8(%ebp) \n\
   pushl   $__STR_PRINT_FLOAT \n\
   call    printf \n\
   addl    $8, %esp \n\
   \n\
   leave \n\
   ret \n\n";
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


Declarator* Main_block::get_function_declarator(std::string fIdentifier){
  auto it1 = functions.find(fIdentifier);
  auto it2 = variables.find(fIdentifier);

  if (it1 != functions.end()){
    return it1->second->declarator;
  }
  else if (it2 != variables.end()){
    return it2->second;
  }
  else {
    throw std::logic_error("undeclared function : " + fIdentifier);
  }
}
