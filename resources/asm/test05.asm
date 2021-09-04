#include "simple_lib.inc"

; Строка с приветствием
str		dw "Press keys..." 13 0

start		; Вводим символы с клавиатуры и печатаем их на экране...
		r0 <- str
		call zstrPrint	; печатаем строку с приветствием
		
		r0 <- 65535
		r1 <- 300
		call r0_div_r1
		call printNum
		call printSpace
		r0 <- r1
		call printNum
		
loop		call inputChar	; вводим (блокирующе) символ с клавиатуры
		call printSpChar	; тут же печатаем его (со спец-символами)
		pc <- loop	; и зацикливаемся...