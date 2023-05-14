#ifndef ALX_VM
#define ALX_VM

#include <stdint.h>
#include <vector>
#include <string>
#include <variant>
#include <map>
#include <memory>
#include <fstream>
#include "alxvm_tokens.h"

namespace AlxVM
{

enum Opcodes
{
	OP_RETURN = 1,	// value
	OP_CALL,	// function_handle, stack_frame
	OP_ECALL,	// function_handle, stack_frame
	OP_JUMP,	// addr
	OP_IF_ZERO,	// a == 0, addr
	OP_IF_NZERO,	// a != 0, addr
	OP_IF_EQ,	// a == b, addr
	OP_IF_NEQ,	// a != b, addr
	OP_IF_LESS,	// a < b, addr
	OP_IF_LESSEQ,	// a <= b, addr
	OP_CONST,	// dest, const
	OP_MOVE,	// dest, const
	OP_ADD,		// a = b + c
	OP_SUB,		// a = b - c
	OP_MUL,		// a = b * c
	OP_DIV,		// a = b / c
	OP_MOD,		// a = b % c
	OP_NOT,		// a = ~b
	OP_AND,		// a = b & c
	OP_OR,		// a = b | c
	OP_XOR,		// a = b ^ c
};

typedef unsigned char	Opcode;		// type of the opcode
typedef int16_t		LabelOffset;	// type of offset to label on the stack in the instruction
typedef uint16_t	StackPos;	// type of the index in the stack
typedef uint16_t	JumpPos;	// type of the index in the code
typedef int32_t		IntType;	// default int type
typedef uint32_t	UIntType;	// default unsigned int type
typedef uint32_t	PtrType;	// type of the pointer in the heap

const int opcodeSize		= sizeof(Opcode);
const int labelOffsetSize	= sizeof(LabelOffset);
const int stackPosSize		= sizeof(StackPos);
const int jumpPosSize		= sizeof(JumpPos);
const int intSize		= sizeof(IntType);
const int ptrSize		= sizeof(PtrType);
const int ifUnarySize		= opcodeSize + 1 * labelOffsetSize + jumpPosSize;
const int ifBinarySize		= opcodeSize + 2 * labelOffsetSize + jumpPosSize;
const int opcodeLabelSize	= opcodeSize + 1 * labelOffsetSize;
const int opcode2LabelSize	= opcodeSize + 2 * labelOffsetSize;
const int opcode3LabelSize	= opcodeSize + 3 * labelOffsetSize;

class Module;
class Runtime;

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
	Function(Module *aModule, const std::string &aName, int aReturnSize): module(aModule), name(aName), returnSize(aReturnSize) {};

	Module *getModule() { return module; };
	const std::string &getName() const { return name; };
	int getReturnSize() const { return returnSize; };
	int getParamsSize() const { return paramsSize; };
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

struct Frame
{	
	std::shared_ptr<Function> function;
	JumpPos ip;	// instruction pointer
	StackPos sp;	// stack pointer
	StackPos rp;	// result pointer

	Frame(std::shared_ptr<Function> aFunction, StackPos aSp, StackPos aRp, JumpPos anIp): function(aFunction), sp(aSp), rp(aRp), ip(anIp) {};
};

class Module
{
	std::string name;
	std::map<std::string, std::shared_ptr<Function> > functions;
public:
	Module(const std::string &aName): name(aName) {};

	const std::string &getName() const { return name; };

	void addFunction(std::shared_ptr<Function> func) { functions.emplace(std::pair(func->getName(), func)); };
	std::shared_ptr<Function> getFunction(const std::string &name) { return functions.at(name); };
	void parseComplete();
	void dump( Runtime &runtime );
};

struct FunctionEntry
{
	std::string name;
	std::shared_ptr<Function> function;
};

class Runtime
{	
	std::vector<char> stack;
	std::vector<char> code;
	std::vector<Frame> frames;
	std::vector<FunctionEntry> functionEntries;
	std::map<std::string, std::shared_ptr<Module> > modules;
	StackPos topParamPos;
	
	void parseEOL(Tokenizer &tokenizer);
	Token parseType(Tokenizer &tokenizer, TokenType type, const std::string expected, bool allow_ends = false);
public:
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
	int oneStep();
	void execFunction( std::shared_ptr<Function> function, StackPos rp );
};

};

#endif