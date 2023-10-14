#include <cmath>
#include <cstring>

#include "booboo/booboo.h"
#include "booboo/internal.h"

namespace booboo {

bool corefunc_int(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)
	Variable &v1 = as_variable(prg, v[0]);

	v1.n = (int)v1.n;

	return true;
}

bool corefunc_set(Program &prg, std::vector<Token> &v)
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
		Variable &v2 = prg.variables[v[1].i];

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

bool corefunc_add(Program &prg, std::vector<Token> &v)
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

bool corefunc_subtract(Program &prg, std::vector<Token> &v)
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

bool corefunc_multiply(Program &prg, std::vector<Token> &v)
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

bool corefunc_divide(Program &prg, std::vector<Token> &v)
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

bool corefunc_intmod(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);
	int d = as_number(prg, v[1]);
	
	if (v1.type == Variable::NUMBER) {
		v1.n = int(v1.n) % int(d);
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool corefunc_fmod(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);
	double d = as_number(prg, v[1]);

	if (v1.type == Variable::NUMBER) {
		v1.n = fmod(v1.n, d);
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool corefunc_neg(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	Variable &v1 = as_variable(prg, v[0]);

	if (v1.type == Variable::NUMBER) {
		v1.n = -v1.n;
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Operation undefined for operands at " + get_error_info(prg));
	}

	return true;
}

bool corefunc_goto(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	prg.s->pc = as_label(prg, v[0]);

	return true;
}

bool corefunc_compare(Program &prg, std::vector<Token> &v)
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
		prg.compare_flag = strcmp(s1.c_str(), s2.c_str());
	}
	else if (a_string || b_string) {
		prg.compare_flag = 0;
	}
	// FIXME: if they're vectors or something then it should report an error
	else {
		double ad = as_number(prg, v[0]);
		double bd = as_number(prg, v[1]);

		if (ad < bd) {
			prg.compare_flag = -1;
		}
		else if (ad == bd) {
			prg.compare_flag = 0;
		}
		else {
			prg.compare_flag = 1;
		}
	}

	return true;
}

bool corefunc_je(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg.compare_flag == 0) {
		prg.s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jne(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg.compare_flag != 0) {
		prg.s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jl(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg.compare_flag < 0) {
		prg.s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jle(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg.compare_flag <= 0) {
		prg.s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jg(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg.compare_flag > 0) {
		prg.s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_jge(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	if (prg.compare_flag >= 0) {
		prg.s->pc = as_label(prg, v[0]);
	}

	return true;
}

bool corefunc_call(Program &prg, std::vector<Token> &v)
{
	int function = as_function(prg, v[0]);

	Variable result;

	call_void_function(prg, function, v, 1);

	return true;
}

bool corefunc_call_result(Program &prg, std::vector<Token> &v)
{
	Variable &result = as_variable(prg, v[0]);
	int function = as_function(prg, v[1]);

	call_function(prg, function, v, result, 2);

	return true;
}

bool corefunc_print(Program &prg, std::vector<Token> &v)
{
	std::string fmt = as_string(prg, v[0]);
	int _tok = 1;
	
	int prev = 0;
	int arg_count = 0;

	for (size_t i = 0; i < fmt.length(); i++) {
		if (fmt[i] == '%' && prev != '%') {
			arg_count++;
		}
		prev = fmt[i];
	}

	std::string result;
	int c = 0;
	prev = 0;

	for (int arg = 0; arg < arg_count; arg++) {
		int start = c;
		while (c < (int)fmt.length()) {
			if (fmt[c] == '%' && prev != '%') {
				break;
			}
			prev = fmt[c];
			c++;
		}

		result += fmt.substr(start, c-start);

		std::string param = v[_tok++].token;

		std::string val;

		if (param[0] == '-' || isdigit(param[0])) {
			val = param;
		}
		else if (param[0] == '"') {
			val = param;
		}
		else {
			Variable &v1 = as_variable(prg, v[_tok-1]);
			if (v1.type == Variable::NUMBER) {
				char buf[1000];
				snprintf(buf, 1000, "%g", v1.n);
				val = buf;
			}
			else if (v1.type == Variable::STRING) {
				val = v1.s;
			}
			else if (v1.type == Variable::VECTOR) {
				val = "-vector-";
			}
			else {
				val = "-unknown-";
			}
		}

		result += val;

		c++;
	}

	if (c < (int)fmt.length()) {
		result += fmt.substr(c);
	}

	printf("%s", result.c_str());

	return true;
}

bool stringfunc_format(Program &prg, std::vector<Token> &v)
{
	Variable &v1 = as_variable(prg, v[0]);
	std::string fmt = as_string(prg, v[1]);
	int _tok = 2;
	
	int prev = 0;
	int arg_count = 0;

	for (size_t i = 0; i < fmt.length(); i++) {
		if (fmt[i] == '%' && prev != '%') {
			arg_count++;
		}
		prev = fmt[i];
	}

	std::string result;
	int c = 0;
	prev = 0;

	for (int arg = 0; arg < arg_count; arg++) {
		int start = c;
		while (c < (int)fmt.length()) {
			if (fmt[c] == '%' && prev != '%') {
				break;
			}
			prev = fmt[c];
			c++;
		}

		result += fmt.substr(start, c-start);

		std::string param = v[_tok++].token;

		std::string val;

		if (param[0] == '-' || isdigit(param[0])) {
			val = param;
		}
		else if (param[0] == '"') {
			val = param;
		}
		else {
			Variable &v1 = as_variable(prg, v[_tok-1]);
			if (v1.type == Variable::NUMBER) {
				char buf[1000];
				snprintf(buf, 1000, "%g", v1.n);
				val = buf;
			}
			else if (v1.type == Variable::STRING) {
				val = v1.s;
			}
			else if (v1.type == Variable::VECTOR) {
				val = "-vector-";
			}
			else {
				val = "-unknown-";
			}
		}

		result += val;

		c++;
	}

	if (c < (int)fmt.length()) {
		result += fmt.substr(c);
	}

	v1.type = Variable::STRING;
	v1.s = result;

	return true;
}

bool stringfunc_char_at(Program &prg, std::vector<Token> &v)
{
	Variable &v1 = as_variable(prg, v[0]);
	std::string s = as_string(prg, v[1]);
	int index = as_number(prg, v[2]);

	int value = s[index];

	if (v1.type == Variable::NUMBER) {
		v1.n = value;
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool stringfunc_length(Program &prg, std::vector<Token> &v)
{
	Variable &v1 = as_variable(prg, v[0]);
	std::string s = as_string(prg, v[1]);

	if (v1.type == Variable::NUMBER) {
		v1.n = s.length();
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool stringfunc_from_number(Program &prg, std::vector<Token> &v)
{
	Variable &v1 = as_variable(prg, v[0]);
	int n = as_number(prg, v[1]);

	static char buf[2] = { 0, 0 };
	
	buf[0] = n;

	if (v1.type == Variable::STRING) {
		v1.s = buf;
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

bool mathfunc_sin(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);

	if (v1.type == Variable::NUMBER) {
		v1.n = sin(as_number(prg, v[1]));
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Operation undefined for operands at " + get_error_info(prg));
	}

	return true;
}

bool mathfunc_cos(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);

	if (v1.type == Variable::NUMBER) {
		v1.n = cos(as_number(prg, v[1]));
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Operation undefined for operands at " + get_error_info(prg));
	}

	return true;
}

bool mathfunc_atan2(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(3)

	Variable &v1 = as_variable(prg, v[0]);

	if (v1.type == Variable::NUMBER) {
		v1.n = atan2(as_number(prg, v[1]), as_number(prg, v[2]));
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Operation undefined for operands at " + get_error_info(prg));
	}

	return true;
}

bool mathfunc_abs(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);

	if (v1.type == Variable::NUMBER) {
		v1.n = fabs(as_number(prg, v[1]));
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Operation undefined for operands at " + get_error_info(prg));
	}

	return true;
}

bool mathfunc_pow(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);

	if (v1.type == Variable::NUMBER) {
		v1.n = pow(as_number(prg, v[1]), as_number(prg, v[2]));
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Operation undefined for operands at " + get_error_info(prg));
	}

	return true;
}

bool mathfunc_sqrt(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &v1 = as_variable(prg, v[0]);

	if (v1.type == Variable::NUMBER) {
		v1.n = sqrt(as_number(prg, v[1]));
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Operation undefined for operands at " + get_error_info(prg));
	}

	return true;
}

static bool vectorfunc_add(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &id = as_variable(prg, v[0]);

	Variable var;

	if (v[1].type == Token::NUMBER) {
		var.type = Variable::NUMBER;
		var.name = "-constant-";
		var.n = v[1].n;
	}
	else if (v[1].type == Token::SYMBOL) {
		var = as_variable(prg, v[1]);
	}
	else {
		var.type = Variable::STRING;
		var.name = "-constant-";
		var.s = v[1].s;
	}

	id.v.push_back(var);

	return true;
}

static bool vectorfunc_size(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &id = as_variable(prg, v[0]);
	Variable &v1 = as_variable(prg, v[1]);

	if (v1.type == Variable::NUMBER) {
		v1.n = id.v.size();
	}
	else {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid type at " + get_error_info(prg));
	}

	return true;
}

static bool vectorfunc_set(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(3)

	Variable &id = as_variable(prg, v[0]);
	double index = as_number(prg, v[1]);
	
	if (index < 0 || index >= id.v.size()) {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid index at " + get_error_info(prg));
	}

	Variable var;

	if (v[2].type == Token::NUMBER) {
		var.type = Variable::NUMBER;
		var.name = "-constant-";
		var.n = v[2].n;
	}
	else if (v[2].type == Token::SYMBOL) {
		var = as_variable(prg, v[2]);
	}
	else {
		var.type = Variable::STRING;
		var.name = "-constant-";
		var.s = v[2].s;
	}

	id.v[index] = var;

	return true;
}

static bool vectorfunc_insert(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(3)

	Variable &id = as_variable(prg, v[0]);
	double index = as_number(prg, v[1]);

	if (index < 0 || index > id.v.size()) {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid index at " + get_error_info(prg));
	}

	Variable var;

	if (v[2].type == Token::NUMBER) {
		var.type = Variable::NUMBER;
		var.name = "-constant-";
		var.n = v[2].n;
	}
	else if (v[2].type == Token::SYMBOL) {
		var = as_variable(prg, v[2]);
	}
	else {
		var.type = Variable::STRING;
		var.name = "-constant-";
		var.s = v[2].s;
	}

	id.v.insert(id.v.begin()+index, var);

	return true;
}

static bool vectorfunc_get(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(3)

	Variable &id = as_variable(prg, v[0]);
	Variable &v1 = as_variable(prg, v[1]);
	double index = as_number(prg, v[2]);

	if (index < 0 || index >= id.v.size()) {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid index at " + get_error_info(prg));
	}

	std::string bak = v1.name;
	v1 = id.v[index];
	v1.name = bak;

	return true;
}

static bool vectorfunc_erase(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(2)

	Variable &id = as_variable(prg, v[0]);
	double index = as_number(prg, v[1]);

	if (index < 0 || index >= id.v.size()) {
		throw Error(std::string(__FUNCTION__) + ": " + "Invalid index at " + get_error_info(prg));
	}

	id.v.erase(id.v.begin() + int(index));

	return true;
}

static bool vectorfunc_clear(Program &prg, std::vector<Token> &v)
{
	COUNT_ARGS(1)

	Variable &id = as_variable(prg, v[0]);

	id.v.clear();

	return true;
}

static std::string tokenfunc_add(booboo::Program &prg)
{
	prg.s->p++;
	return "+";
}

static std::string tokenfunc_subtract(booboo::Program &prg)
{
	char s[2];
	s[1] = 0;

	prg.s->p++;
	if (prg.s->p < prg.s->code.length() && isdigit(prg.s->code[prg.s->p])) {
		std::string tok = "-";
		while (prg.s->p < prg.s->code.length() && (isdigit(prg.s->code[prg.s->p]) || prg.s->code[prg.s->p] == '.')) {
			s[0] = prg.s->code[prg.s->p];
			tok += s;
			prg.s->p++;
		}
		return tok;
	}
	else {
		return "-";
	}
}

static std::string tokenfunc_equals(booboo::Program &prg)
{
	prg.s->p++;
	return "=";
}

static std::string tokenfunc_compare(booboo::Program &prg)
{
	prg.s->p++;
	return "?";
}

static std::string tokenfunc_multiply(booboo::Program &prg)
{
	prg.s->p++;
	return "*";
}

static std::string tokenfunc_divide(booboo::Program &prg)
{
	prg.s->p++;
	return "/";
}

static std::string tokenfunc_modulus(booboo::Program &prg)
{
	prg.s->p++;
	return "%";
}

static void init_token_map()
{
	booboo::add_token('+', tokenfunc_add);
	booboo::add_token('-', tokenfunc_subtract);
	booboo::add_token('=', tokenfunc_equals);
	booboo::add_token('?', tokenfunc_compare);
	booboo::add_token('*', tokenfunc_multiply);
	booboo::add_token('/', tokenfunc_divide);
	booboo::add_token('%', tokenfunc_modulus);
}	

void start_lib_basics()
{
	init_token_map();

	add_syntax("int", corefunc_int);
	add_syntax("=", corefunc_set);
	add_syntax("+", corefunc_add);
	add_syntax("-", corefunc_subtract);
	add_syntax("*", corefunc_multiply);
	add_syntax("/", corefunc_divide);
	add_syntax("%", corefunc_intmod);
	add_syntax("fmod", corefunc_fmod);
	add_syntax("neg", corefunc_neg);
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
	add_syntax("print", corefunc_print);
	add_syntax("string_format", stringfunc_format);
	add_syntax("string_char_at", stringfunc_char_at);
	add_syntax("string_length", stringfunc_length);
	add_syntax("string_from_number", stringfunc_from_number);
	add_syntax("sin", mathfunc_sin);
	add_syntax("cos", mathfunc_cos);
	add_syntax("atan2", mathfunc_atan2);
	add_syntax("abs", mathfunc_abs);
	add_syntax("pow", mathfunc_pow);
	add_syntax("sqrt", mathfunc_sqrt);
	add_syntax("vector_add", vectorfunc_add);
	add_syntax("vector_size", vectorfunc_size);
	add_syntax("vector_set", vectorfunc_set);
	add_syntax("vector_insert", vectorfunc_insert);
	add_syntax("vector_get", vectorfunc_get);
	add_syntax("vector_erase", vectorfunc_erase);
	add_syntax("vector_clear", vectorfunc_clear);
}

} // end namespace booboo
