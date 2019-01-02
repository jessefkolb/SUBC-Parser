//
//  LexAnalyze.hpp
//  SUBC-Parser
//
//  Created by Jesse Kolb on 4/25/18.
//  Copyright © 2018 Jesse Kolb. All rights reserved.
//

#ifndef LexAnalyze_hpp
#define LexAnalyze_hpp

#include <fstream>
#include <iostream>
using namespace std;

enum TokenDefinition {Keyword, Identifier, Operator, Integer, Character, Str};

struct Token{
    TokenDefinition def; //Permanent token definition assignment to send to Parser.cpp
    string token; //Temporary string "token" assignment to check if token is valid
};

class LexAnalyze {
    
public:
//File parsing variables
    ifstream f;
    char c;
    string str;
    struct Token Token;
    string builtInKeywords[54] = {"program", "var", "const","type","function","return","begin","end",":=:",":=","output","if","then","else","while","do","case","of","..","otherwise","repeat","for","until","loop","pool","exit","<=","<>","<",">=",">","=","mod","and","or","not","read","succ","pred","chr","ord","eof","{","}",":",";",".",",","(",")","+","-","*","/"};
    
//Core functions
    LexAnalyze(string file);
    struct Token Parse();
    bool endOfFile();
    void setDefinition(struct Token& Token);
    bool keywordCheck(struct Token Token);
    
//Parsing handlers
    void eatComment();
    void eatSingleComment();
    struct Token handleSymbols();
};

#endif /* LexAnalyze_hpp */
