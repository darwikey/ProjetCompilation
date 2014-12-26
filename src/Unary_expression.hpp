#ifndef UNARY_EXPRESSION_HPP
#define UNARY_EXPRESSION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Expression.hpp"


enum class Unary_type{
  NOT, NEGATIVE
};

class Unary_expression : public Expression{
public:
  
  Unary_expression(Unary_type fType, Expression* fExpression) : type(fType), 
  expression(fExpression){
  }
  

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code = expression->get_code(fParent_blocks, fFunction);

    if (type == Unary_type::NOT){
      code += "cmpl $0, %eax \n\
sete %al \n\
movzbl %al, %eax \n";
    }
    else if (type == Unary_type::NEGATIVE){
      code += "negl %eax\n";
    }

    return code;
  }


private:
  Unary_type type;
  Expression* expression;
};



#endif
