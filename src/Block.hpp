#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include "Statement.hpp"


enum class Type{
  INT, FLOAT, POINTER, FLOAT_POINTER, INT_ARRAY, FLOAT_ARRAY
    };

struct Declarator;

/* block de declaration et d'instruction
   un nouveau block est creer dans chaque {...}*/

class Block : public Statement{  
public:
  Block() {
  }

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) override;

  // indique si une variable s'appelant fIdentifier est déclaré dans ce bloc
  virtual bool is_variable(std::string fIdentifier);

  // retourne le type associé à la variable
  virtual Type get_variable_type(std::string fIdentifier);

  // renvoie l'instruction assembleur correspondant au chargement d'une variable dans un registre
  virtual std::string get_code_load_variable(std::string fIdentifier, std::string fRegister, Type fVar_type);

  // renvoie l'instruction assembleur correspondant au chargement d'une case d'un tableau dans un registre
  virtual std::string get_code_load_array(std::string fIdentifier, std::string fRegister_index, std::string fRegister_dest, Type fVar_type);

  // renvoie l'instruction assembleur correspondant au stockage d'un registre dans une variable
  virtual std::string get_code_store_variable(std::string fIdentifier, std::string fRegister, Type fVar_type);

  // renvoie l'instruction assembleur correspondant au stockage d'un registre dans une case de tableau
  virtual std::string get_code_store_array(std::string fIdentifier, std::string fRegister_index, std::string fRegister_value, Type fVar_type);

  void add_declaration(std::vector<Declarator*> fList);


protected:
  std::map<std::string, Declarator*> variables;
  
  //indique le sommet de la pile
  int top_stack_position = 0;

};


#endif
