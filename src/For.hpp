#ifndef FOR_HPP
#define FOR_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Iteration.hpp"
#include "Expression.hpp"


class For : public Iteration{
public:

  For(Expression* fArg1, Expression* fArg2, Expression* fArg3, Statement* fBody){
    arguments[0] = fArg1;
    arguments[1] = fArg2;
    arguments[2] = fArg3;
    body = fBody;
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code;
    std::string label1 = Iteration::get_unique_label();
    std::string label2 = Iteration::get_unique_label();

    code += arguments[0]->get_code(fParent_blocks, fFunction);

    code += "jmp " + label1 + "\n";
    code += label2 + ":\n";
    
    code += body->get_code(fParent_blocks, fFunction);
    code += arguments[2]->get_code(fParent_blocks, fFunction);

    code += label1 + ":\n";
    code += arguments[1]->get_code(fParent_blocks, fFunction);
    code += "cmpl $0, %eax\n";
    code += "jne " + label2 + "\n";

    return code;
  }


private:
  Expression* arguments[3];

};

#endif
