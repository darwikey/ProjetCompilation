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
  

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) override {
    std::string code = expression1->get_code(fParent_blocks, fFunction, fVectorize);
    
    // vérif types
    Type expr_type1 = expression1->get_expression_type(fParent_blocks);
    Type expr_type2 = expression2->get_expression_type(fParent_blocks);

    //*** INT ***
    if (expr_type1 == Type::INT && expr_type2 == Type::INT){

      // on empile le resultat
      code += "pushl %eax\n";
      
      code += expression2->get_code(fParent_blocks, fFunction, fVectorize);
      code += "movl %eax, %ecx \n";
      //on recupère le résultat de notre première expression
      code += "popl %eax\n";

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
    }

    //*** FLOAT ***
    else if (expr_type1 == Type::FLOAT && expr_type2 == Type::FLOAT){
      // on empile le resultat
      code += "subl $16, %esp \n";
      code += "movups %xmm0, (%esp) \n";
      
      code += expression2->get_code(fParent_blocks, fFunction, fVectorize);
      code += "movups %xmm0, %xmm1 \n";
      //on recupère le résultat de notre première expression
      code += "movups (%esp), %xmm0 \n";
      code += "addl $16, %esp \n";


      switch(type){
      case Binary_type::MULTIPLICATION:
	code += "mulps %xmm1, %xmm0\n";
	break;
	
      case Binary_type::ADDITION:
	code += "addps %xmm1, %xmm0\n";
	break;
	
      case Binary_type::SUBTRACTION:
	code += "subps %xmm1, %xmm0\n";
	break;
	
      case Binary_type::LOWER:
	code += "cmpltps %xmm1, %xmm0 \n";
	break;
	
      case Binary_type::GREATER:
	code += "cmpnleps %xmm1, %xmm0 \n";
	break;
	  
      case Binary_type::LOWER_EQUAL:
	code += "cmpleps %xmm1, %xmm0 \n";
	break;
	  
      case Binary_type::GREATER_EQUAL:
	code += "cmpnltps %xmm1, %xmm0 \n";
	break;
	  
      case Binary_type::EQUAL:
	code += "cmpeqps %xmm1, %xmm0 \n";
	break;
	  
      case Binary_type::NOT_EQUAL:
	code += "cmpneqps %xmm1, %xmm0 \n";
	break;
	
      }
      if (type >= Binary_type::LOWER){
	code += "subl  $4, %esp \n\
movss %xmm0, (%esp) \n\
cmpl $0, (%esp) \n\
setne %al \n\
movzbl %al, %eax \n\
addl $4, %esp \n";
      }

    }
    else{
      throw std::logic_error("only an int or a float can be used in a binary expression and the two operands have to be the same type");
    }
    return code;
  }


  virtual Type get_expression_type(std::vector<Block*> fParent_blocks) override{
    if (type >= Binary_type::LOWER){
      return Type::INT;
    }
    else{
      return expression1->get_expression_type(fParent_blocks);
    }
  }


private:
  Binary_type type;
  Expression* expression1;
  Expression* expression2;
};



#endif
