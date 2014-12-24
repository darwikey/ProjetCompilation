#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <string>
#include <ostream>
#include <vector>
#include "Block.hpp"
#include "Declarator.hpp"

class Block;

struct Function{  

  Function(Declarator* fDeclarator, Block* fBlock) : declarator(fDeclarator), block(fBlock) {
  }

  std::string get_code(std::vector<Block*> fParent_blocks) {
    std::string code;
    // entête de la fonction
    code += ".globl " + declarator->name + "\n";
    code += declarator->name + ":\n";
    
    // adresse de retour
    code += "pushl %ebp\n";
    code += "movl %esp, %ebp\n\n";

    // recherche des emplacements des arguments sur la pile
    int position = 8;
    for (Declarator* parameter : declarator->parameter_list) {
      parameter->stack_position = position;

      position += 4; // a modif pour les float et les tableaux
    }
    
    code += block->get_code(fParent_blocks, this);
      
    // fin de la fonction
    code += "\nleave\n";
    code += "ret\n\n";

    return code;
  }


  friend std::ostream& operator<< (std::ostream& fStream, const Function& fFunction){
    fStream << *fFunction.declarator << std::endl;

    return fStream;
  } 


  // contient le prototype de la fonction
  Declarator* declarator;
  
  Block* block;
};


#endif
