#include "simple_lib.inc"

; Строка с приветствием
str		dw "Press keys..." 13 0

start		; Вводим символы с клавиатуры и печатаем их на экране...
		r0 <- str
		call zstrPrint	; печатаем строку с приветствием
		
loop		call inputChar	; вводим (блокирующе) символ с клавиатуры
		call printSpChar	; тут же печатаем его (со спец-символами)
		pc <- loop	; и зацикливаемся...
