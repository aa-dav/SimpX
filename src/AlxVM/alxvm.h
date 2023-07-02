#ifndef ALX_VM
#define ALX_VM

#include <stdint.h>
#include <vector>
#include <string>
#include <variant>
#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include <functional>
#include "alxvm_tokens.h"

namespace AlxVM
{

enum Opcodes
{
	// return
	OP_RETURN = 1,		// 
	// typed returns
	OP_X32_RETURN,		// value
	OP_X64_RETURN,		// value
	OP_F64_RETURN,		// value
	// calls/jumps
	OP_STACK,		// value
	// calls/jumps
	OP_CALL,		// function_handle, stack_frame
	OP_NCALL,		// function_handle, stack_frame
	OP_JUMP,		// addr
	// conversions
	OP_I32_FROM_I64,	// a = b
	OP_I32_FROM_F64,	// a = b
	OP_U32_FROM_U64,	// a = b
	OP_U32_FROM_F64,	// a = b
	OP_I64_FROM_I32,	// a = b
	OP_I64_FROM_F64,	// a = b
	OP_U64_FROM_U32,	// a = b
	OP_U64_FROM_F64,	// a = b
	OP_F64_FROM_I32,	// a = b
	OP_F64_FROM_U32,	// a = b
	OP_F64_FROM_I64,	// a = b
	OP_F64_FROM_U64,	// a = b
	// load/store
	OP_GET_ADDR,
	OP_X32_LOAD,
	OP_X64_LOAD,
	OP_F64_LOAD,
	OP_X32_STORE,
	OP_X64_STORE,
	OP_F64_STORE,
	// integers
	OP_X32_IF_ZERO,		// a == 0, addr
	OP_X32_IF_NZERO,	// a != 0, addr
	OP_X32_IF_EQ,		// a == b, addr
	OP_X32_IF_NEQ,		// a != b, addr
	OP_I32_IF_LESS,		// a < b, addr
	OP_I32_IF_LESSEQ,	// a <= b, addr
	OP_U32_IF_LESS,		// a < b, addr
	OP_U32_IF_LESSEQ,	// a <= b, addr
	OP_X32_CONST,		// dest, const
	OP_X32_MOVE,		// dest, const
	OP_X32_ADD,		// a = b + c
	OP_X32_SUB,		// a = b - c
	OP_I32_MUL,		// a = b * c
	OP_I32_DIV,		// a = b / c
	OP_I32_MOD,		// a = b % c
	OP_U32_NOT,		// a = ~b
	OP_U32_AND,		// a = b & c
	OP_U32_OR,		// a = b | c
	OP_U32_XOR,		// a = b ^ c
	// long integers
	OP_X64_IF_ZERO,		// a == 0, addr
	OP_X64_IF_NZERO,	// a != 0, addr
	OP_X64_IF_EQ,		// a == b, addr
	OP_X64_IF_NEQ,		// a != b, addr
	OP_I64_IF_LESS,		// a < b, addr
	OP_I64_IF_LESSEQ,	// a <= b, addr
	OP_U64_IF_LESS,		// a < b, addr
	OP_U64_IF_LESSEQ,	// a <= b, addr
	OP_X64_CONST,		// dest, const
	OP_X64_MOVE,		// dest, const
	OP_X64_ADD,		// a = b + c
	OP_X64_SUB,		// a = b - c
	OP_I64_MUL,		// a = b * c
	OP_I64_DIV,		// a = b / c
	OP_I64_MOD,		// a = b % c
	OP_U64_NOT,		// a = ~b
	OP_U64_AND,		// a = b & c
	OP_U64_OR,		// a = b | c
	OP_U64_XOR,		// a = b ^ c
	// doubles
	OP_F64_IF_ZERO,		// a == 0, addr
	OP_F64_IF_NZERO,	// a != 0, addr
	OP_F64_IF_EQ,		// a == b, addr
	OP_F64_IF_NEQ,		// a != b, addr
	OP_F64_IF_LESS,		// a < b, addr
	OP_F64_IF_LESSEQ,	// a <= b, addr
	OP_F64_CONST,		// dest, const
	OP_F64_MOVE,		// dest, const
	OP_F64_ADD,		// a = b + c
	OP_F64_SUB,		// a = b - c
	OP_F64_MUL,		// a = b * c
	OP_F64_DIV,		// a = b / c
};

typedef unsigned char	Opcode;		// type of the opcode
typedef int16_t		LabelOffset;	// type of offset to label on the stack in the instruction
typedef uint16_t	StackPos;	// type of the index in the stack
typedef uint16_t	JumpPos;	// type of the index in the code
typedef int32_t		IntType;	// int type
typedef uint32_t	UIntType;	// unsigned int type
typedef int64_t		LIntType;	// long int type
typedef uint64_t	LUIntType;	// long unsigned int type
typedef double		DoubleType;	// double type
typedef uint64_t 	PtrType;	// type of the pointer in the heap

const int opcodeSize		= sizeof(Opcode);
const int labelOffsetSize	= sizeof(LabelOffset);
const int stackPosSize		= sizeof(StackPos);
const int jumpPosSize		= sizeof(JumpPos);
const int intSize		= sizeof(IntType);
const int lIntSize		= sizeof(LIntType);
const int ptrSize		= sizeof(PtrType);
const int callSize		= opcodeSize + 1 * labelOffsetSize + jumpPosSize;
const int ifUnarySize		= opcodeSize + 1 * labelOffsetSize + jumpPosSize;
const int ifBinarySize		= opcodeSize + 2 * labelOffsetSize + jumpPosSize;
const int opcodeLabelSize	= opcodeSize + 1 * labelOffsetSize;
const int opcode2LabelSize	= opcodeSize + 2 * labelOffsetSize;
const int opcode3LabelSize	= opcodeSize + 3 * labelOffsetSize;

class Module;
class Runtime;
class NativeFunctionBase;

// ******************************************
// *                Function                *
// ******************************************

struct Variable
{
	std::string name;
	LabelOffset offset;
	int size;

	Variable(const std::string &aName, LabelOffset anOffset, int aSize): name(aName), offset(anOffset), size(aSize) {};
};

class Function
{
	Module *module;
	std::string name;
	int paramsSize;
	int localsSize;
	int returnSize;
	JumpPos start;
	JumpPos end;
	std::vector<Variable> params;
	std::vector<Variable> locals;
public:
	Function(Module *aModule, const std::string &aName, int aReturnSize): module(aModule), name(aName), returnSize(aReturnSize), localsSize(0) {};

	Module *getModule() { return module; };
	const std::string &getName() const { return name; };
	int getReturnSize() const { return returnSize; };
	int getParamsSize() const { return paramsSize; };
	int getParamsCount() const { return params.size(); };
	int getParamSize( int index ) const { return params[ index ].size; };
	int getLocalsSize() const { return localsSize; };
	void setStart(JumpPos aStart) { start = aStart; }
	JumpPos getStart() { return start; };
	void setEnd(JumpPos anEnd) { end = anEnd; };
	JumpPos getEnd() { return end; };

	void addParam(const std::string &aName, StackPos anOffset, int aSize) { params.emplace_back(aName, anOffset, aSize); };
	void addLocal(const std::string &aName, StackPos anOffset, int aSize) { locals.emplace_back(aName, anOffset, aSize); };

	int getVarOffset(const std::string &name);
	void bindVariables();
	void dump( Runtime &runtime );

	friend class Frame;
};

// ******************************************
// *                  Frame                 *
// ******************************************

struct Frame
{	
	std::shared_ptr<Function> function;
	JumpPos ip;	// instruction pointer
	StackPos sp;	// beginning of function stack
	StackPos activeSp;	// actual stack pointer
	StackPos rp;	// result pointer

	Frame(std::shared_ptr<Function> aFunction, StackPos aSp, StackPos aRp, JumpPos anIp): function(aFunction), sp(aSp), activeSp(aSp), rp(aRp), ip(anIp) {};
};

// ******************************************
// *                 Module                 *
// ******************************************

class Module
{
	std::string name;
	std::map<std::string, std::shared_ptr<Function> > functions;
public:
	Module(const std::string &aName): name(aName) {};

	const std::string &getName() const { return name; };

	void addFunction(std::shared_ptr<Function> func) { functions.emplace(std::pair(func->getName(), func)); };
	bool hasFunction(const std::string &name) { return functions.find(name) != functions.end(); };
	std::shared_ptr<Function> getFunction(const std::string &name) { return functions.at(name); };
	void parseComplete();
	void dump( Runtime &runtime );
};

// ******************************************
// *                Runtime                 *
// ******************************************

struct FunctionEntry
{
	std::string name;
	std::shared_ptr<Function> function;
	FunctionEntry( const std::string &aName ): name( aName ) {};
};

struct NativeFunctionEntry
{
	std::string name;
	NativeFunctionBase *function;
	NativeFunctionEntry( const std::string &aName, NativeFunctionBase *aFunction ): name( aName ), function( aFunction ) {};
};

class Runtime
{	
	std::vector<char> stack;
	std::vector<char> code;
	std::vector<Frame> frames;
	std::vector<FunctionEntry> functionEntries;
	std::vector<NativeFunctionEntry> nativeFunctions;
	std::map<std::string, std::shared_ptr<Module> > modules;
	StackPos topParamPos;
	
	void parseEOL(Tokenizer &tokenizer);
	Token parseType(Tokenizer &tokenizer, TokenType type, const std::string expected, bool allow_ends = false);
public:
	~Runtime() { freeNativeFunctions(); };
	Runtime(StackPos stackSize): stack(stackSize) {};
	
	template<class T>
	T &stackAs(StackPos index) { return *(reinterpret_cast<T*>(&stack[index])); };

	template<class T>
	T &codeAs(StackPos index) { return *(reinterpret_cast<T*>(&code[index])); };
	template<class T>
	void codeEmit(T value) { code.resize(code.size() + sizeof(T)); codeAs<T>(code.size() - sizeof(T)) = value; };

	void compile(Tokenizer &tokenizer);
	void dump();
	std::shared_ptr<Function> getFunction( const std::string &moduleName, const std::string &functionName );
	void activateFunction( std::shared_ptr<Function> function, StackPos sp, StackPos rp );
	void startParams( StackPos top ) { topParamPos = top; };
	template<class T>
	void addParam(T value) { stackAs<T>( topParamPos ) = value; topParamPos += sizeof( T ); };
	void addParams() {};
	template<class T, typename...Args>
	void addParams(T value, Args...args) { addParam(value); addParams(args...); };
	int oneStep();
	void execFunction( std::shared_ptr<Function> function, StackPos rp );
	JumpPos findOrCreateFunctionEntry( const std::string &name );
	void freeNativeFunctions();
	void addNativeFunction( const std::string &name, NativeFunctionBase *function );
	JumpPos findNativeFunctionEntry( const std::string &name );
};

class NativeFunctionBase
{	
public:
	virtual ~NativeFunctionBase() {};
	virtual void invoke( Runtime &runtime, StackPos params, StackPos rp ) = 0;
};

}

#endif