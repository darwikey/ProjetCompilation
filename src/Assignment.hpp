#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Expression.hpp"


class Assignment : public Expression{
public:

  // pour les variables
  // fIdentifier = fExpression
  Assignment(std::string fIdentifier, Expression* fExpression) : identifier(fIdentifier),
								 expression(fExpression),
								 index(nullptr){
  }
  
  // Pour les tableaux
  // fIdentifier[fIndex] = fExpression
  Assignment(std::string fIdentifier, Expression* fIndex, Expression* fExpression) : identifier(fIdentifier),
								 expression(fExpression),
								 index(fIndex){
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction) override {
    std::string code;

    // cas d'un tableau
    if (index != nullptr){
      code += index->get_code(fParent_blocks, fFunction);
      // on empile le resultat
      code += "pushl %eax\n";
    }

    Block* declaration_block = Expression::get_block(fParent_blocks, identifier);

    code += expression->get_code(fParent_blocks, fFunction);
    
    if (index == nullptr){
      code += declaration_block->get_code_store_variable(identifier, "eax");
    }
    else{
      code += "movl %eax, %ecx \n";
      //on recupère l'index
      code += "popl %eax\n";
      code += declaration_block->get_code_store_array(identifier, "eax", "ecx");
    }

    return code;
  }


private:
  std::string identifier;
  Expression* expression;
  Expression* index;

};



#endif
