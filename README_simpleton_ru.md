# Simpleton 4

Некоторые видео ютубера 8bit-guy включая про 'Gigatron TTL' вдохновили меня придумать некоторую простую процессорную архитектуру (ISA).
Это четвёртое её поколение пытающееся использовать одну и ту же идею.

Во первых, эта ISA субоптимальна.

**Главная цель - это сделать формат инструкции как можно более простым сохраняя программирование тоже простым, но гибким и не эзотерическим.**

Я знаю, что плотность когда можно улучшить и получиться что то наподобие MSP 430, но это не вариант. :)

Во вторых, для простоты и регистры и ячейки памяти 16-битны.
Имеется восемь регистров r0-r7 и 128Кб или 65536 ячеек 16-битной памяти.

Поэтому опкод инструкции тоже 16-битен. И есть только один формат инструкции:  
![Instruction format picture](https://cdn.jpg.wtf/futurico/95/66/1613283583-9566e916e4b56fca243a37105c20898d.png)

Каждая инструкция делает только одну вещь: берёт два операнда X и Y, записывает их в АЛУ (арифметико-логическое устройство) с кодом инструкции и получив результат записывает его в R. Даже вызовы процедур или условные переходы делают только это.
Поля X, Y и R в инструкции это просто номера регистров 0-7. Плюс биты косвенности (XI, YI, RI) которые если установлены, то значит мы работаем с ячейкой памяти с адресом из соответствующего регистра. Замечу, что формат 4:4:4:4 позволяет легко считывать смысл инструкции прямо по её 16-ричному коду.
Таким образом любая инструкция выполняет выражение: R = Y op X, где 'op' это код операции. Квадратные скобки вокруг любого из операндов означают режим косвенности (бит косвенности = 1).


Регистры **r5-r7** имеют псевдонимы и особый функционал:
- **r5 = sp** - указатель стека - пост-инкрементируется после косвенного чтения и пре-декрементируется после косвенной записи.
- **r6 = pc** - счётчик инструкций - всегда пост-инкрементируется после косвенного чтения. Именно из него процессор считывает поток команд. Косвенная запись в pc не имеет смысла и процессор отменяет такую запись. Т.е. АЛУ исполняет операцию, обновляет регистр флагов, но результат никуда не записывается. В таком случае можно использовать еще псевдоним 'void'.
- **r7 = psw** = processor status word (регистр флагов). Косвенные чтения/запись в него бесмысленны, но такую комбинацию выбрать можно и она работает как режим адресации 'непосредственный адрес' ('immediate indirect') (данное из [ pc++ ] служит как адрес ячейки памяти откуда берется аргумент или куда записывается результат).

Технически непосредственные данные (числовые константы) в X или Y реализуются как косвенное чтение из pc, т.к. после такого чтения он автоматически увеличится (как всегда это делает) и будет указывать на следующее слово/инструкцию в памяти.
Во время исполнения инструкции операнды и результат считываются из [ pc++ ] (если надо) в следующем порядке: X, Y, R. Вот почему проще говорить о форме инструкции как R = Y + X.
Первые четыре опкода (0-3) инструкций трактуют поле XI+X как четырёхбитное 'inplace immediate' - константа встроенная в инструкцию -8..+7 или 0..15. В ассемблерной мнемонике такие инструкции имеют суффикс (возможно один из) 'i'.

Итак, давайте уже рассмотрим несколько инструкций в классическом ассемблерном синтаксисе:
```
add r0 r1 r2		; сложение регистров: r0 = r1 + r2
adc r1 r1 r3		; сложение с учётом флага переноса: r1 = r1 +c r3
add [ r2 ] 100 r4		; сложить r4 с константой ( режим адресации [ pc ] ) 100 и записать в ячейку памяти с адресом r2: [ r2 ] = 100 + r4
add [ r3 ] r4 [ $200 ]		; взять из ячейки памяти с адресом $200 (16-ричный адрес/число) число, сложить с r4 и записать в [ r3 ]: [ r3 ] = r4 + [ $200 ]
add [ 100 ] [ 20 ] [ 30 ]	; работаем с ячейками памяти минуя регистры (вся инструкция будет занимать четыре слова): [ 100 ] = [ 20 ] + [ 30 ]
```
Заметьте, что в синтаксисе этого ассемблера нет запятых между аргументами, поэтому если аргумент является сложным выражением времени компиляции, то его надо заключать в круглые скобки:
```
add r0 r0 (label + 4 * offset)	; в скобках может быть сложное выражение вычислимое на этапе компиляции
```

Как понятно всего возможно 16 кодов операций максимум. Сейчас реализованы следующие:
- 00 **ADDIS** - сложить Y с inplace immediate в X (-8..+7) без обновления флагов (S - silent, т.е. без обновления флагов)
- 01 **ADDI** - сложить Y с inplace immediate в X (-8..+7)
- 02 **RRCI** - прокрутка Y вправо (в русской литературе обычно переводят как 'циклический сдвиг') на INPLACE immediate (0..15) бит, во флаг переноса заносится последний перенесённый бит

- 04 **ADDS** - сложение без обновления флагов (silent)
- 05 **ADD** - сложение
- 06 **ADC** - сложение с учётом флага переноса
- 07 **SUB** - вычитение
- 08 **SBC** - вычитание с учётом флага переноса
- 09 **AND** - and
- 0A **OR** - or
- 0B **XOR** - xor
- 0C **CADD** - условное сложение. не обновляет флаги. см. ниже.
- 0D **RRC** - прокрутка Y вправо на X бит, во флаг переноса заносится последний перенесённый бит

## ПОЯСНЕНИЯ:

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

