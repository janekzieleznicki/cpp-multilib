#pragma once
#include <string>
#include <sstream>
#ifdef LOGGING_TRACE
 
#define LOG_TRACE(X) std::cerr << X;
 
#else
//  std::stringstream ss;
#define LOG_TRACE(X)  
 
#endif

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;