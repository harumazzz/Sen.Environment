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

	#define assert_not_null(conditional, message, function_name)\
	if (!(conditional)) { \
		throw NullPointerException{message, std::source_location::current(), function_name}; \
	}\

	#define assert_eof(conditional, message, function_name)\
	if (!(conditional)) { \
		throw EOFException{message, std::source_location::current(), function_name}; \
	}\

	inline auto parse_exception(
	) -> std::unique_ptr<Exception> {
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (OutOfBoundsException& e) {
			return std::make_unique<OutOfBoundsException>(e);
		}
		catch (DirectoryNotFoundException& e) {
			return std::make_unique<DirectoryNotFoundException>(e);
		}
		catch (NullPointerException& e) {
			return std::make_unique<NullPointerException>(e);
		}
		catch (SystemException& e) {
			return std::make_unique<SystemException>(e);
		}
		catch (RuntimeException& e) {
			return std::make_unique<RuntimeException>(e);
		}
		catch (std::system_error& e) {
			return std::make_unique<SystemException>(e.what());
		}
		catch (Exception& e) {
			return std::make_unique<Exception>(e);
		}
		catch (const std::exception& e) {
			return std::make_unique<Exception>(e.what());
		}
		catch (...) {
			return std::make_unique<Exception>("Undefined exception caught");
		}
	}

}