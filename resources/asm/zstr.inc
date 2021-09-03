; ZSTR - функции по работе с ASCIIZ-строками (как в Си)

; zstrPrint - напечатать строку терминирующуюся нулём
; in: r0 - указатель на строку				  
zstrPrint		[ sp ] <- r0
		[ sp ] <- r1
		r1 <- r0
.loop		r0 <= [ r1 ]
		jz .end
		call printSpChar
		r1 <- r1 + 1
		pc <- .loop
.end		r1 <- [ sp ]
		r0 <- [ sp ]
		ret
		
; zstrLength - длина ASCIIZ-строки
; вход:	r0 - указатель на начало строки
; выход:	r0 - указатель на конец строки (символ 0)
;	r1 - длина строки
zstrLength	r1 = r1 - r1
.loop		void <= [ r0 ]
		jz .end
		r0 <- r0 + 1
		r1 <- r1 + 1
		pc <- .loop
.end		ret		

; zstrCopy - скопировать ASCIIZ-строку в буфер
; вход:	r0 - указатель куда копируем
;	r1 - указатель откуда копируем
; выход:	r0 - конец буфера куда копировали (символ 0)
zstrCopy		[ sp ] <- r1
.loop		[ r0 ] <= [ r1 ]
		jz .end
		r0 <- r0 + 1
		r1 <- r1 + 1
		pc <- .loop
.end		r1 <- [ sp ]
		ret		