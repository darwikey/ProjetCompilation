#include "Main_block.hpp"
#include <stdexcept>
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
  
  // TODO variables globales
  
  // code pour toutes les fonctions
  for (auto f : functions){
    
    code += f.second->get_code(fParent_blocks);
  }


  return code;
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
