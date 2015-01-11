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
    }
    //TODO verif type

    // Si on retourne un float on l'empile sur la pile des floats
    if (expression->get_expression_type(fParent_blocks) == Type::FLOAT){
      code += "subl $4, %esp\n";
      code += "movss %xmm0, (%esp)\n";
      code += "flds (%esp)\n";
      code += "add $4, %esp\n";
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
