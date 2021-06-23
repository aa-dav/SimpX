#include "simpleton4disasm.h"

namespace Simpleton
{

/*

static const char *NameCmds[] = {
"addis",
"addi ",
"adds ",
"add  ",
"adc  ",
"sub  ",
"sbc  ",
"and  ",
"or   ",
"xor  ",
"cadd ",
"rrci ",
"rrc  " };

static const char *NameRegs[] = {
"r0",
"r1",
"r2",
"r3",
"r4",
"sp",
"pc",
"pw",
"[ r0 ]",
"[ r1 ]",
"[ r2 ]",
"[ r3 ]",
"[ r4 ]",
"[ sp ]",
"[ pc ]",
"[ pw ]"
};

std::string Machine::operandToStr( mTag r, mTag i, int &addr, bool result )
{
    std::stringstream ss;
    if ( result && (i == 1) && (r == REG_PC) )
    {
        ss << "void";
    }
    else if ( (i == 1) && (r == REG_PC) )
    {
        ss << "$" << std::uppercase << std::hex << mem[ addr++ ];
    }
    else if ( (i == 1) && (r == REG_PSW) )
    {
        ss << "[ $" << std::uppercase << std::hex << std::setw( 4 ) << std::setfill( '0' ) << mem[ addr++ ] << " ]";
    }
    else
    {
        ss << NameRegs[ i * 8 + r ];
    }
    return ss.str();
};

void Machine::showDisasm( int addr )
{
    Instruction instr;
    std::string sr, sy, sx;
    std::cout << std::uppercase << std::hex << std::setw( 4 ) << std::setfill( '0' ) << addr  << ": ";
    instr.decode( mem[ addr++ ] );
    std::cout << NameCmds[ instr.cmd ] << " ";
    if ( instr.isInplaceImmediate( instr.cmd ) )
    {
        sx = std::to_string( (int) ((instr.xi == 1) ? instr.x - 8 : instr.x) );
    }
    else
    {
        sx = operandToStr( instr.x, instr.xi, addr );
    }
    sy = operandToStr( instr.y, instr.yi, addr );
    sr = operandToStr( instr.r, instr.ri, addr, true );
    std::cout << sr << " " << sy << " " << sx << "\n";
};

*/

template <typename I>
static std::string num2hex( I w, size_t hex_len = sizeof( I ) << 1 ) {
    static const char *digits = "0123456789ABCDEF";
    std::string rc( hex_len, '0' );
    for (size_t i = 0, j = 4 * ( hex_len - 1 ); i < hex_len; ++i, j -= 4 )
        rc[ i ] = digits[ (w >> j) & 0x0f ];
    return rc;
}

std::string simpleDump( CPU &cpu )
{
    std::string res;
    for ( int i = 0; i < 8; i++ )
    {
        if ( i == REG_SP )
            res += "SP";
        else if ( i == REG_PC )
            res += "PC";
        else if ( i == REG_PSW )
            res += "PW";
        else
            res += "R" + std::to_string( i );
        res += ":" + num2hex( cpu.getReg( i ) ) + " ";
    };
    res += (cpu.getFlag( FLAG_IRQ_ENABLE )	? "I" : "-" );
    res += (cpu.getFlag( FLAG_HALT )        ? "H" : "-" );
    res += (cpu.getFlag( FLAG_DEBUG )       ? "D" : "-" );
    res += ":";
    res += (cpu.getFlag( FLAG_OVERFLOW )	? "V" : "-" );
    res += (cpu.getFlag( FLAG_SIGN )        ? "S" : "-" );
    res += (cpu.getFlag( FLAG_CARRY )       ? "C" : "-" );
    res += (cpu.getFlag( FLAG_ZERO )        ? "Z" : "-" );
    res += "  CLCK " + std::to_string( cpu.getClocks() ) + "\n";
    return res;
}

}
