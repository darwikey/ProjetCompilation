#ifndef VECT_FOR_HPP
#define VECT_FOR_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Iteration.hpp"
#include "Expression.hpp"


class Vectorized_for : public Iteration{
public:

  Vectorized_for(std::string fIterator, int fIt_begin, int fIt_end, Statement* fBody, std::string fReduced_variable = "") : iterator(fIterator), it_begin(fIt_begin), it_end(fIt_end){
    reduced_variable = fReduced_variable;
    body = fBody;
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) override {
    std::string code;
    std::ostringstream str1, str2;
    std::string label1 = Iteration::get_unique_label();
    std::string label2 = Iteration::get_unique_label();

    // chargement variable
    Block* declaration_block = Expression::get_block(fParent_blocks, iterator);
    str1  << "movl $" << it_begin << " ,%eax \n";
    code += str1.str();
    code += declaration_block->get_code_store_variable(iterator, "eax", Type::INT);

    code += "jmp " + label1 + "\n";
    code += label2 + ":\n";
    
    code += body->get_code(fParent_blocks, fFunction, true);
    
    // increment iterator
    code += declaration_block->get_code_load_variable(iterator, "eax", Type::INT);
    code += "addl $4, %eax\n";
    code += declaration_block->get_code_store_variable(iterator, "eax", Type::INT);

    code += label1 + ":\n";
    str2 << "cmpl $" << it_end << " , %eax\n";
    code += str2.str();
    code += "setl %al \n\
movzbl %al, %eax \n	\
cmpl $0, %eax\n";
    code += "jne " + label2 + "\n";
    
    return code;
  }


  static std::string get_reduced_variable(){
    return reduced_variable;
  }

protected:
  std::string iterator;
  int it_begin;
  int it_end;
  static std::string reduced_variable;
};

#endif
