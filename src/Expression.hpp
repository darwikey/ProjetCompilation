#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdexcept>
#include "Statement.hpp"


class Expression : public Statement{
public:


  virtual std::string get_code() override {
    return "";
  }


private:


};



#endif
