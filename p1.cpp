//
//  p1.cpp
//  SUBC-Parser
//
//  Created by Jesse Kolb on 4/25/18.
//  Copyright © 2018 Jesse Kolb. All rights reserved.
//

#include <string.h>

#include <iostream>
#include "LexAnalyze.hpp"
#include "Parser.hpp"
using namespace std;

int main(int argc, const char * argv[]) {

    string filename;
	
	//AST flag has been passed in
    if(argc <= 2)
    {
        filename = string(argv[1]);
    }
	//No AST flag
    else
    {
        filename = string(argv[2]);
    }

	//Pass file to our functions
    LexAnalyze* lex = new LexAnalyze(filename);
    Parser* parser = new Parser(lex);

	//If we have the ast flag print out AST
    if(argc > 2 && strcmp(argv[1], "-ast") == 0)
    {
        parser->printAST();
    }
    
    return 0;
}
