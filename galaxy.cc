#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
/**
 * Skip the grammar part of the input.
 *
 * @param in the file pointer for reading input
 */
void skipLine(FILE* in) {
  char buf[256];
  fgets(buf, 256, in);
}

void skipGrammar(FILE* in) {
  int i, numTerm, numNonTerm, numRules;

  // read the number of terminals and move to the next line
  fscanf(in, "%d", &numTerm);
  skipLine(in);

  // skip the lines containing the terminals
  for (i = 0; i < numTerm; i++) {
    skipLine(in);
  }

  // read the number of non-terminals and move to the next line
  fscanf(in, "%d", &numNonTerm);
  skipLine(in);

  // skip the lines containing the non-terminals
  for (i = 0; i < numNonTerm; i++) {
    skipLine(in);
  }

  // skip the line containing the start symbol
  skipLine(in);

  // read the number of rules and move to the next line
  fscanf(in, "%d", &numRules);
  skipLine(in);

  // skip the lines containing the production rules
  for (i = 0; i < numRules; i++) {
    skipLine(in);
  }
}

char* trim(char* str) {
  int i, len;
  char* result;

  len = strlen(str);
  if (str[len-1] == '\n'){
    str[--len] = '\0';
  }

  for (i=len-1; i>=0 && ' '==str[i]; --i) len--;
  str[i+1] = '\0';

  result = str;
  for (i=0; i<len && ' '==str[i]; ++i) {
    result = result+1;
  }

  return result;
}



/**
 * Prints the derivation with whitespace trimmed.
 *
 * @param in the file pointer for reading input
 */
void printDerivation(FILE *in) {
  char line[256];
  int lineSize;

  while (1) {
    fgets(line, 256, in);
    if (feof(in)) break;
    printf ("%s\n", trim(line));
  }

}

bool isterm(std::string str){
  std::string terminal[6];
  terminal[0] = "BOF";
  terminal[1] = "EOF";
  terminal[2] = "id";
  terminal[3] = "-";
  terminal[4] = "(";
  terminal[5] = ")";

  std::string nterminal[3];
  nterminal[0] = "expr";
  nterminal[1] = "term";
  nterminal[2] = "S";

  for(int i = 0; i<6; i++){
    if(str == terminal[i]) return true;
  }
  return false;
}

int printResult(FILE* in){
  char line[256];
  int i = 0;
  fgets(line, 256, in);
  //if (feof(in)) return;
  char* each = trim(line);
  int len = strlen(each);
  for (; i<len && ' '!=each[0]; ++i) each = each+1;
  if(' '==each[0]) each = each + 1;
  std::string word;
  std::string copy = each;
  std::istringstream ss(copy);
  ss>>word;

  if(isterm(word)){//terminal
    if(word == "id") return 42;
    else if(word == "(") return printResult(stdin);
  }else{//non-terminal
    if(word == "term") return printResult(stdin);
    else if(word == "expr") return (printResult(stdin)-printResult(stdin));
  }
}
///////see whether the 1st one is terminal or non-terminal

/**
 * Reads a .cfg file and prints the left-canonical
 * derivation without leading or trailing spaces.
 */
int main(int argc, char** argv) {
  skipGrammar(stdin);
  //get rid off the 1st line
  char line[256];
  fgets(line, 256, stdin);

  int a = printResult(stdin);
  std::cout<<a<<std::endl;
  return 0;
}
