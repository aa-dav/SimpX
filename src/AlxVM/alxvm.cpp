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
	// l - label, 
	// n - function name
	// m - native function name
	// i - integer 32 literal
	// d - double literal
	// j - jump direction
	{OP_RETURN,		"return",		""},
	// typed returns
	{OP_X32_RETURN,		"return.x32",		"l"},
	{OP_X64_RETURN,		"return.x64",		"l"},
	{OP_F64_RETURN,		"return.f64",		"l"},
	// calls/jumps
	{OP_CALL,		"call",			"lln"},
	{OP_NCALL,		"ncall",		"llm"},
	{OP_JUMP,		"jump",			"j"},
	// conversions
	{OP_CAST_I32_TO_F64,	"cast.i32.f64",		"ll"},
	{OP_CAST_F64_TO_I32,	"cast.f64.i32",		"ll"},
	{OP_CAST_I32_TO_F64,	"cast.u32.f64",		"ll"},
	{OP_CAST_I32_TO_F64,	"cast.f64.u32",		"ll"},
	// integers
	{OP_X32_IF_ZERO,	"if_zero.x32",		"lj"},
	{OP_X32_IF_NZERO,	"if_nzero.x32",		"lj"},
	{OP_X32_IF_EQ,		"if_eq.x32",		"llj"},
	{OP_X32_IF_NEQ,		"if_nqe.x32",		"llj"},
	{OP_I32_IF_LESS,	"if_less.i32",		"llj"},
	{OP_I32_IF_LESSEQ,	"if_lesseq.i32",	"llj"},
	{OP_U32_IF_LESS,	"if_less.u32",		"llj"},
	{OP_U32_IF_LESSEQ,	"if_lesseq.u32",	"llj"},
	{OP_X32_CONST,		"const.x32",		"li"},
	{OP_X32_MOVE,		"move.x32",		"ll"},
	{OP_X32_ADD,		"add.x32",		"lll"},
	{OP_X32_SUB,		"sub.x32",		"lll"},
	{OP_I32_MUL,		"mul.i32",		"lll"},
	{OP_I32_DIV,		"div.i32",		"lll"},
	{OP_I32_MOD,		"mod.i32",		"lll"},
	{OP_U32_NOT,		"not.u32",		"ll"},
	{OP_U32_AND,		"and.u32",		"lll"},
	{OP_U32_OR,		"or.u32",		"lll"},
	{OP_U32_XOR,		"xor.u32",		"lll"},
	// doubles
	{OP_F64_IF_ZERO,	"if_zero.f64",		"lj"},
	{OP_F64_IF_NZERO,	"if_nzero.f64",		"lj"},
	{OP_F64_IF_EQ,		"if_eq.f64",		"llj"},
	{OP_F64_IF_NEQ,		"if_nqe.f64",		"llj"},
	{OP_F64_IF_LESS,	"if_less.f64",		"llj"},
	{OP_F64_IF_LESSEQ,	"if_lesseq.f64",	"llj"},
	{OP_F64_CONST,		"const.f64",		"ld"},
	{OP_F64_MOVE,		"move.f64",		"ll"},
	{OP_F64_ADD,		"add.f64",		"lll"},
	{OP_F64_SUB,		"sub.f64",		"lll"},
	{OP_F64_MUL,		"mul.f64",		"lll"},
	{OP_F64_DIV,		"div.f64",		"lll"},
	{0,			nullptr,		nullptr}	// end of table
};

// ******************************************
// *                Function                *
// ******************************************

void Function::dump( Runtime &runtime )
{
	std::cout << "Function " << name << " start: " << start << " end: " << end << " paramsSize " << paramsSize << " localsSize " << localsSize << "\n";
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
	/*
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
	*/
	for ( auto &local: locals )
	{
		if ( localsSize < local.offset + local.size )
			localsSize = local.offset + local.size;
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

void Runtime::freeNativeFunctions()
{
	for ( auto &it : nativeFunctions )
		delete it.function;
	nativeFunctions.clear();
};

void Runtime::addNativeFunction( const std::string &name, NativeFunctionBase *function )
{
	nativeFunctions.emplace_back( name, function );
};

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

/*struct CallEntry
{
	JumpPos position;
	JumpPos callEntry;
	CallEntry( JumpPos aPosition, JumpPos aCallEntry ): position( aPosition ), callEntry( aCallEntry ) {};
};*/

JumpPos Runtime::findOrCreateFunctionEntry( const std::string &name )
{
	for ( int i = 0; i < functionEntries.size(); i++ )
		if ( functionEntries[ i ].name == name )
			return i;
	functionEntries.emplace_back( name );
	return functionEntries.size() - 1;
}

JumpPos Runtime::findNativeFunctionEntry( const std::string &name )
{
	for ( int i = 0; i < nativeFunctions.size(); i++ )
		if ( nativeFunctions[ i ].name == name )
			return i;
	throw std::runtime_error( std::string( "Unknown native function name '" + name + "'!" ) );
}

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
		std::vector<LabelOffset> slices;
		LabelOffset lastLabelOffset;
		int64_t size, offset;
		functionEntries.clear();
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
				slices.clear();
				lastLabelOffset = 0;
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
					curFunction->addLocal(curName, lastLabelOffset, size);
					lastLabelOffset += size;
				}
				else
				{
					offset = token.asInt();
					curFunction->addLocal(curName, offset, size);
					parseEOL(tokenizer);
				}
			}
			else if ( token.asStr() == "slice" )
			{
				if ( !curFunction )
					throw std::runtime_error("slice without function!");
				slices.push_back(lastLabelOffset);
			}
			else if ( token.asStr() == "~slice" )
			{
				if ( !curFunction )
					throw std::runtime_error("~slice without function!");
				if ( slices.empty() )
					throw std::runtime_error("Disbalanced slices!");
				lastLabelOffset = slices.back();
				slices.pop_back();
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
						JumpPos callPos = findOrCreateFunctionEntry( argToken.asStr() );
						codeEmit<JumpPos>( callPos );
					}
					else if ( *args == 'm' )
					{
						argToken = parseType(tokenizer, TOKEN_NAME, "native function name");
						JumpPos callPos = findNativeFunctionEntry( argToken.asStr() );
						codeEmit<JumpPos>( callPos );
					}
					else if ( *args == 'i' )
					{
						argToken = parseType(tokenizer, TOKEN_INTEGER, "integer");
						codeEmit<IntType>( argToken.asInt() );
					}
					else if ( *args == 'd' )
					{
						argToken = parseType(tokenizer, TOKEN_DOUBLE, "double");
						codeEmit<DoubleType>( argToken.asDouble() );
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
		for ( auto &entry: functionEntries )
		{
			for ( auto &module: modules )
			{
				if ( module.second->hasFunction( entry.name ) )
				{
					entry.function = module.second->getFunction( entry.name );
					break;
				}
			}
			if ( !entry.function  )
				throw std::runtime_error(std::string("Unknown function '") + entry.name + "'!" );
			
		};
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
	if ( modules.find( moduleName ) == modules.end() )
		throw std::runtime_error( std::string( "Module " ) + moduleName + " not found." );
	std::shared_ptr< Module > module = modules.at( moduleName );
	if ( !module->hasFunction( functionName ) )
		throw std::runtime_error( std::string( "Module " ) + moduleName + " has no function " + functionName + "." );
	return module->getFunction(functionName);
};

#define STACK_CODEREF(type, index) stackAs<type>(frame.sp + codeAs<LabelOffset>(frame.ip + index))

#define OPERATION_CAST( typeFrom, typeTo ) STACK_CODEREF( typeTo, opcodeSize ) = STACK_CODEREF( typeFrom, opcodeLabelSize ); \
	frame.ip += opcode2LabelSize; \
	break;
#define OPERATION_IMMED( type, immType, op ) STACK_CODEREF( type, opcodeSize ) = op codeAs<immType>( frame.ip + opcodeLabelSize ); \
	frame.ip += opcodeSize + labelOffsetSize + sizeof(immType); \
	break;
#define OPERATION_UNARY( type, op ) STACK_CODEREF( type, opcodeSize ) = op STACK_CODEREF( type, opcodeLabelSize ); \
	frame.ip += opcode2LabelSize; \
	break;
#define OPERATION_BINARY( type, op ) STACK_CODEREF( type, opcodeSize ) = STACK_CODEREF( type, opcodeLabelSize ) op STACK_CODEREF( type, opcode2LabelSize ); \
	frame.ip += opcode3LabelSize; \
	break;
#define IF_UNARY( type, cond ) if ( STACK_CODEREF( type, opcodeSize ) cond ) \
		frame.ip = codeAs<JumpPos>( frame.ip + opcodeLabelSize ); \
	else \
		frame.ip += ifUnarySize; \
	break;
#define IF_BINARY( type, cond ) if ( STACK_CODEREF( type, opcodeSize ) cond STACK_CODEREF( type, opcodeLabelSize ) ) \
		frame.ip = codeAs<JumpPos>( frame.ip + opcode2LabelSize ); \
	else \
		frame.ip += ifBinarySize; \
	break;


int Runtime::oneStep()
{
	if ( frames.empty() )
		return -1;
	Frame &frame = frames.back();
	Opcode opcode = codeAs<Opcode>( frame.ip );
	//std::cout << "Step at " << frame.ip << " opcode: " << (unsigned int) opcode << "\n";
	switch ( opcode )
	{
	// return
	case OP_RETURN:
		frames.pop_back();
		break;
	// typed returns
	case OP_X32_RETURN:
		stackAs<IntType>( frame.rp ) = STACK_CODEREF( IntType, opcodeSize );
		frames.pop_back();
		break;
	case OP_X64_RETURN:
		stackAs<LIntType>( frame.rp ) = STACK_CODEREF( LIntType, opcodeSize );
		frames.pop_back();
		break;
	case OP_F64_RETURN:
		stackAs<DoubleType>( frame.rp ) = STACK_CODEREF( DoubleType, opcodeSize );
		frames.pop_back();
		break;
	// calls/jumps
	case OP_CALL:
		activateFunction( functionEntries[ codeAs<JumpPos>( frame.ip + opcode2LabelSize ) ].function,
				codeAs<LabelOffset>( frame.ip + opcodeLabelSize ) + frame.sp,
				codeAs<LabelOffset>( frame.ip + opcodeSize ) + frame.sp );
		frame.ip += ifBinarySize;
		break;
	case OP_NCALL:
		nativeFunctions[ codeAs<JumpPos>( frame.ip + opcode2LabelSize ) ].function->invoke( 
				*this, 
				codeAs<LabelOffset>( frame.ip + opcodeLabelSize ) + frame.sp,
				codeAs<LabelOffset>( frame.ip + opcodeSize ) + frame.sp );
		frame.ip += ifBinarySize;
		break;
	case OP_JUMP:
		frame.ip = codeAs<JumpPos>( frame.ip + opcodeSize );
		break;
	// conversions
	case OP_CAST_I32_TO_F64:	OPERATION_CAST( IntType, DoubleType );
	case OP_CAST_F64_TO_I32:	OPERATION_CAST( DoubleType, IntType );
	case OP_CAST_U32_TO_F64:	OPERATION_CAST( UIntType, DoubleType );
	case OP_CAST_F64_TO_U32:	OPERATION_CAST( DoubleType, UIntType );
	// integers
	case OP_X32_IF_ZERO:	IF_UNARY( IntType, == 0 );
	case OP_X32_IF_NZERO:	IF_UNARY( IntType, != 0 );
	case OP_X32_IF_EQ:	IF_BINARY( IntType, == );
	case OP_X32_IF_NEQ:	IF_BINARY( IntType, != );
	case OP_I32_IF_LESS:	IF_BINARY( IntType, < );
	case OP_I32_IF_LESSEQ:	IF_BINARY( IntType, <= );
	case OP_U32_IF_LESS:	IF_BINARY( UIntType, < );
	case OP_U32_IF_LESSEQ:	IF_BINARY( UIntType, <= );
	case OP_X32_CONST:	OPERATION_IMMED( IntType, IntType, );
	case OP_X32_MOVE:	OPERATION_UNARY( IntType, );
	case OP_X32_ADD:	OPERATION_BINARY( IntType, + );
	case OP_X32_SUB:	OPERATION_BINARY( IntType, - );
	case OP_I32_MUL:	OPERATION_BINARY( IntType, * );
	case OP_I32_DIV:	OPERATION_BINARY( IntType, / );
	case OP_I32_MOD:	OPERATION_BINARY( IntType, % );
	case OP_U32_NOT:	OPERATION_UNARY( UIntType, ~ );
	case OP_U32_AND:	OPERATION_BINARY( UIntType, & );
	case OP_U32_OR:		OPERATION_BINARY( UIntType, | );
	case OP_U32_XOR:	OPERATION_BINARY( UIntType, ^ );
	// doubles
	case OP_F64_IF_ZERO:	IF_UNARY( DoubleType, == 0.0 );
	case OP_F64_IF_NZERO:	IF_UNARY( DoubleType, != 0.0 );
	case OP_F64_IF_EQ:	IF_BINARY( DoubleType, == );
	case OP_F64_IF_NEQ:	IF_BINARY( DoubleType, != );
	case OP_F64_IF_LESS:	IF_BINARY( DoubleType, < );
	case OP_F64_IF_LESSEQ:	IF_BINARY( DoubleType, <= );
	case OP_F64_CONST:	OPERATION_IMMED( DoubleType, DoubleType, );
	case OP_F64_MOVE:	OPERATION_UNARY( DoubleType, );
	case OP_F64_ADD:	OPERATION_BINARY( DoubleType, + );
	case OP_F64_SUB:	OPERATION_BINARY( DoubleType, - );
	case OP_F64_MUL:	OPERATION_BINARY( DoubleType, * );
	case OP_F64_DIV:	OPERATION_BINARY( DoubleType, / );
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