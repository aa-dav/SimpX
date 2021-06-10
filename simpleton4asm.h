#ifndef SIMPLETON_4_ASM_H
#define SIMPLETON_4_ASM_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <fstream>
#include "simpleton4.h"

namespace Simpleton
{

class File
{
public:
    virtual std::string get_line() = 0;
    virtual bool eof() = 0;
    virtual ~File() {};
};

class FileProvider
{
public:
    virtual std::shared_ptr< File > open( const std::string &name ) = 0;
    virtual ~FileProvider() {};
};

class FileStd: public File
{
    std::ifstream ifs;
public:
    FileStd( const std::string &fname ): ifs( fname, std::ios::in | std::ios::binary ) {};

    std::string get_line() override
    {
        std::string ret;
        std::getline( ifs, ret );
        return ret;
    };
    bool eof() override
    {
        return ifs.eof();
    };
    bool fail()
    {
        return ifs.fail();
    };
};

class FileProviderStd: public FileProvider
{
public:
    std::shared_ptr<File> open(const std::string &name) override
    {
        std::shared_ptr< FileStd > ret = std::make_shared< FileStd >( name );
        if ( ret->fail() )
            ret.reset();
        return ret;
    };
};

class ParseError
{
	std::string reason;
	int line;
public:
    ParseError( int _line, const std::string &_reason ): reason( _reason ), line( _line ) {};
	int getLine() const { return line; };
	const std::string &getReason() const { return reason; };
};

class PreProcessorError
{
    int file;
    int line;
    std::string reason;
public:
	PreProcessorError( int _file, int _line, const std::string &_reason ): file( _file ), line( _line ), reason( _reason ) {};
	int getLine() const { return line; };
	int getFile() const { return file; };
	const std::string &getReason() const { return reason; };
};

class Assembler
{
private:
	struct SourceFile
	{
		std::string name;
		SourceFile( const std::string &src ): name( src ) {};
	};
	struct SourceLine
	{
		int file;
		int num;
		bool label;
		std::vector< std::string > lexems;

		SourceLine() {};
		SourceLine(	int f, int n, bool lb,
				const std::vector< std::string > &lx ): 
				file( f ), num( n ), label( lb ), lexems( lx ) {};
	};
	std::vector< SourceFile > files;
	std::vector< SourceLine > lines;

    std::shared_ptr< FileProvider > provider;

	class Identifier;

	class ExprNode
	{
	private:
		int type;
		std::string name;
		int value;
		int lineNum;
		std::shared_ptr< ExprNode > left, right;

	public:
		enum Type
		{
			Literal,
			Symbol,
			Operator
		};

		void setup( int _lineNum )
		{
			type = Literal;
			value = 0;
			name.clear();
			lineNum = _lineNum;
			left.reset();
			right.reset();
		}
		int getLine() { return lineNum; };
		std::string getName() { return name; };
		bool isReady() { return type == Literal; };
		bool resolve( Assembler &assembler );
		void setValue( int val )
		{
			type = Literal;
			value = val;
		}
		int getValue()
		{
			if ( type != Literal )
				throw ParseError( lineNum, "cannot resolve expression '" + name + "'!" );
			return value;
		};
		std::string getDesc() const
		{
			std::string res;
			if ( type == Literal )
			{
				res = std::to_string( value );
			}
			else if ( type == Symbol )
			{
				res = name;
			}
			else if ( type == Operator )
			{
				res = "( " + left->getDesc() + " " + name + " " + right->getDesc() + " )";
			}
			return res;
		}
		void setSymbol( const std::string &_name )
		{
			type = Symbol;
			name = _name;
		}
		void setArgs( std::shared_ptr< ExprNode > _left, std::shared_ptr< ExprNode > _right )
		{
			left = _left;
			right = _right;
		}

		ExprNode(): type( Literal ), value( 0 ), lineNum( 0 ) {};
		ExprNode( int _lineNum ): type( Literal ), value( 0 ), lineNum( _lineNum ) {};
		ExprNode( Type _type, std::string _name, int _value, int _lineNum ): type( _type ), name( _name ), value( _value ), lineNum( _lineNum ) {};
		ExprNode( const ExprNode &src ): type( src.type ), name( src.name ), value( src.value ), lineNum( src.lineNum ), left( src.left ), right( src.right ) {};
		void operator=( const ExprNode &src )
		{
			type = src.type;
			name = src.name;
			value = src.value;
			lineNum = src.lineNum;
			left = src.left;
			right = src.right;
		}
	};

	class Identifier
	{
	private:
		ExprNode node;

	public:
		enum Type
		{
			Register,
			Symbol,
			Command,
			CondBranch
		};
		enum Mode
		{
			AsmClassic,
			AsmNew,
			AsmBoth
		};
		
		std::string name;
		Mode mode;
		Type type;
		int getValue() 
		{ 
			return node.getValue();
		};
		void setValue( int val ) { node.setValue( val ); };
		bool isReady()
		{
			return node.isReady();
		}
		bool resolve( Assembler &assembler )
		{
			return node.resolve( assembler );
		}
		void setExpr( const ExprNode &src )
		{
			type = Symbol;
			node = src;
		}

		//Identifier() {};
        Identifier( const std::string &_name, Type _type, int _value, Mode _mode ):
            node( ExprNode::Literal, "", _value, 0 ), name( _name ), mode( _mode ), type( _type ) {};
        Identifier( const Identifier &src ):
            node( src.node ), name( src.name ), mode( src.mode ), type( src.type ) {};
	};

	struct ForwardReference
	{
	public:
		int type;
		mWord addr;
		ExprNode node;

	public:
		enum Type
		{
			Address,
			CondAdd,
			InplaceImm
		};

		int getLine() { return node.getLine(); };
        ForwardReference( const std::string _name, mWord _addr, int _lineNum, int _type = Address ):
            type( _type ), addr( _addr ), node( ExprNode::Symbol, _name, 0, _lineNum ) {};
        ForwardReference( const ExprNode &_node, mWord _addr, int _type = Address ):
            type( _type ), addr( _addr ), node( _node ) {};
	};

    MMU         &mmu;
	mWord		org = 0;
	std::string	errorMessage;
	int		lineNum;
	std::string	lastLabel;
	std::string	curLabel;
    size_t		curLexem;
	std::vector< Identifier >	identifiers;
	std::vector< ForwardReference >	forwards;
	bool newSyntaxMode = false;
	// Current state of line parsing
	bool newSyntax, indirect;
	ExprNode nodeR, nodeY, nodeX;
	int r, x, y, cmd, cond, stage;

	void processArgument( const std::string &kind, const int reg, const ExprNode *expr );

	Identifier *findIdentifier( const std::string &name, bool newSyntex );

    std::string extractNextLexem( const std::string &parseString, size_t &parsePos );
	void extractLexems( const std::string &parseString, std::vector< std::string > &data, bool &hasLabel );

	void parseLine();
	std::string getNextLexem();
	std::string peekNextLexem();
	void putBackLexem();

public:
	Assembler() = delete;
	Assembler( const Assembler &src ) = delete;
    Assembler( MMU &_mmu ): mmu( _mmu ) {};

	void setOrg( mWord newOrg )
	{
		org = newOrg;
	};
	void op( mTag _cmd, mTag _r, mTag _y, mTag _x, int addr = -1 )
	{	
		if ( addr == -1 )
			addr = org++;
        mmu.write( addr, Instruction::encode( _cmd, _r, _y, _x ) );
	};
	void data( mWord _data, int addr = -1 )
	{
		if ( addr == -1 )
			addr = org++;
        mmu.write( addr, _data );
	};
	bool getNewSyntax() { return newSyntax; };

	void resolveForwards();
	void resolve( ExprNode &node )
	{	
		node.resolve( *this );
		if ( !node.isReady() )
		{
			resolveForwards();
			node.resolve( *this );
		}
	}
	void reset();
	void parseStart();
	void parseEnd();
	ExprNode parseBrackets( bool allowEndOfLine );
	ExprNode parseExpr( int addrForForward, bool tryToResolve );

	void preProcessFile( const std::string &fileName );

    bool parseFile( const std::string &fileName );
    const std::string &getErrorMessage() { return errorMessage; };
    void setSourceFileProvider( const std::shared_ptr< FileProvider > _provider ) { provider = _provider; };

};

}	// namespace Simpleton

#endif // SIMPLETON_4_ASM_H
