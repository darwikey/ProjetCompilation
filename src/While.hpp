#ifndef WHILE_HPP
#define WHILE_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Iteration.hpp"
#include "Expression.hpp"


class While : public Iteration{
public:

  While(Expression* fExpression, Statement* fBody) : expression(fExpression){
    body = fBody;
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code;
    std::string label1 = Iteration::get_unique_label();
    std::string label2 = Iteration::get_unique_label();

    code += "jmp " + label1 + "\n";
    code += label2 + ":\n";
    
    code += body->get_code(fParent_blocks, fFunction);

    code += label1 + ":\n";
    code += expression->get_code(fParent_blocks, fFunction);
    code += "cmpl $0, %eax\n";
    code += "jne " + label2 + "\n";

    return code;
  }


private:
  Expression* expression;

};

#endif
