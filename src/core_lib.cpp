#include <cmath>
#include <cstring>

#include "booboo/booboo.h"
#include "booboo/internal.h"

namespace booboo {

bool breaker_reset(Program *prg, std::vector<Token> &v)
{
	std::string name = as_string(prg, v[0]);

	reset_game_name = name;

	return false;
}

bool breaker_exit(Program *prg, std::vector<Token> &v)
{
	return_code = as_number(prg, v[0]);
	reset_game_name = "";
	quit = true;
	return false;
}

bool breaker_return(Program *prg, std::vector<Token> &v)
{
	if (v[0].type == Token::NUMBER) {
		prg->s->result.type = Variable::NUMBER;
		prg->s->result.n = v[0].n;
	}
	else if (v[0].type == Token::SYMBOL) {
		prg->s->result = prg->variables[v[0].i];
	}
	else {
		prg->s->result.type = Variable::STRING;
		prg->s->result.s = v[0].s;
	}

	prg->s->result.name = "result";

	return false;
}

bool corefunc_number(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)
	return true;
}

bool corefunc_string(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)
	return true;
}

bool corefunc_vector(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)
	prg->variables[v[0].i].v.clear();
	return true;
}

bool corefunc_set(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);

	if (v[1].type == Token::NUMBER) {
		if (v1.type == Variable::NUMBER) {
			v1.n = v[1].n;
		}
		else if (v1.type == Variable::STRING) {
			char buf[1000];
			snprintf(buf, 1000, "%g", v[1].n);
			v1.s = buf;
		}
		else {
			throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
		}
	}
	else if (v[1].type == Token::STRING) {
		if (v1.type == Variable::NUMBER) {
			v1.n = atof(v[1].s.c_str());
		}
		else if (v1.type == Variable::STRING) {
			v1.s = v[1].s;
		}
		else {
			throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
		}
	}
	else {
		Variable &v2 = prg->variables[v[1].i];

		if (v1.type == Variable::NUMBER && v2.type == Variable::NUMBER) {
			v1.n = v2.n;
		}
		else if (v1.type == Variable::STRING && v2.type == Variable::NUMBER) {
			v1.s = itos(v2.n);
		}
		else if (v1.type == Variable::STRING && v2.type == Variable::STRING) {
			v1.s = v2.s;
		}
		else if (v1.type == Variable::VECTOR && v2.type == Variable::VECTOR) {
			v1.v = v2.v;
		}
		else {
			throw Error(std::string(__FUNCTION__) + ": " + "Operation undefined for operands at " + get_error_info(prg));
		}
	}

	return true;
}

bool corefunc_add(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);

	if (v1.type == Variable::NUMBER) {
		double d = as_number(prg, v[1]);
		v1.n += d;
	}
	else if (v1.type == Variable::STRING) {
		std::string s = as_string(prg, v[1]);
		v1.s += s;
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool corefunc_subtract(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);
	double d = as_number(prg, v[1]);

	if (v1.type == Variable::NUMBER) {
		v1.n -= d;
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool corefunc_multiply(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);
	double d = as_number(prg, v[1]);

	if (v1.type == Variable::NUMBER) {
		v1.n *= d;
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool corefunc_divide(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);
	double d = as_number(prg, v[1]);

	if (v1.type == Variable::NUMBER) {
		v1.n /= d;
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool corefunc_goto(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	prg->s->pc = as_label(prg, v[0]);

	return true;
}

bool corefunc_compare(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	bool a_string = false;
	bool b_string = false;
	std::string s1;
	std::string s2;

	if (v[0].type == Token::STRING) {
		a_string = true;
		s1 = v[0].s;
	}
	else if (v[0].type == Token::SYMBOL) {
		Variable &var = as_variable(prg, v[0]);
		if (var.type == Variable::STRING) {
			a_string = true;
			s1 = var.s;
		}
	}
	
	if (v[1].type == Token::STRING) {
		b_string = true;
		s2 = v[1].s;
	}
	else if (v[1].type == Token::SYMBOL) {
		Variable &var = as_variable(prg, v[1]);
		if (var.type == Variable::STRING) {
			b_string = true;
			s2 = var.s;
		}
	}
	
	if (a_string && b_string) {
		prg->compare_flag = strcmp(s1.c_str(), s2.c_str());
	}
	else if (a_string || b_string) {
		prg->compare_flag = 0;
	}
	// FIXME: if they're vectors or something then it should report an error
	else {
		double ad = as_number(prg, v[0]);
		double bd = as_number(prg, v[1]);

		if (ad < bd) {
			prg->compare_flag = -1;
		}
		else if (ad == bd) {
			prg->compare_flag = 0;
		}
		else {
			prg->compare_flag = 1;
		}
	}

	return true;
}

bool corefunc_je(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg->compare_flag == 0) {
		prg->s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jne(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg->compare_flag != 0) {
		prg->s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jl(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg->compare_flag < 0) {
		prg->s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jle(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg->compare_flag <= 0) {
		prg->s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jg(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg->compare_flag > 0) {
		prg->s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jge(Program *prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg->compare_flag >= 0) {
		prg->s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_call(Program *prg, std::vector<Token> &v)
{
	int function = as_function(prg, v[0]);

	Variable result;

	call_void_function(prg, function, v, 1);

	return true;
}

bool corefunc_call_result(Program *prg, std::vector<Token> &v)
{
	Variable &result = as_variable(prg, v[0]);
	int function = as_function(prg, v[1]);

	call_function(prg, function, v, result, 2);

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
	
	add_syntax("=", corefunc_set);
	add_syntax("+", corefunc_add);
	add_syntax("-", corefunc_subtract);
	add_syntax("*", corefunc_multiply);
	add_syntax("/", corefunc_divide);
	
	add_syntax("goto", corefunc_goto);
	add_syntax("?", corefunc_compare);
	add_syntax("je", corefunc_je);
	add_syntax("jne", corefunc_jne);
	add_syntax("jl", corefunc_jl);
	add_syntax("jle", corefunc_jle);
	add_syntax("jg", corefunc_jg);
	add_syntax("jge", corefunc_jge);
	add_syntax("call", corefunc_call);
	add_syntax("call_result", corefunc_call_result);
}

} // end namespace booboo
