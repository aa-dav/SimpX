#include <iostream>
#include "alxvm.h"
#include "alxvm_tokens.h"

int main(int argc, const char *agrv[])
{
	AlxVM::FileTokenizer tokenizer( "test.avil" );	// будем разбирать на токены файл test.avil
	AlxVM::Runtime runtime( 1024 );		// параметр - размер стека
	runtime.compile(tokenizer);		// компилируем из токенайзера
	//runtime.dump();				// выведем отладочную информацию
	
	AlxVM::TypedFunction<int32_t()> funcGet10( runtime, "testModule", "get10" );
	std::cout << "get10: " << funcGet10() << "\n";

	AlxVM::TypedFunction<int32_t(int32_t, int32_t)> funcMax( runtime, "testModule", "max" );
	std::cout << "max: " << funcMax( -10, 20 ) << "\n";
	std::cout << "max: " << funcMax( 777, 5 ) << "\n";
	std::cout << "max: " << funcMax( -908, 333 ) << "\n";
	std::cout << "max: " << funcMax( 5, 6 ) << "\n";

	AlxVM::TypedFunction<void(int32_t)> funcVoidSome( runtime, "testModule", "voidSome" );
	funcVoidSome( 10 );
	std::cout << "voidSome\n";

	AlxVM::TypedFunction<void()> funcVoidFull( runtime, "testModule", "voidFull" );
	funcVoidFull();
	std::cout << "voidFull\n";
}