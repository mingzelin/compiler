#include "scanner.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <map>
using std::vector;
using std::string;
using CS241::Token;

/**
 * C++ Starter code for CS241 A3
 * Written by Sean Harrap in Winter 2018
 * All code requires C++14, so if you're getting
 * compile errors make sure to use -std=c++14.
 *
 * This file contains the main function of your program.
 * You will want to begin by modifying the assemble() function,
 * and are welcome to add more functions and files as needed.
 */

// You can throw this class to signal exceptions encountered while assembling
class AssemblingFailure {
  std::string message;
  public:
    AssemblingFailure(std::string message):message(message) {}

    std::string getMessage() const { return message; }
};

/* Scans some input (can be the entire file or just one line)
   * and produces the corresponding list of tokens.
   *
   * Scan returns tokens of the following kinds:
   * ID: Identifiers and keywords.                        jr, jalr, labelname
   * LABEL: Labels (identifiers ending in a colon).       label(init)
   * WORD: The special ".word" keyword.                   .word
   * COMMA: A comma.
   * LPAREN: A left parenthesis.
   * RPAREN: A right parenthesis.
   * INT: A signed or unsigned 32-bit integer written in decimal.
   * HEXINT: An unsigned 32-bit integer written in hexadecimal.
   * REG: A register between $0 and $31.
   */

void assemble(vector<vector<Token>> &tokenLines) {
  int index = 0;

  int s, t, d, count = -1, allowComma = 0;
  long long i;
  std:: map<string, int> m;
  //pass 1: insert symbol table
  for (auto &line: tokenLines){
    if(line.size() == 0)continue;
    for(int i = 0; i< line.size(); i++){
      Token& tmp = line[i];
      if(tmp.getKind() == "LABEL"){
        if(m.count(tmp.getLexeme().substr(0, (tmp.getLexeme().length() - 1)))) {std:: cerr<<"ERROR: duplicate label"<<std:: endl; return;}
        else m[tmp.getLexeme().substr(0, (tmp.getLexeme().length() - 1))] = index;
      }
      if((tmp.getKind() == "WORD")||(tmp.getLexeme() == "jr")||(tmp.getLexeme() == "jalr")||
         (tmp.getLexeme() == "sub")||(tmp.getLexeme() == "add")||(tmp.getLexeme() == "slt")||
         (tmp.getLexeme() == "sltu")||(tmp.getLexeme() == "beq")||(tmp.getLexeme() == "bne")||
         (tmp.getLexeme() == "mflo")||(tmp.getLexeme() == "lis")||(tmp.getLexeme() == "mfhi")||
         (tmp.getLexeme() == "mult")||(tmp.getLexeme() == "multu")||
         (tmp.getLexeme() == "div")|| (tmp.getLexeme() == "divu")||
         (tmp.getLexeme() == "lw")||(tmp.getLexeme() == "sw")){
          index+=4;
      }
    }
  }
  index = 0;
  //pass 2
  for (auto &line: tokenLines){
      if(line.size() == 0)continue;
      long long word;
      int labelsize = 0;
      Token& tk = line[0];   //tk is the only effective keyword
      if((tk.getKind() == "WORD")||(tk.getLexeme() == "jr")||(tk.getLexeme() == "jalr")||
         (tk.getLexeme() == "sub")||(tk.getLexeme() == "add")||(tk.getLexeme() == "slt")||
         (tk.getLexeme() == "sltu")||(tk.getLexeme() == "beq")||(tk.getLexeme() == "bne")||
         (tk.getLexeme() == "mflo")||(tk.getLexeme() == "lis")||(tk.getLexeme() == "mfhi")||
         (tk.getLexeme() == "mult")||(tk.getLexeme() == "multu")||
         (tk.getLexeme() == "div")|| (tk.getLexeme() == "divu")||
         (tk.getLexeme() == "lw")||(tk.getLexeme() == "sw")){
          index+=4;
      }
      for(int j = 0; j< line.size(); j++){
        Token& tmp = line[j];
        if(((tmp.getLexeme() == "lw")||(tmp.getLexeme() == "sw")||(tmp.getLexeme() == "mult")||
        (tmp.getLexeme() == "multu")||(tmp.getLexeme() == "div")||(tmp.getLexeme() == "mfhi")||
          (tmp.getLexeme() == "divu")||(tmp.getLexeme() == "mflo")||(tmp.getLexeme() == "lis")||
           (tmp.getKind() == "WORD")||(tmp.getLexeme() == "jr")||(tmp.getLexeme() == "jalr")||
           (tmp.getLexeme() == "sub")||(tmp.getLexeme() == "add")||(tmp.getLexeme() == "slt")||
           (tmp.getLexeme() == "sltu")||(tmp.getLexeme() == "beq")||(tmp.getLexeme() == "bne"))&&(j!=line.size()-1)){
          if(tk.getKind() == "LABEL"){ tk = tmp;} //tk is locked here
          if((tmp.getKind() == "WORD")||(tmp.getLexeme() == "jr")||(tmp.getLexeme() == "jalr")||
          (tmp.getLexeme() == "mflo")||(tmp.getLexeme() == "lis")||(tmp.getLexeme() == "mfhi")) {
            //2 element instructions
            if((line.size()-labelsize)!= 2){std::cerr <<"ERROR: token amount1"<< std::endl; return;}
          }else if((tmp.getLexeme() == "mult")||(tmp.getLexeme() == "multu")||
                  (tmp.getLexeme() == "div")|| (tmp.getLexeme() == "divu")){
            //3 element instructions
            if((line.size()-labelsize)!= 4){std::cerr <<"ERROR: token amount2"<< std::endl; return;}
          }else if((tmp.getLexeme() == "lw")||(tmp.getLexeme() == "sw")){
            //lw, sw
            if((line.size()-labelsize)!= 7){std::cerr <<"ERROR: token amount4"<< std::endl; return;}
          }else{
            //4 element instructions
            if((line.size()-labelsize)!= 6){std::cerr <<"ERROR: token amount3"<< std::endl; return;}
          }
          continue;
        }

        if(tk.getKind() == "LABEL"){
          labelsize++;
        }else{
          if(tk.getKind() == "WORD"){
            if((tmp.getKind() == "INT")||(tmp.getKind() == "HEXINT")) word = tmp.toLong();
            else if(tmp.getKind() == "ID"){
              if(m.count(tmp.getLexeme())) word = m[tmp.getLexeme()];
              else{std::cerr <<"ERROR: undefined Label"<< std::endl; return; }
            }else {std:: cerr<<"ERROR: invalid composition"<<std:: endl; return; }
          }else if(tk.getLexeme() == "jr"){
            if(tmp.getKind() == "REG"){
              s = tmp.toInt();
            }else{std:: cerr<<"ERROR: invalid composition"<<std:: endl; return; }
            word = (0<<26)|(s<<21)|(0<<5)|8;
          }else if(tk.getLexeme() == "jalr"){
            if(tmp.getKind() == "REG"){
              s = tmp.toInt();
            }else{std:: cerr<<"ERROR: invalid composition"<<std:: endl; return; }
            word = (0<<26)|(s<<21)|(0<<4)|9;
          }else if((tk.getLexeme() == "add")||(tk.getLexeme() == "sub")||
                   (tk.getLexeme() == "slt")||(tk.getLexeme() == "sltu")){
            if(tmp.getKind() == "REG"){
              if(count == -1){count = 1;}
              else{ ++count;}
              if(count == 1){ d = tmp.toInt();
              }else if(count == 2){ s = tmp.toInt();
              }else if(count == 3){
                t = tmp.toInt();
                if(tk.getLexeme() == "add")word = (0<<26|s<<21|t<<16|d<<11|32);
                else if(tk.getLexeme() == "sub")word = (0<<26|s<<21|t<<16|d<<11|34);
                else if(tk.getLexeme() == "slt")word = (0<<26|s<<21|t<<16|d<<11|42);
                else if(tk.getLexeme() == "sltu")word =(0<<26|s<<21|t<<16|d<<11|43);
                count = -1;
              }
            }else if(allowComma&&(tmp.getKind() == "COMMA")){
              allowComma = 0;
              continue;
            }else{std:: cerr<<"ERROR: invalid composition"<<std:: endl; return; }
          }else if((tk.getLexeme() == "beq")||(tk.getLexeme() == "bne")){
            if(tmp.getKind() == "REG"){
              if(count == -1){count = 1;}
              else{ ++count;}
              if(count == 1){ s = tmp.toInt();
              }else if(count == 2){ t = tmp.toInt(); }
              else{std::cerr <<"ERROR: invalid format"<< std::endl; return; }
            }else if(allowComma&&(tmp.getKind() == "COMMA")){
              allowComma = 0;
              continue;
            }else if((count==2)&&((tmp.getKind() == "INT")||(tmp.getKind() == "HEXINT")||(tmp.getKind() == "ID"))){
              if(tmp.getKind() == "ID"){
                if(m.count(tmp.getLexeme())){
                  i = m[tmp.getLexeme()];
                  i = (i - index)/4;
                  if((i< -32768)||(i > 32767)){std::cerr <<"ERROR: invalid int"<< std::endl; return;}
                }
                else{std::cerr <<"ERROR: undefined Label"<< std::endl; return; }
              }else i = tmp.toLong();
              if((tmp.getKind() == "INT")&&((i< -32768)||(i > 32767))){std::cerr <<"ERROR: invalid int"<< std::endl; return;}
              else if((tmp.getKind() == "HEXINT")&&(0xffff<i)){std::cerr <<"ERROR: invalid int"<< std::endl; return; }

              if(tk.getLexeme() == "beq")word = ((i & 0xffff)|4<<26|s<<21|t<<16);
              else if(tk.getLexeme() == "bne")word = ((i & 0xffff)|5<<26|s<<21|t<<16);
              count = -1;
            }else{std:: cerr<<"ERROR: invalid composition"<<std::endl; return; }
          }else if((tk.getLexeme() == "mflo")||(tk.getLexeme() == "lis")||(tk.getLexeme() == "mfhi")){
            if(tmp.getKind() == "REG"){
              d = tmp.toInt();
              if(tk.getLexeme() == "mfhi"){
                word = (0<<16)|(d<<11)|16;
              }else if(tk.getLexeme() == "mflo"){
                word = (0<<16)|(d<<11)|18;
              }else if(tk.getLexeme() == "lis"){
                word = (0<<16)|(d<<11)|20;
              }
            }else{std:: cerr<<"ERROR: invalid composition"<<std::endl; return; }
          }else if((tk.getLexeme() == "mult")||(tk.getLexeme() == "multu")||
                  (tk.getLexeme() == "div")|| (tk.getLexeme() == "divu")) {

            if(tmp.getKind() == "REG"){
              if(count == -1){count = 1;}
              else{ ++count;}
              if(count == 1){ s = tmp.toInt();
              }else if(count == 2){
                t = tmp.toInt();
                count = -1;
                if(tk.getLexeme() == "mult"){
                  word = (0<<26)|(s<<21)|(t<<16)|24;
                }else if(tk.getLexeme() == "multu"){
                  word = (0<<26)|(s<<21)|(t<<16)|25;
                }else if(tk.getLexeme() == "div"){
                  word = (0<<26)|(s<<21)|(t<<16)|26;
                }else if(tk.getLexeme() == "divu"){
                  word = (0<<26)|(s<<21)|(t<<16)|27;
                }
              }else{std::cerr <<"ERROR: invalid format"<< std::endl; return; }
            }else if(allowComma&&(tmp.getKind() == "COMMA")){
              allowComma = 0;
              continue;
            }else{std:: cerr<<"ERROR: invalid composition"<<std::endl; return; }

          }else if((tk.getLexeme() == "lw")||(tk.getLexeme() == "sw")){
            if(tmp.getKind() == "REG"){
              if(count == -1){count = 1;}
              else{ ++count;}
              if(count == 1){ t = tmp.toInt();
              }else if(count == 5){ s = tmp.toInt();
              }else{std:: cerr<<"ERROR: invalid composition"<<std::endl; return; }
            }else if((count==1)&&(tmp.getKind() == "COMMA")){
              ++count;
              allowComma = 0;
              continue;
            }else if((count==2)&&((tmp.getKind() == "HEXINT")||(tmp.getKind() == "INT"))){
              ++count;
              i = tmp.toLong();
              if((tmp.getKind() == "HEXINT")&&(0xffff<i)){std::cerr <<"ERROR: invalid int"<< std::endl; return;}
              if((tmp.getKind() == "INT")&&((i< -32768)||(i > 32767))){std::cerr <<"ERROR: invalid int"<< std::endl; return;}
            }else if((count==3)&&(tmp.getLexeme() == "(")){ ++count; continue;
            }else if((count==5)&&(tmp.getLexeme() == ")")){
              count = -1;
              if(tk.getLexeme()=="lw"){ word = (i & 0xffff)|(35<<26)|(s<<21)|(t<<16);
              }else if(tk.getLexeme()=="sw"){ word = (i & 0xffff)|(43<<26)|(s<<21)|(t<<16);  }

            }else{
              std:: cerr<<"ERROR: invalid composition"<<std::endl; return; }
          }else{std::cerr <<"ERROR: invalid input"<< std::endl; return; }
          allowComma = 1;
          if(count == -1){
            putchar((word >> 24) & 0xff);
            putchar((word >> 16) & 0xff);
            putchar((word >> 8) & 0xff);
            putchar(word & 0xff);
          }
        }
      }
    }

    //print label finally
    for(auto& elem : m){
      std::cerr <<elem.first<< " "<<elem.second<< std::endl;
    }
}

// Convert the input into a sequence of tokens.
// This should not need to be modified, but you can if you want to.
int main() {
  CS241::AsmDFA theDFA;
  vector<vector<Token>> tokenLines;
  string line;

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
