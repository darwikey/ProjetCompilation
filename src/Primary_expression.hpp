#ifndef PRIMARY_EXPRESSION_HPP
#define PRIMARY_EXPRESSION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "Expression.hpp"


enum class Primary_type {
  VARIABLE, INT, FLOAT, EXPRESSION, FUNCTION, INCREMENTED_VARIABLE, DECREMENTED_VARIABLE, ARRAY
    };

class Primary_expression : public Expression{
public:

  Primary_expression(Primary_type fType, std::string fIdentifier, Expression* fExpression = nullptr) : type(fType),
  identifier(fIdentifier) {
  }

  // pour les appels de fonctions
  Primary_expression(Primary_type fType, std::vector<Expression*> fFunction_parameter) : type(fType),
  function_parameter(fFunction_parameter){
  }

  // pour un int
  Primary_expression(int fValue) : type(Primary_type::INT), int_value(fValue) {
  }

  // pour un float
  Primary_expression(float fValue) : type(Primary_type::FLOAT), float_value(fValue) {
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code;

    switch(type){
    case Primary_type::VARIABLE:
      {
	Block* declaration_block = Expression::get_block(fParent_blocks, identifier);
	code += declaration_block->get_code_load_variable(identifier, "eax");
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
      break;

    case Primary_type::EXPRESSION:
      break;

    case Primary_type::FUNCTION:
      break;

    case Primary_type::INCREMENTED_VARIABLE:
      break;

    case Primary_type::DECREMENTED_VARIABLE:
      break;

    case Primary_type::ARRAY:
      break;
    }

    return code;
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
