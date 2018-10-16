#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class TNode{
public:
    vector<TNode*> children;
    string str = "";
    //vector<vector> code;
  public:
    TNode() = default;
    TNode(const TNode& n):str(n.str){
      for(int i=0; i<n.children.size();i++){
        children.emplace_back(n.children[i]);
      }
    }
    ~TNode(){
      for(int i=0; i<children.size();i++){
        delete children[i];
      }
      children.clear();
    }
};

void print(TNode* t, int s){

  for(int i=0; i<t->children.size(); i++){
    print(t->children[i], s+1);
  }
  cout<<t->str<<endl;
}

int isTvector(string word, vector<string>& t){
  for(auto item: t){
    if(word==item)return 1;
  }
  return 0;
}
int hasword(string str){
  istringstream ss(str);
  int result = 0;
  string word;
  while(ss>>word){
    result++;
  }
  return result;
}

void build(TNode* c, vector<string>& trans){
  string tmp;
  int cycle = 1;
  if(isTvector(c->str, trans)) cycle = hasword(c->str)-1;
  for(int i=0; i<cycle; i++){
    getline(cin, tmp);
    if((isTvector(tmp, trans))&&(cycle!=0)) {
      TNode* t = new TNode;
      t->str = tmp;
      c->children.emplace_back(t);
      build(t, trans);
    }else{
      TNode* t = new TNode;
      t->str = tmp;
      c->children.emplace_back(t);
    }
  }
}

void push(int s){
  cout<<"sw $" <<s << ", -4($30)\nsub $30, $30, $4\n";
}

void pop(int s){
  cout<<"add $30, $30, $4\nlw $" << s << ", -4($30)\n";
}

void getVariable(int v, int s){
  cout<<"lw $" << s <<", " << v << "($29)\n";
}

void getcharFunc(){
  cout<<"Fgetchar: lis $5\n.word 0xffff0004\nlw $3, 0($5)\njr $31"<<endl;
}

void putcharFunc(){
  cout<<"Fputchar: lis $5\n.word 0xffff000c\nsw $1, 0($5)\njr $31"<<endl;
}


//returns 0 or the register number
int isDuplicate(vector<pair<string, int>>& symtable, string str){
  for(auto& item: symtable){
    if(item.first == str){
      return item.second;
    }
  }
  return 0;
}

int inRange(long long value){
  if((-2147483647<=value)&&(value<=2147483647)){
    return 1;
  }
  return 0;
}

int wordcount(string str){
  string tmp;
  istringstream ss(str);
  int count = 0;
  while(ss>>tmp){
    count++;
  }
  return count;
}


void code(TNode& node, vector<pair<string, int>>& symtable, vector<pair<string, int>>& functable){
  static int offset = -4;
  static int nameorder = 0;
  static int paramnum = 0;
  string tmp;
  if(node.str == "start BOF procedures EOF"){
    cout<<"lis $11\n.word 1\nlis $4\n.word 4\n";
    push(31);
    cout<<"lis $31\n.word Fwain"<<"\njalr $31\n";
    pop(31);
    cout<<"jr $31\n";
    for(auto &item: node.children) code(*item, symtable, functable);
    getcharFunc();
    putcharFunc();
  }else if(node.str == "procedures procedure procedures"){
    for(auto &item: node.children) code(*item, symtable, functable);
  }else if(node.str == "procedures"
          ||node.str == "dcls"
          ||node.str == "statements"){

  }else if(node.str == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
    vector<pair<string, int>> newtable;
    vector<pair<string, int>> ftable;
    string name = node.children[1]->str;
    istringstream ss(name);
    ss>>tmp;
    ss>>tmp;
    cout<<"F"<<tmp<<":"<<endl;
    offset = -4;
    push(31);
    cout<<"add $29, $30, $0"<<endl;
    cout<<"sub $29, $29, $4"<<endl;
    push(29);
    paramnum = wordcount(node.children[3]->str);


    pair<string, int> fpair;
    string idpart = node.children[1]->str;
    istringstream sst(idpart);
    sst>>tmp;
    sst>>fpair.first;
    fpair.second = paramnum;

    for(auto& item: functable){
      if((item.first==fpair.first)&&(item.second == fpair.second)){
        string exc = "ERROR: duplicate functions";
        throw exc;
      }
    }
    functable.emplace_back(fpair);

    if(paramnum == 4){
      push(1);
      push(2);
    }else if(paramnum== 2){
      push(1);
    }else if(paramnum== 1){

    }else{
      string ex = "ERROR: weird";
      throw ex;
    }

    for(auto &item: node.children) code(*item, newtable, functable);
    for(int i=0; i<newtable.size()+2; i++){
      pop(31);
    }
    cout<<"jr $31\n";
  }else if(node.str == "params dcl COMMA dcl"){
    for(auto &item: node.children) code(*item, symtable, functable);
  }else if(node.str == "dcl INT ID"){
    //construct sym table
    pair<string, int> p;
    string str = node.children[1]->str;
    istringstream ss(str);
    ss>>tmp;
    ss>> p.first;
    if(isDuplicate(symtable, p.first)){
      string s="ERROR: duplicate";
      throw s;
    }
    p.second = offset;
    offset-=4;
    symtable.emplace_back(p);

  }else if(node.str == "expr term"){
    for(auto &item: node.children) code(*item, symtable, functable);
  }else if(node.str == "term factor"){
    for(auto &item: node.children) code(*item, symtable, functable);
  }else if(node.str == "factor NUM"){
    long long value;
    string str = node.children[0]->str;
    istringstream ss(str);
    ss>>tmp;
    ss>>value;
    if(inRange(value)==0){
      string s="ERROR: out of range";
      throw s;
    }
    cout<<"lis $3\n.word "<<value<<endl;
  }else if(node.str == "factor ID"){
    string id;
    string str = node.children[0]->str;
    istringstream ss(str);
    ss>>tmp;
    ss>>id;
    int reg = isDuplicate(symtable, id);
    if(reg == 0){
      string s="ERROR: not found";
      throw s;
    }
    cout<<"lw $3, "<<reg<<"($29)"<<endl;
  }else if(node.str == "expr expr PLUS term"){
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"add $3, $3, $5"<<endl;
  }else if(node.str == "expr expr MINUS term"){
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"sub $3, $5, $3"<<endl;
  }else if(node.str == "term term STAR factor"){
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"mult $5, $3"<<endl;
    cout<<"mflo $3"<<endl;
  }else if(node.str == "term term SLASH factor"){
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"div $5, $3"<<endl;
    cout<<"mflo $3"<<endl;
  }else if(node.str == "term term PCT factor"){
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"div $5, $3"<<endl;
    cout<<"mfhi $3"<<endl;
  } else if(node.str == "factor LPAREN expr RPAREN"){
    code(*(node.children[1]), symtable, functable);
  } else if(node.str == "factor pcall"){
    code(*(node.children[0]), symtable, functable);
  } else if(node.str == "pcall ID LPAREN RPAREN"){
    string str = node.children[0]->str;
    istringstream ss(str);
    ss>>tmp;
    ss>>tmp;
    cout<<"add $1, $0, $0"<<endl;
    cout<<"add $2, $0, $0"<<endl;
    cout<<"lis $31"<<endl;
    cout<<".word F"<<tmp<<endl;
    cout<<"jalr $31"<<endl;
  } else if(node.str == "statements statements statement"){
    for(auto &item: node.children) code(*item, symtable, functable);
  } else if(node.str == "statement pcall SEMI"){
    for(auto &item: node.children) code(*item, symtable, functable);
  } else if(node.str == "pcall ID LPAREN expr RPAREN"){   //parameter is is $1
    code(*(node.children[2]), symtable, functable);
    cout<<"add $1, $0, $3"<<endl;
    cout<<"add $2, $0, $0"<<endl;
    string st = node.children[0]->str;
    istringstream ss(st);
    ss>>tmp;
    ss>>tmp;
    cout<<"lis $31"<<endl;
    cout<<".word F"<<tmp<<endl;
    cout<<"jalr $31"<<endl;
  } else if(node.str == "dcls dcls dcl BECOMES NUM SEMI"){
    code(*(node.children[0]), symtable, functable);
    string st = node.children[1]->children[1]->str;
    string value = node.children[3]->str;
    int a;
    istringstream ss(st);
    pair<string, int> p;
    ss>>tmp;
    ss>>p.first;
    if(isDuplicate(symtable, p.first)){
      string s="ERROR: duplicate";
      throw s;
    }
    p.second = offset;
    offset-=4;
    symtable.emplace_back(p);

    istringstream sst(value);
    sst>>tmp;
    sst>>a;

    cout<<"lis $5"<<endl;
    cout<<".word "<<a  <<endl;
    push(5);

  } else if( node.str == "statement ID BECOMES expr SEMI"){
    code(*(node.children[2]), symtable, functable);
    string st = node.children[0]->str;
    istringstream ss(st);
    ss>>tmp;
    ss>>tmp;
    int off = isDuplicate(symtable, tmp);
    if(off == 0){
      string s = "ERROR: not found 2";
      cerr<<tmp <<endl;
      throw s;
    }
    cout<<"sw $3, "<<off<<"($29)"<<endl;
  } else if( node.str == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
    int subnameorder = nameorder;
    nameorder++;
    cout<<"loopY"<<subnameorder<<":"<<endl;
    code(*(node.children[2]), symtable, functable);
    cout<<"beq $3, $0, endloop"<<subnameorder<<endl;
    code(*(node.children[5]), symtable, functable);
    cout<<"beq $0, $0, loopY"<<subnameorder<<endl;
    cout<<"endloop"<<subnameorder<<":"<<endl;
  } else if(node.str == "test expr LT expr"){
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"slt $3, $5, $3"<<endl;
  } else if(node.str == "test expr EQ expr"){
    int subnameorder = nameorder;
    nameorder++;
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"slt $6, $5, $3"<<endl;
    cout<<"slt $5, $3, $5"<<endl;
    cout<<"lis $3\n.word 0"<<endl;
    cout<<"bne $5, $6, beqend"<<subnameorder<<endl;
    cout<<"lis $3\n.word 1"<<endl;
    cout<<"beqend"<<subnameorder<<":"<<endl;
  }else if(node.str == "test expr NE expr"){
    int subnameorder = nameorder;
    nameorder++;
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"slt $6, $5, $3"<<endl;
    cout<<"slt $5, $3, $5"<<endl;
    cout<<"lis $3\n.word 0"<<endl;
    cout<<"beq $5, $6, bneend"<<subnameorder<<endl;
    cout<<"lis $3\n.word 1"<<endl;
    cout<<"bneend"<<subnameorder<<":"<<endl;
  }else if(node.str == "test expr LE expr"){
    int subnameorder = nameorder;
    nameorder++;
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    push(7);
    push(8);
    cout<<"slt $7, $5, $3"<<endl;
    cout<<"slt $8, $3, $5"<<endl;
    cout<<"slt $3, $5, $3"<<endl;
    cout<<"bne $7, $8, leend"<<subnameorder<<endl;
    cout<<"lis $3\n.word 1"<<endl;
    cout<<"leend"<<subnameorder<<":"<<endl;
    pop(8);
    pop(7);
  }else if(node.str == "test expr GT expr"){
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    cout<<"slt $3, $3, $5"<<endl;
  }else if(node.str == "test expr GE expr"){
    int subnameorder = nameorder;
    nameorder++;
    code(*(node.children[0]), symtable, functable);
    push(3);
    code(*(node.children[2]), symtable, functable);
    pop(5);
    push(7);
    push(8);
    cout<<"slt $7, $5, $3"<<endl;
    cout<<"slt $8, $3, $5"<<endl;
    cout<<"slt $3, $3, $5"<<endl;
    cout<<"bne $7, $8, gtend"<<subnameorder<<endl;
    cout<<"lis $3\n.word 1"<<endl;
    cout<<"gtend"<<subnameorder<<":"<<endl;
    pop(8);
    pop(7);
  }else if(node.str == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){
    int subnameorder = nameorder;
    nameorder++;
    code(*(node.children[2]), symtable, functable);
    cout<<"beq $3, $0, else"<<subnameorder<<endl;
    code(*(node.children[5]), symtable, functable);
    cout<<"beq $0, $0, ifend"<<subnameorder<<endl;
    cout<<"else"<<subnameorder<<":"<<endl;
    code(*(node.children[9]), symtable, functable);
    cout<<"ifend"<<subnameorder<<":"<<endl;
  }else if(node.str == "pcall ID LPAREN expr COMMA expr RPAREN"){
    string st = node.children[0]->str;
    istringstream ss(st);
    ss>>tmp;
    ss>>tmp;
    code(*(node.children[2]), symtable, functable);
    cout<<"add $1, $3, $0"<<endl;
    code(*(node.children[4]), symtable, functable);
    cout<<"add $2, $3, $0"<<endl;

    cout<<"lis $31"<<endl<<".word F"<<tmp<<endl;
    cout<<"jalr $31"<<endl;

  }else if(node.str == "params"){


  }else if(node.str == "params dcl"){
    code(*(node.children[0]), symtable, functable);
  }

}


int main(){
  string ultimate=R"(27
BECOMES
BOF
COMMA
ELSE
EOF
EQ
GE
GT
ID
IF
INT
LBRACE
LE
LPAREN
LT
MINUS
NE
NUM
PCT
PLUS
RBRACE
RETURN
RPAREN
SEMI
SLASH
STAR
WHILE
13
start
params
dcl
dcls
expr
factor
pcall
procedures
procedure
statement
statements
term
test
start
36
start BOF procedures EOF
procedures procedure procedures
procedures
procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
params
params dcl
params dcl COMMA dcl
dcls
dcls dcls dcl BECOMES NUM SEMI
dcl INT ID
statements
statements statements statement
statement ID BECOMES expr SEMI
statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
statement WHILE LPAREN test RPAREN LBRACE statements RBRACE
statement pcall SEMI
test expr EQ expr
test expr NE expr
test expr LT expr
test expr LE expr
test expr GE expr
test expr GT expr
expr term
expr expr PLUS term
expr expr MINUS term
term factor
term term STAR factor
term term SLASH factor
term term PCT factor
factor ID
factor NUM
factor LPAREN expr RPAREN
factor pcall
pcall ID LPAREN RPAREN
pcall ID LPAREN expr RPAREN
pcall ID LPAREN expr COMMA expr RPAREN
)";

  int termnum, ntermnum, transnum;
  vector<string> term, nterm, trans, target;
  string tmp, start;
  istringstream ss(ultimate);
  //read in language
  ss>>termnum;
  int i = termnum;
  for(; i>0; i--){
    ss>>tmp;
    term.emplace_back(tmp);
  }
  ss>>ntermnum;
  i = ntermnum;
  for(; i>0; i--){
    ss>>tmp;
    nterm.emplace_back(tmp);
  }
  ss>>start;
  ss>>transnum;
  getline(ss, tmp); //skip
  i = transnum;
  for(; i>0; i--){
    getline(ss, tmp);
    trans.emplace_back(tmp);
  }


  //build tree
  TNode* root = new TNode(); //root
  getline(cin, tmp);
  root->str = tmp;
  build(root, trans);

  vector<pair<string, int>> symtable;
  vector<pair<string, int>> functable;
  try{
    code(*root, symtable, functable);
  }catch(string s){
    cerr<<s<<endl;
  }
  delete root;
}
