#ifndef ALXVM_TEMPLATES
#define ALXVM_TEMPLATES

#include <stdint.h>
#include <vector>
#include <string>
#include <variant>
#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include <functional>

namespace AlxVM
{

// ******************************************
// *           ArgumentsChecker             *
// ******************************************

template<typename... Args> struct ArgumentsChecker;

template<>
struct ArgumentsChecker<>
{
	static void check( std::shared_ptr< Function > function, int index ) {};
};

template<typename T, typename... Args>
struct ArgumentsChecker<T, Args...>
{
	static void check( std::shared_ptr< Function > function, int index )
	{
		if ( function->getParamSize( index ) != sizeof( T ) )
			throw std::runtime_error( std::string( "Incompatible function " ) + function->getName() + 
						": wrong param " + std::to_string( index + 1 ) + " size " +
						std::to_string( function->getParamSize( index ) ) + " instead of " + std::to_string( sizeof( T ) ) + "." );
		ArgumentsChecker< Args... >::check( function, index + 1 );
	}
};

template< typename... Args >
static void checkFunctionCompatibility( std::shared_ptr< Function > function, int sizeOfReturn )
{
	if ( function->getReturnSize() != sizeOfReturn )
		throw std::runtime_error( std::string( "Incompatible function " ) + function->getName() + ": wrong return size " +
					std::to_string( function->getReturnSize() ) + " instead of " + std::to_string( sizeOfReturn ) + "." );
	if ( function->getParamsCount() != sizeof...( Args ) )
		throw std::runtime_error( std::string( "Incompatible function " ) + function->getName() + ": wrong params count " +
					std::to_string( function->getParamsCount() ) + " instead of " + std::to_string( sizeof...( Args ) ) + "." );
	ArgumentsChecker< Args... >::check( function, 0 );
};

// ******************************************
// *             TypedFunction              *
// ******************************************

template< typename... Args > class TypedFunction;

template< class T, typename... Args >
class TypedFunction< T( Args... ) >
{
	std::shared_ptr< Function > function;
	Runtime &runtime;

public:
	TypedFunction( std::shared_ptr< Function > aFunction, Runtime &aRuntime ): function( aFunction ), runtime( aRuntime ) 
	{
		checkFunctionCompatibility< Args... >( function, sizeof( T ) );
	};
	TypedFunction( Runtime &aRuntime, const std::string &moduleName, const std::string &functionName ): runtime( aRuntime )
	{
		function = runtime.getFunction( moduleName, functionName );
		checkFunctionCompatibility< Args... >( function, sizeof( T ) );
	};
	T operator()( Args...args )
	{
		//std::cout << "T exec(" << sizeof...(Args) << ")\n";
		runtime.startParams( sizeof( T ) );
		runtime.addParams( args... );
		runtime.execFunction( function, 0 );
		return runtime.stackAs< T >( 0 );
	}
};

template< typename... Args >
class TypedFunction< void( Args... ) >
{
	std::shared_ptr< Function > function;
	Runtime &runtime;

public:
	TypedFunction( std::shared_ptr< Function > aFunction, Runtime &aRuntime ): function( aFunction ), runtime( aRuntime ) 
	{
		checkFunctionCompatibility< Args... >( function, 0 );
	};
	TypedFunction( Runtime &aRuntime, const std::string &moduleName, const std::string &functionName ): runtime( aRuntime )
	{
		function = runtime.getFunction( moduleName, functionName );
		checkFunctionCompatibility< Args... >( function, 0 );
	};
	void operator()( Args... args )
	{
		//std::cout << "void exec(" << sizeof...(Args) << ")\n";
		runtime.startParams( 0 );
		runtime.addParams( args... );
		runtime.execFunction( function, 0 );
	}
};

// ******************************************
// *             NativeFunction             *
// ******************************************

template <typename... Args> class NativeFunction;

template <typename R>
class NativeFunction< R() >: public NativeFunctionBase
{
	std::function< R() > func;
public:
	NativeFunction( const std::function< R() > &aFunc ): func( aFunc ) {};
	void invoke( Runtime &runtime, StackPos params, StackPos rp ) override
	{
		runtime.stackAs<R>( rp ) = func();
	}	
};

template <typename R, typename P1>
class NativeFunction< R(P1) >: public NativeFunctionBase
{
	std::function< R(P1) > func;
public:
	NativeFunction( const std::function< R(P1) > &aFunc ): func( aFunc ) {};
	void invoke( Runtime &runtime, StackPos params, StackPos rp ) override
	{
		runtime.stackAs<R>( rp ) = func(
			runtime.stackAs<P1>(params - sizeof(P1))
		);
	}	
};

template <typename R, typename P1, typename P2>
class NativeFunction< R(P1, P2) >: public NativeFunctionBase
{
	std::function< R(P1, P2) > func;
public:
	NativeFunction( const std::function< R(P1, P2) > &aFunc ): func( aFunc ) {};
	void invoke( Runtime &runtime, StackPos params, StackPos rp ) override
	{
		runtime.stackAs<R>( rp ) = func(
			runtime.stackAs<P1>(params - sizeof(P2) - sizeof(P1)),
			runtime.stackAs<P2>(params - sizeof(P2))
		);
	}	
};

template <typename R, typename P1, typename P2, typename P3>
class NativeFunction< R(P1, P2, P3) >: public NativeFunctionBase
{
	std::function< R(P1, P2, P3) > func;
public:
	NativeFunction( const std::function< R(P1, P2, P3) > &aFunc ): func( aFunc ) {};
	void invoke( Runtime &runtime, StackPos params, StackPos rp ) override
	{
		runtime.stackAs<R>( rp ) = func(
			runtime.stackAs<P1>(params - sizeof(P3) - sizeof(P2) - sizeof(P1)),
			runtime.stackAs<P2>(params - sizeof(P3) - sizeof(P2)),
			runtime.stackAs<P3>(params - sizeof(P3))
		);
	}	
};

template< typename R, typename... Args >
void bindNativeFunction( Runtime &runtime, const std::string &name, std::function< R( Args... ) > func )
{
	runtime.addNativeFunction( name, new NativeFunction< R( Args... ) >( func ) );
}

template< typename R, typename... Args >
void bindNativeFunction( Runtime &runtime, const std::string &name, R(*func)( Args... ) )
{
	runtime.addNativeFunction( name, new NativeFunction< R( Args... ) >( func ) );
}

};

#endif