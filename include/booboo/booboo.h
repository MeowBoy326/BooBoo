#ifndef BOOBOO_H
#define BOOBOO_H

#include <string>
#include <vector>
#include <map>

namespace booboo {

class Error {
public:
	Error();
	Error(std::string error_message);
	virtual ~Error();
	
	std::string error_message;
};

struct Variable
{
	enum Variable_Type {
		NUMBER = 0,
		STRING,
		VECTOR,
		LABEL,
		FUNCTION
	} type;

	std::string name;

	double n;
	std::string s;
	std::vector<Variable> v;
};

struct Label {
	std::string name;
	int pc;
};

struct Token {
	enum Token_Type {
		STRING = 0,
		SYMBOL, // alphanumeric and underscores like variable names
		NUMBER
	};

	Token_Type type;

	std::string s;
	double n;
	int i;

	std::string token;
};

struct Statement {
	int method;
	std::vector<Token> data;
};

enum Pass {
	PASS0, // nothing started yet
	PASS1,
	PASS2
};

// This stuff is simply grouped to make swapping it out of the running program fast with a pointer
// when doing a function call (most of the state of the program remains the same but this stuff
// gets swapped out for the function's versions of these variables, then some of them like p/line
// get set to 0/1
struct Function_Swap {
	std::string name;

	std::string code;

	unsigned int p;
	unsigned int line;
	unsigned int start_line;

	Variable result;
	std::vector<Statement> program;
	unsigned int pc;
};

struct Program {
	Function_Swap *s;

	int compare_flag;
	Pass complete_pass;

	std::vector<Variable> variables;
	std::map<std::string, int> variables_map;
	std::vector<Program> functions;
	std::vector<int> params;

	std::vector<int> line_numbers;

	std::vector<int> real_line_numbers;
	std::vector<std::string> real_file_names;

	std::map<std::string, void *> black_box;
};

// These are for adding syntax
typedef bool (*library_func)(Program &prg, std::vector<Token> &v);

// This stuff can be used but it's used by the BooBoo interpreter
extern std::string reset_game_name;
extern std::string main_program_name;
extern int return_code;
extern bool quit;

// Call these before/after using BooBoo
void start();
void end();

// These are the how you create, destroy and run programs
Program create_program(std::string code);
void destroy_program(Program &prg);
bool interpret(Program &prg);

// Functions calling
void call_function(Program &prg, int function, std::vector<Token> &params, Variable &result, int ignore_params = 0);
void call_function(Program &prg, std::string function, std::vector<Token> &params, Variable &result, int ignore_params = 0);
void call_void_function(Program &prg, int function, std::vector<Token> &params, int ignore_params = 0);
void call_void_function(Program &prg, std::string function, std::vector<Token> &params, int ignore_params = 0);

// To create token vectors for calling functions
Token token_number(std::string token, double n);
Token token_string(std::string token, std::string s);

// For dealing with results
double get_number(Variable &v);
std::string get_string(Variable &v);
std::vector<Variable> get_vector(Variable &v);

void add_syntax(std::string name, library_func func);
int get_line_num(Program &prg);
std::string get_file_name(Program &prg);
std::string get_error_info(Program &prg);

// These are helpful within your own library functions
Variable &as_variable(Program &prg, Token &t);
double as_number(Program &prg, Token &t);
std::string as_string(Program &prg, Token &t);
int as_label(Program &prg, Token &t);
int as_function(Program &prg, Token &t);

// This adds only the core syntax (6 library functions and a few extra keywords)
void start_lib_core();

// This one adds basic syntax like arithmetic and vector manipulation
void start_lib_basics();

typedef std::string (*token_func)(Program &);

void add_token(char token, token_func func);

} // End namespace booboo

// You can use this at the start of your library functions to ensure correct number of arguments
// (if you are not using variable number of arguments)
#define COUNT_ARGS(n) if (v.size() != n) throw Error(std::string(__FUNCTION__) + ": " + "Incorrect number of arguments at " + get_error_info(prg));

#endif // BOOBOO_H
