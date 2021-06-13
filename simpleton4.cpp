#include "simpleton4.h"
#include <sstream>

namespace Simpleton
{

/*static*/ bool Instruction::isInplaceImmediate( mTag cmd )
{
	return (cmd == OP_ADDI) || (cmd == OP_ADDIS) || (cmd == OP_RRCI);
}

void CPU::reset()
{
    mmu.reset();
	for ( int i = 0; i < 8; i++ )
		reg[ i ] = 0;
	clocks = 0;
}

mWord CPU::fetchPC()
{
    return getMem( reg[ REG_PC ]++ );
}

mWord CPU::readArg( mTag r, mTag i )
{
	if ( i )
	{
		mWord addr;
		if ( r == REG_PSW )
            addr = fetchPC();
		else
			addr = reg[ r ];
		if ( (r == REG_PC) || (r == REG_SP) )
			reg[ r ]++;
		return getMem( addr );
	}
	else
	{
		return reg[ r ];
	}
}

void CPU::mathTempApply()
{
    a = tmp & 0xFFFF;
    setFlag( FLAG_CARRY, (tmp & 0x10000) != 0 );
    setFlag( FLAG_ZERO, (a == 0) );
    setFlag( FLAG_SIGN, (a & 0x8000) != 0 );
    setFlag( FLAG_OVERFLOW, false ); // ?
}

void CPU::mathOverflow(bool sub)
{
    bool ys = (y & 0x8000) != 0;
    bool xs = (x & 0x8000) != 0;
    bool as = (a & 0x8000) != 0;
    if ( sub )
        setFlag( FLAG_OVERFLOW, (!ys && xs && as) || (ys && !xs && !as) );
    else
        setFlag( FLAG_OVERFLOW, (!ys && !xs && as) || (ys && xs && !as) );
}

int CPU::step()
{
    mWord cond;

    if ( getFlag( FLAG_HALT ) )
    {
        clocks++;
        return 1; //
    }
    uint64_t start_clocks = clocks;

    // fetch & decode instruction
    instr.decode( fetchPC() );

    // read x
    if ( instr.isInplaceImmediate( instr.cmd ) )
    {
        if ( instr.xi )
            x = 0xFFF8 | instr.x;
        else
            x = instr.x;
    }
    else
    {
        x = readArg( instr.x, instr.xi );
	}
    y = readArg( instr.y, instr.yi );

	// ALU
	tmp = 0;
	switch ( instr.cmd )	// combined opcode
	{
	case OP_ADDIS:	// addis
	case OP_ADDS:	// adds
			a = y + x;
			//std::cout << "acc:" << a << "\n";
			break;
	case OP_ADD:	// add
	case OP_ADDI:	// addi
			tmp = y + x;
			mathTempApply();
			mathOverflow( false );
			break;
	case OP_ADC:	// adc
			tmp = y + x + (getFlag( FLAG_CARRY ) ? 1 : 0);
			mathTempApply();
			mathOverflow( false );
			break;
	case OP_SUB:	// sub
			tmp = y - x;
			mathTempApply();
			mathOverflow( true );
			break;
	case OP_SBC:	// sbc
			tmp = y - x - (getFlag( FLAG_CARRY ) ? 1 : 0);
			mathTempApply();
			mathOverflow( true );
			break;
	case OP_AND:	// and
			tmp = x & y;
			mathTempApply();
			break;
	case OP_OR:	// or
			tmp = x | y;
			mathTempApply();
			break;
	case OP_XOR:	// xor
			tmp = x ^ y;
			mathTempApply();
			break;
	case OP_CADD:	// conditional add
			cond = (x >> 12) & 0b1111;
			x = x & 0b111111111111; // 12 bit
			if ( x & 0b100000000000 )
				x = x | 0b1111000000000000; // negative extension
			if ( 	((cond == COND_ZERO) && (getFlag( FLAG_ZERO ))) ||
				((cond == COND_NZERO) && (!getFlag( FLAG_ZERO ))) ||
				((cond == COND_CARRY) && (getFlag( FLAG_CARRY ))) ||
				((cond == COND_NCARRY) && (!getFlag( FLAG_CARRY ))) ||
				((cond == COND_SIGN) && (getFlag( FLAG_SIGN ))) ||
				((cond == COND_NSIGN) && (!getFlag( FLAG_SIGN ))) ||
				((cond == COND_OVERFLOW) && (getFlag( FLAG_OVERFLOW ))) ||
				((cond == COND_NOVERFLOW) && (!getFlag( FLAG_OVERFLOW ))) ||

				((cond == COND_ABOVE) && (!getFlag( FLAG_CARRY ) && !getFlag( FLAG_ZERO ))) ||
				((cond == COND_BELOW_EQ) && (getFlag( FLAG_CARRY ) || getFlag( FLAG_ZERO ))) ||
				((cond == COND_GREATER_EQ) && (getFlag( FLAG_SIGN ) == getFlag( FLAG_OVERFLOW ))) ||
				((cond == COND_LESS) && (getFlag( FLAG_SIGN ) != getFlag( FLAG_OVERFLOW ))) ||
				((cond == COND_GREATER) && (!getFlag( FLAG_ZERO ) && (getFlag( FLAG_OVERFLOW ) == getFlag( FLAG_SIGN )))) ||
				((cond == COND_LESS_EQ) && (getFlag( FLAG_ZERO ) || (getFlag( FLAG_OVERFLOW ) != getFlag( FLAG_SIGN )))) )
			{
				a = y + x;
			}
			else
			{
				a = y;
			}
			break;
    case OP_RRCI:	// rrci
            x = x & 0b1111;
    case OP_RRC:	// rrc
            a = (y << (16 - x)) | (y >> x);
            setFlag( FLAG_CARRY, (y & (1 << (x - 1))) != 0 );
            setFlag( FLAG_ZERO, (a == 0) );
            setFlag( FLAG_SIGN, (a & 0x8000) != 0 );
            setFlag( FLAG_OVERFLOW, false );
            break;
	};
	clocks++;	// ALU tick
	// store
	if ( instr.ri )
	{
		if ( instr.r != REG_PC )	// Indirect writes to PC are ignored (destination 'void')
		{
			mWord addr;
			if ( instr.r == REG_SP )
				reg[ instr.r ]--;
			if ( instr.r == REG_PSW )
                addr = fetchPC();
			else
				addr = reg[ instr.r ];
			//std::cout << "addr:" << addr << " writ:" << a << "\n";
			setMem( addr, a );
		}
	}
	else
	{
		reg[ instr.r ] = a;
    }
    return clocks - start_clocks;
}

int CPU::triggerIRQ()
{
    uint64_t start_clocks = clocks;
    setFlag( FLAG_HALT, false );
    if ( getFlag( FLAG_IRQ_ENABLE ) )
    {
        // save flags & pc
        setMem( --reg[ REG_SP ], reg[ REG_PC ] );
        setMem( --reg[ REG_SP ], reg[ REG_PSW ] );
        reg[ REG_PC ] = 0x10;
        clocks++; // 1 clock time
    }
    return clocks - start_clocks;
};

}	// namespace Simpleton
