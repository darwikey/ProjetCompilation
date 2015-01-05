#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "Statement.hpp"
#include "Expression.hpp"


class Selection : public Statement{
public:

  Selection(Expression* fExpression, Statement* fThen, Statement* fElse) : expression(fExpression),
									  then_statement(fThen),
									  else_statement(fElse){
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) override {
    std::string code = expression->get_code(fParent_blocks, fFunction);

    std::string label_else = this->get_unique_label();
    std::string label_end = this->get_unique_label();

    code += "cmpl $0, %eax\n";
    code += "je " + label_else + "\n";

    code += then_statement->get_code(fParent_blocks, fFunction);
    
    if (else_statement != nullptr){
      code += "jmp " + label_end + "\n";
    }

    code += label_else + ":\n";
    
    if (else_statement != nullptr){
      code += else_statement->get_code(fParent_blocks, fFunction);
      code += label_end + ":\n";      
    }

    return code;
  }


private:
  static std::string get_unique_label(){
    std::ostringstream str;
    str << "L" << label_counter;

    label_counter++;
    return str.str();
  }


  Expression* expression;
  Statement* then_statement;
  Statement* else_statement;

  static size_t label_counter;
};



#endif
