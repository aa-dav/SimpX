#include "alxvm_tokens.h"

namespace AlxVM
{

static bool isTerminal( char c )
{
    return (c == '(') || (c == ')') || (c == '[') || (c == ']') || (c == ';');
};

static bool isNumberBeginning( char c )
{
    return isdigit( c ) || ( c == '-' );
};

static bool isNumberPart( char c )
{
    return isdigit( c ) || ( c == '.' );
};

// ******************************************
// *                 Token                  *
// ******************************************

std::string Token::presentation()
{
	std::string res;
	switch ( type )
	{
		case AlxVM::TOKEN_EOF:
			res = "EOF";
			break;
		case AlxVM::TOKEN_EOL:
			res = "EOL";
			break;
		case AlxVM::TOKEN_INTEGER:
			res = "int{" + std::to_string(asInt()) + "}";
			break;
		case AlxVM::TOKEN_DOUBLE:
			res = "double{" + std::to_string(asDouble()) + "}";
			break;
		case AlxVM::TOKEN_CHAR:
			res = "char{" + std::string(1, asChar()) + "}";
			break;
		case AlxVM::TOKEN_STRING:
			res = "string{" + asStr() + "}";
			break;
		case AlxVM::TOKEN_NAME:
			res = "name{" + asStr() + "}";
			break;
		default:
			res = "unknown";
			break;
	};
	return res;
};

// ******************************************
// *               Tokenizer                *
// ******************************************

Token Tokenizer::extractLexemFromString( const std::string &parseString, size_t &parsePos, size_t &curPos )
{
	Token res;
	res.type = TOKEN_EOL;

	if ( parsePos >= parseString.length() )
	{
		return res;
	};
	while ( isspace( parseString[ parsePos ] ) )
	{
		if ( ++parsePos >= parseString.length() )
		{
			return res;
		}
		curPos = parsePos;
	};
	char sym = parseString[ parsePos ];
	if ( isTerminal( sym ) )
	{
		res.type = TOKEN_NAME;
		res.value.emplace<std::string>(1, sym);
		++parsePos;
	}
	else if ( sym == '"' )
	{
		res.type = TOKEN_STRING;
		res.value.emplace<std::string>();
		while( true )
		{
			if ( ++parsePos >= parseString.length() )
				throw std::runtime_error( "unexpected end of string '" + res.asStr() + "'!" );
			if ( parseString[ parsePos ] == '"' )
			{
				parsePos++;
				break;
			}
			res.asStr() += parseString[ parsePos ];
		};
	}
	else if ( isNumberBeginning(sym) )
	{
		std::string buffer;
		bool isDouble = false;
		while( true )
		{
			buffer += parseString[ parsePos ];
			if ( parseString[ parsePos ] == '.' )
				isDouble = true;
			if ( ++parsePos >= parseString.length() )
				break;
			if ( !isNumberPart(parseString[ parsePos ]) )
			{
				break;
			}
		};
		if ( isDouble )
		{
			res.type = TOKEN_DOUBLE;
			res.value.emplace<double>(stod(buffer));		
		}
		else
		{
			res.type = TOKEN_INTEGER;
			res.value.emplace<int64_t>(stoll(buffer));		
		}
	}
	else if ( sym == '\'' )
	{
		res.type = TOKEN_CHAR;
		res.value.emplace<char>();
		if ( ++parsePos >= parseString.length() )
			throw std::runtime_error( std::string("unexpected end of char '") + res.asChar() + "'!" );
		std::get<char>(res.value) = parseString[ parsePos ];
		if ( ++parsePos >= parseString.length() )
			throw std::runtime_error( std::string("unexpected end of char '") + res.asChar() + "'!" );
		if ( parseString[ parsePos ] == '\'' )
		{
			parsePos++;
		}
		else
			throw std::runtime_error( std::string("unterminated char '") + res.asChar() + "'!" );
	}
	else
	{
		res.type = TOKEN_NAME;
		res.value.emplace<std::string>();
		while ( !isspace( parseString[ parsePos ] ) )
		{
			res.asStr() += parseString[ parsePos ];
			if ( ++parsePos >= parseString.length() )
				break;
			if ( isTerminal( parseString[ parsePos ] ) )
				break;
		};
	};
	if ( (res.type == TOKEN_NAME) && (res.asStr() == ";") )
	{
		// comment terminates parsing...
		res.type = TOKEN_EOL;
		res.value.emplace<int64_t>(0);
		parsePos = parseString.length();
	};
	return res;
};

// ******************************************
// *             FileTokenizer              *
// ******************************************

Token FileTokenizer::next()
{
	Token token;
	token.type = TOKEN_EOF;
	if ( needFeed )
	{
		if (ifs.eof() || ifs.fail())
			return token;
		needFeed = false;
		pos = 0;
		curLine++;
		std::getline( ifs, curLexem );
	}
	curPos = pos;
	token = extractLexemFromString( curLexem, pos, curPos );
	if ( token.type == TOKEN_EOL )
		needFeed = true;
	return token;
};

int FileTokenizer::getLine()
{
	return curLine;
};

int FileTokenizer::getPos()
{
	return curPos + 1;
};

}
