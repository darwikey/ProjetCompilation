#ifndef BINARY_EXPRESSION_HPP
#define BINARY_EXPRESSION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Expression.hpp"


enum class Binary_type{
  MULTIPLICATIVE, ADDITIVE, COMPARISON
};

class Binary_expression : public Expression{
public:
  
  Binary_expression(Binary_type fType, Expression* fExpression1, Expression* fExpression2) : type(fType), 
  expression1(fExpression1), 
  expression2(fExpression2) {
  }
  

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    return "";
  }


private:
  Binary_type type;

};



#endif
