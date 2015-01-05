#ifndef PRIMARY_EXPRESSION_HPP
#define PRIMARY_EXPRESSION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include <sstream>
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

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code;

    switch(type){
    case Primary_type::VARIABLE:
      {
	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);
	Type var_type = this->get_expression_type(fParent_blocks);

	if (var_type == Type::INT){
	  code += declaration_block->get_code_load_variable(identifier, "eax", var_type); 
	}
	else if (var_type == Type::FLOAT){
	  code += declaration_block->get_code_load_variable(identifier, "xmm0", var_type);
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
      }
      break;

    case Primary_type::EXPRESSION:
      code += expression->get_code(fParent_blocks, fFunction);
      break;

    case Primary_type::FUNCTION:
      {
	Main_block* main = dynamic_cast<Main_block*>(fParent_blocks[0]);
	Declarator* function = main->get_function_declarator(identifier);
	int stack_size = 0;

	// parcours du conteneur dans le sens inverse
	for (auto expr = function_parameter.rbegin(); expr != function_parameter.rend(); ++expr){
	  code += (*expr)->get_code(fParent_blocks, fFunction);
	  //TODO verif type
	  
	  code += "pushl %eax\n";
	  stack_size += 4;
	}

	code += "call " + function->name + "\n";
	
	std::ostringstream str;
	str << "addl $" << stack_size << " ,%esp\n";
	code += str.str();

      }
      break;

    case Primary_type::INCREMENTED_VARIABLE:
      {
	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);
	Type var_type = this->get_expression_type(fParent_blocks);

	if (var_type == Type::INT){
	  code += declaration_block->get_code_load_variable(identifier, "eax", var_type);
	  code += "incl %eax\n";
	  code += declaration_block->get_code_store_variable(identifier, "eax", var_type);
	}
	else if (var_type == Type::FLOAT){
	  //TODO
	  code += declaration_block->get_code_load_variable(identifier, "xmm0", var_type);
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
	  code += declaration_block->get_code_load_variable(identifier, "eax", var_type);
	  code += "decl %eax\n";
	  code += declaration_block->get_code_store_variable(identifier, "eax", var_type);
	}
	else if (var_type == Type::FLOAT){
	  //TODO
	  code += declaration_block->get_code_load_variable(identifier, "xmm0", var_type);
	}
	else{
	  throw std::logic_error("variable " + identifier + " can't be used in an expression");
	}
      }
      break;

    case Primary_type::ARRAY:
      {
	code += expression->get_code(fParent_blocks, fFunction);

	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);

	Type var_type = this->get_expression_type(fParent_blocks);

	if (var_type == Type::INT){
	  code += declaration_block->get_code_load_array(identifier, "eax", "eax", var_type);
	}
	else if (var_type == Type::FLOAT){
	  //TODO
	  code += declaration_block->get_code_load_array(identifier, "eax", "eax", var_type);
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
