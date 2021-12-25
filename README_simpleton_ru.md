# Simpleton 4

��������� ����� ������� 8bit-guy ������� ��� 'Gigatron TTL' ���������� ���� ��������� ��������� ������� ������������ ����������� (ISA).
��� �������� � ��������� ���������� ������������ ���� � �� �� ����.

�� ������, ��� ISA �������������.

**������� ���� - ��� ������� ������ ���������� ��� ����� ����� ������� �������� ���������������� ���� �������, �� ������ � �� �������������.**

� ����, ��� ��������� ����� ����� �������� � ���������� ��� �� ��������� MSP 430, �� ��� �� �������. :)

�� ������, ��� �������� � �������� � ������ ������ 16-�����.
������� ������ ��������� r0-r7 � 128�� ��� 65536 ����� 16-������ ������.

������� ����� ���������� ���� 16-�����. � ���� ������ ���� ������ ����������:
![Instruction format picture](https://cdn.jpg.wtf/futurico/95/66/1613283583-9566e916e4b56fca243a37105c20898d.png)

������ ���������� ������ ������ ���� ����: ���� ��� �������� X � Y, ���������� �� � ��� (����������-���������� ����������) � ����� ���������� � ������� ��������� ���������� ��� � R. ���� ������ �������� ��� �������� �������� ������ ������ ���.
���� X, Y � R � ���������� ��� ������ ������ ��������� 0-7. ���� ���� ����������� (XI, YI, RI) ������� ���� �����������, �� ������ �� �������� � ������� ������ � ������� �� ���������������� ��������. ������, ��� ������ 4:4:4:4 ��������� ����� ��������� ����� ���������� ����� �� � 16-������� ����.
����� ������� ����� ���������� ��������� ���������: R = Y op X, ��� 'op' ��� ��� ��������. ���������� ������ ������ ������ �� ��������� �������� ����� ����������� (��� ����������� = 1).


�������� **r5-r7** ����� ���������� � ������ ����������:
- **r5 = sp** - ��������� ����� - ����-���������������� ����� ���������� ������ � ���-���������������� ����� ��������� ������.
- **r6 = pc** - ������� ���������� - ������ ����-���������������� ����� ���������� ������. ������ �� ���� ��������� ��������� ����� ������. ��������� ������ � pc �� ����� ������ � ��������� �������� ����� ������. �.�. ��� ��������� ��������, ��������� ������� ������, �� ��������� ������ �� ������������. � ����� ������ ����� ������������ ��� ��������� 'void'.
- **r7 = psw** = processor status word (������� ������). ��������� ������/������ � ���� �����������, �� ����� ���������� ������� ����� � ��� �������� ��� ����� ��������� '���������������� �����' ('immediate indirect') (������ �� [ pc++ ] ������ ��� ����� ������ ������ ������ ������� �������� ��� ���� ������������ ���������).

���������� ���������������� ������ (�������� ���������) � X ��� Y ����������� ��� ��������� ������ �� pc, �.�. ����� ������ ������ �� ������������� ���������� (��� ������ ��� ������) � ����� ��������� �� ��������� �����/���������� � ������.
�� ����� ���������� ���������� �������� � ��������� ����������� �� [ pc++ ] (���� ����) � ��������� �������: X, Y, R. ��� ������ ����� �������� � ����� ���������� ��� R = Y + X.
������ ������ ������ (0-3) ���������� �������� ���� XI+X ��� ������������ 'inplace immediate' - ��������� ���������� � ���������� -8..+7 ��� 0..15. � ������������ ��������� ����� ���������� ����� ������� (�������� ���� ��) 'i'.

����, ������� ��� ���������� ��������� ���������� � ������������ ������������ ����������:
```
add r0 r1 r2		; �������� ���������: r0 = r1 + r2
adc r1 r1 r3		; �������� � ������ ����� ��������: r1 = r1 +c r3
add [ r2 ] 100 r4		; ������� r4 � ���������� ( ����� ��������� [ pc ] ) 100 � �������� � ������ ������ � ������� r2: [ r2 ] = 100 + r4
add [ r3 ] r4 [ $200 ]		; ����� �� ������ ������ � ������� $200 (16-������ �����/�����) �����, ������� � r4 � �������� � [ r3 ]: [ r3 ] = r4 + [ $200 ]
add [ 100 ] [ 20 ] [ 30 ]	; ������ ��� ��������� �������� � �������� ������ � ���������� ��������: [ 100 ] = [ 20 ] + [ 30 ]
```
��������, ��� � ���������� ����� ���������� ��� ������� ����� �����������, ������� ���� �������� �������� ������� ���������� ������� ����������, �� ��� ���� ��������� � ������� ������:
```
add r0 r0 (label + 4 * offset)	; � ������� ����� ���� ������� ��������� ���������� �� ����� ����������
```

��� ������� ����� �������� 16 ����� �������� ��������. ������ ����������� ���������:
- 00 **ADDIS** - ������� Y � inplace immediate � X (-8..+7) ��� ���������� ������ (S - silent, �.�. ��� ���������� ������)
- 01 **ADDI** - ������� Y � inplace immediate � X (-8..+7)
- 02 **RRCI** - ��������� Y ������ (� ������� ���������� ������ ��������� ��� '����������� �����') �� INPLACE immediate (0..15) ���, �� ���� �������� ��������� ��������� ����������� ���

- 04 **ADDS** - �������� ��� ���������� ������ (silent)
- 05 **ADD** - ��������
- 06 **ADC** - �������� � ������ ����� ��������
- 07 **SUB** - ���������
- 08 **SBC** - ��������� � ������ ����� ��������
- 09 **AND** - and
- 0A **OR** - or
- 0B **XOR** - xor
- 0C **CADD** - �������� ��������. �� ��������� �����. ��. ����.
- 0D **RRC** - ��������� Y ������ �� X ���, �� ���� �������� ��������� ��������� ����������� ���

## ���������:

1. There is no separate opcode for 'move' because it's 'addis' with 0 in field X of opcode.
But for simplicity assembler hase 'move' instruction which is shortcut for 'addis R Y 0'.
Inplace immediate in X could be omitted in assembler syntax:
```
addis r0 r1 1 ; adding 1 to r1 and placing result to r0
addis r0 r1 0 ; adding 0 to r1 and placing result (that is r1) to r0
move [ r2 ] r1 		; Move from r1 to memory cell r2 points to (shortcut for addis [ r2 ] r1 0)
move [ r3 ] [ 100 ]	; Move from memory cell with address 100 to r3
```
2. writing immediate in pc is jump and adding (silent) pc with immediate is relative jump:
```
move pc address		; jump
adds pc pc offset	; relative jump
```
3. conditional add is the key to the conditional branching, it works in this way: X argument (16 bit) is decoupled in two parts: upper 4 bits are conditional code and lower 12 bits are sign-extended to 16 bit of -2048..+2047 addendum. That is condition code is not part of opcode, but part of data! If condition is false - ALU skips addition and returns Y without changes.
So, to implement conditional branching we just do:
```
cadd pc pc condition_with_offset
```
Assembler provides automatic offset calculation for labels of course.
For simplicity alternate syntax is supported (jz, jnz, jc, jnc and so on):
```
jnz label
```
Possible conditional codes and their mnemonics:
```
z, nz, c, nc, s, ns, o, no - test of corresponding flag (n* for 'NOT set')
a  - unsigned 'above'
be - unsigned 'below or equal'
ge - signed 'greater or equal'
l  - signed 'less'
g  - signed 'greater'
le - signed 'less or equal'
```
4. CALL may be implemented as:
```
addis sp pc 2 		; precalculate return address
move pc proc_address
```
There is shortcut for this in asssembler in usual form:
```
call proc_address
```
So, it's two instructions and 3 words. This is the most visible penalty of unified instruction format.

5. But RET is just:
```
move pc [ sp ] ; one-word addis instruction
```
6. ADDI could be used as move with updating flags (testing move, useful in loops like STRCPY).
```
StrCpy: 		; R1 - pointer to src, R2 - pointer to dst
  movet [ r2 ] [ r1 ] 	; movet is shortcut for addi x y 0 ; 'move with Test'
  jz Exit
  addis r1 r1 1		; increment r1
  addis r2 r2 1		; increment r2
  move pc StrCpy
Exit:
  ret 			; shortcut for move pc [ sp ]
```
7. Disabling/enabling interrupts is just simple as:
```
and psw psw flag_mask ; enable
or psw psw inv_flag_mask ; disable (inverse of flag bit)
```
Note, that ALU updates flags first and then write from ALU to R happens. So then psw as destination do not take into account changes of flags from ALU.

8.  Keyword 'void' in place of R means writing to [ PC ] - that is cancelation of writing result from ALU anywhere.
This allows to make non-destructing comparisons:
```                                              
sub void A B ; acts like 'cmp A B' in many other ISAs
jnz ...
```
or bit tests (of any kind):
```
and void r0 $0001
jz ...
```
...or comparison of number with constant in range -8..+7 via one-word instruction (inplace immediate:
```
addi void r0 -3
jz ... ; r0 is equal to 3
```
...or checking of i-th bit of operand via placing it in carry flag during RRCI instruction execution:
```
rrci void r0 3 ; CF gets bit 3
jc ... ; jump if CF=1
```

### New assembler syntax

Assembler instructions 'mode classic' and 'mode new' can switch assembler back and forth new 'math notation'.
Most of instructions could be expressed as 'R = Y op X' where 'op' is operation sign.
Next instructions fulfill this pattern (example for R=R0, Y=R1 and X=[ label ]):
```
04 - ADDS : r0 = r1 +s [ label ] ; add silent (doesn't update flags)
05 - ADD  : r0 = r1 +  [ label ] ; add
06 - ADC  : r0 = r1 +c [ lavel ] ; add with carry
07 - SUB  : r0 = r1 -  [ label ] ; sub
08 - SBC  : r0 = r1 -c [ label ] ; sub with carry
09 - AND  : r0 = r1 &  [ label ] ; and
0A - OR   : r0 = r1 |  [ label ] ; or
0B - XOR  : r0 = r1 ^  [ label ] ; xor
0C - CADD : r0 = r1 +? [ label ] ; conditional add. never updates flags.
0D - RRC  : r0 = r1 >> [ label ] ; rotate Y right (cyclic) by X bits
```
But there are 3 opcodes (right now) which fall out of this pattern and have special syntax:
```
00 - ADDIS : r0 <- r1 - 1         ; add Y with INPLACE immediate in XI+X SILENT (flags are not updated)
01 - ADDI  : r0 <= r1 + 3         ; add Y with INPLACE immediate in XI+X
02 - RRCI  : r0 <= r1 >> 15       ; rotate Y right (cyclic) by INPLACE immediate bits
```
First of all - it's 'inplace immediate' commands: addi, addis and rcci. These of them who updates flags use '<=' as sign of this special case. 
The only exceptions is 'addis' which uses '<-' to signal that it's not updates flags.
If X and op are omitted with <- it assumes + 0 and makes no need in special move instruction: r0 <- r1 is equal to r0 <- r1 + 0

Note, that 'addis a b -1' (negative inplace immediate) could be expressed in new syntax as 'a <- b + -1'. But also 'a <- b - 1' is correct (that is like 'sub' opcode). This is another reason for exclusion of addi(s) from regular '=' syntax.
Also, 'move' is as simple as: 'a <- b' which is shortcut for 'a <- b + 0'.

Note, that all pseudoops used for simplicity are still in place: 'jnz/call/ret' and so on.

