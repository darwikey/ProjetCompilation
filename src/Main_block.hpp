#ifndef MAIN_BLOCK_HPP
#define MAIN_BLOCK_HPP

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include "Block.hpp"


struct Declarator;
struct Function;

/* block principal contenant toute les variables globales et les declarations de fonctions*/

class Main_block : public Block{  
public:
  Main_block();

  virtual std::string get_code(std::vector<Block*> fParent_blocks, Function* fFunction, bool fVectorize = false) override;


  // renvoie l'instruction assembleur correspondant au chargement d'une variable dans un registre
  virtual std::string get_code_load_variable(std::string fIdentifier, std::string fRegister, Type fVar_type, bool fVectorize = false) override;

  // renvoie l'instruction assembleur correspondant au chargement d'une case d'un tableau dans un registre
  virtual std::string get_code_load_array(std::string fIdentifier, std::string fRegister_index, std::string fRegister_dest, Type fVar_type, bool fVectorize) override;

  // renvoie l'instruction assembleur correspondant au stockage d'un registre dans une variable
  virtual std::string get_code_store_variable(std::string fIdentifier, std::string fRegister, Type fVar_type, bool fVectorize = false) override;

  // renvoie l'instruction assembleur correspondant au stockage d'un registre dans une case de tableau
  virtual std::string get_code_store_array(std::string fIdentifier, std::string fRegister_index, std::string fRegister_value, Type fVar_type, bool fVectorize) override;

  //charge un float dans le registre xmm0
  static std::string get_code_load_float(float fValue);

  void add_function(Function* fFunction);
  
  Declarator* get_function_declarator(std::string fIdentifier);
  

private:
  // code source placé au debut du code assembleur
  std::string get_header();

  std::map<std::string, Function*> functions;

  static std::string code_data_float;
};


#endif
