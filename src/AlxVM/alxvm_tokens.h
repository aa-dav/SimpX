#ifndef ALXVM_TOKENS
#define ALXVM_TOKENS

#include <stdint.h>
#include <string>
#include <variant>
#include <fstream>

namespace AlxVM
{

enum TokenType
{
	TOKEN_EOL,	// конец строки
	TOKEN_EOF,	// конец файла
	TOKEN_INTEGER,	// целое число
	TOKEN_DOUBLE,	// вещественное число
	TOKEN_CHAR,	// символ (char 8 бит)
	TOKEN_STRING,	// строка (заключенная в кавычки)
	TOKEN_NAME	// имя (идентификатор)
};

struct Token
{
	typedef std::variant<int64_t, double, char, std::string> Variant;
	TokenType type;
	Variant value;

	std::string presentation();	// текстовое представление (в виде тип{значение})

	bool isEnd() { return (type == TOKEN_EOL) || (type == TOKEN_EOF); };

	int64_t &asInt() { return std::get<int64_t>(value); };
	double &asDouble() { return std::get<double>(value); };
	char &asChar() { return std::get<char>(value); };
	std::string &asStr() { return std::get<std::string>(value); };
};

class Tokenizer
{
public:
	virtual ~Tokenizer() {};
	virtual Token next() = 0;
	virtual int getLine() = 0;
	virtual int getPos() = 0;

	static Token extractLexemFromString( const std::string &parseString, size_t &parsePos, size_t &curPos );
};

class FileTokenizer: public Tokenizer
{
	std::ifstream ifs;
	std::string curLexem;
	size_t pos, curLine, curPos;
	bool needFeed;
	
public:
	~FileTokenizer() override {};
	FileTokenizer(const std::string &fileName): ifs( "test.avil", std::ios::in | std::ios::binary ), pos(0), needFeed(true), curLine(0), curPos(0) {};
	Token next() override;
	int getLine() override;
	int getPos() override;
};

}

#endif