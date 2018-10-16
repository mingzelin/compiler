#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class TNode{
public:
    vector<TNode*> children;
    string str = "";
  public:
    TNode() = default;
    TNode(const TNode& n):str(n.str){
      for(int i=0; i<n.children.size();i++){
        children.emplace_back(n.children[i]);
      }
    }
    ~TNode() = default;
};

void print(TNode* t){
  cout<<t->str<<endl;
  for(int i=t->children.size()-1; i>=0; i--){
    print(t->children[i]);
  }
}

int isTerminal(string word, vector<string>& t){
  for(auto item: t){
    if(word==item)return 1;
  }
  return 0;
}
string nthword(int k, string word){
  istringstream ss(word);
  string result;
  for(int i=0; i<k;i++){
    ss>>result;
  }
  return result;
}

int transfer(int curr, string in, int& ros, vector<string>& p){
  int state;
  string tmp;
  istringstream ss;

  for(int i = 0; i<p.size(); i++){
    ss.str(p[i]);
    ss>>state;
    if(state == curr){
      ss>>tmp;
      if(tmp == in){
        ss>>tmp;
        if(tmp == "reduce") ros = 1;
        else ros = 0;
        ss >>state;
        return state;
      }
    }
  }
  return -1;
}

int countshift(string str){
  string tmp;
  istringstream ss(str);
  int count = 0;
  while(ss>>tmp){
    count++;
  }
  count--;
  return count;
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
90
491
1 GE reduce 29
38 ID shift 1
3 ID shift 1
36 term shift 2
77 LT reduce 23
78 LT reduce 24
60 INT reduce 3
2 LT reduce 22
21 MINUS shift 3
11 STAR reduce 30
7 STAR reduce 32
59 ID shift 1
20 ID shift 1
39 ID shift 1
50 ID shift 1
57 ID shift 1
64 ID shift 1
51 STAR reduce 31
76 WHILE reduce 15
77 PLUS reduce 23
78 PLUS reduce 24
4 INT reduce 7
12 MINUS shift 3
13 MINUS shift 3
14 MINUS shift 3
15 MINUS shift 3
16 MINUS shift 3
17 MINUS shift 3
77 NE reduce 23
78 NE reduce 24
33 WHILE reduce 14
31 WHILE reduce 12
2 NE reduce 22
5 MINUS reduce 25
34 WHILE reduce 13
67 MINUS reduce 26
68 MINUS reduce 27
69 MINUS reduce 28
54 LBRACE shift 4
2 PLUS reduce 22
8 factor shift 5
9 factor shift 5
1 GT reduce 29
18 ID shift 1
61 SEMI reduce 35
28 SEMI reduce 34
79 SEMI reduce 33
63 WHILE reduce 8
1 SLASH reduce 29
88 WHILE shift 6
22 ID shift 1
36 pcall shift 7
61 RPAREN reduce 35
25 LPAREN shift 8
6 LPAREN shift 9
2 LE reduce 22
79 RPAREN reduce 33
28 RPAREN reduce 34
45 pcall shift 10
46 pcall shift 10
77 LE reduce 23
78 LE reduce 24
22 factor shift 5
84 MINUS shift 3
71 EOF reduce 2
74 NUM shift 11
48 pcall shift 10
23 term shift 2
56 RPAREN reduce 6
72 EOF reduce 1
8 ID shift 1
9 ID shift 1
11 NE reduce 30
7 NE reduce 32
59 expr shift 12
20 expr shift 13
39 expr shift 14
50 expr shift 15
57 expr shift 16
64 expr shift 17
51 NE reduce 31
38 pcall shift 7
3 pcall shift 7
70 COMMA shift 18
86 BECOMES shift 19
70 MINUS shift 3
81 WHILE reduce 10
89 NE shift 20
82 WHILE reduce 10
83 WHILE reduce 10
18 expr shift 21
88 RETURN shift 22
36 NUM shift 11
59 LPAREN shift 23
20 LPAREN shift 23
39 LPAREN shift 23
50 LPAREN shift 23
57 LPAREN shift 23
64 LPAREN shift 23
2 GE reduce 22
88 ID shift 24
88 IF shift 25
77 GE reduce 23
78 GE reduce 24
88 statement shift 26
77 PCT shift 27
78 PCT shift 27
1 PCT reduce 29
70 RPAREN shift 28
55 ID shift 29
85 COMMA shift 30
49 NUM shift 11
37 NUM shift 11
27 NUM shift 11
5 RPAREN reduce 25
67 RPAREN reduce 26
68 RPAREN reduce 27
69 RPAREN reduce 28
1 MINUS reduce 29
66 SEMI shift 31
1 EQ reduce 29
2 GT reduce 22
77 GT reduce 23
78 GT reduce 24
45 RBRACE shift 32
46 RBRACE shift 33
60 EOF reduce 3
48 RBRACE shift 34
67 PCT reduce 26
68 PCT reduce 27
69 PCT reduce 28
84 SEMI shift 35
24 LPAREN shift 36
22 pcall shift 7
1 LPAREN shift 36
34 RBRACE reduce 13
31 RBRACE reduce 12
26 RETURN reduce 11
5 PCT reduce 25
20 term shift 2
59 term shift 2
39 term shift 2
50 term shift 2
57 term shift 2
64 term shift 2
77 SLASH shift 37
78 SLASH shift 37
33 RBRACE reduce 14
2 SLASH shift 37
76 RBRACE reduce 15
34 IF reduce 13
5 SLASH reduce 25
18 term shift 2
34 ID reduce 13
33 IF reduce 14
79 EQ reduce 33
28 EQ reduce 34
33 ID reduce 14
31 IF reduce 12
67 SLASH reduce 26
68 SLASH reduce 27
69 SLASH reduce 28
51 RPAREN reduce 31
26 IF reduce 11
67 EQ reduce 26
68 EQ reduce 27
69 EQ reduce 28
11 RPAREN reduce 30
7 RPAREN reduce 32
26 ID reduce 11
5 EQ reduce 25
89 PLUS shift 38
34 RETURN reduce 13
31 ID reduce 12
76 RETURN reduce 15
76 IF reduce 15
31 RETURN reduce 12
76 ID reduce 15
33 RETURN reduce 14
89 LT shift 39
11 LT reduce 30
7 LT reduce 32
51 LT reduce 31
12 RPAREN reduce 16
13 RPAREN reduce 17
14 RPAREN reduce 18
15 RPAREN reduce 19
16 RPAREN reduce 20
17 RPAREN reduce 21
8 test shift 40
9 test shift 41
45 IF shift 25
46 IF shift 25
29 COMMA reduce 9
48 ID shift 24
11 PLUS reduce 30
7 PLUS reduce 32
77 COMMA reduce 23
78 COMMA reduce 24
51 PLUS reduce 31
2 COMMA reduce 22
47 ID shift 42
45 ID shift 24
46 ID shift 24
11 LE reduce 30
7 LE reduce 32
51 LE reduce 31
40 RPAREN shift 43
41 RPAREN shift 44
82 statements shift 45
83 statements shift 46
71 INT shift 47
81 statements shift 48
77 STAR shift 49
78 STAR shift 49
67 GT reduce 26
68 GT reduce 27
69 GT reduce 28
45 statement shift 26
46 statement shift 26
5 GT reduce 25
48 IF shift 25
48 statement shift 26
89 LE shift 50
75 RPAREN shift 51
2 STAR shift 49
32 ELSE shift 52
67 GE reduce 26
68 GE reduce 27
69 GE reduce 28
80 procedures shift 53
5 GE reduce 25
62 RPAREN shift 54
11 PCT reduce 30
7 PCT reduce 32
81 RETURN reduce 10
51 PCT reduce 31
30 INT shift 55
81 RBRACE reduce 10
8 term shift 2
9 term shift 2
30 dcl shift 56
59 factor shift 5
20 factor shift 5
39 factor shift 5
50 factor shift 5
57 factor shift 5
64 factor shift 5
1 PLUS reduce 29
82 RETURN reduce 10
83 RETURN reduce 10
89 GE shift 57
79 MINUS reduce 33
61 GT reduce 35
28 MINUS reduce 34
61 MINUS reduce 35
19 NUM shift 58
51 GE reduce 31
74 pcall shift 7
18 pcall shift 7
1 LE reduce 29
11 GT reduce 30
7 GT reduce 32
89 MINUS shift 3
51 GT reduce 31
89 EQ shift 59
35 RBRACE shift 60
74 ID shift 1
11 GE reduce 30
7 GE reduce 32
21 RPAREN shift 61
74 term shift 2
36 LPAREN shift 23
67 LT reduce 26
68 LT reduce 27
69 LT reduce 28
5 LT reduce 25
59 pcall shift 7
20 pcall shift 7
39 pcall shift 7
50 pcall shift 7
57 pcall shift 7
64 pcall shift 7
28 PCT reduce 34
79 PCT reduce 33
22 term shift 2
61 PCT reduce 35
5 LE reduce 25
66 MINUS shift 3
61 EQ reduce 35
1 LT reduce 29
65 params shift 62
67 LE reduce 26
68 LE reduce 27
69 LE reduce 28
75 MINUS shift 3
79 GT reduce 33
80 EOF reduce 2
28 GT reduce 34
28 GE reduce 34
61 GE reduce 35
1 NE reduce 29
58 SEMI shift 63
89 GT shift 64
38 LPAREN shift 23
3 LPAREN shift 23
49 LPAREN shift 23
37 LPAREN shift 23
27 LPAREN shift 23
82 RBRACE reduce 10
83 RBRACE reduce 10
23 ID shift 1
79 GE reduce 33
74 factor shift 5
18 factor shift 5
23 pcall shift 7
79 LT reduce 33
1 COMMA reduce 29
28 LT reduce 34
61 LT reduce 35
22 NUM shift 11
61 PLUS reduce 35
67 NE reduce 26
68 NE reduce 27
69 NE reduce 28
79 LE reduce 33
28 PLUS reduce 34
5 NE reduce 25
28 LE reduce 34
79 PLUS reduce 33
61 LE reduce 35
42 LPAREN shift 65
70 PLUS shift 38
48 WHILE shift 6
65 RPAREN reduce 4
45 WHILE shift 6
46 WHILE shift 6
63 INT reduce 8
23 LPAREN shift 23
74 expr shift 66
23 factor shift 5
82 ID reduce 10
83 ID reduce 10
49 factor shift 67
37 factor shift 68
27 factor shift 69
81 ID reduce 10
82 IF reduce 10
83 IF reduce 10
81 IF reduce 10
51 MINUS reduce 31
77 RPAREN reduce 23
78 RPAREN reduce 24
67 PLUS reduce 26
68 PLUS reduce 27
69 PLUS reduce 28
2 SEMI reduce 22
11 MINUS reduce 30
7 MINUS reduce 32
2 RPAREN reduce 22
77 SEMI reduce 23
78 SEMI reduce 24
5 PLUS reduce 25
4 RETURN reduce 7
36 expr shift 70
11 EQ reduce 30
7 EQ reduce 32
51 EQ reduce 31
18 NUM shift 11
74 LPAREN shift 23
4 WHILE reduce 7
21 PLUS shift 38
1 RPAREN reduce 29
88 pcall shift 10
73 ID reduce 10
73 IF reduce 10
67 STAR reduce 26
68 STAR reduce 27
69 STAR reduce 28
12 PLUS shift 38
13 PLUS shift 38
14 PLUS shift 38
15 PLUS shift 38
16 PLUS shift 38
17 PLUS shift 38
80 procedure shift 71
8 NUM shift 11
9 NUM shift 11
2 MINUS reduce 22
77 MINUS reduce 23
78 MINUS reduce 24
49 ID shift 1
37 ID shift 1
27 ID shift 1
73 RETURN reduce 10
71 procedures shift 72
66 PLUS shift 38
1 STAR reduce 29
75 PLUS shift 38
79 NE reduce 33
28 NE reduce 34
61 NE reduce 35
38 NUM shift 11
3 NUM shift 11
5 STAR reduce 25
11 SLASH reduce 30
7 SLASH reduce 32
51 SLASH reduce 31
4 dcls shift 73
80 INT shift 47
8 pcall shift 7
9 pcall shift 7
24 BECOMES shift 74
23 expr shift 75
10 SEMI shift 76
11 SEMI reduce 30
7 SEMI reduce 32
26 RBRACE reduce 11
49 pcall shift 7
37 pcall shift 7
27 pcall shift 7
36 ID shift 1
38 term shift 77
3 term shift 78
28 SLASH reduce 34
79 SLASH reduce 33
5 COMMA reduce 25
2 PCT shift 27
67 COMMA reduce 26
68 COMMA reduce 27
69 COMMA reduce 28
85 RPAREN reduce 5
77 EQ reduce 23
78 EQ reduce 24
2 EQ reduce 22
61 SLASH reduce 35
36 RPAREN shift 79
63 IF reduce 8
63 ID reduce 8
73 RBRACE reduce 10
61 STAR reduce 35
28 STAR reduce 34
79 STAR reduce 33
84 PLUS shift 38
38 factor shift 5
3 factor shift 5
63 RETURN reduce 8
11 COMMA reduce 30
7 COMMA reduce 32
51 COMMA reduce 31
0 BOF shift 80
18 LPAREN shift 23
51 SEMI reduce 31
67 SEMI reduce 26
68 SEMI reduce 27
69 SEMI reduce 28
5 SEMI reduce 25
52 LBRACE shift 81
36 factor shift 5
4 IF reduce 7
43 LBRACE shift 82
44 LBRACE shift 83
4 ID reduce 7
22 LPAREN shift 23
61 COMMA reduce 35
28 COMMA reduce 34
79 COMMA reduce 33
22 expr shift 84
65 INT shift 55
23 NUM shift 11
65 dcl shift 85
73 INT shift 55
73 dcl shift 86
8 LPAREN shift 23
9 LPAREN shift 23
53 EOF shift 87
29 BECOMES reduce 9
29 RPAREN reduce 9
59 NUM shift 11
20 NUM shift 11
39 NUM shift 11
50 NUM shift 11
57 NUM shift 11
64 NUM shift 11
71 procedure shift 71
73 statements shift 88
8 expr shift 89
9 expr shift 89
1 SEMI reduce 29
26 WHILE reduce 11
73 WHILE reduce 10
  )";

  int termnum, ntermnum, transnum, parsenum;
  vector<string> term, nterm, trans, parse, target;
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
  ss>>tmp; //skip
  ss>>parsenum;
  getline(ss, tmp); //skip
  i = parsenum;
  for(; i>0; i--){
    getline(ss, tmp);
    parse.emplace_back(tmp);
  }

  target.emplace_back("BOF BOF");
  while(getline(cin, tmp)){
    target.emplace_back(tmp);
  }
  target.emplace_back("EOF EOF");


  //parse
  vector<TNode*> result;
  vector<int> sstack;
  int first = 0;
  vector<TNode*> keeper;
  sstack.emplace_back(first);

  int nextchar = 0, currstate = 0, ros, out;
  string lookahead = nthword(1, target[nextchar]);
  nextchar++;

  while(true){
    out = transfer(currstate, lookahead, ros, parse);


    //Error state
    if(out == -1){
      cerr<<"ERROR at "<<nextchar<<endl;
      return 1;
    }

    if(ros == 1){ //reduce

      //pop stack of state
      int pop = countshift(trans[out]);
      for(int j = pop; j>0; j--) {
        sstack.erase(sstack.end()-1);
      }
      //result add the transition

      TNode* n = new TNode;
      keeper.emplace_back(n);
      n->str  = trans[out];

      result.erase(result.end()-1);
      for(int j = pop; j>0; j--) {
        TNode* t = result[result.size()-1];
        (n->children).emplace_back(t);
        result.erase(result.end()-1);
      }
      result.emplace_back(n);
      currstate = sstack[sstack.size()-1]; //re-calc currstate

      lookahead = nthword(1, trans[out]);

    }else{ //shift
      currstate = out;
      if((lookahead != "BOF")&&(isTerminal(lookahead, term)))nextchar++;
      if(nextchar>=target.size()) break;
      sstack.emplace_back(currstate);

      TNode* n = new TNode;
      keeper.emplace_back(n);
      n->str = target[nextchar];
      result.emplace_back(n);
      lookahead = nthword(1, target[nextchar]);
    }
  }

  cout<<"start BOF procedures EOF"<<endl;
  cout<<"BOF BOF"<<endl;
  print(result[0]);
  cout<<"EOF EOF"<<endl;

  while(keeper.size()>0){
    TNode* tmp = keeper[keeper.size()-1];
    keeper.erase(keeper.end()-1);
    delete tmp;
  }

  return 1;
}
