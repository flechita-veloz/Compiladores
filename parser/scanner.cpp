#include "scanner.hpp"
#include "token.hpp"
#include "parser.hpp"
using namespace std;

// id no debe comenzar con un numero
bool id_starts_with_num(Token &token) {
  
  if (is_digit(token.val[0])) {
    for (int i = 1; i < token.val.size(); i++) {
      if (!is_digit(token.val[i])) {
        token.resumen_error = "Identificador no puede comenzar con un numero";
        token.resumen_solution = "Cambia el nombre del identificador que "
                                 "comienze con un numero o subguion";
        return 1;
      }
    }
  }
  return 0;
}

// palabras reservadas no deben ser id
bool id_is_reserved_word(Token &token) {
  if (token.val == "print" || token.val == "while" || token.val == "if" ||
      token.val == "for") {
    return 0;
  }
  if (is_identifier(token.val[0]) && is_reserved_word(token.val)) {
    token.resumen_error = "Identificador no puede ser una palabra reservada";
    token.resumen_solution = "Cambiar el nombre del identificador";
    return 1;
  }
  return 0;
}

// Simbolo que no pertenezca a la especificacion de ningun token.
bool is_valid_token(Token &token) {
  int n = token.val.size();
  for (int i = 0; i < n; i++) {
    char curr = token.val[i];
    if (('0' <= curr && curr <= '9') || ('a' <= curr && curr <= 'z') ||
        ('A' <= curr && curr <= 'Z') || ('_' == curr)) {
      continue;
    } else if (is_operator(curr)) {
      if (i + 1 < n && is_operator(token.val[i + 1])) {
        string str = token.val.substr(i, 2);
        if (twoCharOperators.find(str) != twoCharOperators.end()) {
          continue;
        }
      } else
        continue;
    }
    token.resumen_error =
        "Simbolo no pertenece a la especificacion de ningun token";
    token.resumen_solution = "Cambiar el simbolo a uno valido";
    return 0;
  }
  return 1;
}

// Verificar tamaño máximo permitido para literales
bool size_number_is_allowed(Token &token) {
  if (is_digit(token.val[0])) {
    try {
      int64_t num = std::stoll(token.val);
      if (num >= std::numeric_limits<int64_t>::min() &&
          num <= std::numeric_limits<int64_t>::max()) {
        return 1;
      }
    } catch (const std::out_of_range &) {
      token.resumen_error = "El valor del entero es mayor a 64 bits";
      token.resumen_solution =
          "Cambiar el valor del entero a un entero de 64 bits";
      return 0;
    }
  }
  return 1;
}

int main() {
  printf("Holaa\n");
  ifstream input("/Users/abimaelruiztrelles/Downloads/compiladores/ejemplo4.txt");
  printf("Holaa\n");
  // cout<< input.rdbuf();
  vector<Token> tokens; 
  long long line_number = 1;
  long long column_number = 1;
  ini_keywords();
  char c;
  // bool start_of_line = true;
  // long long cccc = 0;
  int ERRORS = 0;
  while (tokens.empty() || !tokens.back().eof()) {
    c = peekchar(input);
    if(c == '\n'){
      line_number++;
      c = getchar(input);
      column_number = 1;
    }
    bool advance = 0;
    while (peekchar(input) == ' ' || peekchar(input) == '\n' ||
           peekchar(input) == '\t')
      c = getchar(input), advance = 1;
    // comentarios
    if (c == '\n' && advance) {
      line_number++;
      column_number = 1;
    }
    auto current_token = gettoken(line_number, column_number, input, true);
    //current_token.print();
    // if(current_token.val == "{"){
    //   char ch = getchar(input);
    //   if(ch == '\n') dbg(ch);
    //   break;
    // }
    if (current_token.eof())
      break;
    if (!current_token.val.empty())
      tokens.push_back(current_token);
  }
  int n = tokens.size();
  for (int i = 0; i < n; i++) {
    if (i + 1 < n && (tokens[i + 1].val == "(" || tokens[i + 1].val == ":") &&
        (id_starts_with_num(tokens[i]) || id_is_reserved_word(tokens[i]))) {
      ERRORS++;
    } else if (!is_valid_token(tokens[i]) ||
               !size_number_is_allowed(tokens[i])) {
      ERRORS++;
    }
  }
  cout << "\n\nINFO SCAN - Start scanning..." << endl;
  for (auto token : tokens) {
    token.print();

  }

   printf("INFO SCAN - Completed with %d errors\n", ERRORS);

  Parser parser(tokens);
  parser.parseProgram();

  return 0;
}

// a = 2 + 7 $