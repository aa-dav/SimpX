; MATH - некоторые арифметические и математические функции

; *** ПРОЦЕДУРЫ ***
; r0_mul_r1	- умножить r0 на r1 (числа без знака)
; r0_div_r1	- поделить r0 на r1 (числа без знака)

; r0_mul_r1 - умножить r0 на r1 (числа без знака)
; вход:	r0, r1 - множители
; выход:	r0 - результат
r0_mul_r1		[ sp ] <- r1	; сохраняем регистры в стек
		[ sp ] <- r2
		r2 <- r0		; перекладываем множитель в r2
		r0 = r0 - r0	; обнуляем r0 - он будет аккумулировать результат
.loop		r1 <= r1 >> 1	; прокручиваем r1 на бит вправо (циклически)
		jnc .skip		; если бит нулевой - пропускаем накопление результата
		r0 = r0 + r2	; аккумулируем результат в r0
.skip		r2 = r2 + r2	; сдвигаем r2 влево (умножаем на 2)
		r1 = r1 & $7FFF	; очищаем "провёрнутый" бит в r1 заодно тестируя на 0
		jnz .loop		; если не 0 - повторяем цикл
		r2 <- [ sp ]	; восстанавливаем регистры из стека
		r1 <- [ sp ]
		ret		; выход из процедуры

; r0_div_r1 - поделить r0 на r1 (числа без знака)
; вход:	r0 - делимое, r1 - делитель
; выход:	r0 - результат деления, r1 - остаток от деления				
r0_div_r1		[ sp ] <- r2
		[ sp ] <- r3
		r3 <- 16		; в r3 число итераций
		r2 <- r1		; делитель теперь в r2
		r1 = r1 - r1	; обнулим r1, он будет накапливать остаток
.loop		r0 = r0 + r0	; сдвигаем r0 влево, верхний бит попадает в CF
		r1 = r1 +c r1	; сдвигаем r1 влево, в нижний бит попадает CF 
				; по сути мы сдвинули как целое регистровую пару
				; r1:r0 влево на один бит.
		void = r1 - r2	; можно ли вычесть r2 из r1 без переполнения?
		jc .skip		; если нет, то идём на следующую итерацию
		r0 <- r0 + 1	; иначе зажигаем нижний бит r0 - в процессе деления
				; делимое в нём полностью заменится на результат
		r1 = r1 - r2	; и вычитаем из текущего остатка делитель
.skip		r3 <= r3 - 1	; уменьшаем счётчик
		jnz .loop		; и если он не ноль - повторяем итерацию
		r3 <- [ sp ]
		r2 <- [ sp ]
		ret		
		
		
	