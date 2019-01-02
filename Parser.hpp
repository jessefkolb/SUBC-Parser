//
//  Parser.hpp
//  SUBC-Parser
//
//  Created by Jesse Kolb on 4/25/18.
//  Copyright © 2018 Jesse Kolb. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <iostream>
#include <stack>
#include "LexAnalyze.hpp"
using namespace std;

class TreeNode
{
//New class for constructor utilization every time tree is built
public:
    TreeNode *child;
    TreeNode *sibling;
    TreeNode *root;
    int n; //number of children
    string token;
    
    TreeNode(string token, int n);
};

class Parser
{
public:
    //Grammar functions
    void Tiny();
    void Consts();
    void Const();
    void ConstValue();
    void Types();
    void Type();
    void LitList();
    void SubProgs();
    void Fcn();
    void Params();
    void Dclns();
    void Dcln();
    void Body();
    void Statement();
    void OutExp();
    void StringNode();
    int Caseclauses();
    void Caseclause();
    void CaseExpression();
    int OtherwiseClause();
    void Assignment();
    void ForStat();
    void ForExp();
    void Expression();
    void Term();
    void Factor();
    void Primary();
    void Name();
    
    //Parsing constructor and functions
    Parser(LexAnalyze* lex);
    void Skip (string str); //Skips over the current string if it's the correct syntax
    void Skip (enum TokenDefinition def); //Find a way to combine the two, possibly with string array
    
    
    //Varaibles from LexAnalyze class to keep track of tokens
    struct Token Token;
    LexAnalyze* lex;
    
    //Tree building and output
    stack<TreeNode*> ASTStack;
    void Build (string token, int n);
    void preOrder(TreeNode* root, int n);
    void printAST();
};

#endif /* Parser_hpp */
