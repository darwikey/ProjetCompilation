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

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code;

    if (expression != nullptr){
      code += expression->get_code(fParent_blocks, fFunction);
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
