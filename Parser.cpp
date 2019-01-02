//
//  Parser.cpp
//  SUBC-Parser
//
//  Created by Jesse Kolb on 4/25/18.
//  Copyright © 2018 Jesse Kolb. All rights reserved.
//

#include "Parser.hpp"
#include <iostream>
using namespace std;

//Tree node data structure for our AST
TreeNode::TreeNode(string token, int n)
{
    this->token = token;
    this->n = n;
}

//Initializes local variables and calls Parse() function from the lexical analyzer
//This is so our tokens can be defined, then sent over to build the tree and parse for syntax errors
Parser::Parser(LexAnalyze *lex)
{
    this->lex = lex;
    this->Token = lex->Parse();
    Tiny();
}

//Skips over built in keywords and symbols such as "program" "const" etc.
//Error handler just in case something was typed wrong or incorrect syntax
void Parser::Skip(string str)
{
    //cout << "List o' tokens: " << Token.token << endl;
    if(!(Token.token == str))
    {
        cout << "Could not parse: Possible syntax error 1" << endl;
        exit(1);
    }
    else if(!lex->endOfFile()) {
        Token = lex->Parse(); //Returns token from the lexical analyzer
    }
}

//If the token definition matches...
//1. Build the token onto the tree
//2. Build the token tag onto the tree
//3. Call the Parse function from our lexical analyzer to skip to the next token
//Error handler just in case something was typed wrong or incorrect syntax
void Parser::Skip(enum TokenDefinition def)
{
    if(Token.def != def)
    {
        cout << "ERROR Could not parse: Possible syntax error 2" << endl;
        exit(1);
    }
    
    else if(def == Identifier)
    {
        Build(Token.token, 0);
        Build("<identifier>", 1);
    }
    
    else if (def == Integer)
    {
        Build(Token.token, 0);
        Build("<integer>", 1);
    }
    
    else if (def == Character)
    {
        Build("'" + Token.token + "'", 0);
        Build("<char>", 1);
    }
    
    else if (def == Str)
    {
        Build(Token.token, 0);
        Build("<string>", 1);
    }
    
    if(!lex->endOfFile())
    {
        Token = lex->Parse();
    }
}

//Builds the tree from the current token
void Parser::Build(string token, int n)
{
    TreeNode* tempNode = NULL;
    TreeNode* root = new TreeNode(token, n);
    
    for(int i = 0; i < n; i++)
    {
        TreeNode* thisChild = this->ASTStack.top();
        this->ASTStack.pop();
        thisChild->sibling = tempNode;
        tempNode = thisChild;
    }
    
    root->sibling = NULL;
    root->child = tempNode;
    this->ASTStack.push(root);
}

//Pre-order tree data structure
void Parser::preOrder(TreeNode* root, int n)
{
    if(root != NULL)
    {
        for(int i=0; i < n; i++)
        {
            cout<< ". ";
        }
        cout << root->token << "(" << root->n << ")" << endl;
        preOrder(root->child, n+1);
        preOrder(root->sibling, n);
    }
}

//Prints AST if -ast flag sent from main
void Parser::printAST()
{
    preOrder(ASTStack.top(), 0);
}

void Parser::Tiny()
{
    Skip("program");
    Name();
    Skip(":");
    Consts();
    Types();
    Dclns();
    SubProgs();
    Body();
    Name();
    Skip(".");
    
    Build("program", 7);
}

void Parser::Consts()
{
    if(Token.token == "const") //If the upcoming token is const then utilize a while loop to get names of variables
    {
        int temp = 0;
        Skip("const");
        
        Const();
        temp++;
        
        while(Token.token == ",")
        {
            Skip(",");
            Const();
            temp++;
        }
        
        Skip(";");
        Build("consts", temp);
    }
    
    else
    {
        Build("consts", 0);
    }
}

//Single variable
void Parser::Const()
{
    Name();
    ConstValue();
    
    Skip("=");
    Build("const", 2);
}

void Parser::ConstValue()
{
    if(Token.def == Identifier)
    {
        Name();
    }
    else if(Token.def == Character || Token.def == Integer)
    {
        Skip(Token.def);
    }
    else
    {
        cout << "Syntax Error 4" << endl;
        exit(1);
    }
}

void Parser::Types()
{
    if(Token.token == "type")
    {
        Skip("type");
        
        int temp = 0;
        while(Token.def == Identifier)
        {
            Type();
            Skip(";");
            temp++;
        }
        Build("types", temp);
    }
    else
    {
        Build("types", 0);
    }
}
void Parser::Type()
{
    Name();
    Skip("=");
    LitList();
    Build("type", 2);
}

void Parser::LitList()
{
    int temp = 0;
    Skip("(");
    
    Name();
    temp++;
    
    while(Token.token == ",")
    {
        Skip(",");
        Name();
        temp++;
    }
    
    Skip(")");
    Build("lit", temp);
}

void Parser::SubProgs()
{
    int temp = 0;
    
    while(Token.token == "function")
    {
        Fcn();
        temp++;
    }
    
    Build("subprogs", temp);
    
}

void Parser::Fcn()
{
    Skip("function");
    Name();
    Skip("(");
    Params();
    Skip(")");
    Skip(":");
    Name();
    Skip(";");
    Consts();
    Types();
    Dclns();
    Body();
    Name();
    Skip(";");
    Build("fcn", 8);
}

void Parser::Params()
{
    int temp = 0;
    Dcln();
    temp++;
    
    while(Token.token == ";")
    {
        Skip(";");
        Dcln();
        temp++;
    }
    Build("params", temp);
}

void Parser::Dclns()
{
    if(Token.token == "var")
    {
        Skip("var");
        int temp = 0;
        Dcln();
        Skip(";");
        temp++;
        
        while(Token.def == Identifier)
        {
            Dcln();
            Skip(";");
            temp++;
        }
        
        Build("dclns", temp);
    }
    else
    {
        Build("dclns", 0);
    }
}

void Parser::Dcln()
{
    int temp = 0;
    Name();
    temp++;
    
    while(Token.token == ",")
    {
        Skip(",");
        Name();
        temp++;
    }
    
    Skip(":");
    Name();
    Build("var", temp+1);
    
}

void Parser::Body()
{
    int temp = 0;
    Skip("begin");
    Statement();
    temp++;
    
    while(Token.token == ";")
    {
        Skip(";");
        Statement();
        temp++;
    }
    Skip("end");
    Build("block", temp);
}

void Parser::Statement()
{
    if(Token.def == Identifier)
    {
        Assignment();
    }
    else if(Token.token == "output")
    {
        int temp = 0;
        Skip("output");
        Skip("(");
        OutExp();
        temp++;
        
        while(Token.token == ",")
        {
            Skip(",");
            OutExp();
            temp++;
        }
        
        Skip(")");
        Build("output", temp);
    }
    else if(Token.token == "if")
    {
        Skip("if");
        Expression();
        Skip("then");
        Statement();
        
        if(Token.token == "else")
        {
            Skip("else");
            Statement();
            Build("if", 3);
        }
        else
        {
            Build("if", 2);
        }
    }
    else if(Token.token == "while")
    {
        Skip("while");
        Expression();
        Skip("do");
        Statement();
        Build("while", 2);
    }
    else if(Token.token == "repeat")
    {
        int temp = 0;
        Skip("repeat");
        Statement();
        temp++;
        
        while(Token.token == ";")
        {
            Skip(";");
            Statement();
            temp++;
        }
        
        Skip("until");
        Expression();
        Build("repeat", temp+1);
    }
    else if(Token.token == "for")
    {
        Skip("for");
        Skip("(");
        ForStat();
        Skip(";");
        ForExp();
        Skip(";");
        ForStat();
        Skip(")");
        Statement();
        Build("for",4);
    }
    else if(Token.token == "loop")
    {
        int temp = 0;
        Skip("loop");
        Statement();
        temp++;
        
        while(Token.token == ";")
        {
            Skip(";");
            Statement();
            temp++;
        }
        
        Skip("pool");
        Build("loop", temp);
        
    }
    else if(Token.token == "case")
    {
        int temp = 1;
        Skip("case");
        Expression();
        Skip("of");
        temp = temp + Caseclauses();
        temp = temp + OtherwiseClause();
        Skip("end");
        Build("case", temp);
        
    }
    else if(Token.token == "read")
    {
        
        Skip("read");
        Skip("(");
        Name();
        
        int temp = 1;
        while(Token.token == ",")
        {
            Skip(",");
            Name();
            temp++;
        }
        
        Skip(")");
        Build("read", temp);
        
    }
    else if(Token.token == "exit")
    {
        Skip("exit");
        Build("exit", 0);
    }
    else if(Token.token == "return")
    {
        Skip("return");
        Expression();
        Build("return", 1);
    }
    else if(Token.token == "begin")
    {
        Body();
    }
    else
    {
        Build("<null>", 0);
    }
}

void Parser::OutExp()
{
    if(Token.def == Str)
    {
        StringNode();
        Build("string", 1);
    }
    else
    {
        Expression();
        Build("integer", 1);
    }
}

void Parser::StringNode()
{
    Skip(Str);
}

int Parser::Caseclauses()
{
    int temp = 1;
    Caseclause();
    Skip(";");
    while(Token.def == Identifier || Token.def == Character || Token.def == Integer)
    {
        Caseclause();
        temp++;
        Skip(";");
    }
    return temp;
}

void Parser::Caseclause()
{
    int temp = 1;
    CaseExpression();
    while(Token.token == ",")
    {
        Skip(",");
        CaseExpression();
        temp++;
    }
    
    Skip(":");
    Statement();
    Build("case_clause", temp+1);
}

void Parser::CaseExpression()
{
    ConstValue();
    if(Token.token == "..")
    {
        Skip("..");
        ConstValue();
        Build("..", 2);
    }
}

int Parser::OtherwiseClause()
{
    if(Token.token == "otherwise")
    {
        Skip("otherwise");
        Statement();
        Build("otherwise", 1);
        return 1;
    }
    else
    {
        return 0;
    }
}

void Parser::Assignment()
{
    Name();
    if(Token.token == ":=")
    {
        Skip(":=");
        Expression();
        Build("assign", 2);
    }
    else if(Token.token == ":=:")
    {
        Skip(":=:");
        Name();
        Build("swap", 2);
    }
}

void Parser::ForStat()
{
    if(Token.def == Identifier)
    {
        Assignment();
    }
    else
    {
        Build("<null>", 0);
    }
}

void Parser::ForExp()
{
    if(Token.token == "+" || Token.token == "-" || Token.token == "not" || Token.token == "eof" || Token.token == "pred" || Token.token == "succ" || Token.token == "chr" || Token.token == "ord" || Token.token == "(" || Token.def == Integer || Token.def == Identifier || Token.def == Character)
    {
        Expression();
    }
    else
    {
        Build("true", 0);
    }
}

void Parser::Expression()
{
    Term();
    
    if(Token.token == "<=")
    {
        Skip("<=");
        Term();
        Build("<=", 2);
    }
    else if(Token.token == "<")
    {
        Skip("<");
        Term();
        Build("<", 2);
    }
    else if(Token.token == "=")
    {
        Skip("=");
        Term();
        Build("=", 2);
    }
    else if(Token.token == ">=")
    {
        Skip(">=");
        Term();
        Build(">=", 2);
    }
    else if(Token.token == ">")
    {
        Skip(">");
        Term();
        Build(">", 2);
    }
    else if(Token.token == "<>")
    {
        Skip("<>");
        Term();
        Build("<>", 2);
    }
}

void Parser::Term()
{
    Factor();
    
    while(Token.token == "+" || Token.token == "-" || Token.token == "or")
    {
        if(Token.token == "+")
        {
            Skip("+");
            Factor();
            Build("+", 2);
        }
        else if(Token.token == "-")
        {
            Skip("-");
            Factor();
            Build("-", 2);
        }
        else if(Token.token == "or")
        {
            Skip("or");
            Factor();
            Build("or", 2);
        }
    }
}

void Parser::Factor()
{
    Primary();
    
    while(Token.token == "*" || Token.token == "/" || Token.token == "and" || Token.token == "mod")
    {
        if(Token.token == "*")
        {
            Skip("*");
            Primary();
            Build("*", 2);
        }
        else if(Token.token == "/")
        {
            Skip("/");
            Primary();
            Build("/", 2);
        }
        else if(Token.token == "and")
        {
            Skip("and");
            Primary();
            Build("and", 2);
        }
        else if(Token.token == "mod")
        {
            Skip("mod");
            Primary();
            Build("mod", 2);
        }
    }
}

void Parser::Primary()
{
    if(Token.def == Character)
    {
        Skip(Token.def);
    }
    
    else if(Token.token == "-")
    {
        Skip("-");
        Primary();
        Build("-", 1);
    }
    else if(Token.token == "+")
    {
        Skip("+");
        Primary();
    }
    
    else if(Token.token == "not")
    {
        Skip("not");
        Primary();
        Build("not", 1);
    }
    
    else if(Token.token == "eof")
    {
        Skip("eof");
        Build("eof",0);
    }
    
    else if(Token.token == "(")
    {
        Skip("(");
        Expression();
        Skip(")");
    }
    
    else if(Token.token == "succ")
    {   Skip("succ");
        Skip("(");
        Expression();
        Skip(")");
        Build("succ", 1);
    }
    
    else if(Token.token == "pred")
    {
        Skip("pred");
        Skip("(");
        Expression();
        Skip(")");
        Build("pred", 1);
    }
    
    else if(Token.token == "chr")
    {
        Skip("chr");
        Skip("(");
        Expression();
        Skip(")");
        Build("chr",1);
    }
    
    else if(Token.token == "ord")
    {
        Skip("ord");
        Skip("(");
        Expression();
        Skip(")");
        Build("ord",1);
    }
    
    else if(Token.def == Identifier)
    {
        Name();
        if(Token.token == "(")
        {
            Skip("(");
            Expression();
            int temp = 2;
            
            while(Token.token == ",")
            {
                Skip(",");
                Expression();
                temp++;
            }
            
            Skip(")");
            Build("call", temp);
        }
    }
    
    else if(Token.def == Integer)
    {
        Skip(Token.def);
    }
    
    else
    {
        cout << "Syntax Error 3" << endl;
        exit(1);
    }
}

void Parser::Name()
{
    Skip(Identifier);
}
