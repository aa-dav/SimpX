#include "alxvm.h"
#include <stdexcept>
#include <iostream>

namespace AlxVM
{

struct OpcodeDescriptor
{
	Opcode opcode;
	const char *name;
	const char *args;
};

static OpcodeDescriptor opcodes[] =
{
	// l - label, n - name, i - integer, j - jump direction
	{OP_RETURN,	"return",	"l"},
	{OP_CALL,	"call",		"nl"},
	{OP_ECALL,	"ecall",	"nl"},
	{OP_JUMP,	"jump",		"j"},
	{OP_IF_ZERO,	"if_zero",	"lj"},
	{OP_IF_NZERO,	"if_nzero",	"lj"},
	{OP_IF_EQ,	"if_eq",	"llj"},
	{OP_IF_NEQ,	"if_nqe",	"llj"},
	{OP_IF_LESS,	"if_less",	"llj"},
	{OP_IF_LESSEQ,	"if_lesseq",	"llj"},
	{OP_CONST,	"const",	"li"},
	{OP_MOVE,	"move",		"ll"},
	{OP_ADD,	"add",		"lll"},
	{OP_SUB,	"sub",		"lll"},
	{OP_MUL,	"mul",		"lll"},
	{OP_DIV,	"div",		"lll"},
	{OP_MOD,	"mod",		"lll"},
	{OP_NOT,	"not",		"ll"},
	{OP_AND,	"and",		"lll"},
	{OP_OR,		"or",		"lll"},
	{OP_XOR,	"xor",		"lll"},
	{0,		nullptr,	nullptr}	// end of tables
};

// ******************************************
// *                Function                *
// ******************************************

void Function::dump( Runtime &runtime )
{
	std::cout << "Function " << name << " start: " << start << " end: " << end << "\n";
	for( auto &param: params )
	{
		std::cout << "    Param " << param.name << " offset " << param.offset << " size " << param.size << "\n";
	}
	for( auto &local: locals )
	{
		std::cout << "    Local " << local.name << " offset " << local.offset << " size " << local.size << "\n";
	}
	for ( int i = start; i < end; i++ )
		std::cout << i << ":" << static_cast<unsigned int>(runtime.codeAs<Opcode>( i )) << "\n";
	
}

void Function::bindVariables()
{
	paramsSize = 0;
	for( int i = params.size() - 1; i >= 0; i-- )
	{
		Variable &param = params[i];
		param.offset = -paramsSize - param.size;
		paramsSize += param.size;
	}
	localsSize = 0;
	for( int i = 0; i < locals.size(); i++ )
	{
		Variable &local = locals[i];
		if ( local.offset == -1 )
		{
			local.offset = localsSize;
			localsSize += local.size;
		};
	}
	for ( auto &local: locals )
	{
		if ( localsSize < -local.offset + local.size )
			localsSize = -local.offset + local.size;
	}
}

int Function::getVarOffset(const std::string &name)
{
	for ( int i = 0; i < params.size(); i++ )
		if ( params[i].name == name )
			return params[i].offset;
	for ( int i = 0; i < locals.size(); i++ )
		if ( locals[i].name == name )
			return locals[i].offset;
	throw std::runtime_error(std::string("unknown variable name ") + name);
}

// ******************************************
// *                Module                  *
// ******************************************

void Module::dump( Runtime &runtime )
{
	for( auto &pair: functions )
	{
		pair.second->dump( runtime );
	}
}

void Module::parseComplete()
{
	//for( auto &pair: functions )
	//{
	//	pair.second->parseComplete();
	//}
}

// ******************************************
// *                Runtime                 *
// ******************************************

void Runtime::parseEOL(Tokenizer &tokenizer)
{
	Token token = tokenizer.next();
	if ( token.type != TOKEN_EOL )
		throw std::runtime_error(std::string("end of line expected, but '") + token.presentation() + "' met!");
};

Token Runtime::parseType(Tokenizer &tokenizer, TokenType type, const std::string expected, bool allow_ends)
{
	Token token = tokenizer.next();
	if ( allow_ends && ((token.type == TOKEN_EOL) || (token.type == TOKEN_EOF)) )
		return token;
	if ( token.type != type )
		throw std::runtime_error(expected + " expected, but '" + token.presentation() + "' met!");
	return token;
};

struct Block
{
	std::weak_ptr<Block> parent;
	JumpPos start, end, middle;
	bool hasMiddle;
	std::vector<std::shared_ptr<Block>> children;
	Block(std::shared_ptr<Block> aParent, JumpPos aStart): parent(aParent), start(aStart), hasMiddle(false) {};
	void dump( int level )
	{
		if ( level > 0 )
		{
			std::cout << std::string(level, ' ') << "[" << start << "\n";
			if ( hasMiddle )
				std::cout << std::string(level, ' ') << "else at " << middle << "\n";
		};
		for ( auto &it: children )
			it->dump( level + 2 );
		if ( level > 0 )
			std::cout << std::string(level, ' ') << "]" << end << "\n";
	};
};

struct VarEntry
{
	std::string name;
	JumpPos position;
	VarEntry( const std::string &aName, JumpPos aPosition ): name(aName), position(aPosition) {};
};

struct JumpEntry
{
	std::shared_ptr<Block> block;
	int direction;
	JumpPos position;
	JumpEntry(JumpPos aPosition, int aDirection, const std::shared_ptr<Block> &aBlock):
		position(aPosition), direction(aDirection), block(aBlock) {};
};

void Runtime::compile(Tokenizer &tokenizer)
{
	try
	{
		Token token;
		std::string curName;
		std::shared_ptr<Module> curModule;
		std::shared_ptr<Function> curFunction;
		std::shared_ptr<Block> rootBlock, curBlock;
		std::vector<VarEntry> varEntries;
		std::vector<JumpEntry> jumpEntries;
		int64_t size, offset;
		while ( true )
		{
			token = parseType(tokenizer, TOKEN_NAME, "keyword", true);
			if ( token.type == TOKEN_EOF )
				break;
			if ( token.type == TOKEN_EOL )
				continue;
			if ( token.asStr() == "module" )
			{
				if ( curFunction )
					throw std::runtime_error(std::string("unfinished function '") + curFunction->getName() + "'!");
				token = parseType(tokenizer, TOKEN_NAME, "module name");
				curName = token.asStr();
				curModule = std::make_shared<Module>(curName);
				modules.emplace(std::make_pair(curName, curModule));
				curFunction.reset();
				parseEOL(tokenizer);
			}
			else if ( token.asStr() == "function" )
			{
				if ( curFunction )
					throw std::runtime_error(std::string("unfinished function '") + curFunction->getName() + "'!");
				if ( !curModule )
					throw std::runtime_error("function without module!");
				token = parseType(tokenizer, TOKEN_NAME, "function name");
				curName = token.asStr();
				token = parseType(tokenizer, TOKEN_INTEGER, "size");
				size = token.asInt();
				curFunction = std::make_shared<Function>(curModule.get(), curName, size);
				curFunction->setStart(code.size());
				curModule->addFunction(curFunction);
				rootBlock = std::make_shared<Block>(nullptr, 0);
				curBlock = rootBlock;
				varEntries.clear();
				jumpEntries.clear();
				parseEOL(tokenizer);
			}
			else if ( token.asStr() == "param" )
			{
				if ( !curFunction )
					throw std::runtime_error("param without function!");
				token = parseType(tokenizer, TOKEN_NAME, "param name");
				curName = token.asStr();
				token = parseType(tokenizer, TOKEN_INTEGER, "size");
				size = token.asInt();
				curFunction->addParam(curName, -1, size);
				parseEOL(tokenizer);
			}
			else if ( token.asStr() == "local" )
			{
				if ( !curFunction )
					throw std::runtime_error("local without function!");
				token = parseType(tokenizer, TOKEN_NAME, "local name");
				curName = token.asStr();
				token = parseType(tokenizer, TOKEN_INTEGER, "size");
				size = token.asInt();
				token = parseType(tokenizer, TOKEN_INTEGER, "offset" , true);
				if ( token.isEnd() )
				{
					curFunction->addLocal(curName, -1, size);
				}
				else
				{
					offset = token.asInt();
					curFunction->addLocal(curName, offset, size);
					parseEOL(tokenizer);
				}
			}
			else if ( token.asStr() == "~function" )
			{
				if ( !curFunction )
					throw std::runtime_error("~function without function!");
				if ( curBlock != rootBlock )
					throw std::runtime_error(std::string("unbalansed blocks at function '") + curFunction->getName() + "'!");
				// finalize function
				curFunction->bindVariables();
				for ( auto &entry: varEntries )
				{
					codeAs<LabelOffset>( entry.position ) = curFunction->getVarOffset( entry.name );
				};
				for ( auto &entry: jumpEntries )
				{
					if ( entry.direction == 0 )
					{
						if ( !entry.block->hasMiddle )
							throw std::runtime_error("jump to absent else!");
						codeAs<JumpPos>( entry.position ) = entry.block->middle;
					}
					else
					{
						std::shared_ptr<Block> curBlock = entry.block;
						while ( std::abs( entry.direction ) > 1 )
						{
							curBlock = curBlock->parent.lock();
							if ( curBlock == rootBlock )
								throw std::runtime_error("jump out of blocks!");
							if ( entry.direction > 1 )
								entry.direction--;
							else
								entry.direction++;
						}
						codeAs<JumpPos>( entry.position ) = entry.direction > 0 ? curBlock->end : curBlock->start;
					};
				};
				curFunction->setEnd(code.size());
				curFunction.reset();
			}
			else if ( token.asStr() == "begin" )
			{
				if ( !curFunction )
					throw std::runtime_error("begin without function!");
				std::shared_ptr newBlock = std::make_shared<Block>(curBlock, code.size());
				curBlock->children.push_back(newBlock);
				curBlock = newBlock;
			}
			else if ( token.asStr() == "else" )
			{
				if ( !curFunction )
					throw std::runtime_error("else without function!");
				if ( curBlock->parent.expired() )
					throw std::runtime_error("else without begin!");
				if ( curBlock->hasMiddle )
					throw std::runtime_error("dublicate else met!");
				curBlock->middle = code.size();
				curBlock->hasMiddle = true;
			}
			else if ( token.asStr() == "end" )
			{
				if ( !curFunction )
					throw std::runtime_error("end without function!");
				if ( curBlock->parent.expired() )
					throw std::runtime_error("end without begin!");
				curBlock->end = code.size();
				curBlock = curBlock->parent.lock();
			}
			else
			{
				// instructions
				if ( !curFunction )
					throw std::runtime_error("instruction without function!");
				OpcodeDescriptor *desc = opcodes;
				while ( desc->opcode != 0 )
				{
					if ( token.asStr() == desc->name )
						break;
					desc++;
				};
				if ( desc->opcode == 0 )
					throw std::runtime_error(std::string("unknown instruction '") + token.asStr() + "'!");
				codeEmit<Opcode>( desc->opcode );
				const char *args = desc->args;
				while ( *args != 0 )
				{
					Token argToken;
					if ( *args == 'l' )
					{
						argToken = parseType(tokenizer, TOKEN_NAME, "variable");
						varEntries.emplace_back(argToken.asStr(), code.size());
						codeEmit<LabelOffset>( 0 );
					}
					else if ( *args == 'n' )
					{
						argToken = parseType(tokenizer, TOKEN_NAME, "function name");
						codeEmit<JumpPos>( 0 );
					}
					else if ( *args == 'i' )
					{
						argToken = parseType(tokenizer, TOKEN_INTEGER, "integer");
						codeEmit<IntType>( argToken.asInt() );
					}
					else if ( *args == 'j' )
					{
						
						if ( curBlock == rootBlock )
							throw std::runtime_error("jump outside of block!");
						argToken = parseType(tokenizer, TOKEN_INTEGER, "integer");
						jumpEntries.emplace_back( code.size(), argToken.asInt(), curBlock );
						codeEmit<JumpPos>( argToken.asInt() );
					}
					else
					{
						throw std::runtime_error(std::string("unknown opcode descriptor arg '") + *args + "'!");
					}
					args++;
				}
				parseEOL(tokenizer);
			}
		};
		if ( curFunction )
			throw std::runtime_error(std::string("unfinished function '") + curFunction->getName() + "'!");
		for ( auto &module: modules )
			module.second->parseComplete();
	}
	catch (std::runtime_error &error)
	{
		throw std::runtime_error(std::string(error.what()) +
					" at line " + std::to_string(tokenizer.getLine()) + 
					" at position "  + std::to_string(tokenizer.getPos()));
	};
};

void Runtime::dump()
{
	for( auto &pair: modules )
	{
		std::cout << "Module " << pair.second->getName() << "\n";
		pair.second->dump( *this );
	}
};

void Runtime::activateFunction( std::shared_ptr<Function> function, StackPos sp, StackPos rp )
{
	frames.emplace_back(function, sp, rp, function->getStart());
};

std::shared_ptr<Function> Runtime::getFunction( const std::string &moduleName, const std::string &functionName )
{
	return modules.at(moduleName)->getFunction(functionName);
};

#define STACK_I32_CODEREF(index) stackAs<IntType>(frame.sp + codeAs<LabelOffset>(frame.ip + index))
#define STACK_U32_CODEREF(index) stackAs<UIntType>(frame.sp + codeAs<LabelOffset>(frame.ip + index))

int Runtime::oneStep()
{
	if ( frames.empty() )
		return -1;
	Frame &frame = frames.back();
	int ip = frame.ip;
	Opcode opcode = codeAs<Opcode>( frame.ip );
	std::cout << "Step at " << ip << " opcode: " << (unsigned int) opcode << "\n";
	switch ( opcode )
	{
	case OP_RETURN: 	// l
		stackAs<IntType>( frame.rp ) = STACK_I32_CODEREF( 1 );
		frames.pop_back();
		break;
	case OP_CALL:	// nl
		break;
	case OP_ECALL:	// nl
		break;
	case OP_JUMP:	// j
		frame.ip = codeAs<JumpPos>( frame.ip + opcodeSize );
		break;
	case OP_IF_ZERO:	// lj
		if ( STACK_I32_CODEREF( opcodeSize ) == 0 )
			frame.ip = codeAs<JumpPos>( frame.ip + opcodeLabelSize );
		else
			frame.ip += ifUnarySize;
		break;
	case OP_IF_NZERO:	// lj
		if ( STACK_I32_CODEREF( opcodeSize ) != 0 )
			frame.ip = codeAs<JumpPos>( frame.ip + opcodeLabelSize );
		else
			frame.ip += ifUnarySize;
		break;
	case OP_IF_EQ:	// llj
		if ( STACK_I32_CODEREF( opcodeSize ) == STACK_I32_CODEREF( opcodeLabelSize ) )
			frame.ip = codeAs<JumpPos>( frame.ip + opcode2LabelSize );
		else
			frame.ip += ifBinarySize;
		break;
	case OP_IF_NEQ:	// llj
		if ( STACK_I32_CODEREF( opcodeSize ) != STACK_I32_CODEREF( opcodeLabelSize ) )
			frame.ip = codeAs<JumpPos>( frame.ip + opcode2LabelSize );
		else
			frame.ip += ifBinarySize;
		break;
	case OP_IF_LESS:	// llj
		if ( STACK_I32_CODEREF( opcodeSize ) < STACK_I32_CODEREF( opcodeLabelSize ) )
			frame.ip = codeAs<JumpPos>( frame.ip + opcode2LabelSize );
		else
			frame.ip += ifBinarySize;
		break;
	case OP_IF_LESSEQ:	// llj
		if ( STACK_I32_CODEREF( opcodeSize ) <= STACK_I32_CODEREF( opcodeLabelSize ) )
			frame.ip = codeAs<JumpPos>( frame.ip + opcode2LabelSize );
		else
			frame.ip += ifBinarySize;
		break;
	case OP_CONST:	// li
		STACK_I32_CODEREF( opcodeSize ) = codeAs<IntType>( frame.ip + opcodeLabelSize );
		frame.ip += opcodeSize + labelOffsetSize + intSize;
		break;
	case OP_MOVE:	// ll
		STACK_I32_CODEREF( opcodeSize ) = STACK_I32_CODEREF( opcodeLabelSize );
		frame.ip += opcode2LabelSize;
		break;
	case OP_ADD:		// lll
		STACK_I32_CODEREF( opcodeSize ) = STACK_I32_CODEREF( opcodeLabelSize ) + STACK_I32_CODEREF( opcode2LabelSize );
		frame.ip += opcode3LabelSize;
		break;
	case OP_SUB:		// lll
		STACK_I32_CODEREF( opcodeSize ) = STACK_I32_CODEREF( opcodeLabelSize ) - STACK_I32_CODEREF( opcode2LabelSize );
		frame.ip += opcode3LabelSize;
		break;
	case OP_MUL:		// lll
		STACK_I32_CODEREF( opcodeSize ) = STACK_I32_CODEREF( opcodeLabelSize ) * STACK_I32_CODEREF( opcode2LabelSize );
		frame.ip += opcode3LabelSize;
		break;
	case OP_DIV:		// lll
		STACK_I32_CODEREF( opcodeSize ) = STACK_I32_CODEREF( opcodeLabelSize ) / STACK_I32_CODEREF( opcode2LabelSize );
		frame.ip += opcode3LabelSize;
		break;
	case OP_MOD:		// lll
		STACK_I32_CODEREF( opcodeSize ) = STACK_I32_CODEREF( opcodeLabelSize ) % STACK_I32_CODEREF( opcode2LabelSize );
		frame.ip += opcode3LabelSize;
		break;
	case OP_NOT:		// ll
		STACK_U32_CODEREF( opcodeSize ) = ~STACK_U32_CODEREF( opcodeLabelSize );
		frame.ip += opcode2LabelSize;
		break;
	case OP_AND:		// lll
		STACK_U32_CODEREF( opcodeSize ) = STACK_U32_CODEREF( opcodeLabelSize ) & STACK_U32_CODEREF( opcode2LabelSize );
		frame.ip += opcode3LabelSize;
		break;
	case OP_OR:		// lll
		STACK_U32_CODEREF( opcodeSize ) = STACK_U32_CODEREF( opcodeLabelSize ) | STACK_U32_CODEREF( opcode2LabelSize );
		frame.ip += opcode3LabelSize;
		break;
	case OP_XOR:		// lll
		STACK_U32_CODEREF( opcodeSize ) = STACK_U32_CODEREF( opcodeLabelSize ) ^ STACK_U32_CODEREF( opcode2LabelSize );
		frame.ip += opcode3LabelSize;
		break;
	default:
		throw std::runtime_error(std::string("unknown opcode ") + std::to_string(opcode) + " at " + std::to_string(frame.ip));
	};
	return 0;
};

void Runtime::execFunction( std::shared_ptr<Function> function, StackPos rp )
{
	activateFunction( function, topParamPos, rp );
	while ( oneStep() != -1 ) {};
};

}