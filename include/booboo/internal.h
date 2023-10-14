#ifndef BOOBOO_INTERNAL_H
#define BOOBOO_INTERNAL_H

namespace booboo {

struct Label {
	std::string name;
	int pc;
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

std::string itos(int i);

} // End namespace booboo

#endif // BOOBOO_INTERNAL_H
