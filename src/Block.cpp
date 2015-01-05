#include "Block.hpp"
#include <stdexcept>
#include "Function.hpp"
#include "Declarator.hpp"
#include "Statement.hpp"
#include <sstream>
#include <iostream>


std::string Block::get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize){
  std::string code;
  
  // on ajoute les variables locales sur la pile
  int position = 0;
  if (!fParent_blocks.empty()){
    position = fParent_blocks.back()->top_stack_position;
  }
  
  // on empile les blocs
  fParent_blocks.push_back(this);

  for (auto it : variables){
    Declarator* var = it.second;

    // si la variable n'a pas encore de position dans la pile, c'est une variable local et on doit reserver de l'espace dans la pile
    if (! var->is_function && var->stack_position == 0) {
      var->stack_position = position - 4;
      
      if ((var->structure == Declarator_structure::VARIABLE && (var->type == Declarator_type::INT || var->type == Declarator_type::FLOAT)) 
	  || var->structure == Declarator_structure::POINTER){
	code += "subl $4, %esp\n";
	position -= 4;
      }
			
      else if (var->structure == Declarator_structure::ARRAY && (var->type == Declarator_type::INT || var->type == Declarator_type::FLOAT)){
	std::ostringstream str;
	str << "subl $" << var->array_size * 4 << ", %esp\n";
	code += str.str();
	position -= 4 * var->array_size;
      }
			
      else {
	throw std::logic_error("local variable (" + Declarator::get_string(var->structure) + " " + Declarator::get_string(var->type) + ") are not supported");
      }
      
    }
  }

  top_stack_position = position;

  for (Statement* s : statements){
    code += s->get_code(fParent_blocks, fFunction);
  }

  return code;
}


bool Block::is_variable(std::string fIdentifier){
  auto it = variables.find(fIdentifier);

  return it != variables.end();
}


Type Block::get_variable_type(std::string fIdentifier){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    if (it->second->structure == Declarator_structure::VARIABLE){
      if (it->second->type == Declarator_type::INT)
	return Type::INT;
      if (it->second->type == Declarator_type::FLOAT)
	return Type::FLOAT;
    }
    else if (it->second->structure == Declarator_structure::POINTER){
      if (it->second->type == Declarator_type::FLOAT)	
	return Type::FLOAT_POINTER;
      else
	return Type::POINTER;
    }
    else  if (it->second->structure == Declarator_structure::ARRAY){
      if (it->second->type == Declarator_type::INT)
	return Type::INT_ARRAY;
      if (it->second->type == Declarator_type::FLOAT)
	return Type::FLOAT_ARRAY;
    }
    
    throw std::logic_error("variable " + fIdentifier + " can't be used in an expression");
  }

  throw std::logic_error("variable " + fIdentifier + " undeclared");
  return Type::INT;
}


std::string Block::get_code_load_variable(std::string fIdentifier, std::string fRegister, Type fVar_type){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    std::ostringstream str;
    if (fVar_type == Type::FLOAT){
      str << "movss " << it->second->stack_position << "(%ebp), %" << fRegister << "\n";
    }
    else{
      str << "movl " << it->second->stack_position << "(%ebp), %" << fRegister << "\n";
    }
    return str.str();
  }

  throw std::logic_error("variable " + fIdentifier + " undeclared");
  return "";
}

std::string Block::get_code_load_array(std::string fIdentifier, std::string fRegister_index, std::string fRegister_dest, Type fVar_type){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    if (it->second->structure == Declarator_structure::ARRAY || it->second->structure == Declarator_structure::POINTER){
      std::ostringstream str;
      
      std::string mov_instr = "movl";
      if (fVar_type == Type::FLOAT){
	mov_instr = "movss";
      }

      if (it->second->structure == Declarator_structure::ARRAY){
	int address = it->second->stack_position - 4 * ((int)it->second->array_size - 1);

	str << mov_instr << " " << address << "(%ebp" << ", %" << fRegister_index << ", 4), %" << fRegister_dest << "\n";
      }
      else if (it->second->structure == Declarator_structure::POINTER){
	
	str << mov_instr << " " << it->second->stack_position << "(%ebp), %edx\n";
	str << mov_instr << " (%edx, %" << fRegister_index << ", 4), %" << fRegister_dest << "\n";
      }

      return str.str();

    }
  }

  throw std::logic_error(fIdentifier + " undeclared or is not an array or a pointer");
  return "";
}


std::string Block::get_code_store_variable(std::string fIdentifier, std::string fRegister, Type fVar_type){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    std::ostringstream str;
    if (fVar_type == Type::FLOAT){
      str << "movss %" << fRegister << ", " << it->second->stack_position << "(%ebp)\n";
    }
    else{
      str << "movl %" << fRegister << ", " << it->second->stack_position << "(%ebp)\n";
    }
    return str.str();
  }

  throw std::logic_error("variable " + fIdentifier + " undeclared");
  return "";
}


std::string Block::get_code_store_array(std::string fIdentifier, std::string fRegister_index, std::string fRegister_dest, Type fVar_type){
  auto it = variables.find(fIdentifier);

  if (it != variables.end()){
    if (it->second->structure == Declarator_structure::ARRAY || it->second->structure == Declarator_structure::POINTER){
      std::ostringstream str;

      std::string mov_instr = "movl";
      if (fVar_type == Type::FLOAT){
	mov_instr = "movss";
      }

      if (it->second->structure == Declarator_structure::ARRAY){
	int address = it->second->stack_position - 4 * ((int)it->second->array_size - 1);

	str << mov_instr << " %" << fRegister_dest << ", " << address << "(%ebp, %" << fRegister_index << ", 4)\n";
      }
      else if (it->second->structure == Declarator_structure::POINTER){
	
	str << mov_instr << " " << it->second->stack_position << "(%ebp), %edx\n";
	str << mov_instr << " %" << fRegister_dest << ", (%edx, %" << fRegister_index << ", 4)\n";
      }

      return str.str();
    }
  }

  throw std::logic_error(fIdentifier + " undeclared or is not an array or a pointer");
  return "";
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

