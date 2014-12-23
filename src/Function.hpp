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

  std::string get_code() {
    std::string code;
    // entête de la fonction
    code += ".glob " + declarator->name + "\n";
    code += declarator->name + ":\n";
    
    // adresse de retour
    code += "pushl %ebp\n";
    code += "movl %esp %ebp\n\n";
    
    code += block->get_code();
      
    // fin de la fonction
    code += "leave\n";
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
