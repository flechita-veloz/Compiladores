#pragma once
#include "token.hpp"
#include <vector>
#include <iostream>
int a = 0;
class Parser {
    vector<Token> tokens;
    size_t current;
    size_t total_tokens;

public:
    Parser(const vector<Token>& tokens) : tokens(tokens), current(0), total_tokens(tokens.size()){}

    Token peek() {
        return tokens[current];
    }

    Token advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd() {
        return peek().eof();
    }

    Token previous() {
        return tokens[current - 1];
    }

    bool match(Token_type type) {
        if (check(type)) {
            advance();
            return true;
        }
        
        return false;
    }

    bool check(Token_type type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    void consume(Token_type type, const std::string& errorMessage) {
        if (check(type)) {
            advance();
            return;
        }
        Token a = peek();
        printf("STUCK TRYING TO CONSUME: "); a.print();
        

        error(errorMessage);
    }

    void error(const std::string& message) {
        std::cerr << "Error: " << message << " at line " << peek().line_number << std::endl;
        exit(1);
    }

    // Program ::= Declaration_list
    void parseProgram() {
        printf("Program ::= Declaration_list\n");
        parseDeclarationList();
        cout << "Parseado Correctamente" << endl;
    }

    // Declaration_list ::= Statement DeclarationList
    //                    | Declaration_fun Declaration_list
    //                    | ''
    void parseDeclarationList() {
        //printf("Declaration_list::== ");
        while (current < total_tokens && !isAtEnd()) {
            printf("Still here\n");
            if (check(Token_type::VOID_KW) || check(Token_type::INT_KW)) {
                printf("Declaration_list::== Declaration_fun Declaration_list\n");
                parseDeclarationFun();
                printf("Parse DeclarationFun\n");
            } else {
                printf("Declaration_list::== Statement DeclarationList\n");
                parseStatement();
                printf("Parsed Statement\n");
                //printf("CURRENT token out of total: %d de %d\n ", current, tokens.size());
                //peek().print();
                //parseDeclarationList();
            }
        }
        //printf("Reached end\n");
    }

    // Declaration_fun ::= void identifier FunctionDecl
    //                   | Type identifier FunctionDecl
    void parseDeclarationFun() {
        printf("Declaration_fun ::= ");
        if (match(Token_type::VOID_KW)) {
            printf("void identifier FunctionDecl\n");
            consume(Token_type::IDENTIFIER, "Expected identifier after 'void'.");
        } else {
            printf("Type identifier FunctionDecl\n");
            parseType();
            consume(Token_type::IDENTIFIER, "Expected identifier after type.");
        }
        parseFunctionDecl();
    }

    // FunctionDecl ::= ( Param_list ) Block
    void parseFunctionDecl() {
        printf("FunctionDecl ::= ( Param_list ) Block \n");
        consume(Token_type::PAREN_OPEN, "Expected '(' after function name.");
        parseParamList();
        consume(Token_type::PAREN_CLOSE, "Expected ')' after parameters.");
        printf("PARSING BLOCK FROM FUNCTION\n");
        parseBlock();
    }

    // Param_list ::= Param Param_list | ''
    void parseParamList() {
        printf("Param_list ::= ");
        if (check(Token_type::PAREN_CLOSE)) {
            printf("empty \n");
            return; // Empty param list
        }
        else{
            printf("Param Param_list \n");
            parseParam();
            if(match(Token_type::COMMA)){
                printf(", \n");
                parseParamList();
            }
            //parseParamList();
        }
        //printf("Param Param_list \n");
        //parseParam();
        
        //while (match(Token_type::COMMA)) {
          //  parseParam();
        //}
    }

    // Param ::= Type identifier Param_tail
    void parseParam() {
        printf("Param :: = Type identifier Param_tail\n");
        parseType();
        consume(Token_type::IDENTIFIER, "Expected parameter name.");
        parseParamTail();
    }

    // Param_tail ::= , | ''
    void parseParamTail() {
        printf("Param_tail ::= ");
        /*if (!check(Token_type::PAREN_CLOSE)) {
            printf(",\n");
            //parseParamList();
        }
        else{
            printf("empty\n");
        }*/
       if(check(Token_type::COMMA)) printf(", \n");
       else printf("empty\n");
    }

    // Type ::= integer | boolean | char | string
    void parseType() {
        printf("Type ::= ");
        if (match(Token_type::INT_KW)) {printf("integer\n"); return;}
        if (match(Token_type::BOOLEAN_KW)) {printf("boolean\n"); return;}
        if (match(Token_type::CHAR_KW)) {printf("char\n"); return;}
        if (match(Token_type::STRING_KW_TYPE)) {printf("string\n"); return;}
       
        error("Expected a type.");
    }

    // Statement ::= IfStmt
    //             | ForStmt
    //             | WhileStmt
    //             | ReturnStmt
    //             | PrintStmt
    //             | Block
    //             | identifier Type_declaration
    //             | Factor Expressions
    void parseStatement() {
        printf("CURRENT_TOKEN->");
        peek().print();printf("\n");
        printf("Statement :: == ");
        if (match(Token_type::IF)) {
            printf("IfStmt\n");
            parseIfStmt();
        } else if (match(Token_type::FOR_KEY)) {
            printf("ForStmt\n");
            parseForStmt();
        } else if (match(Token_type::WHILE_KEY)) {
            printf("WhileStmt\n");
            parseWhileStmt();
        } else if (match(Token_type::RETURN)) {
            printf("ReturnStmt\n");
            parseReturnStmt();
        } else if (match(Token_type::PRINT_KEY)) {
            printf("PrintStmt\n");
            parsePrintStmt();
        } else if (check(Token_type::BRACE_OPEN)) {
            printf("BlockStmt\n");
            parseBlock();
        } else if (check(Token_type::IDENTIFIER)) {
            //printf("Identifier Type_declaration\n");
            printf("Identifier Statement_more\n");
            consume(Token_type::IDENTIFIER, "Expected identifier.");
            //parseTypeDeclaration();
            parseStatement_more();      
        } else {
            printf("Factor Expressions\n");
            parseFactor();
            parseExpressions();
        }
    }
    // Statement_more ::= Type_declaration
    // Statement_more ::= VarDecl
    void parseStatement_more(){
        printf("Statement_more ::= ");
        if(check(Token_type::ASSIGNMENT)){
            printf("VarDecl_more\n");
            parseVarDecl_more();
        }
        else{
            printf("Type_declaration\n");
            parseTypeDeclaration();
            // consume(Token_type::SEMICOLON, "Expected a SEMICOLON\n");        
        }
    }

    // Type_declaration ::= Type_factor Expressions
    //                    | : Type Type_factor VarDecl_more
    void parseTypeDeclaration() {
        printf("Type_declaration ::== ");
        if (match(Token_type::COLON)) {
            printf(": Type Type_factor VarDecl_more\n");
            parseType();
            parseTypeFactor();
            parseVarDecl_more();
        } else {
            printf("Type_factor Expressions\n");
            parseTypeFactor();
            parseExpressions();
        }
    }

    // Type_factor ::= '' | [ Factor_more Expressions ]
    void parseTypeFactor() {
        if (match(Token_type::ARRAY_SUBSCRIPT_OPEN)) {
            printf("Type_factor ::== [ Factor_more Expressions ]\n");
            parseFactorMore();
            parseExpressions();
            consume(Token_type::ARRAY_SUBSCRIPT_CLOSE, "Expected ']' after array subscript.");
        }
        else{printf("Type_factor ::= ''\n");}
    }

    // Expressions ::= Expression Expressions | ''
    void parseExpressions() {
        printf("Expressions ::= Expression Expressions\n");
        // if(a++ > 9) return;
        // else printf("A value: %d\n", a);
        printf("POSSIBLE EXPRESSION->");
        peek().print();
        if(parseExpression() == 1){
            
            parseExpressions();
        }
        else {
            printf("Expressions::= empty \n");
        }
    }

    // Expression ::= OrExpr Factor_more
    //              | AndExpr Factor_more
    //              | EqExpr Factor_more
    //              | RelExpr Factor_more
    //              | AddExpr Factor_more
    //              | MulExpr Factor_more
    //              | UnaryExpr Factor_more
    //              | SelfExpr 
    //              | VarDecl_more 
    int parseExpression() {
        Token a = peek();
        printf("Token_type: %s\n", a.type._to_string());
        if (match(Token_type::LOGICAL_OR)) {
            parseFactorMore();
            return 1;
        } else if (match(Token_type::LOGICAL_AND)) {
            parseFactorMore();
            return 1;
        } else if (match(Token_type::COMPARISON) || match(Token_type::NOT_EQUAL)) {
            printf("Expression ::= EqExpr Factor_more\n");
            parseFactorMore();
            return 1;
        } else if (match(Token_type::LESS_THAN) || match(Token_type::MORE_THAN)
            || match(Token_type::LESS_THAN_EQUAL) || match(Token_type::MORE_THAN_EQUAL)){
            printf(" Expression ::= EqExpr Factor_more \n");
            parseFactorMore();
            return 1;
        } else if (match(Token_type::SUM_) || match(Token_type::SUBSTRACTION)) {
            printf(" Expression ::= AddEXPR Factor_more \n");
            parseFactorMore();
            return 1;
        } else if (match(Token_type::MULTIPLICATION) || match(Token_type::DIVISION) || match(Token_type::MODULUS)) {
            parseFactorMore();
            return 1;
        } else if (match(Token_type::LOGICAL_NOT)) {
            parseFactorMore();
            return 1;
        } else if(match(Token_type::SELF_ADDITION) || match(Token_type::SELF_SUBSTRACTION)){
            printf(" Expression ::= SelfExpr \n");
            return 1;
        } else if(check(Token_type::ASSIGNMENT)){
            parseVarDecl_more();
            return 1;
        }

        printf("No match for this\n");
        return -1; 
    }

    // Factor ::= Literal | ( Factor_more Expressions )
    void parseFactor() {
        printf("Factor ::= ");
        if (match(Token_type::PAREN_OPEN)) {
            printf("( Factor_more Expressions )\n");
            parseFactorMore();
            parseExpressions();
            consume(Token_type::PAREN_CLOSE, "Expected ')' after expression.");
        } else {
            printf("Literal\n");
            parseLiteral();
        }
    }

    // Factor_more ::= UnaryExpr Factor | identifier Type_Factor
    void parseFactorMore() {
        printf("Factor_more::= ");
        if (match(Token_type::IDENTIFIER)) {
            printf("identifier Type_Factor\n");
            parseTypeFactor();
            //peek().print();
            return;
        }
        printf("UnaryExpr Factor\n");
        if(check(Token_type::BINARY_NEGATION)){
            advance();
        }
        parseFactor();
    }

    // IfStmt ::= if ( Factor_more Expressions ) StatementList Else
    void parseIfStmt() {
        printf("IfStmt ::= if ( Factor_more Expression) StatementList Else\n");
        consume(Token_type::PAREN_OPEN, "Expected '(' after 'if'.");
        parseFactorMore();
        parseExpressions();
        consume(Token_type::PAREN_CLOSE, "Expected ')' after condition.");
        parseStmtList();
        parseElse();
    }

    // Else ::= else StatementList | ''
    void parseElse() {
        if(current < total_tokens && match(Token_type::ELSE)) {
            printf("Else ::= else StatementList\n");
            parseStmtList();
        }
        else printf("Else ::= empty\n");
    }
    

    // ForStmt ::= for ( VarDecl ; Factor_more Expressions ; Factor_more Expressions ) Statement
    void parseForStmt() {
        printf("ForStmt ::= for ( VarDecl ; Factor_more Expressions ; Factor_more Expressions ) Statement\n");
        consume(Token_type::PAREN_OPEN, "Expected '(' after 'for'.");
        parseVarDecl();
        //consume(Token_type::SEMICOLON, "Expected ';' after VarDecl.");
        //printf("----------------------Second part of ForStmt\n");
        //printf("wtf");
        parseFactorMore();
        parseExpressions();
        consume(Token_type::SEMICOLON, "Expected ';' after expression.");
        parseFactorMore();
        parseExpressions();
        consume(Token_type::PAREN_CLOSE, "Expected ')' after for loop conditions.");
        peek().print();printf("\n");
        parseStatement();
        printf("PARSED FOR STATEMENT\n");
    }

    // WhileStmt ::= while ( Factor_more Expressions ) Statement
    void parseWhileStmt() {
        consume(Token_type::PAREN_OPEN, "Expected '(' after 'while'.");
        parseFactorMore();
        parseExpressions();
        consume(Token_type::PAREN_CLOSE, "Expected ')' after condition.");
        parseStatement();
    }

    // ReturnStmt ::= RETURN Factor_more Expressions;
    void parseReturnStmt() {
        parseFactorMore();
        parseExpressions();
        consume(Token_type::SEMICOLON, "Expected ';' after return statement.");
    }

    // PrintStmt ::= PRINT ( Factor_more Expressions ) ;
    void parsePrintStmt() {
        consume(Token_type::PAREN_OPEN, "Expected '(' after 'print'.");
        parseFactorMore();
        parseExpressions();
        consume(Token_type::PAREN_CLOSE, "Expected ')' after expression list.");
        consume(Token_type::SEMICOLON, "Expected ';' after print statement.");
    }

    // VarDecl_more ::= = FactorMore Expressions ;
    void parseVarDecl_more() {
        printf("VarDecl_more ::= = FactorMore Expressions ;\n");
        consume(Token_type::ASSIGNMENT, "Expected '=' after variable declaration.");
        //printf("Does it get here?\n");
        parseFactorMore();
        printf("FROM VarDecl_more-> ");
        peek().print();
        if(check(Token_type::SEMICOLON)){

            // printf("FINISHED FIRST PART OF FOR\n");
            consume(Token_type::SEMICOLON, "Expected ';' after variable declaration.");
            printf("FINISHED PARSING VAR DECLARATION\n");
            //return;
        } 
        else {
            
            parseExpressions();
            consume(Token_type::SEMICOLON, "Expected ';' after variable declaration.");
            printf("FINISHED PARSING VAR DECLARATION\n");
            // dbg(peek().val);
        }
        //consume(Token_type::SEMICOLON, "Expected ';' after variable declaration.");
    }

    //VarDecl ::= identifier Type_factor VarDecl_more | ;

    void parseVarDecl(){
        if(match(Token_type::SEMICOLON)){
            printf("VarDecl::= ;\n");
            return;
        }
        printf("VarDecl::= identifier Type_factor VarDecl_more\n");
        consume(Token_type::IDENTIFIER, "Expected an identifier for variable assignment.");
        //printf("")
        parseTypeFactor();
        parseVarDecl_more();
        //return ;
    }

    // Block ::= { StmtList }
    void parseBlock() {
        consume(Token_type::BRACE_OPEN, "Expected '{' before block.");
        printf("Block ::= { StmtList }\n");
        parseStmtList();
        consume(Token_type::BRACE_CLOSE, "Expected '}' after block.");
        printf("FINISHED PARSING BLOCK\n");
    }

    // StmtList ::= Statement StmtList | ''
    void parseStmtList() {
        /*while (current < total_tokens && !isAtEnd() && !check(Token_type::BRACE_CLOSE)) {
            printf("StmtList ::= Statement StmtList\n");
            parseStatement();
        }*/
       if(current < total_tokens && !isAtEnd() && !check(Token_type::BRACE_CLOSE)) {
            printf("StmtList ::= Statement StmtList\n");
            parseStatement();
        }
        printf("StmtList ::= emtpy\n");
    }

    // Literal ::= integer_literal | boolean_literal | char_literal | string_literal
    void parseLiteral() {
        printf("Literal ::= ");
        string &value = tokens[current].val;

        // Literal ::= integer_literal | boolean_literal | char_literal | string_literal
        if (value == "true" || value == "false") {printf("boolean_literal\n"); advance(); return;}

        // Verificar si el valor es un número (integer literal)
        bool isIntegerLiteral = 1;
        for(auto &dig : value){
            if(dig >= '0' && dig <= '9') continue;
            else isIntegerLiteral = 0;
        }
        if(isIntegerLiteral) {printf("integer_literal\n"); advance(); return;}

        // Verificar si es un literal de carácter (entre comillas simples)
        if (value.size() >= 3 && value.front() == '\'' && value.back() == '\'') {printf("char_literal\n"); advance(); return;}

        // Verificar si es un literal de cadena (entre comillas dobles)
        if (value.size() >= 3 && value.front() == '\"' && value.back() == '\"') {printf("string literal\n"); advance(); return;}

        error("Expected a literal.");
    }

};
