#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "Expression.hpp"
#include "Vectorized_for.hpp"


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

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) override {
    std::string code;

    // cas d'un tableau
    if (index != nullptr){
      code += index->get_code(fParent_blocks, fFunction);
      // on empile le resultat
      code += "pushl %eax\n";

      //Verif type
      if (index->get_expression_type(fParent_blocks) != Type::INT){
	throw std::logic_error("variable " + identifier + " can't be an index");
      }
    }

    Block* declaration_block = Expression::get_block(fParent_blocks, identifier);

    code += expression->get_code(fParent_blocks, fFunction, fVectorize);
    
    //Verif type
    Type expr_type = expression->get_expression_type(fParent_blocks);
    Type identifier_type = declaration_block->get_variable_type(identifier);
    
    if (index == nullptr){
      if (identifier_type == Type::INT && expr_type != Type::INT){
	throw std::logic_error("assignment for variable " + identifier + " : left expression must be an int");
      }
      else if (identifier_type == Type::FLOAT && expr_type != Type::FLOAT){
	throw std::logic_error("assignment for variable " + identifier + " : left expression must be an float");
      }
      else if ((identifier_type == Type::FLOAT_POINTER || identifier_type == Type::POINTER) 
	       && expr_type != Type::POINTER && expr_type != Type::FLOAT_POINTER){
	throw std::logic_error("assignment for variable " + identifier + " : left expression must be a pointer");
      }
      else if (identifier_type != Type::INT && identifier_type != Type::FLOAT && identifier_type != Type::POINTER && identifier_type != Type::FLOAT_POINTER){
	throw std::logic_error(identifier + " is not a int, a float, or a pointer");
      }
    }
    else{ // cas tableau
      if (identifier_type == Type::INT_ARRAY && expr_type != Type::INT){
	throw std::logic_error("assignment for array " + identifier + " : left expression must be an int");
      }
      else if (identifier_type == Type::FLOAT_ARRAY && expr_type != Type::FLOAT){
	throw std::logic_error("assignment for array " + identifier + " : left expression must be an float");
      }
      else if (identifier_type != Type::POINTER && identifier_type != Type::INT_ARRAY && identifier_type != Type::FLOAT_ARRAY){
	throw std::logic_error(identifier + " is not an array or a pointer");
      }
    }


    //*** Variable ***
    if (index == nullptr){
      // on regarde si c'est une boucle for vectorisé ou l'on doit réduire une variable
      bool vectorized_store = fVectorize && Vectorized_for::get_reduced_variable() == identifier;
      if (expr_type == Type::FLOAT){
	code += declaration_block->get_code_store_variable(identifier, "xmm0", expr_type, vectorized_store);
      }
      else{
	code += declaration_block->get_code_store_variable(identifier, "eax", expr_type, vectorized_store);
      }
    }
    //*** Tableau ***
    else{
      code += "movl %eax, %ecx \n";
      //on recupère l'index
      code += "popl %eax\n";

      if (expr_type == Type::FLOAT){
	code += declaration_block->get_code_store_array(identifier, "eax", "xmm0", expr_type, fVectorize);
      }
      else{
	code += declaration_block->get_code_store_array(identifier, "eax", "ecx", expr_type, fVectorize);
      }
    }

    return code;
  }

 
private:
  std::string identifier;
  Expression* expression;
  Expression* index;

};



#endif
