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

	/*
	AlxVM::TypedFunction<void(int)> funcPrintSome( runtime, "testModule", "printSome" );
	funcPrintSome( 111 );
	std::cout << "\n";
	funcPrintSome( 222 );
	std::cout << "\n";

	AlxVM::TypedFunction<int(int, int)> funcCallNative2( runtime, "testModule", "callNative2" );
	for ( int i = 1; i <= 10; i ++ )
		std::cout << "callNative(" << i << "): " << funcCallNative2(100, i) << "\n";

	AlxVM::TypedFunction<int(int)> funcFactorial( runtime, "testModule", "factorial" );
	for ( int i = 0; i <= 9; i ++ )
		std::cout << "factorial(" << i << "): " << funcFactorial(i) << "\n";

	AlxVM::TypedFunction<long long(int)> funcConvert32to64( runtime, "testModule", "convert32to64" );
	for ( int i = -5; i <= 5; i ++ )
		std::cout << "convert32to64(" << i << "): " << funcConvert32to64(i) << "\n";
	*/
	AlxVM::TypedFunction<void(int*)> funcIncByRef( runtime, "testModule", "incByRef" );
	int x = 3;
	std::cout << "Before: " << x << "\n";
	funcIncByRef( &x );
	std::cout << "After: " << x << "\n";

	return 0;
}