#include <iostream>
#include "alxvm.h"
#include "alxvm_tokens.h"
#include "alxvm_templates.h"

int sub( int x, int y )
{
	return x - y;
};

void print_num( int num )
{
	std::cout << num;
};

void print_ln()
{
	std::cout << "\n";
};

int main(int argc, const char *agrv[])
{
	AlxVM::FileTokenizer tokenizer( "test.avil" );	// будем разбирать на токены файл test.avil
	AlxVM::Runtime runtime( 1024 );		// параметр - размер стека

	AlxVM::bindNativeFunction( runtime, "sub", sub );
	AlxVM::bindNativeFunction( runtime, "print_num", print_num );
	AlxVM::bindNativeFunction( runtime, "print_ln", print_ln );

	runtime.compile(tokenizer);		// компилируем из токенайзера
	//runtime.dump();				// выведем отладочную информацию

	AlxVM::TypedFunction<void(int)> funcPrintSome( runtime, "testModule", "printSome" );
	funcPrintSome( 111 );
	std::cout << "\n";
	funcPrintSome( 222 );
	std::cout << "\n";

	AlxVM::TypedFunction<int(int, int)> funcCallNative2( runtime, "testModule", "callNative2" );
	for ( int i = 1; i <= 10; i ++ )
		std::cout << "callNative(" << i << "): " << funcCallNative2(100, i) << "\n";

	return 0;
}