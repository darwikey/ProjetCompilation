#ifndef BINARY_EXPRESSION_HPP
#define BINARY_EXPRESSION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Expression.hpp"


enum class Binary_type{
  MULTIPLICATION, ADDITION, SUBTRACTION, LOWER, GREATER, LOWER_EQUAL, GREATER_EQUAL, EQUAL, NOT_EQUAL
};

class Binary_expression : public Expression{
public:
  
  Binary_expression(Binary_type fType, Expression* fExpression1, Expression* fExpression2) : type(fType), 
  expression1(fExpression1), 
  expression2(fExpression2) {
  }
  

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code = expression1->get_code(fParent_blocks, fFunction);
    // on empile le resultat
    code += "pushl %eax\n";

    code += expression2->get_code(fParent_blocks, fFunction);
    code += "movl %eax, %ecx \n";
    //on recupère le résultat de notre première expression
    code += "popl %eax\n";
    
    // vérif types
    Type expr_type1 = expression1->get_expression_type(fParent_blocks);
    Type expr_type2 = expression2->get_expression_type(fParent_blocks);
    if ((expr_type1 != Type::INT && expr_type1 != Type::FLOAT)
	|| (expr_type2 != Type::INT && expr_type2 != Type::FLOAT)){
      throw std::logic_error("only an int or a float can be used in a binary expression");
    }

    switch(type){
    case Binary_type::MULTIPLICATION:
      code += "imul %ecx, %eax\n";
      break;

    case Binary_type::ADDITION:
      code += "addl %ecx, %eax\n";
      break;

    case Binary_type::SUBTRACTION:
      code += "subl %ecx, %eax\n";
      break;
      
    case Binary_type::LOWER:
      code += "cmpl %ecx, %eax \n\
setl %al \n\
movzbl %al, %eax \n";
      break;

    case Binary_type::GREATER:
      code += "cmpl %ecx, %eax \n\
setg %al \n\
movzbl %al, %eax \n";
      break;
      
    case Binary_type::LOWER_EQUAL:
      code += "cmpl %ecx, %eax \n\
setle %al \n\
movzbl %al, %eax \n";
      break;
  
    case Binary_type::GREATER_EQUAL:
      code += "cmpl %ecx, %eax \n\
setge %al \n\
movzbl %al, %eax \n";
      break;

    case Binary_type::EQUAL:
      code += "cmpl %ecx, %eax \n\
sete %al \n\
movzbl %al, %eax \n";
      break;
  
    case Binary_type::NOT_EQUAL:
      code += "cmpl %ecx, %eax \n\
setne %al \n\
movzbl %al, %eax \n";   
      break;

    }
    return code;
  }


  virtual Type get_expression_type(std::vector<Block*> fParent_blocks) override{
    return expression1->get_expression_type(fParent_blocks);
  }


private:
  Binary_type type;
  Expression* expression1;
  Expression* expression2;
};



#endif
