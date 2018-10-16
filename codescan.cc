//#include "scanner.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <cstdint>
/**
 * C++ Starter code for CS241 A3
 * Written by Sean Harrap in Winter 2018
 * All code requires C++14, so if you're getting
 * compile errors make sure to use -std=c++14.
 *
 * This file contains helpers for asm.cc and
 * should not need to be modified by you.
 */

namespace CS241 {

  class Token {
    private:
      std::string kind;
      std::string lexeme;

    public:
      Token(std::string kind, std::string lexeme);

      std::string getKind() const;
      std::string getLexeme() const;
      long long toLong() const;
      int toInt() const;
  };

  class ScanningFailure {
    std::string message;
    public:
      ScanningFailure(std::string message);

      std::string getMessage() const;
  };

  class AsmDFA {
    private:
      std::set<std::string> acceptingStates;
      std::vector<Token> simplifiedMaximalMunch(std::string input) const;

    public:
      AsmDFA();
      std::string transition(std::string state, char nextChar) const;
      bool failed(std::string state) const;
      bool accept(std::string state) const;
      std::string start() const;
      std::vector<Token> scan(std::string input) const;
  };

Token::Token(std::string kind, std::string lexeme):kind(kind), lexeme(lexeme) {}

std::string Token::getKind() const { return kind; }
std::string Token::getLexeme() const { return lexeme; }

long long Token::toLong() const {
  std::istringstream iss;
  long long result;

  if (kind == "INT") {
    iss.str(lexeme);
  } else if (kind == "HEXINT") {
    iss.str(lexeme.substr(2));
    iss >> std::hex;
  } else if (kind == "REG") {
    iss.str(lexeme.substr(1));
  } else {
    // This should never happen if the user calls this function correctly
    return 0;
  }

  iss >> result;
  return result;
}

int Token::toInt() const {
  if (kind == "REG") {
    return (int)toLong();
  } else {
    // This should never happen if the user calls this function correctly
    return 0;
  }
}

ScanningFailure::ScanningFailure(std::string message):message(message) {}

std::string ScanningFailure::getMessage() const { return message; }

AsmDFA::AsmDFA() {
  acceptingStates = {"ID", "NUM", "LPAREN", "RPAREN", "LBRACE",
                    "RBRACE", "ZERO", "PLUS", "MINUS", "STAR", "SLASH", "PCT",
                    "COMMA","SEMI","RETURN","IF","INT","WHILE","LT","GT","BECOMES",
                    "LE","GE","EQ","NE","ELSE","R","RE","RET","RETU","RETUR","W",
                    "WH", "WHI", "WHIL", "I","IN", "E", "EL","ELS", "COMMENT", "EXCLAMATION"};
  //Non-accepting states are "DOT", "MINUS", "ZEROX", "DOLLARS", "START"
}

// The transition function for the DFA. Could be implemented
// using a map or something more fancy, but this is implemented using nested
// if statements for simplicity.
std::string AsmDFA::transition(std::string state, char nextChar) const {
    if (isalpha(nextChar)&&
        ((nextChar !='r')&&(nextChar !='i')&&
        (nextChar !='e')&&(nextChar !='w'))&&(state == "START"))     return "ID";
    else if ((isdigit(nextChar)||isalpha(nextChar))&&(state == "ID"))  return "ID";
    else if ((nextChar == '0')&&(state == "START"))   return "ZERO";
    else if ((state == "ZERO")&& (isdigit(nextChar)||isalpha(nextChar))) throw ScanningFailure("ERROR: Exception");

    else if (isdigit(nextChar)&&(state=="NUM")) return "NUM";
    else if (isdigit(nextChar)&&(nextChar!='0')&&(state == "START")) return "NUM";
    else if (isalpha(nextChar)&&(state=="NUM")) throw ScanningFailure("ERROR: Exception");

    else if ((nextChar == '(')&&(state == "START"))   return "LPAREN";
    else if ((nextChar == ')')&&(state == "START"))   return "RPAREN";
    else if ((nextChar == '{')&&(state == "START"))   return "LBRACE";
    else if ((nextChar == '}')&&(state == "START"))   return "RBRACE";
    else if ((nextChar == '+')&&(state == "START"))   return "PLUS";
    else if ((nextChar == '-')&&(state == "START"))   return "MINUS";
    else if ((nextChar == '*')&&(state == "START"))   return "STAR";
    else if ((nextChar == '/')&&(state == "START"))   return "SLASH";
    else if ((nextChar == '%')&&(state == "START"))   return "PCT";
    else if ((nextChar == ',')&&(state == "START"))   return "COMMA";
    else if ((nextChar == ';')&&(state == "START"))   return "SEMI";
    else if ((nextChar == '/')&&(state == "SLASH"))   return "COMMENT";
    else if (state =="COMMENT") return "COMMENT";

    else if ((nextChar == '<')&&(state == "START"))   return "LT";
    else if ((nextChar == '>')&&(state == "START"))   return "GT";
    else if ((nextChar == '=')&&(state == "START"))   return "BECOMES";
    else if ((nextChar == '!')&&(state == "START"))   return "EXCLAMATION";
    else if ((nextChar == '=')&&(state == "LT")) return "LE";
    else if ((nextChar == '=')&&(state == "GT")) return "GE";
    else if ((nextChar == '=')&&(state == "BECOMES")) return "EQ";
    else if ((nextChar == '=')&&(state == "EXCLAMATION")) return "NE";
    else if (((state == "LE")||(state == "GE")||(state == "LT")||(state == "GT")||(state == "BECOMES"))
            &&((nextChar=='>')||(nextChar=='<'))) throw ScanningFailure("ERROR: Exception");
    else if (((state == "LE")||(state == "GE")||(state == "EQ")||(state == "LT")||(state == "GT"))
            &&(nextChar=='=')) throw ScanningFailure("ERROR: Exception");


    else if((state == "START")&&(nextChar == 'r')) return "R";
    else if((state == "START")&&(nextChar == 'e')) return "E";
    else if((state == "START")&&(nextChar == 'w')) return "W";
    else if((state == "START")&&(nextChar == 'i')) return "I";

    else if(state == "R"){if(nextChar == 'e') return "RE"; else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "RE"){if(nextChar == 't') return "RET"; else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "RET"){if(nextChar == 'u') return "RETU";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "RETU"){if(nextChar == 'r') return "RETUR";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "RETUR"){if(nextChar == 'n') return "RETURN";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}

    else if(state == "E"){if(nextChar == 'l') return "EL";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "EL"){if(nextChar == 's') return "ELS";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "ELS"){if(nextChar == 'e') return "ELSE";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}


    else if(state == "W"){if(nextChar == 'h') return "WH";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "WH"){if(nextChar == 'i') return "WHI";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "WHI"){if(nextChar == 'l') return "WHIL";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "WHIL"){if(nextChar == 'e') return "WHILE";else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}


    else if(state == "I"){if(nextChar == 'f') return "IF"; else if(nextChar == 'n') return "IN"; else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if(state == "IN"){if(nextChar == 't') return "INT"; else if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}
    else if((state == "ELSE")||(state == "WHILE")||(state == "IF")||(state == "INT")||(state == "RETURN")){if(isdigit(nextChar)||isalpha(nextChar)) return "ID";}

  return "FAIL";
}

bool AsmDFA::failed(std::string state) const {
  return state == "FAIL";
}

bool AsmDFA::accept(std::string state) const {
  return acceptingStates.count(state) > 0;
}

std::string AsmDFA::start() const {
  return "START";
}

std::vector<Token> AsmDFA::simplifiedMaximalMunch(std::string input) const {
  std::vector<Token> result;

  std::string state = start();
  std::string munched_input;

  // We can't use a range-based for loop effectively here
  // since the iterator doesn't always increment.
  for (std::string::iterator input_posn = input.begin();
       input_posn != input.end();) {

      std::string old_state = state;
      state = transition(state, *input_posn);
        //
        // std::cout<<"old state:"<<old_state<<std::endl;
        // std::cout<<"state:"<<state<<std::endl;
        // std::cout<<"input:"<<*input_posn<<std::endl<<std::endl;
      if (!failed(state)) {  //succeed
        munched_input += *input_posn;
        old_state = state;

        ++input_posn;
      }



      if (input_posn == input.end() || failed(state)) {
        if (accept(old_state)) {
          if (old_state=="ZERO") {old_state = "NUM";}
          else if ((old_state=="E")||(old_state=="EL")||(old_state=="ELS")||
                  (old_state=="W")||(old_state=="WH")||(old_state=="WHI")||(old_state=="WHIL")||
                  (old_state=="I")||(old_state=="IN")||(old_state=="R")||
                  (old_state=="RE")||(old_state=="RET")||(old_state=="RETU")||(old_state=="RETUR")){
                    old_state = "ID";}
          result.push_back(Token(old_state, munched_input));
          munched_input = "";
          state = start();
          if(input_posn == input.end()){ break;}
          if(((*input_posn) == ' ')||((*input_posn) == 9)) ++input_posn;
        }else if((*input_posn) == ' '||((*input_posn) == 9)){
          munched_input = "";
          state = start();
          ++input_posn;
        }else {
          if (failed(state)) {
            munched_input += *input_posn;
          }

          throw ScanningFailure("ERROR: Simplified maximal munch failed on input: "
                               + munched_input);
        }
      }

  }
  return result;
}

std::vector<Token> AsmDFA::scan(std::string input) const {
  std::vector<Token> tokens = simplifiedMaximalMunch(input);
  std::vector<Token> newTokens;

  for (auto &token : tokens) {
      if(token.getKind() != "COMMENT"){
          if(token.getKind() == "EXCLAMATION"){
            throw ScanningFailure("ERROR: Simplified maximal munch failed on input: !");
          }
          newTokens.push_back(Token(token.getKind(),token.getLexeme()));
      }
      //we do not check the content of tokens here
  }

  return newTokens;
}
}



class AssemblingFailure {
  std::string message;
  public:
    AssemblingFailure(std::string message):message(message) {}

    std::string getMessage() const { return message; }
};

void assemble(std::vector<std::vector<CS241:: Token>> &tokenLines) {
  for (auto &line : tokenLines) {
    // For now just print out the tokens, one line at a time.
    // You will probably want to replace this with your code.
    for (auto & token : line) {
      std::cout <<token.getKind() << " " << token.getLexeme()<<std::endl;
    }
  }
}

int main() {
  CS241::AsmDFA theDFA;
  std:: vector<std::vector<CS241::Token>> tokenLines;
  std:: string line;

  try {
    while (getline(std::cin, line)) {
      tokenLines.push_back(theDFA.scan(line));
    }
  } catch (CS241::ScanningFailure &f) {
    std::cerr << f.getMessage() << std::endl;

    return 1;
  }

  try {
    assemble(tokenLines);
  } catch (AssemblingFailure &f) {
    std::cerr << f.getMessage() << std::endl;

    return 1;
  }

  return 0;
}
