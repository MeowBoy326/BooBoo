#include <cmath>
#include <cstring>

#include "booboo/booboo.h"
#include "booboo/internal.h"

namespace booboo {

bool breaker_reset(Program &prg, std::vector<Token> &v)
{
	std::string name = as_string(prg, v[0]);

	reset_game_name = name;

	return false;
}

bool breaker_exit(Program &prg, std::vector<Token> &v)
{
	return_code = as_number(prg, v[0]);
	reset_game_name = "";
	quit = true;
	return false;
}

bool breaker_return(Program &prg, std::vector<Token> &v)
{
	if (v[0].type == Token::NUMBER) {
		prg.s->result.type = Variable::NUMBER;
		prg.s->result.n = v[0].n;
	}
	else if (v[0].type == Token::SYMBOL) {
		prg.s->result = prg.variables[v[0].i];
	}
	else {
		prg.s->result.type = Variable::STRING;
		prg.s->result.s = v[0].s;
	}

	prg.s->result.name = "result";

	return false;
}

bool corefunc_number(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)
	return true;
}

bool corefunc_string(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)
	return true;
}

bool corefunc_vector(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)
	prg.variables[v[0].i].v.clear();
	return true;
}

void start_lib_core()
{
	add_syntax("reset", breaker_reset);
	add_syntax("exit", breaker_exit);
	add_syntax("return", breaker_return);
	add_syntax("number", corefunc_number);
	add_syntax("string", corefunc_string);
	add_syntax("vector", corefunc_vector);
}

} // end namespace booboo
