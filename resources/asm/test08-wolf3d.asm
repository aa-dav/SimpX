#include "simple_lib.inc"
@core
@2A
 r1 <- [ sp ]
 r0 <- [ sp ]
 call r0_mul_r1
 [ sp ] <- r0
 pc <- [ r4 ]
@2F
 r1 <- [ sp ]
 r0 <- [ sp ]
 call r0_div_r1
 [ sp ] <- r0
 pc <- [ r4 ]
@3D
 r0 = [ sp ] - [ sp ]
 jz @eq
 [ sp ] <= 0
 pc <- [ r4 ]
@eq
 [ sp ] <= $FFFF
 pc <- [ r4 ]
@3E
 r1 <- [ sp ]
 r0 <= [ sp ]
 [ sp ] <- r1
 [ sp ] <- r0
@3C
 r0 = [ sp ] - [ sp ]
 r0 = r0 & $8000
 jz @ge
 [ sp ] <= $FFFF
 pc <- [ r4 ]
@ge
 [ sp ] <= 0
 pc <- [ r4 ]
start
 call initGraphicMode
 r4 <- $7000

 pc <- @00000000
@707574706978656C
 r4 <- r4 + 1
 r0 <= [ sp ]
 r1 = [ sp ] >> 8
 r1 = r1 + $8000
 r1 = r1 + [ sp ]
 [ r1 ] <- r0 
 r1 = r1 + $40
 [ r1 ] <- r0 
 r1 = r1 + $40
 [ r1 ] <- r0 
 r1 = r1 + $40
 [ r1 ] <- r0 
 r4 <- r4 - 1
 pc <- [ r4 ]
@6B6579
 r4 <- r4 + 1
 [ LastKey ] <- 0
 call inputChar
 [ sp ] <= r0
 r4 <- r4 - 1
 pc <- [ r4 ]
@00000000


 pc <- @FFFFFFFF
@727368696674
 r4 <- r4 + 1
@FFFFFFFE
 r0 <= [ sp ]
 r0 <- r0 - 1
 [ sp ] <- r0
 jz @FFFFFFFD

 r1 <- [ sp ]
 r0 <= [ sp ]
 [ sp ] <- r1
 [ sp ] <- r0
 r0 <= [ sp ]
 r1 = r0 & $8000
 r0 = r0 & $FFFE
 r0 <= r0 >> 1
 [ sp ] = r0 | r1
 r1 <- [ sp ]
 r0 <= [ sp ]
 [ sp ] <- r1
 [ sp ] <- r0
 pc <- @FFFFFFFE
@FFFFFFFD
 r0 <= [ sp ]

 r4 <- r4 - 1
 pc <- [ r4 ]
@4C6F6358
 [ sp ] <= $00006000
 pc <- [ r4 ]
@4C6F6359
 [ sp ] <= $00006001
 pc <- [ r4 ]
@416E676C65
 [ sp ] <= $00006002
 pc <- [ r4 ]
@636F6C6F72
 [ sp ] <= $00006003
 pc <- [ r4 ]
@78
 [ sp ] <= $00006004
 pc <- [ r4 ]
@79
 [ sp ] <= $00006005
 pc <- [ r4 ]
@68
 [ sp ] <= $00006006
 pc <- [ r4 ]
@6A31
 [ sp ] <= $00006007
 pc <- [ r4 ]
@6A32
 [ sp ] <= $00006008
 pc <- [ r4 ]
@6930
 [ sp ] <= $00006009
 pc <- [ r4 ]
@6A30
 [ sp ] <= $0000600A
 pc <- [ r4 ]
@7530
 [ sp ] <= $0000600B
 pc <- [ r4 ]
@7630
 [ sp ] <= $0000600C
 pc <- [ r4 ]
@6461
 [ sp ] <= $0000600D
 pc <- [ r4 ]
@6462
 [ sp ] <= $0000600E
 pc <- [ r4 ]
@61
 [ sp ] <= $0000600F
 pc <- [ r4 ]
@62
 [ sp ] <= $00006010
 pc <- [ r4 ]
@75
 [ sp ] <= $00006011
 pc <- [ r4 ]
@76
 [ sp ] <= $00006012
 pc <- [ r4 ]
@7531
 [ sp ] <= $00006013
 pc <- [ r4 ]
@7631
 [ sp ] <= $00006014
 pc <- [ r4 ]
@6131
 [ sp ] <= $00006015
 pc <- [ r4 ]
@6231
 [ sp ] <= $00006016
 pc <- [ r4 ]
@69
 [ sp ] <= $00006017
 pc <- [ r4 ]
@6A
 [ sp ] <= $00006018
 pc <- [ r4 ]
@6469
 [ sp ] <= $00006019
 pc <- [ r4 ]
@646A
 [ sp ] <= $0000601A
 pc <- [ r4 ]
@77616C6C
 [ sp ] <= $0000601B
 pc <- [ r4 ]
@4D6170
 [ sp ] <= $0000601C
 pc <- [ r4 ]
@436F735461626C65
 [ sp ] <= $00006029
 pc <- [ r4 ]
@5749445448
 r4 <- r4 + 1
 [ sp ] <= $00000040
 r4 <- r4 - 1
 pc <- [ r4 ]
@484549474854
 r4 <- r4 + 1
 [ sp ] <= $00000030
 r4 <- r4 - 1
 pc <- [ r4 ]
@42495453
 r4 <- r4 + 1
 [ sp ] <= $00000005
 r4 <- r4 - 1
 pc <- [ r4 ]
@53544550
 r4 <- r4 + 1
 [ sp ] <= $00000020
 r4 <- r4 - 1
 pc <- [ r4 ]
@4D41534B
 r4 <- r4 + 1
 [ sp ] <= $0000001F
 r4 <- r4 - 1
 pc <- [ r4 ]
@6F766572
 r4 <- r4 + 1
 [ sp ] <= $00000001
 r0 <- sp
 r0 = r0 + [ sp ]
 r0 = r0 + 1
 [ sp ] <= [ r0 ]
 r4 <- r4 - 1
 pc <- [ r4 ]
@616273
 r4 <- r4 + 1
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ sp ] <= $0000000F
 [ r4 ] <- pc + 2
 pc <- @727368696674
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ sp ] <= $00000001
 [ sp ] = [ sp ] + [ sp ]
 [ r4 ] <- pc + 2
 pc <- @2A
 r4 <- r4 - 1
 pc <- [ r4 ]
@636F7A
 r4 <- r4 + 1
 [ r4 ] <- pc + 2
 pc <- @616273
 [ sp ] <= $00000008
 [ r4 ] <- pc + 2
 pc <- @2A
 [ sp ] <= $00000040
 [ sp ] = [ sp ] - [ sp ]
 [ r4 ] <- pc + 2
 pc <- @616273
 [ sp ] <= $00000002
 [ r4 ] <- pc + 2
 pc <- @2A
 [ sp ] <= $00000040
 [ sp ] = [ sp ] - [ sp ]
 r4 <- r4 - 1
 pc <- [ r4 ]
@636F73
 r4 <- r4 + 1
 [ r4 ] <- pc + 2
 pc <- @416E676C65
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @436F735461626C65
 [ sp ] = [ sp ] + [ sp ]
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r4 <- r4 - 1
 pc <- [ r4 ]
@73696E
 r4 <- r4 + 1
 [ r4 ] <- pc + 2
 pc <- @416E676C65
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] <= $00000004
 [ sp ] = [ sp ] - [ sp ]
 [ sp ] <= $0000000F
 [ sp ] = [ sp ] & [ sp ]
 [ r4 ] <- pc + 2
 pc <- @436F735461626C65
 [ sp ] = [ sp ] + [ sp ]
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r4 <- r4 - 1
 pc <- [ r4 ]
@7370616E
 r4 <- r4 + 1
 [ r4 ] <- pc + 2
 pc <- @636F6C6F72
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @78
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @484549474854
 r0 <- sp
 [ sp ] <= [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @68
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
@FFFFFFFC
 r0 <= [ sp ]
 r0 <- r0 - 1
 [ sp ] <- r0
 jz @FFFFFFFB

 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @79
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @2A
 [ r4 ] <- pc + 2
 pc <- @6231
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @3C
 r0 <= [ sp ]
 jz @FFFFFFFA

 [ r4 ] <- pc + 2
 pc <- @68
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ sp ] <= $00000000
@FFFFFFFA

 pc <- @FFFFFFFC
@FFFFFFFB
 r0 <= [ sp ]

 [ r4 ] <- pc + 2
 pc <- @68
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
 [ sp ] <= $00000001
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ r4 ] <- pc + 2
 pc <- @6A31
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6A31
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @68
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6A32
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6A31
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
@FFFFFFF9
 r0 <= [ sp ]
 r0 <- r0 - 1
 [ sp ] <- r0
 jz @FFFFFFF8

 [ r4 ] <- pc + 2
 pc <- @78
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6F766572
 [ sp ] <= $00000000
 [ r4 ] <- pc + 2
 pc <- @707574706978656C
 pc <- @FFFFFFF9
@FFFFFFF8
 r0 <= [ sp ]

 [ r4 ] <- pc + 2
 pc <- @68
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
@FFFFFFF7
 r0 <= [ sp ]
 r0 <- r0 - 1
 [ sp ] <- r0
 jz @FFFFFFF6

 [ r4 ] <- pc + 2
 pc <- @78
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6F766572
 [ r4 ] <- pc + 2
 pc <- @6A31
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ r4 ] <- pc + 2
 pc <- @636F6C6F72
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @707574706978656C
 pc <- @FFFFFFF7
@FFFFFFF6
 r0 <= [ sp ]

 [ r4 ] <- pc + 2
 pc <- @6A32
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
@FFFFFFF5
 r0 <= [ sp ]
 r0 <- r0 - 1
 [ sp ] <- r0
 jz @FFFFFFF4

 [ r4 ] <- pc + 2
 pc <- @78
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6F766572
 [ r4 ] <- pc + 2
 pc <- @6A32
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ sp ] <= $00002222
 [ r4 ] <- pc + 2
 pc <- @707574706978656C
 pc <- @FFFFFFF5
@FFFFFFF4
 r0 <= [ sp ]

 r4 <- r4 - 1
 pc <- [ r4 ]
@7363656E65
 r4 <- r4 + 1
 [ r4 ] <- pc + 2
 pc <- @4C6F6358
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @42495453
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ r4 ] <- pc + 2
 pc <- @6930
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @4D41534B
 [ sp ] = [ sp ] & [ sp ]
 [ r4 ] <- pc + 2
 pc <- @7530
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @4C6F6359
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @42495453
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ r4 ] <- pc + 2
 pc <- @6A30
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @4D41534B
 [ sp ] = [ sp ] & [ sp ]
 [ r4 ] <- pc + 2
 pc <- @7630
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @73696E
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ sp ] <= $00000003
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ r4 ] <- pc + 2
 pc <- @6461
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @636F73
 [ sp ] <= $00000001
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ sp ] = [ sp ] + [ sp ]
 [ sp ] <= [ sp ] >> 13
 [ r4 ] <- pc + 2
 pc <- @62
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @636F73
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ sp ] <= $00000003
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ r4 ] <- pc + 2
 pc <- @6462
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @73696E
 [ sp ] <= $00000001
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ sp ] = [ sp ] - [ sp ]
 [ sp ] <= [ sp ] >> 13
 [ r4 ] <- pc + 2
 pc <- @61
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @5749445448
@FFFFFFF3
 r0 <= [ sp ]
 r0 <- r0 - 1
 [ sp ] <- r0
 jz @FFFFFFF2

 [ r4 ] <- pc + 2
 pc <- @61
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] <= $00000000
 [ r4 ] <- pc + 2
 pc <- @3C
 r0 <= [ sp ]
 jz @FFFFFFF1

 [ sp ] <= $0000FFFF
 [ r4 ] <- pc + 2
 pc <- @7530
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] <= $00000000
 [ r4 ] <- pc + 2
 pc <- @61
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
 pc <- @FFFFFFF0
@FFFFFFF1

 [ sp ] <= $00000001
 [ r4 ] <- pc + 2
 pc <- @53544550
 [ r4 ] <- pc + 2
 pc <- @7530
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
 [ r4 ] <- pc + 2
 pc <- @61
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
@FFFFFFF0

 [ r4 ] <- pc + 2
 pc <- @6131
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @75
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6469
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @62
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] <= $00000000
 [ r4 ] <- pc + 2
 pc <- @3C
 r0 <= [ sp ]
 jz @FFFFFFEF

 [ sp ] <= $0000FFFF
 [ r4 ] <- pc + 2
 pc <- @7630
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] <= $00000000
 [ r4 ] <- pc + 2
 pc <- @62
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
 pc <- @FFFFFFEE
@FFFFFFEF

 [ sp ] <= $00000001
 [ r4 ] <- pc + 2
 pc <- @53544550
 [ r4 ] <- pc + 2
 pc <- @7630
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
 [ r4 ] <- pc + 2
 pc <- @62
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
@FFFFFFEE

 [ r4 ] <- pc + 2
 pc <- @6231
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @76
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @646A
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @75
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @53544550
 [ sp ] = [ sp ] - [ sp ]
 [ r4 ] <- pc + 2
 pc <- @78
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6231
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ sp ] <= [ sp ] >> 11
 [ r4 ] <- pc + 2
 pc <- @7531
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @2A
 [ r4 ] <- pc + 2
 pc <- @75
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @76
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @53544550
 [ sp ] = [ sp ] - [ sp ]
 [ r4 ] <- pc + 2
 pc <- @79
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6131
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ sp ] <= [ sp ] >> 11
 [ r4 ] <- pc + 2
 pc <- @7631
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @2A
 [ r4 ] <- pc + 2
 pc <- @76
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6930
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @69
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6A30
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6A
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ sp ] <= $00000001
@FFFFFFED
 r0 <= [ sp ]
 jz @FFFFFFEC

 [ r4 ] <- pc + 2
 pc <- @75
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @76
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @3C
 r0 <- sp
 [ sp ] <= [ r0 ]
 r0 <= [ sp ]
 jz @FFFFFFEB

 [ r4 ] <- pc + 2
 pc <- @78
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @53544550
 [ sp ] = [ sp ] + [ sp ]
 [ r4 ] <- pc + 2
 pc <- @78
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @76
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @75
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
 [ r4 ] <- pc + 2
 pc <- @76
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @7531
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @75
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @69
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6469
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ r4 ] <- pc + 2
 pc <- @69
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 pc <- @FFFFFFEA
@FFFFFFEB

 [ r4 ] <- pc + 2
 pc <- @79
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @53544550
 [ sp ] = [ sp ] + [ sp ]
 [ r4 ] <- pc + 2
 pc <- @79
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @75
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @76
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
 [ r4 ] <- pc + 2
 pc <- @75
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @7631
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @76
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6A
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @646A
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ r4 ] <- pc + 2
 pc <- @6A
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
@FFFFFFEA

 [ r4 ] <- pc + 2
 pc <- @77616C6C
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @69
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @4D6170
 [ sp ] = [ sp ] + [ sp ]
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6A
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ sp ] <= $00000001
 [ sp ] = [ sp ] & [ sp ]
 pc <- @FFFFFFED
@FFFFFFEC

 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @77616C6C
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 r0 <= [ sp ]
 jz @FFFFFFE9

 [ r4 ] <- pc + 2
 pc <- @6131
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6231
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @78
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @79
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
@FFFFFFE9

 [ r4 ] <- pc + 2
 pc <- @69
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6A
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ sp ] <= $00000003
 [ sp ] = [ sp ] & [ sp ]
 r0 <= [ sp ]
 jz @FFFFFFE8

 [ sp ] <= $00001111
 pc <- @FFFFFFE7
@FFFFFFE8

 [ sp ] <= $00004444
@FFFFFFE7

 [ r4 ] <- pc + 2
 pc <- @7370616E
 [ r4 ] <- pc + 2
 pc <- @61
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6461
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ r4 ] <- pc + 2
 pc <- @61
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @62
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6462
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] - [ sp ]
 [ r4 ] <- pc + 2
 pc <- @62
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 pc <- @FFFFFFF3
@FFFFFFF2
 r0 <= [ sp ]

 r4 <- r4 - 1
 pc <- [ r4 ]
@72756E
 r4 <- r4 + 1
 [ r4 ] <- pc + 2
 pc <- @4C6F6358
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @636F73
 [ sp ] <= $00000004
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ sp ] = [ sp ] + [ sp ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @78
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @42495453
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ r4 ] <- pc + 2
 pc <- @69
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @4C6F6359
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @73696E
 [ sp ] <= $00000004
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ sp ] = [ sp ] + [ sp ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @79
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @42495453
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ r4 ] <- pc + 2
 pc <- @6A
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @69
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @4D6170
 [ sp ] = [ sp ] + [ sp ]
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @6A
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @727368696674
 [ sp ] <= $00000001
 [ sp ] = [ sp ] & [ sp ]
 r0 <= [ sp ]
 jz @FFFFFFE6

 [ r4 ] <- pc + 2
 pc <- @78
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @4C6F6358
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @79
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @4C6F6359
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @7363656E65
@FFFFFFE6

 r4 <- r4 - 1
 pc <- [ r4 ]
@7475726E
 r4 <- r4 + 1
 [ r4 ] <- pc + 2
 pc <- @416E676C65
 r0 <= [ sp ]
 [ sp ] <- [ r0 ]
 [ sp ] = [ sp ] + [ sp ]
 [ sp ] <= $0000000F
 [ sp ] = [ sp ] & [ sp ]
 [ r4 ] <- pc + 2
 pc <- @416E676C65
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @7363656E65
 r4 <- r4 - 1
 pc <- [ r4 ]
@6D61696E
 r4 <- r4 + 1
 [ sp ] <= $00000000
 [ sp ] <= $00003FFE
 [ sp ] <= $00002AAA
 [ sp ] <= $00003FFE
 [ sp ] <= $00003C9E
 [ sp ] <= $00003DDE
 [ sp ] <= $00003F7E
 [ sp ] <= $00003DDE
 [ sp ] <= $00003C9E
 [ sp ] <= $00003FFE
 [ sp ] <= $00002AAA
 [ sp ] <= $00003FFE
 [ sp ] <= $00000000
 [ sp ] <= $0000000D
@FFFFFFE5
 r0 <= [ sp ]
 r0 <- r0 - 1
 [ sp ] <- r0
 jz @FFFFFFE4

 r1 <- [ sp ]
 r0 <= [ sp ]
 [ sp ] <- r1
 [ sp ] <- r0
 [ r4 ] <- pc + 2
 pc <- @6F766572
 [ r4 ] <- pc + 2
 pc <- @4D6170
 [ sp ] = [ sp ] + [ sp ]
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 pc <- @FFFFFFE5
@FFFFFFE4
 r0 <= [ sp ]

 [ sp ] <= $00000010
@FFFFFFE3
 r0 <= [ sp ]
 r0 <- r0 - 1
 [ sp ] <- r0
 jz @FFFFFFE2

 r0 <- sp
 [ sp ] <= [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @636F7A
 r1 <- [ sp ]
 r0 <= [ sp ]
 [ sp ] <- r1
 [ sp ] <- r0
 [ r4 ] <- pc + 2
 pc <- @436F735461626C65
 [ sp ] = [ sp ] + [ sp ]
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 pc <- @FFFFFFE3
@FFFFFFE2
 r0 <= [ sp ]

 [ sp ] <= $00000004
 [ r4 ] <- pc + 2
 pc <- @416E676C65
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ sp ] <= $00000028
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ r4 ] <- pc + 2
 pc <- @4C6F6358
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @4C6F6359
 r0 <- [ sp ]
 [ r0 ] <- [ sp ]
 [ r4 ] <- pc + 2
 pc <- @7363656E65
 [ sp ] <= $00000001
@FFFFFFE1
 r0 <= [ sp ]
 jz @FFFFFFE0

 [ r4 ] <- pc + 2
 pc <- @6B6579
 r0 <- sp
 [ sp ] <= [ r0 ]
 r0 <- sp
 [ sp ] <= [ r0 ]
 [ sp ] <= $00000077
 [ r4 ] <- pc + 2
 pc <- @3D
 r0 <= [ sp ]
 jz @FFFFFFDF

 [ r4 ] <- pc + 2
 pc <- @72756E
@FFFFFFDF

 [ sp ] <= $00000061
 [ r4 ] <- pc + 2
 pc <- @3D
 r0 <= [ sp ]
 jz @FFFFFFDE

 [ sp ] <= $0000FFFF
 [ r4 ] <- pc + 2
 pc <- @7475726E
@FFFFFFDE

 [ sp ] <= $00000064
 [ r4 ] <- pc + 2
 pc <- @3D
 r0 <= [ sp ]
 jz @FFFFFFDD

 [ sp ] <= $00000001
 [ r4 ] <- pc + 2
 pc <- @7475726E
@FFFFFFDD

 [ sp ] <= $00000001
 pc <- @FFFFFFE1
@FFFFFFE0

 r4 <- r4 - 1
 pc <- [ r4 ]
@FFFFFFFF
 [ r4 ] <- pc + 2
 pc <- @6D61696E

@end
