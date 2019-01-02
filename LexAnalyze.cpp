//
//  LexAnalyze.cpp
//  SUBC-Parser
//
//  Created by Jesse Kolb on 4/25/18.
//  Copyright © 2018 Jesse Kolb. All rights reserved.
//

#include "LexAnalyze.hpp"
#include <iostream>
using namespace std;

LexAnalyze::LexAnalyze(string file)
{
    f.open(file);
}

Token LexAnalyze::Parse()
{
    //STARTING STRATEGY:
    //Use char c to pull each individual character from the file
    //Eat comments and white space
    //Separate variables from symbols (isalnum)
    //If/else statements for every possible condition
    //Add the valid chars into the temporary string str, then see if str is a valid token
    //If it is a valid token, assign it a definition from the token enum
    
    str = "";
    
    while(f.get(c))
    {

//--------------WHITE SPACE HANDLERS-------------//
        if (c == '\n' || c == '\t' || c == ' ')
        {
            if(str.length() > 0)
            {
                Token.token = str;
                setDefinition(Token);
                return Token;
            }
            
            while((c == '\n' || c == '\t' || c == ' ') && !f.eof())
            {
                f.get(c);
                //Eats white space
            }
            f.unget(); //Fixes bug where it gets one too many times
        }
        
//--------------COMMENT HANDLERS-------------//
        
        else if(c == '{')
        {
            eatComment();
        }
        else if (c == '#')
        {
            eatSingleComment();
        }
//--------------PARSING TOKENS TO BE DEFINED-------------//
        
        else if(isalnum(c))
        {
            str = str + c; //Add all valid characters to the string to later check if it's a token
        }
        
        else if(!isalnum(c))
        {
            //found a symbol that is neither a letter, number, white space, nor comment
            //str.length() > 0 checks to see if we already have a potential token
            //parse it out before moving on to symbols
            
            if(str.length() > 0)
            {
                Token.token = str;
                setDefinition(Token);
                f.unget();
                return Token;
            }
        
            str = str + c;
            
            /*
             SYMBOLIC TOKENS:
             .  ..  :=:   :=   <=  <>  <   >=  >    =  :   ;
             ,   (   )   +   -   *   /  '   "
             
             STRATEGY:
             If there are multiple char tokens such as :=:, create nested if/else statements
             If it is a single char token such as =, handle it at end
             */
            
            //. and ..
            if(c == '.')
            {
                f.get(c);
                if(c == '.')
                {
                    str = str + c;
                    Token.token = str;
                    Token.def = Operator;
                    return Token;
                }
                f.unget();
                Token.token = str;
                Token.def = Operator;
                return Token;
            }
            
            //:=    :    :=:
            if(c == ':')
            {
                
                f.get(c);
                
                if(c == '=')
                {
                    str = str + c;
                    f.get(c);
                    
                    if(c == ':')
                    {
                        str = str + c;
                        Token.token = str;
                        Token.def = Operator;
                    }
                    
                    f.unget();
                    Token.token = str;
                    Token.def = Operator;
                    return Token;
                }
                else
                {
                    f.unget();
                    Token.token = str;
                    Token.def = Operator;
                    return Token;
                }
            }
            
            // <  <>  <=
            
            if(c == '<')
            {
                f.get(c);
                if(c == '>')
                {
                    str = str + '>';
                    Token.token = str;
                    Token.def = Operator;
                    return Token;
                }
                else if(c == '=')
                {
                    str = str + '=';
                    Token.token = str;
                    Token.def = Operator;
                    return Token;
                }
                else
                {
                    f.unget();
                    Token.token = str;
                    Token.def = Operator;
                    return Token;
                }
            }
            
            // >  >=
            
            if(c == '>')
            {
                f.get(c);
                if(c == '=')
                {
                    str = str + '=';
                    Token.token = str;
                    Token.def = Operator;
                    return Token;
                }
                else
                {
                    f.unget();
                    Token.token = str;
                    Token.def = Operator;
                    return Token;
                }
            }
            
            //chars (single quotes ') and strings (double quotes ")
            
            if(c == '\'')
            {
                f.get(c);
                str = c;
                Token.token = str;
                Token.def = Character;
                
                f.get(c); //should get rid of ending '
                
                return Token;
            }
            
            if(c == '\"')
            {
                f.get(c);
                while(c != '\"')
                {
                    str = str + c;
                    f.get(c); //should get rid of ending "
                }
                Token.token = str;
                Token.def = Str;
                return Token;
            }
            
            Token.token = str;
            setDefinition(Token);
            return Token;
        }
    }
    Token.token = str;
    setDefinition(Token);
    return Token;
}

void LexAnalyze::eatComment()
{
    while(c != '}' && !f.eof())
    {
        f.get(c);
    }
    f.get(c);
    //Eat the last bracket
}

void LexAnalyze::eatSingleComment()
{
    while(c != '\n' && !f.eof())
    {
        f.get(c);
    }
}


void LexAnalyze::setDefinition(struct Token& Token)
{
    if(keywordCheck(Token))
    {
        Token.def = Keyword;
    }
    else
    {
        if(Token.token.length() == 1)
        {
            if(isdigit(Token.token.at(0)))
            {
                Token.def = Integer;
            }
            else
            {
                Token.def = Identifier;
            }
        }
        
        else //token length > 1
        {
            for(int i = 0; i < Token.token.length(); i++)
            {
                if(isalpha(Token.token.at(i)))
                {
                    Token.def = Identifier;
                    return;
                }
            }
            
            //Did not contain letters, only numbers, is int
            Token.def = Integer;
        }
    }
}

bool LexAnalyze::keywordCheck(struct Token Token)
{
    for(int i = 0; i < 54; i++)
    {
        if(Token.token == builtInKeywords[i])
        {
            return true;
        }
    }
    return false;
}

bool LexAnalyze::endOfFile()
{
    if(f.eof())
        return true;
    else
        return false;
}
