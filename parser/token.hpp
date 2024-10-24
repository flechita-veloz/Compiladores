#pragma once
#include "resources.hpp"




BETTER_ENUM(Token_type, int,
//Terminals
IDENTIFIER = 0, //IDENTIFIER ES 0, LA UNICA PALABRA NO CLAVE
SUM_, ASSIGNMENT, SUBSTRACTION, DIVISION, MULTIPLICATION, PAREN_OPEN, PAREN_CLOSE, NUMBER, KEYWORD, LINE_COMMENT, LONG_COMMENT, END_OF_FILE, TRUE_KW, FALSE_KW, NULL_KW, BRACE_OPEN, BRACE_CLOSE, ARRAY_SUBSCRIPT_OPEN, ARRAY_SUBSCRIPT_CLOSE, COLON, SEMICOLON, COMMA, DOT, IF, ELSE, PRINT_KEY, FOR_KEY, EXPONENTIATION, LOGICAL_NOT, RETURN, VOID_KW,
BINARY_NEGATION, MODULUS, LESS_THAN, MORE_THAN, NEW_LINE, FUNCTION, WHILE_KEY, STRING, CHAR, SINGLE_QUOTE, DOUBLE_QUOTE,
//TWO CHAR OPERATORS
LESS_THAN_EQUAL, MORE_THAN_EQUAL, COMPARISON, NOT_EQUAL, SELF_ADDITION,
SELF_SUBSTRACTION, LOGICAL_AND, LOGICAL_OR,
//TIPOS
INT_KW, BOOLEAN_KW, CHAR_KW, STRING_KW_TYPE, ARRAY_KW,  //#
	

//NON TERMINAL
EXPRESSION, FUNCTION_CALL, DECLARATION

);

unordered_map<string, Token_type> keywords;
unordered_map<char, Token_type> operators;
unordered_map<string, Token_type> twoCharOperators;
	
void ini_keywords(){
	keywords.insert({
		{"if", Token_type::IF},
		{"else", Token_type::ELSE},
		{"print", Token_type::PRINT_KEY},
		{"for", Token_type::FOR_KEY},
		{"while", Token_type::WHILE_KEY},
		{"function", Token_type::FUNCTION},
		{"true", Token_type::TRUE_KW},
		{"false", Token_type::FALSE_KW},
		{"integer", Token_type::INT_KW},
		{"array", Token_type::ARRAY_KW},
		{"boolean", Token_type::BOOLEAN_KW},
		{"char", Token_type::CHAR_KW},
		{"string", Token_type::STRING_KW_TYPE},
		{"return", Token_type::RETURN},
		{"void", Token_type::VOID_KW}
	});

	operators.insert({
		{'+', Token_type::SUM_},
		{'=', Token_type::ASSIGNMENT},
		{'-', Token_type::SUBSTRACTION},
		{'*', Token_type::MULTIPLICATION},
		{'/', Token_type::DIVISION},
		{':', Token_type::COLON},
		{';', Token_type::SEMICOLON},
		{',', Token_type::COMMA},
		{'(', Token_type::PAREN_OPEN},
		{')', Token_type::PAREN_CLOSE},
		{'{', Token_type::BRACE_OPEN},
		{'}', Token_type::BRACE_CLOSE},
		{'[', Token_type::ARRAY_SUBSCRIPT_OPEN},
		{']', Token_type::ARRAY_SUBSCRIPT_CLOSE},
		{'^', Token_type::EXPONENTIATION},
		{'~', Token_type::LOGICAL_NOT},
		{'!', Token_type::BINARY_NEGATION},
		{'%', Token_type::MODULUS},
		{'<', Token_type::LESS_THAN},
		{'>', Token_type::MORE_THAN},
		// {'\'', Token_type::SINGLE_QUOTE}, 
		// {'\"', Token_type::DOUBLE_QUOTE}  
	});

	twoCharOperators.insert({
		{"++", Token_type::SELF_ADDITION},
		{"--", Token_type::SELF_SUBSTRACTION},
		{"==", Token_type::COMPARISON},
		{"!=", Token_type::NOT_EQUAL},
		{"||", Token_type::LOGICAL_OR},
		{"&&", Token_type::LOGICAL_AND},
		{"<=", Token_type::LESS_THAN_EQUAL},
		{">=", Token_type::MORE_THAN_EQUAL}
		
	
	});
}


	
struct Token
{
	Token_type type;
	string val;
	int line_number = 1;
	int column_number = 1;
	bool error = 0;
	string resumen_error;
	string resumen_solution;
	Token(Token_type _type = Token_type::IDENTIFIER, string _val = "", int _line_number = 0) 
	: type(_type), val(_val), line_number(_line_number) {}

	bool eof() const {
	return type._to_integral() == Token_type::END_OF_FILE;
}

	bool terminal() const {
	return type._to_integral() < (+Token_type::EXPRESSION)._to_integral();
}

	void print()
{
	if(!resumen_error.empty()){
		printf("ERROR [%s] found at (%d:%d)\nRESUMEN ERROR: %s\nRESUMEN SOLUTION: %s\n", val.c_str(), line_number, column_number, resumen_error.c_str(), resumen_solution.c_str());
	}
	else {
		printf("Token(%s, [ %s ] found at (%d:%d))\n", type._to_string(), val.c_str(), line_number, column_number);
	}
}



};


	
