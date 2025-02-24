#pragma once

#include "kernel/utility/exception/common.hpp"

namespace Sen::Kernel {

	#define assert_conditional(conditional, message, function_name)\
	if (!(conditional)) { \
		throw RuntimeException{message, std::source_location::current(), function_name}; \
	}\
	static_assert(true)

	#define assert_index(conditional, message, function_name)\
	if (!(conditional)) { \
		throw OutOfBoundsException{message, std::source_location::current(), function_name}; \
	}\
	static_assert(true)

	#define assert_has_directory(conditional, message, function_name)\
	if (!(conditional)) { \
		throw DirectoryNotFoundException{message, std::source_location::current(), function_name}; \
	}\
	static_assert(true)

	inline static auto parse_exception(

	) -> Exception
	{
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch(Exception &ex){
			return Exception{ ex };
		}
		catch (std::system_error& ex) {
			return static_cast<Exception>(ex.what());
		}
		catch(const std::exception &ex){
			return static_cast<Exception>(ex.what());
		}
		catch(...){
			return static_cast<Exception>("Undefined exception caught");
		}
		return Exception{"Undefined exception"};
	}
}