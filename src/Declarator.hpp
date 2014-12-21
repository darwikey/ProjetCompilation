#ifndef DECLARATOR_HPP
#define DECLARATOR_HPP

#include <string>
#include <vector>


enum class Declarator_type{
  VOID, INT, FLOAT
};

enum class Declarator_structure{
  VARIABLE, POINTER, ARRAY
};

struct Declarator{  

  Declarator(std::string fName, Declarator_structure fStructure) : name(fName), 
  structure(fStructure){
}

  std::string name;
  Declarator_type type = Declarator_type::VOID;
  Declarator_structure structure;

  // dans le cas d'un prototype de fonction
  bool is_function = false;
  std::vector<Declarator*> parameter_list;
};


#endif
