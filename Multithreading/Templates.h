#include <iostream>
#include <tuple>

// Print any standard container
template <template <typename...> typename Container, typename...Args>
void print_container(const Container<Args...>& cont)
{
	for(auto& i : cont)
		std::cout << i << " ";
	std::cout << std::endl;
}

// Convert array into a tuple
template<typename Array, std::size_t... I>
decltype(auto) a2t_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}
 
template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
decltype(auto) a2t(const std::array<T, N>& a)
{
    return a2t_impl(a, Indices{});
}
 
template<int...Nums> 
 struct Numbers;
 
template<> 
 struct Numbers<> { enum {size = 10}; };
 
template<int El, int ...Nums> 
 struct Numbers<El, Nums...> { enum {size = 1 + Numbers<Nums...>::size};};
 
template<typename T>
int foo(T&& )
{
	return 1;
}

template<typename T, typename K>
void tf(K, T&& arg)
{
	foo(std::forward<T>(arg));
}

template<typename T>
decltype(auto) bar(T arg) 
{ 
	return arg; 
}

template<typename T>
T fold(T arg) { return arg; }

template<typename T, typename...T2>
T fold(T arg, T2 ...args) { return arg + fold(args...); }

template<size_t ... args>
struct Fold;

template<size_t a>
struct Fold<a> { enum {res = a}; };

template<size_t a, size_t...args>
struct Fold<a, args...> { enum {res = a + Fold<args...>::res}; };

template<size_t ...args>
size_t fold(std::index_sequence<args...>) 
{
	return Fold<args...>::res;
	//return fold(args...);
}

/*
template<typename ...T>
int fold(T ...args)
{
	return (... + args);
}
*/