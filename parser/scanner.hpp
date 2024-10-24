#pragma once
#include "resources.hpp"
#include "token.hpp"

char getchar(ifstream& input)
{
	return input.get();	//avanza el puntero
}


char peekchar(ifstream& input)
{
	return input.peek(); //sin avanzar 
}


bool is_comment(ifstream& input, char &current_char, long long &line_number){ 
	if(input.eof()){
		return 0;
	}
	char next_char = peekchar(input);
	if(current_char == '/' && next_char == '/'){
		line_number++;
		while(current_char != '\n'){
			current_char = getchar(input);
		}
		return 1;
	}
	if(current_char == '#'){
		line_number++;
		while(current_char != '\n'){
			current_char = getchar(input);
		}
		dbg(current_char);
		return 1;
	}
	if(current_char == '/' && next_char == '*'){
		line_number++;
		while(current_char != '*' || next_char != '/'){
			if(current_char == '\n') line_number++;
			current_char = getchar(input);
			next_char = peekchar(input);
		}
		current_char = getchar(input);
		return 1;
	}
	return 0;
}



int is_reserved_word(string word){
	auto it = keywords.find(word);
	if(it != keywords.end()) return it->second;
	return Token_type::IDENTIFIER;

}

int is_operator(char& c){
	auto it = operators.find(c);
	if(it != operators.end()) return it->second;
	return 0;
}

int is_operator2(char& c, ifstream& p, long long& column_number, Token& token){
	auto it = operators.find(c);
	if (it != operators.end()) {
			char next_char = peekchar(p);
			string possible_two_char = string(1, c) + next_char;

			auto it_2 = twoCharOperators.find(possible_two_char);
			if (it_2 != twoCharOperators.end()) {
					getchar(p); 
					column_number++; 
					token.val = possible_two_char;
					return it_2->second;
					
			}
		token.val = c;	
		return it->second; 
		
	}
	
	if (c == '&' || c == '|' || c == '=' || c == '!') {
		char next_char = peekchar(p);
		string possible_two_char = string(1, c) + next_char;
		auto it_2 = twoCharOperators.find(possible_two_char);
		if (it_2 != twoCharOperators.end()) {
				getchar(p); 
				column_number++; 
				token.val = possible_two_char;
				return it_2->second; 
		}

	}

	return 0; 


	
}

bool is_digit(char current_char){
	return current_char >= '0' && current_char <= '9';
}

bool is_identifier(char current_char){
	//solo a-z,A-Z,_,0-9
	return  ((current_char >= 'a' && current_char <= 'z') || 
					(current_char >= 'A' && current_char <= 'Z') || 
					(current_char == '0' && current_char == '9') ||
					(current_char == '_'));
}

Token gettoken(long long& line_number, long long& column_number,  ifstream& input, bool start_of_line = false)
{
	Token token; 
	char current_char;
	int token_start_column = 1;
	bool band = 0;
	do
	{
		current_char = getchar(input);
		// dbg(current_char);
		if (current_char == '\n') {
				line_number++;
				column_number = 1;
				token_start_column = 1;
		}
	} while(current_char == ' ' || current_char == '\t' || current_char == '\n'); // || current_char == '\n'

	
	if(input.eof())
	{
		printf("end of file \n");
		return Token(Token_type::END_OF_FILE);	
	}
	token.line_number = line_number;
	token.column_number = column_number;
	//comment handling
	if (is_comment(input, current_char, line_number)){
		return gettoken(line_number, column_number, input, false);
	}

	//Operation handling
	int possible_op = is_operator2(current_char, input, column_number, token);
	if(possible_op != 0){
		// column_number++;
		// dbg("operadores", current_char);
		token.type = Token_type::_from_integral(possible_op);
		dbg(current_char);
		return token;
	}

	//string handling
	if(current_char == '"'){
		token.type = Token_type::STRING;
		token.val = "\""; 
		while (true) {
				current_char = getchar(input);
				column_number++;

				if (current_char == '"') {
						token.val += "\""; 
						break;
				} 
				else if (current_char == '\\') {
						token.val += '\\';
						current_char = getchar(input);
						column_number++;
						token.val += current_char;
				} 
				else if (current_char == '\n') {
						token.val += "\\n";
				} 
				else {
						token.val += current_char;
				}

		}
		return token;
	}

	//char handling
	if (current_char == '\'') {
			token.type = Token_type::CHAR;
			token.val = "'";

			current_char = getchar(input);
			column_number++;

		if (current_char == '\\') { 
			token.val += '\\';
			current_char = getchar(input);
			column_number++;
			token.val += current_char;
		} 
		else {
			token.val += current_char;
		}

			current_char = getchar(input);
			column_number++;

		if (current_char == '\'') {
			token.val += "'";  //at to the end
		} else {

		}
			return token;
	}

	//number handling
	if (is_digit(current_char)){ 
		while (is_digit(current_char)){
			token.val.push_back(current_char);
			if(is_digit(peekchar(input))){
				 current_char = getchar(input);
				column_number++;
			}
			else break;
		}
		token.type = Token_type::NUMBER;
	} 

	
	else
	{
		if(is_identifier(current_char)){
			while (is_identifier(current_char)){
				token.val.push_back(current_char);
				if (is_identifier(peekchar(input))) {
					current_char = getchar(input);
					column_number++;
				}
				else break;
			}
			token.type = Token_type::_from_integral(is_reserved_word(token.val));
		}
		else{
			token.val.push_back(current_char);
		}
	}
	return token;
}

