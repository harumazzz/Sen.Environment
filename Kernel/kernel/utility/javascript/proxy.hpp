#pragma once

#include "kernel/utility/javascript/context.hpp"
#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/trait.hpp"

namespace Sen::Kernel::Javascript {

    template <auto Callable> requires is_global_function_v<Callable> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<Callable>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
        std::is_void_v<typename is_global_function<std::decay_t<type_of<Callable>>>::ReturnType>
    inline auto proxy_native_function (
        Subprojects::quickjs::JSContext* ctx,
        Subprojects::quickjs::JSValue this_val,
        int argc,
        Subprojects::quickjs::JSValue* argv
    ) -> Subprojects::quickjs::JSValue {
        auto result = Value::new_value(ctx);
        auto context = Context::new_ref(ctx);
        auto value = Value::new_ref(ctx, this_val);
        auto arguments = Array<Value>{static_cast<usize>(argc)};
        for (auto i = 0; i < argc; ++i) {
            arguments[i] = Value::new_ref(ctx, argv[i]);
        }
        Callable(context, value, arguments, result);
        return result.release();
    }

    template <auto function> requires (is_global_function_v<function> || std::is_invocable_v<decltype(function)>)
    inline auto proxy_native_function_wrapper(
        Context& context,
        Value& object,
        Array<Value>& arguments,
        Value& result
    ) -> void {
        using traits = callable_traits<decltype(function)>;
        constexpr auto count = traits::arg_count;
        assert_conditional(arguments.size() >= count, fmt::format("Expected at least {} arguments, got {}", count, arguments.size()), "proxy_native_function_wrapper");
        using Argument = typename traits::args_tuple;
        auto call = [&](auto&& callable, auto&&... extra_args) -> void {
            auto args = Argument{};
            [&]<auto... Index>(std::index_sequence<Index...>) {
                ((arguments[Index].template get<std::tuple_element_t<Index, Argument>>(std::get<Index>(args))), ...);
            }(std::make_index_sequence<count>{});
            if constexpr (std::is_void_v<typename traits::return_type>) {
                std::apply(callable, std::tuple_cat(std::forward_as_tuple(extra_args...), args));
                result.set_undefined();
            } else {
                result.set<typename traits::return_type>(
                    std::apply(callable, std::tuple_cat(std::forward_as_tuple(extra_args...), args))
                );
            }
        };
        if constexpr (std::is_member_function_pointer_v<decltype(function)>) {
            using ClassName = typename traits::class_type;
            auto instance = std::add_pointer_t<ClassName>{nullptr};
            object.template get<Pointer<ClassName>>(instance);
            return call([&]<typename... Arguments> requires (!std::is_void_v<Arguments> && ...) (Arguments&&... args) -> auto {
                return (instance->*function)(std::forward<Arguments>(args)...);
            });
        } else {
            return call(function);
        }
    }

    template<auto function> requires is_global_function_v<function> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
        std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType>
    inline auto Value::add_function(
        const String &name,
        const bool is_constructor
    ) const -> void {
        thiz.define_property(
            name,
            JS_NewCFunction2(
                thiz.m_context,
                &proxy_native_function<function>,
                name.cbegin(),
                0,
                is_constructor ? JS_CFUNC_constructor : JS_CFUNC_generic,
                0
            )
        );
    }

    template <auto Function> requires std::is_invocable_v<decltype(Function)>
    struct CallableLambda {

        using ReturnType = decltype(Function());

        using Trait = callable_traits<decltype(Function)>;

        inline static auto invoke(
            Context& context,
            Value& object,
            Array<Value>& arguments,
            Value& result
        ) -> void {
            return proxy_native_function_wrapper<Function>(context, object, arguments, result);
        }
    };

    template <auto Function> requires std::is_invocable_v<decltype(Function)>
    inline constexpr auto normalize_lambda_to_function(
    ) -> auto {
        return &CallableLambda<Function>::invoke;
    }

}