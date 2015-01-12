#ifndef JUMP_HPP
#define JUMP_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Statement.hpp"
#include "Expression.hpp"


class Jump : public Statement{
public:

  Jump(Expression* fExpression = nullptr) : expression(fExpression){
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) override {
    std::string code;

    if (expression != nullptr){
      code += expression->get_code(fParent_blocks, fFunction);

      //Verif type
      Type expr_type = expression->get_expression_type(fParent_blocks);

      if (fFunction->declarator->structure == Declarator_structure::VARIABLE){
	if (fFunction->declarator->type == Declarator_type::INT && expr_type != Type::INT)
	  throw std::logic_error("function must return a int");
	else if (fFunction->declarator->type == Declarator_type::FLOAT && expr_type != Type::FLOAT)
	  throw std::logic_error("function must return a float");
	else if (fFunction->declarator->type == Declarator_type::VOID)
	  throw std::logic_error("void function can't return a value");	  
      }
      else if (fFunction->declarator->structure == Declarator_structure::POINTER && !(expr_type == Type::POINTER || expr_type == Type::FLOAT_POINTER)){
	throw std::logic_error("function must return a pointer");
      }
      
      // Si on retourne un float on l'empile sur la pile des floats
      if (expr_type == Type::FLOAT){
	code += "subl $4, %esp\n";
	code += "movss %xmm0, (%esp)\n";
	code += "flds (%esp)\n";
	code += "add $4, %esp\n";
      }
    }
    else{ // return;
      
      // Verif type
      if (!(fFunction->declarator->type == Declarator_type::VOID && fFunction->declarator->structure != Declarator_structure::VARIABLE)){
	throw std::logic_error("non void function have to return a value");
      }
    }
      
    // fin de la fonction
    code += "leave\n";
    code += "ret\n\n";


    return code;
  }


private:
  Expression* expression;

};



#endif
