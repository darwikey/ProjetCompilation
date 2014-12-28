#ifndef DECLARATOR_HPP
#define DECLARATOR_HPP

#include <string>
#include <ostream>
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

  //indique la position de la variable dans la pile
  int stack_position = 0;

  // dans le cas d'un tableau
  size_t array_size = 0;

  // dans le cas d'un prototype de fonction
  bool is_function = false;
  std::vector<Declarator*> parameter_list;

  // fonctions de debug
  static std::string get_string (Declarator_type fType){
    switch (fType){
    default:
    case Declarator_type::VOID: return "void";
    case Declarator_type::INT: return "int";
    case Declarator_type::FLOAT: return "float";
    }
  }

  static std::string get_string (Declarator_structure fType){
    switch (fType){
    default:
    case Declarator_structure::VARIABLE: return "variable";
    case Declarator_structure::POINTER: return "pointer";
    case Declarator_structure::ARRAY: return "array";
    }
  }

  friend std::ostream& operator<< (std::ostream& fStream, const Declarator& fDeclarator){
    fStream << "name : " << fDeclarator.name << "\ttype : " << get_string(fDeclarator.type) << "\tstructure : " << get_string(fDeclarator.structure);

    if (fDeclarator.is_function){
      fStream << std::endl << "function prototype\t parameter : " << std::endl;
      for (Declarator* it: fDeclarator.parameter_list){
	fStream << "\t" << *it << std::endl;
      }  
    }

    return fStream;
  } 
  
};


#endif
