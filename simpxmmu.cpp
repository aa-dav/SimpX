#include "simpxmmu.h"

namespace Simpleton
{

void Simpleton::SimpXMMU::reset()
{
    for ( int i = 0; i < 8; i++ )
        pages[ i ] = i;
    for ( int i = 0; i < 256; i++ )
        palette[ i ] = 0;
    for ( unsigned int i = 0; i < pagesCount * pageSize; i++ )
        mem[ i ] = 0;
}

mWord Simpleton::SimpXMMU::read(mWord addr)
{
    if ( addr >= portStart ) // IO ports
    {
        if ( addr >= mmuPage0 )
        {
            return pages[ addr - mmuPage0 ];
        }
        else if ( addr == vidPalData )
        {
            return palette[ mem[ vidPalPtr ] & 0xFF ];
        }
        {
            return mem[ addr ]; // direct read
        }
    }
    else
    {
        return getMem( addr );
    }
}

void Simpleton::SimpXMMU::write(mWord addr, mWord data)
{
    if ( addr >= portStart ) // IO ports
    {
        if ( addr >= mmuPage0 )
        {
            pages[ addr - mmuPage0 ] = data % pagesCount;
        }
        else if ( addr == vidPalData )
        {
            palette[ mem[ vidPalPtr ] & 0xFF ] = data;
        }
        else
            mem[ addr ] = data; // direct write
    }
    else
    {
        getMem( addr ) = data;
    }
}

mWord *Simpleton::SimpXMMU::getPtr(uint32_t addr)
{
    return &mem[ addr ];
}

}
