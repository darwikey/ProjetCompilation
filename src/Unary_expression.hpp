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

    // vérif type
    Type expr_type = expression->get_expression_type(fParent_blocks);
    if (expr_type != Type::INT && expr_type != Type::FLOAT){
      throw std::logic_error("only an int or a float can be used in an unary expression");
    }

    if (type == Unary_type::NOT){
      //TODO FLOAT
      code += "cmpl $0, %eax \n\
sete %al \n\
movzbl %al, %eax \n";
    }
    else if (type == Unary_type::NEGATIVE){
      if (expr_type == Type::FLOAT){
	code += "movups ZERO, %xmm1 \n";
	code += "subps %xmm0, %xmm1 \n";
	code += "movups %xmm1, %xmm0 \n";
      }
      else{
	code += "negl %eax\n";
      }
    }

    return code;
  }

  virtual Type get_expression_type(std::vector<Block*> fParent_blocks){

    return expression->get_expression_type(fParent_blocks);
  }

private:
  Unary_type type;
  Expression* expression;
};



#endif
