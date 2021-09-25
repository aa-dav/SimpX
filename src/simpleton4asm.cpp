#include "simpleton4asm.h"

namespace Simpleton
{

void Assembler::reset()
{
	org = 0;
	files.clear();
	lines.clear();
	identifiers.clear();
	forwards.clear();
}

void Assembler::parseStart()
{
	lastLabel.clear();
	curLabel.clear();
	lineNum = 0;
	errorMessage.clear();

	identifiers.clear();
	identifiers.emplace_back( "r0",		Identifier::Register, REG_R0,	Identifier::AsmBoth );
	identifiers.emplace_back( "r1",		Identifier::Register, REG_R1,	Identifier::AsmBoth );
	identifiers.emplace_back( "r2",		Identifier::Register, REG_R2,	Identifier::AsmBoth );
	identifiers.emplace_back( "r3",		Identifier::Register, REG_R3,	Identifier::AsmBoth );
	identifiers.emplace_back( "r4",		Identifier::Register, REG_R4,	Identifier::AsmBoth );
	identifiers.emplace_back( "r5",		Identifier::Register, REG_R5,	Identifier::AsmBoth );
	identifiers.emplace_back( "r6",		Identifier::Register, REG_R6,	Identifier::AsmBoth );
	identifiers.emplace_back( "r7",		Identifier::Register, REG_R7,	Identifier::AsmBoth );
	identifiers.emplace_back( "pc",		Identifier::Register, REG_PC,	Identifier::AsmBoth );
	identifiers.emplace_back( "void",	Identifier::Register, IND_PC,	Identifier::AsmBoth );
	identifiers.emplace_back( "sp",		Identifier::Register, REG_SP,	Identifier::AsmBoth );
	identifiers.emplace_back( "psw",	Identifier::Register, REG_PSW,	Identifier::AsmBoth );

	identifiers.emplace_back( "addi",	Identifier::Command, OP_ADDI,	Identifier::AsmClassic );
	identifiers.emplace_back( "addis",	Identifier::Command, OP_ADDIS,	Identifier::AsmClassic );	
	identifiers.emplace_back( "adds",	Identifier::Command, OP_ADDS,	Identifier::AsmClassic );
	identifiers.emplace_back( "add",	Identifier::Command, OP_ADD,	Identifier::AsmClassic );
	identifiers.emplace_back( "adc",	Identifier::Command, OP_ADC,	Identifier::AsmClassic );
	identifiers.emplace_back( "sub",	Identifier::Command, OP_SUB,	Identifier::AsmClassic );
	identifiers.emplace_back( "sbc",	Identifier::Command, OP_SBC,	Identifier::AsmClassic );
	identifiers.emplace_back( "and",	Identifier::Command, OP_AND,	Identifier::AsmClassic );
	identifiers.emplace_back( "or",		Identifier::Command, OP_OR,	Identifier::AsmClassic );
	identifiers.emplace_back( "xor",	Identifier::Command, OP_XOR,	Identifier::AsmClassic );
	identifiers.emplace_back( "cadd",	Identifier::Command, OP_CADD,	Identifier::AsmClassic );
	identifiers.emplace_back( "rrci",	Identifier::Command, OP_RRCI,	Identifier::AsmClassic );
	identifiers.emplace_back( "rrc",	Identifier::Command, OP_RRC,	Identifier::AsmClassic );

	identifiers.emplace_back( "move",	Identifier::Command, OP_ADDIS,	Identifier::AsmClassic );	// shortcut for addis (move)
	identifiers.emplace_back( "movet",	Identifier::Command, OP_ADDI,	Identifier::AsmClassic );	// shortcut for addi (move with test)

	identifiers.emplace_back( "+s",		Identifier::Command, OP_ADDS,	Identifier::AsmNew );
	identifiers.emplace_back( "+",		Identifier::Command, OP_ADD,	Identifier::AsmNew );
	identifiers.emplace_back( "+c",		Identifier::Command, OP_ADC,	Identifier::AsmNew );
	identifiers.emplace_back( "-",		Identifier::Command, OP_SUB,	Identifier::AsmNew );
	identifiers.emplace_back( "-c",		Identifier::Command, OP_SBC,	Identifier::AsmNew );
	identifiers.emplace_back( "&",		Identifier::Command, OP_AND,	Identifier::AsmNew );
	identifiers.emplace_back( "|",		Identifier::Command, OP_OR,	Identifier::AsmNew );
	identifiers.emplace_back( "^",		Identifier::Command, OP_XOR,	Identifier::AsmNew );
	identifiers.emplace_back( "+?",		Identifier::Command, OP_CADD,	Identifier::AsmNew );
	identifiers.emplace_back( ">>",		Identifier::Command, OP_RRC,	Identifier::AsmNew );
	// addis, addi, rrci

	identifiers.emplace_back( "jz",		Identifier::CondBranch, COND_ZERO,		Identifier::AsmBoth );
	identifiers.emplace_back( "jnz",	Identifier::CondBranch, COND_NZERO,		Identifier::AsmBoth );
	identifiers.emplace_back( "jc",		Identifier::CondBranch, COND_CARRY,		Identifier::AsmBoth );
	identifiers.emplace_back( "jnc",	Identifier::CondBranch, COND_NCARRY,		Identifier::AsmBoth );
	identifiers.emplace_back( "js",		Identifier::CondBranch, COND_SIGN,		Identifier::AsmBoth );
	identifiers.emplace_back( "jns",	Identifier::CondBranch, COND_NSIGN,		Identifier::AsmBoth );
	identifiers.emplace_back( "jo",		Identifier::CondBranch, COND_OVERFLOW,		Identifier::AsmBoth );
	identifiers.emplace_back( "jno",	Identifier::CondBranch, COND_NOVERFLOW,		Identifier::AsmBoth );

	identifiers.emplace_back( "ja",		Identifier::CondBranch, COND_ABOVE,		Identifier::AsmBoth );
	identifiers.emplace_back( "jbe",	Identifier::CondBranch, COND_BELOW_EQ,		Identifier::AsmBoth );
	identifiers.emplace_back( "jge",	Identifier::CondBranch, COND_GREATER_EQ,	Identifier::AsmBoth );
	identifiers.emplace_back( "jl",		Identifier::CondBranch, COND_LESS,		Identifier::AsmBoth );
	identifiers.emplace_back( "jg",		Identifier::CondBranch, COND_GREATER,		Identifier::AsmBoth );
	identifiers.emplace_back( "jle",	Identifier::CondBranch, COND_LESS_EQ,		Identifier::AsmBoth );

	forwards.clear();
}

void Assembler::parseEnd()
{
	resolveForwards();
	for ( auto &fwd : forwards )
	{
		if ( !fwd.node.isReady() )
            throw ParseError( fwd.getLine(), "unresolved forward reference '" + fwd.node.getUnresolvedName() + "'!" );
		if ( fwd.type == ForwardReference::CondAdd )
		{
			int offs = (fwd.node.getValue() & 0xFFFF) - fwd.addr - 1;
			if ( (offs < -2048) || (offs > 2047) )
				throw ParseError( fwd.getLine(), "conditional jump offset is too big (" + std::to_string( offs ) + ")!" );
            mmu.write( fwd.addr, mmu.read( fwd.addr ) | (offs & 0x0FFF) );
		}
		else if ( fwd.type == ForwardReference::InplaceImm )
		{
			throw ParseError( fwd.getLine(), "implace-immediate forwards are not implemented yet!" );
		}
		else
		{
            mmu.write( fwd.addr, fwd.node.getValue() );
		}
	};
}

Identifier *Assembler::findIdentifier( const std::string &name, bool newSyntax )
{
	Identifier *res = nullptr;
    for ( size_t i = 0; i < identifiers.size(); i++ )
	{
        Identifier &ident = identifiers[ i ];
        if ( !newSyntax && (ident.mode == Identifier::AsmNew) )
			continue;
		if ( newSyntax && (ident.mode == Identifier::AsmClassic) )
			continue;
		if ( ident.name == name )
		{
			res = &ident;
			break;
		}
	}
	return res;
};

std::string ExprNode::getUnresolvedName() const
{
    if ( left && !left->isReady() )
        return left->getUnresolvedName();
    if ( right && !right->isReady() )
        return right->getUnresolvedName();
    return getName();
}

bool ExprNode::resolve( Assembler &assembler )
{
	if ( type == ExprNode::Literal )
	{
		return false; // all is ready
	}
	else if ( type == ExprNode::Symbol )
	{
		Identifier *iden = assembler.findIdentifier( name, assembler.getNewSyntax() );
		if ( iden == nullptr )
			return false;	// it's not parsed yet?
			//throw ParseError( lineNum, "unknown forward reference '" + name + "'!" );
		if ( iden->type != Identifier::Symbol )
			throw ParseError( lineNum, "forward reference '" + name + "' is not symbol!" );
		if ( iden->isReady() )
		{
			value = iden->getValue();
			type = ExprNode::Literal;
            return true;
        }
        return false;
	}
	else
	{
		bool res = false;
		if ( !left )
			throw ParseError( lineNum, "left is null in expr " + name + "!" );
		if ( !right )
			throw ParseError( lineNum, "right is null in expr " + name + "!" );
		if ( !left->isReady() )
			res = res || left->resolve( assembler );
		if ( !right->isReady() )
			res = res || right->resolve( assembler );
		if ( left->isReady() && right->isReady() )
		{
			int l = left->getValue();
			int r = right->getValue();
			if ( name == "+" ) {
				value = l + r;
			} else if ( name == "-" ) {
				value = l - r;
			} else if ( name == "/" ) {
				value = l / r;
			} else if ( name == "*" ) {
				value = l * r;
			} else if ( name == "%" ) {
				value = l % r;
			} else if ( name == "<<" ) {
				value = l << r;
			} else if ( name == ">>" ) {
				value = l >> r;
			} else if ( name == "&" ) {
				value = l & r;
			} else if ( name == "|" ) {
				value = l | r;
			} else if ( name == "^" ) {
				value = l ^ r;
			}
			left.reset();
			right.reset();
			type = Literal;
			res = true;
		}
		return res;
		//throw ParseError( lineNum, "expressions are not implemented yet!" );
	}
}

void Assembler::resolveForwards()
{
	// Try to finally calc all forwards...
	int tries = 0;
	while ( true )
	{
		tries++;
		bool changed = false;
        for ( auto &iden : identifiers )
        {
            if ( !iden.isReady() )
                if ( iden.resolve( *this ) )
                    changed = true;
        }
        for ( auto &fwd : forwards )
		{
			if ( !fwd.node.isReady() )
            {
                if ( fwd.node.resolve( *this ) )
					changed = true;
            }
		}
		if ( !changed )
			break;
	}
}

static bool isTerminal( char c )
{
	return (c == '(') || (c == ')') || (c == '[') || (c == ']');
};

std::string Assembler::extractNextLexem( const std::string &parseString, size_t &parsePos )
{
	std::string res;
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
	};
	char sym = parseString[ parsePos ];
	if ( isTerminal( sym ) )
	{
		res += sym;
		++parsePos;
	}
	else if ( sym == '"' )
	{
		while( true )
		{
			res += parseString[ parsePos ];
			if ( ++parsePos >= parseString.length() )
				throw ParseError( lineNum, "unexpected end of string '" + res + "'!" );
			if ( parseString[ parsePos ] == '"' )
			{
				parsePos++;
				break;
			}
		};
	}
    else if ( sym == '\'' )
    {
         res += parseString[ parsePos ];
         if ( ++parsePos >= parseString.length() )
             throw ParseError( lineNum, "unexpected end of char '" + res + "'!" );
         res += parseString[ parsePos ];
         if ( ++parsePos >= parseString.length() )
             throw ParseError( lineNum, "unexpected end of char '" + res + "'!" );
         if ( parseString[ parsePos ] == '\'' )
         {
            parsePos++;
         }
         else
             throw ParseError( lineNum, "unterminated char '" + res + "'!" );
    }
    else
	{
		while ( !isspace( parseString[ parsePos ] ) )
		{
			res += parseString[ parsePos ];
			if ( ++parsePos >= parseString.length() )
				break;
			if ( isTerminal( parseString[ parsePos ] ) )
				break;
		};
	};
	if ( res == ";" )
	{
		// comment terminates parsing...
		res.clear();
		parsePos = parseString.length();
	};
	return res;
};

void Assembler::extractLexems( const std::string &parseString, std::vector< std::string > &data, bool &hasLabel )
{
	data.clear();
	hasLabel = false;
	bool first = true;
    size_t parsePos = 0;
	while ( true )
	{
		std::string cur = extractNextLexem( parseString, parsePos );
		if ( cur.empty() )	
			break;
		if ( first && !isspace( parseString[ 0 ] ) )
		{
			hasLabel = true;
		}
		data.push_back( cur );
	        first = false;
	}
}

std::string Assembler::getNextLexem()
{
	if ( curLexem < lines[ lineNum - 1 ].lexems.size() )
		return lines[ lineNum - 1 ].lexems[ curLexem++ ];
	else
		return std::string();
};

std::string Assembler::peekNextLexem()
{
	if ( curLexem < lines[ lineNum - 1 ].lexems.size() )
		return lines[ lineNum - 1 ].lexems[ curLexem ];
	else
		return std::string();
};

void Assembler::putBackLexem()
{
    if ( curLexem <= 0 )
        throw ParseError( lineNum, "invalid putBackLexem()!" );
    curLexem--;
};

static bool lexemIsNumberLiteral( const std::string &lexem  )
{
    return (lexem[ 0 ] == '$') || isdigit( lexem[ 0 ] ) ||
            ( (lexem[ 0 ] == '-') && (lexem.size() > 1) && (isdigit( lexem[ 1 ] )) ) ||
            ( (lexem.size() == 2) && (lexem[ 0 ] == '\'') );
}

static int parseNumberLiteral( const std::string &lexem )
{
	int res;
    if ( (lexem.size() == 2) && (lexem[ 0 ] == '\'') )
    {
        res = lexem[ 1 ];
    }
    else if ( lexem[ 0 ] == '$' )
    {
		res = strtol( lexem.c_str() + 1, nullptr, 16 );
    }
	else
    {
        if ( lexem[ lexem.length() - 1 ] == 'b' )
        {
            res = strtol( lexem.c_str(), nullptr, 2 );
        }
        else
        {
            res = strtol( lexem.c_str(), nullptr, 10 );
        }
    }
	return res;
}

struct OperDesc
{
	const char *name;
	mTag priority;
};

static OperDesc opers[] = {
	{ "^", 21 },
	{ "|", 22 },
	{ "&", 23 },
	{ "<<", 40 },
	{ ">>", 40 },
	{ "+", 50 },
	{ "-", 50 },
	{ "*", 70 },
	{ "/", 70 },
	{ "%", 70 },
	{ nullptr, 0 }
};

static bool lexemIsOperator( const std::string &lexem )
{
	OperDesc *opDesc = opers;
	while ( opDesc->name )
	{
		if ( lexem == opDesc->name )
			return true;
		opDesc++;
	}
	return false;
}

static mTag operatorPriority( const std::string &op )
{
	OperDesc *opDesc = opers;
	while ( opDesc->name )
	{
		if ( op == opDesc->name )
			return opDesc->priority;
		opDesc++;
	}
	return 0;
}

ExprNode Assembler::parseBrackets( bool allowEndOfLine )
{
	std::vector< ExprNode > nodes;
	nodes.push_back( parseExpr( -1, false ) );
	while ( true )
	{
		std::string next = getNextLexem();
		if ( next.empty() )
		{
			if ( !allowEndOfLine )
				throw ParseError( lineNum, "unexpected termination of expression!" );
			break;
		}
		if ( next == ")" )
		{
			break;
		}
		else if ( lexemIsOperator( next ) )
		{
			nodes.emplace_back( ExprNode::Operator, next, 0, lineNum );
		}
		else
			throw ParseError( lineNum, "operator expected, but '" + next + "' was met!" );
		// Get next 
		nodes.push_back( parseExpr( -1, false ) );
	}
	// normalize results
	while ( nodes.size() > 1 )
	{
		int maxOpIdx = 1;
		mTag maxOpPr = operatorPriority( nodes[ 1 ].getName() );
        for ( size_t i = 3; i < nodes.size(); i++ )
		{
			mTag curPr = operatorPriority( nodes[ i ].getName() );
			if ( curPr > maxOpPr )
			{
				maxOpPr = curPr;
				maxOpIdx = i;
			}
		}
		std::shared_ptr< ExprNode > left( new ExprNode( nodes[ maxOpIdx - 1 ] ) );
		std::shared_ptr< ExprNode > right( new ExprNode( nodes[ maxOpIdx + 1 ] ) );
		nodes.erase( nodes.begin() + maxOpIdx + 1 );
		nodes.erase( nodes.begin() + maxOpIdx - 1 );
		nodes[ maxOpIdx - 1 ].setArgs( left, right );
	}
	
	return nodes[ 0 ];
}

ExprNode Assembler::parseExpr( int addrForForward, bool tryToResolve )
{
	ExprNode res( lineNum );
	std::string expr = getNextLexem();
	if ( expr.empty() )
		throw ParseError( lineNum, "expression expected!" );
	if ( lexemIsNumberLiteral( expr ) )
	{
		res.setValue( parseNumberLiteral( expr ) );
	}
	else if ( expr == "(" )
	{
		// compound expression met!
		res = parseBrackets( false );
        if ( addrForForward != -1 )
            forwards.emplace_back( res, addrForForward );
	}
	else
	{
		// try to find symbol
		Identifier *iden = findIdentifier( expr, newSyntax );
		if ( iden != nullptr )
		{
			if ( iden->type == Identifier::Symbol )
			{
				res.setSymbol( expr );
			}
			else
				throw ParseError( lineNum, "identifier for expression '" + expr + "' is not symbol!" );
		}
		else 
		{
			res.setSymbol( expr );
			if ( addrForForward != -1 )
				forwards.emplace_back( expr, addrForForward, lineNum );
		}
	};
	if ( tryToResolve )
		resolve( res );
	return res;
};

void Assembler::processArgument( const std::string &kind, const int reg, const ExprNode *expr )
{
	if (	(!newSyntax && (stage == 1)) ||
		(newSyntax && (stage == 0)) )
	{
		r = reg;
		if ( expr )
			nodeR = *expr;
	}
	else if (	(!newSyntax && (stage == 2)) ||
			(newSyntax && (stage == 2)) )
	{
		y = reg;
		if ( expr )
			nodeY = *expr;
	}
	else if (	(!newSyntax && (stage == 3)) ||
			(newSyntax && (stage == 4)) )
	{
        //if ( Instruction::isInplaceImmediate( cmd ) && indirect )
        //	throw ParseError( lineNum, "inplace immediate cannot be indirect!" );
        if ( Instruction::isInplaceImmediate( cmd ) && (reg != IMMED) )
            throw ParseError( lineNum, "inplace immediate must be immediate!" );
        x = reg;
		if ( expr )
			nodeX = *expr;
	}
	else
	{
        throw ParseError( lineNum, kind + " '" + (expr ? expr->getDesc() : std::to_string( reg ) ) + "' at wrong place!" );
	};
}

void Assembler::parseLine()
{
	bool invertX = false;
	bool emitForCall = false;
	int eqSign = 0; // 0 - "=", 1 - "<=", 2 - "<-" in new syntax

	// Preset common state variables...
	indirect = false;
	newSyntax = newSyntaxMode;
	stage = 0;
	cmd = -1; 
	cond = -1;
	r = -1; x = -1; y = -1; 
	int ln = lines[ lineNum - 1 ].num;
	nodeR.setup( ln );
	nodeY.setup( ln );
	nodeX.setup( ln );

	if ( !curLabel.empty() )
	{
		if ( findIdentifier( curLabel, newSyntax ) != nullptr )
			throw ParseError( lineNum, "identifier " + curLabel + " is redefined!" );
		identifiers.emplace_back( curLabel, Identifier::Symbol, org, Identifier::AsmBoth );
	}
	std::string lexem;
	bool first = true;
	while ( !(lexem = getNextLexem()).empty() )
	{
		if ( first && (lexem == "org") )
		{
			ExprNode node = parseExpr( -1, true );
			org = node.getValue();
			if ( !curLabel.empty() )
			{
				Identifier *iden = findIdentifier( curLabel, newSyntax );
				if ( iden == nullptr )
					throw ParseError( lineNum, "current label does not exist!" );
				iden->setValue( org );
			}
			return;	// no futher actions required
		}
		else if ( first && (lexem == "=") )
		{
			if ( curLabel.empty() )
				throw ParseError( lineNum, "new symbol is required for equatation!" );
			Identifier *iden = findIdentifier( curLabel, newSyntax );
			if ( iden == nullptr )
				throw ParseError( lineNum, "current label does not exist!" );
			iden->setExpr( parseBrackets( true ) );
			return;	// no futher actions required
		}
		else if ( first && (lexem == "mode") )
		{
			lexem = getNextLexem();
			if ( lexem == "classic" )
			{
				newSyntaxMode = false;
			}
			else if ( lexem == "new" )
			{
				newSyntaxMode = true;
			}
			else
				throw ParseError( lineNum, "wrong assembler mode operand ('classic' or 'new' expected)!" );
			return;	// no futher actions required
		}
		else if ( first && (lexem == "dw") )
		{
			while ( true )
			{
				lexem = peekNextLexem();
				if ( lexem.empty() )
					break;	// end of data
				if ( lexem[ 0 ] == '"' )
				{
					lexem = getNextLexem();
                    for ( size_t i = 1; i < lexem.length(); i++ )
					{
						data( lexem[ i ] );
					}
				}
				else
				{
					ExprNode node = parseExpr( org, false );
					if ( node.isReady() )
						data( node.getValue() );
					else
						data( 0 );
				};
			};
			return;	// no futher actions required
		}
		else if ( first && (lexem == "ds") )
		{
		        mWord size = parseExpr( -1, true ).getValue();
		        mWord fill = 0;
		        lexem = peekNextLexem();
		        if ( !lexem.empty() )
		        {
		        	fill = parseExpr( -1, true ).getValue();
		        };
		        for ( int i = 0; i < size; i++ )
		        	data( fill );
			return;	// no futher actions required
		}
		else if ( (lexem == "=") && newSyntax )
		{
			if ( stage != 1 )
				throw ParseError( lineNum, "keyword '=' at wrong place!" );
			stage++;
		}
		else if ( (lexem == "<=") && newSyntax )
		{
			if ( stage != 1 )
				throw ParseError( lineNum, "keyword '<=' at wrong place!" );
			cmd = OP_ADDI;
			eqSign = 1;
			stage++;
		}
		else if ( (lexem == "<-") && newSyntax )
		{
			if ( stage != 1 )
				throw ParseError( lineNum, "keyword '<-' at wrong place!" );
			cmd = OP_ADDIS;
			eqSign = 2;
			stage++;
		}
		else if ( lexem == "[" )
		{
			indirect = true;
		}
		else if ( lexem == "]" )
		{
			indirect = false;
		}
		else if ( lexem == "ret" )
		{
			if ( stage != 0 )
				throw ParseError( lineNum, "'ret' in wrong place!" );
			cmd = OP_ADDIS;
			r = REG_PC;
			y = IND_SP;
			x = IMMED;
			stage = 4;
			newSyntax = false;	// revert to classic mode
		}
		else if ( lexem == "call" )
		{
			if ( stage != 0 )
				throw ParseError( lineNum, "'call' in wrong place!" );
			emitForCall = true;
			cmd = OP_ADDIS;
			r = REG_PC;
			stage = 2;
			newSyntax = false;	// revert to classic mode
		}
		else
		{
			Identifier *iden = findIdentifier( lexem, newSyntax );
			if ( iden != nullptr )
			{
				if ( iden->type == Identifier::Register )
				{
					processArgument( "register", iden->getValue() + (indirect ? 8 : 0), nullptr );
				}
				else if ( iden->type == Identifier::Command )
				{
					// C 1 2 3
					// 0 = 2 + 4
					if (	(!newSyntax && (stage == 0)) ||
						(newSyntax && (stage == 3)) )
					{
						cmd = iden->getValue();
						if ( newSyntax )
						{
							// corrections for eqSign
							if ( eqSign == 1 ) {
								if ( cmd == OP_ADD ) {
									cmd = OP_ADDI;
                                } else if ( cmd == OP_SUB ) {
                                    cmd = OP_ADDI;
                                    invertX = true;
                                } else if ( cmd == OP_RRC ) {
									cmd = OP_RRCI;
								} else
                                    throw ParseError( lineNum, "'<=' is used with wrong operator '" + lexem + "'!" );
							} else if ( eqSign == 2 ) {
								if ( cmd == OP_ADD ) {
									cmd = OP_ADDIS;
								} else if ( cmd == OP_SUB ) {
									cmd = OP_ADDIS;
									invertX = true;
								} else
                                    throw ParseError( lineNum, "'<-' is used with wrong operator '" + lexem + "'!" );
							}
						}
					}
					else
					{
						throw ParseError( lineNum, "command '" + lexem + "' at wrong place!" );
					}
				}
				else if ( iden->type == Identifier::CondBranch )
				{
					if ( stage != 0 )
						throw ParseError( lineNum, " conditional branch '" + lexem + "' at wrong place!" );
					cmd = OP_CADD;
					r = REG_PC;
					y = REG_PC;
					stage = 2;
					newSyntax = false;	// revert to classic syntax
					cond = iden->getValue();
				}
				else
				{
					putBackLexem();
					ExprNode node = parseExpr( -1, false );
					processArgument( "expr", indirect ? IND_IMMED : IMMED, &node ); // revert to forward reference
				};
			}
			else
			{
				putBackLexem();
				ExprNode node = parseExpr( -1, false );
				processArgument( "expr", indirect ? IND_IMMED : IMMED, &node ); // revert to forward reference
			}
			stage++;
		}
		first = false;
	};

	if ( stage == 0 )
		return;	// just comment?

	if ( cmd == -1 )
		throw ParseError( lineNum, "CMD is undefined!" );
	if ( r == -1 )
		throw ParseError( lineNum, "R is undefined!" );
	if ( y == -1 )
		throw ParseError( lineNum, "Y is undefined!" );
	if ( x == -1 )
	{
		x = IMMED;
	};

	if ( Instruction::isInplaceImmediate( cmd ) )
	{
		if ( !nodeX.isReady() )
			resolve( nodeX );
		if ( invertX )
			nodeX.setValue( -nodeX.getValue() );
		// inplace immediate mode
        if ( cmd == OP_RRCI )
        {
            if ( (nodeX.getValue() < 0) || (nodeX.getValue() > +15) )
                throw ParseError( lineNum, "inplace immediate X is too big!" );
        }
        else
        {
            if ( (nodeX.getValue() < -8) || (nodeX.getValue() > +7) )
                throw ParseError( lineNum, "inplace immediate X is too big!" );
        }
		x = nodeX.getValue() & 0b1111;
	}

	if ( emitForCall )
		op( OP_ADDIS, IND_SP, REG_PC, 2 );

	op( cmd, r, y, x );

	// immediates...
	if ( !Instruction::isInplaceImmediate( cmd ) && ((x == IMMED) || (x == IND_IMMED)) )
	{
		if ( cond != -1 )
		{
			if ( nodeX.isReady() )
			{
				int offs = (nodeX.getValue() & 0xFFFF) - org - 1;
				if ( (offs < -2048) || (offs > 2047) )
					throw ParseError( lineNum, "conditional jump offset is too big (" + std::to_string( offs ) + ")!" );
				data( (cond << 12) | (offs & 0x0FFF) );
			}
			else
			{
				data( (cond << 12) ); // place condition
				forwards.emplace_back( nodeX, org - 1, ForwardReference::CondAdd ); // conditional forward!!!
			}
		}
		else
		{
			if ( nodeX.isReady() ) {
				data( nodeX.getValue() );
			} else {
				data( 0 );
				forwards.emplace_back( nodeX, org - 1 );
			}
		}
	}
	if ( (y == IMMED) || (y == IND_IMMED) )
	{
		if ( nodeY.isReady() ) {
			data( nodeY.getValue() );
		} else {
			data( 0 );
			forwards.emplace_back( nodeY, org - 1 );
		}
	}
	if ( r == IND_IMMED )
	{
		if ( nodeR.isReady() ) {
			data( nodeR.getValue() );
		} else {
			data( 0 );
			forwards.emplace_back( nodeR, org - 1 );
		}
	}

}

static std::string getFilePath( const std::string &fileName )
{
    std::string res;
    if ( !fileName.empty() )
    {
        auto it = fileName.end() - 1;
        while ( it != fileName.begin() )
        {
            if ( (*it == '\\') || (*it == '/') )
            {
                res = fileName.substr( 0, it - fileName.begin() + 1 );
                        break;
            }
            it--;
        }
    }
    return res;
}

void Assembler::preProcessFile( const std::string &fileName )
{
	std::string line;
	int innerLineNum = 0;
	int fileNum = files.size();
	files.emplace_back( fileName );
    std::shared_ptr< File > ifs;
    if ( !provider )
        throw PreProcessorError( fileNum - 1, innerLineNum, "File provider is not set!" );
    ifs = provider->open( fileName );
    if ( !ifs )
        throw PreProcessorError( fileNum - 1, innerLineNum, "Cannot open file '" + fileName + "'!" );
    while ( !ifs->eof() )
	{
        line = ifs->get_line();
		lineNum++;
		innerLineNum++;
		std::vector< std::string > lexems;
		bool hasLabel;
		extractLexems( line, lexems, hasLabel );
		if ( (lexems.size() > 0) && (lexems[ 0 ][ 0 ] == '#') )
		{
			if ( lexems[ 0 ] == "#include" )
			{
				if ( lexems.size() != 2 )
					throw PreProcessorError( fileNum, innerLineNum, "#include directive must has one string parameter!" );
				if ( lexems[ 1 ][ 0 ] != '"' )
					throw PreProcessorError( fileNum, innerLineNum, "#include directive parameter must be quoted string!" );
                std::string fn = getFilePath( fileName ) + &lexems[ 1 ][ 1 ];
                preProcessFile( fn );
			}
			else
			{
				throw PreProcessorError( fileNum, innerLineNum, "unknown preprocessor directive '" + lexems[ 0 ] + "'!" );
			}
		}
		else
		{
			if ( lexems.size() > 0 )
				lines.emplace_back( fileNum, innerLineNum, hasLabel, lexems );
		}
	};
}

bool Assembler::parseFile( const std::string &fileName )
{
    bool ret = true;
	try
	{
		lineNum = 0;
		preProcessFile( fileName ); // preprocess
		// Preprocessed source dump:
		/*
		for ( int i = 0; i < lines.size(); i++ )
		{
			std::cout << "File " << lines[ i ].file << " line " << lines[ i ].num << ":";
			if ( !lines[ i ].label ) std::cout << "    ";
			for ( int j = 0; j < lines[ i ].lexems.size(); j++ )
			{
				std::cout << " " << lines[ i ].lexems[ j ];
			}
			std::cout << "\n";
		}
		*/
		// Assemble source code:
		parseStart();
		std::string line;
        for ( size_t i = 0; i < lines.size(); i++ )
		{
			lineNum++;
			curLexem = 0;
			curLabel.clear();

			std::vector< std::string > &lexems = lines[ i ].lexems;
			if ( lines[ i ].label )
				if ( lexems[ 0 ][ 0 ] != '.' )
					lastLabel = lexems[ 0 ];	// update last label if it is not local
			// explode local ifentifiers
            for ( size_t j = 0; j < lexems.size(); j++ )
			{
				if ( lexems[ j ][ 0 ] == '.' )
					lexems[ j ] = lastLabel + lexems[ j ];
			}
			if ( lines[ i ].label )	// assign current label if needed
			{
				curLabel = lines[ i ].lexems[ 0 ];
				curLexem++;
			}
			parseLine();
		}
		parseEnd();
		// Dump identifiers...
        /*
		for ( auto &i : identifiers )
		{
			if ( i.type == Identifier::Symbol )
				std::cout << "Symbol: " << i.name << " value: " << i.getValue() << "\n";
		}
        */
	}
	catch ( const Simpleton::PreProcessorError &error )
	{
		int errorLine = error.getLine();
		std::string fname;
		if ( error.getFile() >= 0 )
			fname = files[ error.getFile() ].name;
        errorMessage = std::string( "Preprocessor error at file '" ) + fname + "' line " + std::to_string( errorLine ) + " Reason: " + error.getReason();
        ret = false;
	}
	catch ( const Simpleton::ParseError &error )
	{
		int errorLine = error.getLine();
		SourceLine line;
		line.file = -1;
		line.num = 0;
		line.label = false;
		SourceFile file{ "<unknown>" };
        if ( (errorLine > 0) && (errorLine <= (int)lines.size()) )
			line = lines[ errorLine - 1 ];
        if ( (line.file >= 0) && (line.file < (int)files.size()) )
			file = files[ line.file ];
		
		errorMessage = std::string( "Parse error at file '" ) + file.name + "' line " + std::to_string( line.num ) + " Reason: " + error.getReason();
        ret = false;
	}
	catch ( const std::exception &error )
	{
		errorMessage = std::string( "Unknown error: " ) + error.what();
        ret = false;
	}
    return ret;
};

}	// namespace Simpleton
