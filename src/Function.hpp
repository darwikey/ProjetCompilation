#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <string>
#include <ostream>
#include <vector>


struct Function{  

  Function(Declarator* fDeclarator) : declarator(fDeclarator)
  {
  }

  // contient le prototype de la fonction
  Declarator* declarator;


  friend std::ostream& operator<< (std::ostream& fStream, const Function& fFunction){
    fStream << *fFunction.declarator << std::endl;

    return fStream;
  } 
  
};


#endif
