#include "token.hpp"
#include "parser.hpp"

using namespace std;

int main(){
    // integer identifier ( integer identifier ) { identifier : integer [ IntegerLiteral ] = identifier + identifier ; }
    // vector<Token> tokens = {
    //     { Token_type::INT_KW, "integer" },
    //     { Token_type::IDENTIFIER, "identifier" },
    //     { Token_type::PAREN_OPEN, "(" },
    //     { Token_type::INT_KW, "integer" },
    //     { Token_type::IDENTIFIER, "identifier" },
    //     { Token_type::PAREN_CLOSE, ")" },
    //     { Token_type::BRACE_OPEN, "{" },
    //     { Token_type::IDENTIFIER, "identifier" },
    //     { Token_type::COLON, ":" },
    //     { Token_type::INT_KW, "integer" },
    //     { Token_type::ARRAY_SUBSCRIPT_OPEN, "[" },
    //     { Token_type::NUMBER, "10324" },
    //     { Token_type::ARRAY_SUBSCRIPT_CLOSE, "]" },
    //     { Token_type::ASSIGNMENT, "=" },
    //     { Token_type::IDENTIFIER, "identifier" },
    //     { Token_type::SUM_, "+" },
    //     { Token_type::IDENTIFIER, "identifier" },
    //     { Token_type::SEMICOLON, ";" },
    //     { Token_type::BRACE_CLOSE, "}" },
    //     { Token_type::END_OF_FILE, "" }
    // };

    // a == 134;
    // vector<Token> tokens = {
    //     { Token_type::IDENTIFIER, "a" },
    //     { Token_type::ASSIGNMENT, "=" },
    //     { Token_type::NUMBER, "134" },
    //     { Token_type::SEMICOLON, ";" },
    //     { Token_type::END_OF_FILE, "" }
    // };

    // for ( identifier [ IntegerLiteral ] = IntegerLiteral; identifier < IntegerLiteral ; IntegerLiteral ) { }
    // vector<Token> tokens = {
    //     { Token_type::FOR_KEY, "for" },
    //     { Token_type::PAREN_OPEN, "(" },
    //     { Token_type::IDENTIFIER, "identifier" },
    //     { Token_type::ARRAY_SUBSCRIPT_OPEN, "[" },
    //     { Token_type::NUMBER, "10324" },
    //     { Token_type::ARRAY_SUBSCRIPT_CLOSE, "]" },
    //     { Token_type::ASSIGNMENT, "=" },
    //     { Token_type::NUMBER, "303" },
    //     { Token_type::SEMICOLON, ";" },
    //     { Token_type::IDENTIFIER, "identifier" },
    //     { Token_type::LESS_THAN, "<"},
    //     { Token_type::NUMBER, "5" },
    //     { Token_type::SEMICOLON, ";" },
    //     { Token_type::NUMBER, "5" },
    //     { Token_type::PAREN_CLOSE, ")" },
    //     { Token_type::BRACE_OPEN, "{" },
    //     { Token_type::BRACE_CLOSE, "}" },
    //     { Token_type::END_OF_FILE, "" }
    // };

    // return ( IntegerLiteral + identifier ) ;
    vector<Token> tokens = {
        { Token_type::INT_KW, "integer" },
        { Token_type::IDENTIFIER, "identifier" },
        { Token_type::PAREN_OPEN, "(" },
        { Token_type::INT_KW, "integer" },
        { Token_type::IDENTIFIER, "identifier" },
        { Token_type::PAREN_CLOSE, ")" },
        { Token_type::BRACE_OPEN, "{" },

        { Token_type::PRINT_KEY, "return" },
        { Token_type::PAREN_OPEN, "(" },
        { Token_type::NUMBER, "303" },
        { Token_type::SUM_, "+" },
        { Token_type::IDENTIFIER, "a" },
        { Token_type::PAREN_CLOSE, ")" },
        { Token_type::SEMICOLON, ";" },

        { Token_type::BRACE_CLOSE, "}" },
        { Token_type::END_OF_FILE, "" },
    };

    // Crear el parser y parsear los tokens
    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
