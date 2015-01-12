#ifndef PRIMARY_EXPRESSION_HPP
#define PRIMARY_EXPRESSION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include "Expression.hpp"
#include "Main_block.hpp"


enum class Primary_type {
  VARIABLE, INT, FLOAT, EXPRESSION, FUNCTION, INCREMENTED_VARIABLE, DECREMENTED_VARIABLE, ARRAY
    };

class Primary_expression : public Expression{
public:

  Primary_expression(Primary_type fType, std::string fIdentifier, Expression* fExpression = nullptr) : type(fType),
												       identifier(fIdentifier),
												       expression(fExpression){
  }

  // pour les appels de fonctions
  Primary_expression(Primary_type fType, std::string fIdentifier, std::vector<Expression*> fFunction_parameter) : type(fType),
														  identifier(fIdentifier),
														  function_parameter(fFunction_parameter){
  }

  // pour un int
  Primary_expression(int fValue) : type(Primary_type::INT), 
				   int_value(fValue) {
  }

  // pour un float
  Primary_expression(float fValue) : type(Primary_type::FLOAT), 
				     float_value(fValue) {
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) override {
    std::string code;

    switch(type){
    case Primary_type::VARIABLE:
      {
	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);
	Type var_type = this->get_expression_type(fParent_blocks);

	if (var_type == Type::INT || var_type == Type::POINTER || var_type == Type::FLOAT_POINTER){
	  code += declaration_block->get_code_load_variable(identifier, "eax", var_type, fVectorize); 
	}
	else if (var_type == Type::FLOAT){
	  code += declaration_block->get_code_load_variable(identifier, "xmm0", var_type, fVectorize);
	}
	else{
	  throw std::logic_error("variable " + identifier + " can't be used in an expression");
	}
      }
      break;

    case Primary_type::INT:
      {
	std::ostringstream str;
	str << "movl $" << int_value << ", %eax\n";
	code += str.str();
      }
      break;

    case Primary_type::FLOAT:
      {
	code += Main_block::get_code_load_float(float_value);
	if (fVectorize){
	  code += "shufps $0x00, %xmm0, %xmm0 \n";
	}
      }
      break;

    case Primary_type::EXPRESSION:
      code += expression->get_code(fParent_blocks, fFunction, fVectorize);
      break;

    case Primary_type::FUNCTION:
      {
	Main_block* main = dynamic_cast<Main_block*>(fParent_blocks[0]);
	Declarator* function = main->get_function_declarator(identifier);
	int stack_size = 0;

	if (function_parameter.size() != function->parameter_list.size()){
	  throw std::logic_error("not the same number of arguments for " + function->name);
	}

	// parcours du conteneur dans le sens inverse
	auto function_param = function->parameter_list.rbegin();
	for (auto expr = function_parameter.rbegin(); expr != function_parameter.rend(); ++expr){
	  code += (*expr)->get_code(fParent_blocks, fFunction, fVectorize);
	  
	 
	  //*** verif type ***
	  Type expr_type = (*expr)->get_expression_type(fParent_blocks);	 
	  bool error_param = false;
	  if ((*function_param)->structure == Declarator_structure::VARIABLE){
	    if (((*function_param)->type == Declarator_type::INT && expr_type != Type::INT) 
		|| ((*function_param)->type == Declarator_type::FLOAT && expr_type != Type::FLOAT)){
	      error_param = true;
	    }
	  }
	  else if ((*function_param)->structure == Declarator_structure::POINTER && expr_type != Type::POINTER && expr_type != Type::FLOAT_POINTER){
	    error_param = true;
	  }

	  if (error_param){
	    throw std::logic_error("bad type of arguments for " + function->name);
	  }

	  //*** Ajout des paramètres sur la pile ****
	  if (expr_type == Type::FLOAT){
	    code += "subl $4, %esp\n";
	    code += "movss %xmm0, (%esp)\n";
	  }
	  else{
	    code += "pushl %eax\n";
	  }
	  stack_size += 4;

	  ++function_param;
	}

	code += "call " + function->name + "\n";
	
	std::ostringstream str;
	str << "addl $" << stack_size << " ,%esp\n";
	code += str.str();

	// si la fonction retourne un float on alloue de la place sur la pile, on copie le resultat dessus, puis on bouge ce float dans %xmm0
	if (function->type == Declarator_type::FLOAT && function->structure == Declarator_structure::VARIABLE){
	  code += "subl $4, %esp\n";
	  code += "fstps (%esp)\n";
	  code += "movss (%esp), %xmm0\n";
	  code += "add $4, %esp\n";
	  if (fVectorize){
	    code += "shufps $0x00, %xmm0, %xmm0\n";
	  }
	}
      }
      break;

    case Primary_type::INCREMENTED_VARIABLE:
      {
	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);
	Type var_type = this->get_expression_type(fParent_blocks);

	if (var_type == Type::INT){
	  code += declaration_block->get_code_load_variable(identifier, "eax", var_type, fVectorize);
	  code += "incl %eax\n";
	  code += declaration_block->get_code_store_variable(identifier, "eax", var_type);
	}
	else if (var_type == Type::FLOAT){
	  code += declaration_block->get_code_load_variable(identifier, "xmm0", var_type, fVectorize);
	  code += "movups ONE, %xmm1\n";
	  code += "addps %xmm1, %xmm0\n";
	  code += declaration_block->get_code_store_variable(identifier, "xmm0", var_type);
	}
	else{
	  throw std::logic_error("variable " + identifier + " can't be used in an expression");
	}
      }
      break;

    case Primary_type::DECREMENTED_VARIABLE:
      {
	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);
	Type var_type = this->get_expression_type(fParent_blocks);

	if (var_type == Type::INT){
	  code += declaration_block->get_code_load_variable(identifier, "eax", var_type, fVectorize);
	  code += "decl %eax\n";
	  code += declaration_block->get_code_store_variable(identifier, "eax", var_type);
	}
	else if (var_type == Type::FLOAT){
	  code += declaration_block->get_code_load_variable(identifier, "xmm0", var_type, fVectorize);
	  code += "movups ONE, %xmm1\n";
	  code += "subps %xmm1, %xmm0\n";
	  code += declaration_block->get_code_store_variable(identifier, "xmm0", var_type);
	}
	else{
	  throw std::logic_error("variable " + identifier + " can't be used in an expression");
	}
      }
      break;

    case Primary_type::ARRAY:
      {
	code += expression->get_code(fParent_blocks, fFunction, fVectorize);

	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);

	Type var_type = this->get_expression_type(fParent_blocks);

	if (var_type == Type::INT){
	  code += declaration_block->get_code_load_array(identifier, "eax", "eax", var_type, fVectorize);
	}
	else if (var_type == Type::FLOAT){
	  code += declaration_block->get_code_load_array(identifier, "eax", "xmm0", var_type, fVectorize);
	}
	else{
	  throw std::logic_error("variable " + identifier + " can't be used in an expression");
	}
    
      }

      break;
    }

    return code;
  }


  virtual Type get_expression_type(std::vector<Block*> fParent_blocks) override{
    switch(type){
    case Primary_type::VARIABLE:
    case Primary_type::INCREMENTED_VARIABLE:
    case Primary_type::DECREMENTED_VARIABLE:
      {
	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);
	return declaration_block->get_variable_type(identifier);
      }

    case Primary_type::INT:
      return Type::INT;

    case Primary_type::FLOAT:
      return Type::FLOAT;

    case Primary_type::EXPRESSION:
      return expression->get_expression_type(fParent_blocks);

    case Primary_type::FUNCTION:
      {
	Main_block* main = dynamic_cast<Main_block*>(fParent_blocks[0]);
	Declarator* ret = main->get_function_declarator(identifier);

	if (ret->structure == Declarator_structure::VARIABLE){
	  if (ret->type == Declarator_type::INT)
	    return Type::INT;
	  if (ret->type == Declarator_type::FLOAT)
	    return Type::FLOAT;
	}
	else if (ret->structure == Declarator_structure::POINTER){
	  if (ret->type == Declarator_type::FLOAT)	
	    return Type::FLOAT_POINTER;
	  else
	    return Type::POINTER;
	}
	throw std::logic_error("function " + identifier + " doesn't return a valid type");
      }

    case Primary_type::ARRAY:
      {
	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);
	Type type = declaration_block->get_variable_type(identifier);

	if (type == Type::INT_ARRAY)
	  return Type::INT;
	else if (type == Type::FLOAT_ARRAY)
	  return Type::FLOAT;
	else if (type == Type::POINTER)
	  return Type::INT;
	else if (type == Type::FLOAT_POINTER)
	  return Type::FLOAT;
	else
	  throw std::logic_error("variable " + identifier + " is not an array");
      }
    }

    return Type::INT;
  }


private:
  Primary_type type;
  std::string identifier;

  // pour un nouveau bloc d'expressions et l'index d'un tableau
  Expression* expression = nullptr;

  //pour les paramètres d'appel d'une fonction
  std::vector<Expression*> function_parameter;

  // pour les int et float
  int int_value = 0;
  float float_value = 0;
};


#endif
