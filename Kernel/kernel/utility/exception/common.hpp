#pragma once

#include <ranges>
#include <source_location>
#include <stdexcept>
#include <string>
#include "kernel/subprojects/fmt.hpp"

namespace Sen::Kernel {

    class Exception : public std::runtime_error {

    protected:

        inline static auto constexpr k_lambda = std::string_view{"lambda"};

    private:

        std::string msg;


    public:

        std::string arg;

        std::string function_name;

        std::string source;

        constexpr Exception() = delete;

        Exception(const Exception&) = default;

        Exception(Exception&&) noexcept = default;

        Exception& operator=(const Exception&) = default;

        Exception& operator=(Exception&&) noexcept = default;

        explicit Exception(std::string_view arg,
                           const std::source_location &loc = std::source_location::current(),
                           const std::string_view &function_name = k_lambda
            )
            : std::runtime_error{std::string{arg}},
              arg{arg},
              function_name{function_name},
              source{fmt::format("{}:{}:{}", loc.file_name(), loc.line(), loc.column())} {
            #ifdef _WIN32
            std::ranges::replace(source, '\\', '/');
            #endif
            msg = fmt::format("{}\n{}", source, arg);
        }

        [[nodiscard]] constexpr auto message(
        ) const noexcept -> std::string_view {
            return arg;
        }

        [[nodiscard]] auto what(
        ) const noexcept -> const char* override {
            return msg.data();
        }

        [[nodiscard]] static constexpr auto name(
        ) noexcept -> std::string_view {
            return "Exception";
        }

        ~Exception(
        ) noexcept override = default;
    };

    class RuntimeException : public Exception {

    public:
        using Exception::Exception;

        [[nodiscard]] static constexpr auto name(
        ) noexcept -> std::string_view {
            return "RuntimeException";
        }
    };

     class OutOfBoundsException final : public RuntimeException {

    public:

        using RuntimeException::RuntimeException;

        [[nodiscard]] static constexpr auto name(
        ) noexcept -> std::string_view {
            return "OutOfBoundsException";
        }
    };

    class IllegalArgumentException final : public RuntimeException {

    public:

        using RuntimeException::RuntimeException;

        [[nodiscard]] static constexpr auto name(
        ) noexcept -> std::string_view {
            return "IllegalArgumentException";
        }
    };

    class NullPointerException final : public RuntimeException {

    public:

        using RuntimeException::RuntimeException;

        [[nodiscard]] static constexpr auto name(
        ) noexcept -> std::string_view {
            return "NullPointerException";
        }
    };

    class ArithmeticException final : public RuntimeException {

    public:

        using RuntimeException::RuntimeException;

        [[nodiscard]] static constexpr auto name(
        ) noexcept -> std::string_view {
            return "ArithmeticException";
        }
    };

    class IllegalStateException final : public RuntimeException {

    public:

        using RuntimeException::RuntimeException;

        [[nodiscard]] static constexpr auto name() noexcept -> std::string_view {
            return "IllegalStateException";
        }
    };

    class UnsupportedOperationException final : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        [[nodiscard]] static constexpr auto name() noexcept -> std::string_view {
            return "UnsupportedOperationException";
        }
    };

    class IOException : public Exception {

    public:

        using Exception::Exception;

        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "IOException";
        }
    };

    class FileNotFoundException final : public IOException {

    public:

        using IOException::IOException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "FileNotFoundException";
        }
    };

    class DirectoryNotFoundException final : public IOException {

    public:

        using IOException::IOException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "DirectoryNotFoundException";
        }
    };

    class EOFException final : public IOException {

    public:

        using IOException::IOException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "EOFException";
        }
    };

    class PermissionDeniedException final: public IOException {

    public:

        using IOException::IOException;

        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "PermissionDeniedException"; }
    };

    class AlgorithmException : public Exception {

    public:

        using Exception::Exception;

        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "AlgorithmException";
        }
    };

    class NoSuchElementException final : public AlgorithmException {

    public:

        using AlgorithmException::AlgorithmException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "NoSuchElementException";
        }
    };

    class ConcurrentModificationException final : public AlgorithmException {

    public:

        using AlgorithmException::AlgorithmException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "ConcurrentModificationException";
        }
    };

    class TimeoutException final : public AlgorithmException {

    public:

        using AlgorithmException::AlgorithmException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "TimeoutException";
        }
    };

    class SystemException : public Exception {

    public:

        using Exception::Exception;

        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "SystemException";
        }
    };

    class MemoryAccessException final : public SystemException {

    public:

        using SystemException::SystemException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "MemoryAccessException";
        }
    };

    class StackOverflowException final : public SystemException {

    public:

        using SystemException::SystemException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "StackOverflowException"; }
    };

    class SegmentationFaultException final : public SystemException {

    public:

        using SystemException::SystemException;
        [[nodiscard]] static constexpr auto name(

        ) noexcept -> std::string_view {
            return "SegmentationFaultException"; }
    };
}