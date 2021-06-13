#ifndef SIMP_X_H
#define SIMP_X_H

#include "simpleton4.h"
#include <vector>
#include <QImage>

#define PPU_SOFT_RENDER 1
#define BUILD_WEBASSEMBLY 1

namespace Simpleton
{

class SimpXMMU : public MMU
{
private:
    unsigned int pagesCount;
    std::vector< mWord > mem;
    mWord pages[ 8 ];
    mWord pal16[ 256 ];
    uint32_t pal32[ 256 ];

    // MMU interface
public:
    static const unsigned int bitsInPage = 3;
    static const unsigned int bitsInAddr = 16 - bitsInPage;
    static const unsigned int pageSize = 1 << bitsInAddr;
    static const unsigned int addrMask = pageSize - 1;
    static const unsigned int pageMask = (1 << bitsInPage) - 1;

    constexpr static mWord getPageNum( mWord addr )
    {
        return (addr >> bitsInAddr) & pageMask;
    }
    mWord &getMem( mWord addr )
    {
        size_t idx = (pages[ getPageNum( addr ) ] << bitsInAddr) | (addr & addrMask);
        if ( idx >= mem.size() )
        {
            return mem[ 0 ];
        }
        return mem[ idx ];
    }
    const mWord    *getPalPtr16() { return pal16; };
    const uint32_t *getPalPtr32() { return pal32; };

    static const mWord portStart        = 0xFFE0;
    // Video IO ports
    static const mWord vidFlags         = 0xFFF0;
    static const mWord vidBitmapPage    = 0xFFF1;
    static const mWord vidCharmapPage   = 0xFFF2;
    static const mWord vidCharmapAddr   = 0xFFF3;
    static const mWord vidScrollX       = 0xFFF4;
    static const mWord vidScrollY       = 0xFFF5;
    static const mWord vidPalPtr        = 0xFFF6;
    static const mWord vidPalData       = 0xFFF7;
    // MMU IO ports
    static const mWord mmuPage0         = 0xFFF8;
    static const mWord mmuPage1         = 0xFFF9;
    static const mWord mmuPage2         = 0xFFFA;
    static const mWord mmuPage3         = 0xFFFB;
    static const mWord mmuPage4         = 0xFFFC;
    static const mWord mmuPage5         = 0xFFFD;
    static const mWord mmuPage6         = 0xFFFE;
    static const mWord mmuPage7         = 0xFFFF;

    SimpXMMU( int _pagesCount ): pagesCount( _pagesCount ), mem( pageSize * _pagesCount, 0 ) {};
    ~SimpXMMU() {};
    void reset() override;
    mWord read(mWord addr) override;
    void write(mWord addr, mWord data) override;
    mWord *getPtr(uint32_t addr) override;
    mWord getPagesCount() { return pagesCount; };
};

class SimpX
{
    SimpXMMU mmu;
    CPU cpu;
    uint64_t clocks;
public:
    SimpX( int _pages ): mmu( _pages ), cpu( mmu ) {}
    SimpXMMU &getMMU() { return mmu; }
    CPU &getCPU() { return cpu; }
    uint64_t getClocks() { return clocks; }
    void stepFrame( mWord *buf16, uint32_t *buf32 );;
    void reset()
    {
        mmu.reset();
        cpu.reset();
        clocks = 0;
    };
};

};  // namespace Simpleton

#endif // SIMPX_H
