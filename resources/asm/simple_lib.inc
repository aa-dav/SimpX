#include "simpx.inc"

		org $8000	; Помещаем текущий адрес в $8000
#include "font-00.asm"		; загружаем битмап шрифта в $8000...

		org $0000	; Возвращаемся в адрес $0000 (стартовая точка процессора)
		mode new	; "математический" режим ассемблера

		psw <- $0000	; выключим прерывания
		sp <- $8000	; установим указатель на стек в $8000
		[ vidBitmapPage ]   <- 4	; настраиваем видеобитмап на $8000-BFFF
		[ vidCharmapPage ]  <- 6	; настраиваем страницу чармапа на $C000
		[ vidCharmapAddr ]  <- 0	; и адрес в странице на $C000-C3FF
		call simpleInit	; инициализируем простую библиотеку ввода-вывода
		pc <- start	; и переходим на начало программы

; palette0 - простая палитра на 16 первых цветов по типу спектрумовской
; перебирает все комбинации вкл/выкл каналов RGB в первых 8 слотах
; и точно то же делает в последних 8 слотах, но с повышенной яркостью			 
palette0	; первые 16 слотов палитры, формат RRRRRGGGGGBBBBB
		; slot 0
		dw 000000000000000b
		dw 000000000010000b
		dw 000001000000000b
		dw 000001000010000b
		dw 100000000000000b
		dw 100000000010000b
		dw 100001000000000b
		dw 100001000010000b
		dw 011110111101111b
		dw 000000000011111b
		dw 000001111100000b
		dw 000001111111111b
		dw 111110000000000b
		dw 111110000011111b
		dw 111111111100000b
		dw 111111111111111b
		; slot 1
		dw 000000000000000b
		dw 000000000010000b
		dw 000001000000000b
		dw 000001000010000b
		dw 100000000000000b
		dw 100000000010000b
		dw 100001000000000b
		dw 100001000010000b
		dw 011110111101111b
		dw 000000000011111b
		dw 000001111100000b
		dw 000001111111111b
		dw 111110000000000b
		dw 111110000011111b
		dw 111111111100000b
		dw 000001111111111b ; !

; loadPalette - процедура по заполнению слотов палитры
; in:	r0 - адрес палитры в памяти
;	r1 - начальный слот палитры
;	r2 - количество слотов палитры к заполнению
loadPalette 
		[ sp ] <- r0
		[ sp ] <- r1
		[ sp ] <- r2
.loop		[ vidPalPtr ] <- r1
		r1 <- r1 + 1
		[ vidPalData ] <- [ r0 ]
		r0 <- r0 + 1
		r2 <= r2 + -1
		jnz .loop
		r2 <- [ sp ]
		r1 <- [ sp ]
		r0 <- [ sp ]
		ret	 

; simpleInit - инициализация простой тестовой среды выполнения
simpleInit
		[ sp ] <- r0
		[ sp ] <- r1
		[ sp ] <- r2
		; загрузим простую палитру
		r0 <- palette0
		r1 <- 0		   
		r2 <- 32
		call loadPalette
		; инициализируем текстовую раскладку видеоданных
		call initTextMode
		r2 <- [ sp ]
		r1 <- [ sp ]
		r0 <- [ sp ]
		ret
		
textPos		dw 0
textEnd		dw ( 24 * 32 )
textAttrs	dw 0
textBase	= $C000

; initTextMode - инициализируем "текстовый режим"
; на самом деле в SimpX это не более чем упорядоченная определенным образом 
; раскладка видеоданных (чармап заполнен не линейно как в графическом режиме, а каждый
; его элемент и есть символ и его атрибуты цветности)
initTextMode
		[ sp ] <- r0
		[ sp ] <- r1
		r0 <- 0
		[ vidScrollX ] <- r0
		[ vidScrollY ] <- r0
		[ textAttrs ] <- r0
		[ textPos ] <- r0
		[ textEnd ] <- ( 24 * 32 )
		r0 <- $C000
		r1 <- 1024
.loop		[ r0 ] <- 0
		r0 <- r0 + 1
		r1 <= r1 + -1
		jnz .loop
		r1 <- [ sp ]
		r0 <- [ sp ]
		ret

; printChar - печать символа в r0 (ascii) в текстовом режиме
; in: r0 - символ для печати
printChar
		[ sp ] <- r0
		[ sp ] <- r1
		[ sp ] <- r2
		r1 <- [ textPos ]
		void = r0 - 13
		jnz .not_cr
		; carriage return
		r2 = textBase + r1
.cr_loop	[ r2 ] <- [ textAttrs ]
		r2 <- r2 + 1
		r1 <- r1 + 1
		void = r1 & 11111b
		jnz .cr_loop
		pc <- .check_ovfl
.not_cr		r0 = r0 +s -32 ; ascii fix
		r0 = r0 | [ textAttrs ] ; apply attrs
		r2 = textBase + r1
		[ r2 ] <- r0
		r1 <- r1 + 1
.check_ovfl	r1 = r1 & $3FF
		[ textPos ] <- r1
		r2 <- [ textEnd ]
		void = r2 - r1
		jnz .end
		; overflow
		[ vidScrollY ] = [ vidScrollY ] +s 8
		r0 <- 32
		r1 = textBase + r2
.clear_loop	[ r1 ] <- 0
		r1 <- r1 + 1
		r2 <- r2 + 1
		r0 <= r0 + -1
		jnz .clear_loop
		r2 = r2 & $3FF
		[ textEnd ] <- r2
.end		r2 <- [ sp ]
		r1 <- [ sp ]
		r0 <- [ sp ]
		ret

hexNumbers	dw "0123456789ABCDEF"			 

; printHex - печать числа в r0 как hex		  
; in: r0 - число для печати
printHex		
		[ sp ] <- r0
		[ sp ] <- r1
		r1 <- r0

		r1 <= r1 >> -4
		r0 = r1 & $000F
		r0 = hexNumbers + r0
		r0 <- [ r0 ]
		call printChar

		r1 <= r1 >> -4
		r0 = r1 & $000F
		r0 = hexNumbers + r0
		r0 <- [ r0 ]
		call printChar

		r1 <= r1 >> -4
		r0 = r1 & $000F
		r0 = hexNumbers + r0
		r0 <- [ r0 ]
		call printChar

		r1 <= r1 >> -4
		r0 = r1 & $000F
		r0 = hexNumbers + r0
		r0 <- [ r0 ]
		call printChar
						
		r1 <- [ sp ]
		r0 <- [ sp ]
		ret

; zstrPrint - напечатать строку терминирующуюся нулём
; in: r0 - указатель на строку				  
zstrPrint	[ sp ] <- r0
		[ sp ] <- r1
		r1 <- r0
.loop		r0 <= [ r1 ]
		jz .end
		call printChar
		r1 <- r1 + 1
		pc <- .loop
.end		r1 <- [ sp ]
		r0 <- [ sp ]
		ret		  

LastKey		dw 0
LastShifts	dw 0
; readKeys - считать кнопку
readKey		[ sp ] <- r1
		[ sp ] <- r2
		[ sp ] <- r3
		r1 <- 1		  ; в r1 линия 1
		[ portInput ] <- r1	  ; активируем линию 1
		r2 <- [ portInput ]	  ; считаем данные из линии 1
		[ LastShifts ] = r2 & 111b ; выделим и запомним шифты
		r3 = r3 - r3	  ; обнулим r3 - счётчик
		r0 = r0 - r0	  ; обнулим r0 - результат
		r2 = r2 & $FFF8	  ; удалим шифты из рассмотрения
.loop_keys	; в r2 следующий ряд, флаг z обновлён!
		jnz .loop_found	  ; тут где то нажата кнопка!
		r3 = r3 + 16	  ; продвигаем счётчик на 16 кнопок
		r1 = r1 + r1	  ; сдвинем r1 влево (умножение на 2)
		void = r1 - $100	  ; проверим на выход за 8 линий клавиатуры
		jz .end		  ; если вышли за пределы - идём на выход
		[ portInput ] <- r1	  ; обновим линию в portInput
		r2 <= [ portInput ]	  ; считаем кнопки с обновлением флагов
		pc <- .loop_keys	  ; идём в начало цикла
.loop_found	; в r2 считанный ряд, в r3 счётчик
		r3 <- r3 + 1
		r2 <= r2 >> 1
		jnc .loop_found
		r0 <- r3
.end		; в r0 результат
		void = r0 - [ LastKey ]
		jz .skip_res
		[ LastKey ] <- r0
		pc <- .exit
.skip_res		r0 = r0 - r0
.exit		r3 <- [ sp ]
		r2 <- [ sp ]
		r1 <- [ sp ]
		ret
		
keysToChars	dw 0	; shift
		dw 0	; control
		dw 0	; alt
		dw ( 'a' + ( 'A' << 8 ) )
		dw ( 'b' + ( 'B' << 8 ) )
		dw ( 'c' + ( 'C' << 8 ) )
		dw ( 'd' + ( 'D' << 8 ) )
		dw ( 'e' + ( 'E' << 8 ) )
		dw ( 'f' + ( 'F' << 8 ) )
		dw ( 'g' + ( 'G' << 8 ) )
		dw ( 'h' + ( 'H' << 8 ) )
		dw ( 'i' + ( 'I' << 8 ) )
		dw ( 'j' + ( 'J' << 8 ) )
		dw ( 'k' + ( 'K' << 8 ) )
		dw ( 'l' + ( 'L' << 8 ) )
		dw ( 'm' + ( 'M' << 8 ) )
		dw ( 'n' + ( 'N' << 8 ) )
		dw ( 'o' + ( 'O' << 8 ) )
		dw ( 'p' + ( 'P' << 8 ) )
		dw ( 'q' + ( 'Q' << 8 ) )
		dw ( 'r' + ( 'R' << 8 ) )
		dw ( 's' + ( 'S' << 8 ) )
		dw ( 't' + ( 'T' << 8 ) )
		dw ( 'u' + ( 'U' << 8 ) )
		dw ( 'v' + ( 'V' << 8 ) )
		dw ( 'w' + ( 'W' << 8 ) )
		dw ( 'x' + ( 'X' << 8 ) )
		dw ( 'y' + ( 'Y' << 8 ) )
		dw ( 'z' + ( 'Z' << 8 ) )
		dw ( '1' + ( '!' << 8 ) )
		dw ( '2' + ( '@' << 8 ) )
		dw ( '3' + ( '#' << 8 ) )
		dw ( '4' + ( '$' << 8 ) )
		dw ( '5' + ( '%' << 8 ) )
		dw ( '6' + ( '^' << 8 ) )
		dw ( '7' + ( '&' << 8 ) )
		dw ( '8' + ( '*' << 8 ) )
		dw ( '9' + ( '(' << 8 ) )
		dw ( '0' + ( ')' << 8 ) )
		dw ( 13 + ( 13 << 8 ) )
		dw 0	; esc
		dw 0	; bksp
		dw 0	; tab
		dw ( ' ' + ( ' ' << 8 ) )
		dw ( '-' + ( '_' << 8 ) )
		dw ( '=' + ( '+' << 8 ) )
		dw ( '[' + ( '{' << 8 ) )
		dw ( ']' + ( '}' << 8 ) )
		dw ( '\' + ( '|' << 8 ) )
		dw 0	; hashtilde ???
		dw ( ';' + ( ':' << 8 ) )
		dw ( ''' + ( '"' << 8 ) )
		dw ( '`' + ( '~' << 8 ) )
		dw ( ',' + ( '<' << 8 ) )
		dw ( '.' + ( '>' << 8 ) )
		dw ( '/' + ( '?' << 8 ) )
		dw 0	; capslock


