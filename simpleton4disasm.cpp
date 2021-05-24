#include "simpleton4disasm.h"

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

void Machine::show( int memStart )
{
    for ( int i = 0; i < 8; i++ )
    {
        if ( i == REG_SP )
            std::cout << "SP";
        else if ( i == REG_PC )
            std::cout << "PC";
        else if ( i == REG_PSW )
            std::cout << "PW";
        else
            std::cout << "R" << i;
        std::cout << ":" << std::uppercase << std::hex << std::setw( 4 ) << std::setfill( '0' ) << reg[ i ];
        //if ( i != 7 )
            std::cout << "  ";
    };
    std::cout << (getFlag( FLAG_OVERFLOW )	? "V" : "-" );
    std::cout << (getFlag( FLAG_SIGN )	? "S" : "-" );
    std::cout << (getFlag( FLAG_CARRY )	? "C" : "-" );
    std::cout << (getFlag( FLAG_ZERO )	? "Z" : "-" );
    std::cout << "  CLCK " << std::dec << clocks << "\n";
    if ( memStart >= 0 )
    {
        mWord start = memStart;
        int cols = 8;
        int rows = 16;
        for ( int y = 0; y < rows; y++ )
        {
            for ( int x = 0; x < cols; x++ )
            {
                if ( x )
                    std::cout << "  ";
                mWord cell = start + y + x * rows;
                std::cout << std::hex << std::setw( 4 ) << std::setfill( '0' ) << cell  << ":";
                std::cout << std::hex << std::setw( 4 ) << std::setfill( '0' ) << mem[ cell ];
            };
            std::cout << "\n";
        };
    }
}

*/
