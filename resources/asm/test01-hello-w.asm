; **************************************************
; * test01.asm - тест вывода строк (Hello, world!) *
; **************************************************

#include "simple_lib.inc"

; Строки с приветствиями
strHello		dw "Hello, world!" 13 0
strWelcome	dw "Welcome to the future" 13 9 "of the past..." 13 0

start		; точка входа из simple_lib
		r0 <- strHello	; в регистр r0 помещаем адрес первой строки
		call zstrPrint	; печатаем эту строку
		; выбираем палитру 1 вместо 0 по умолчанию
		; это верхние 4 бита переменной textAttrs
		[ textAttrs ] <- $1000	; выбираем палитру 1
		r0 <- strWelcome	; в регистр r0 помещаем адрес второй строки
		call zstrPrint

		; зацикливаемся в энергосберегающем режиме
		; (энергосберегаемость тут необязательна)		
forever		psw = psw | CPU_HALT	; переводим процессор в режим ожидания прерывания
		pc <- forever		; зацикливаемся навечно