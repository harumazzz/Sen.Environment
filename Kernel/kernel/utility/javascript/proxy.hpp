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
        Subprojects::quickjs::JSValueConst this_val,
        int argc,
        Subprojects::quickjs::JSValueConst* argv
    ) -> Subprojects::quickjs::JSValue {
        auto context = Context::new_ref(ctx);
        auto value = Value::new_ref(ctx, this_val);
        auto result = Value::new_value(ctx);
        auto arguments = Array<Value>{static_cast<usize>(argc)};
        for (auto i = 0; i < argc; ++i) {
            arguments[i] = Value::new_ref(ctx, argv[i]);
        }
        try {
            std::invoke(Callable, context, value, arguments, result);
        } catch (...) {
            const auto exception = parse_exception();
            context.throw_exception(
                context.evaluate(make_string_view(
                fmt::format(
                    R"((() => {{
				        let e = new Error(`{0}`);
                        e.name = `{1}`;
                        e.stack = `    at {2} ({3})\n` + e.stack.substring(e.stack.indexOf('\n') + 1);
				        return e;
			        }}))",
                    exception->message(), exception->name(), exception->function_name, exception->source
                )), "proxy_native_function").call(make_list<Value>())
            );
            result.set_value(Subprojects::quickjs::$JS_EXCEPTION);
        }
        return result.release();
    }

    template <auto function> requires (is_global_function_v<function> || std::is_invocable_v<decltype(function)> || std::is_member_function_pointer_v<decltype(function)>)
    inline auto proxy_native_function_wrapper(
        Context& context,
        Value& value,
        Array<Value>& arguments,
        Value& result
    ) -> void {
        using traits = callable_traits<decltype(function)>;
        constexpr auto count = traits::arg_count;
        assert_conditional(arguments.size() == count, fmt::format("Expected at least {} arguments, got {}", count, arguments.size()), "proxy_native_function_wrapper");
        using Argument = ConvertTuple<typename traits::args_tuple, TransformWrapper>;
        auto call = [&]<typename... Arguments> requires (is_valid_tuple<Arguments> && ...) (auto&& callable, Arguments&&... extra_args) -> void {
            auto args = Argument{};
            [&]<auto... Index> requires (std::is_same_v<type_of<Index>, usize> && ...) (std::index_sequence<Index...>) -> void {
                ((arguments[Index].template get<std::remove_reference_t<std::tuple_element_t<Index, Argument>>>(std::get<Index>(args))), ...);
            }(std::make_index_sequence<count>{});
            auto argument = std::tuple_cat(std::forward_as_tuple(std::forward<Arguments>(extra_args)...), std::move(args));
            if constexpr (std::is_void_v<typename traits::return_type>) {
                std::apply(callable, argument);
                result.set_undefined();
            } else {
                result.set<typename traits::return_type>(
                    std::apply(callable, argument)
                );
            }
        };
        if constexpr (std::is_member_function_pointer_v<decltype(function)>) {
            using ClassName = typename traits::class_type;
            auto instance = std::add_pointer_t<ClassName>{nullptr};
            value.template get<Pointer<ClassName>>(instance);
            return call([&]<typename... Arguments> requires (!std::is_void_v<Arguments> && ...) (Arguments&&... args) -> decltype(std::invoke(function, instance, std::forward<Arguments>(args)...)) {
                return std::invoke(function, instance, std::forward<Arguments>(args)...);
            });
        } else {
            return call(function);
        }
    }

    template<auto function, auto is_constructor> requires is_global_function_v<function> &&
    std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
    std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType> && std::is_same_v<type_of<is_constructor>, bool>
    inline auto Value::add_function(
        const String &name
    ) const -> void {
        thiz.define_property(
            name,
            Subprojects::quickjs::JS_NewCFunction2(
                thiz.m_context,
                &proxy_native_function<function>,
                name.begin(),
                0,
                is_constructor ? JS_CFUNC_constructor : JS_CFUNC_generic,
                0
            )
        );
    }

    template<auto function, auto is_constructor> requires is_global_function_v<function> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
        std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType> && std::is_same_v<type_of<is_constructor>, bool>
    inline auto Value::set_function (
        const String &name
    ) -> void {
        thiz.m_value = Subprojects::quickjs::JS_NewCFunction2(
            thiz.m_context,
            &proxy_native_function<function>,
            name.begin(),
            0,
            is_constructor ? Subprojects::quickjs::$JS_CFUNC_constructor : Subprojects::quickjs::$JS_CFUNC_generic,
            0
        );
    }

    template <auto Function> requires std::is_invocable_v<decltype(Function)>
    struct CallableLambda {

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

    template <auto Function>
    inline constexpr auto normalize_lambda_to_function(
    ) -> auto {
        using FunctionPointer = void (*)(Context&, Value&, Array<Value>&, Value&);
        if constexpr (std::is_pointer_v<decltype(Function)>) {
            return static_cast<FunctionPointer>(&CallableLambda<Function>::invoke);
        } else {
            return static_cast<FunctionPointer>(+CallableLambda<Function>::invoke);
        }
    }

    template <typename T> requires std::is_class_v<T>
    class ClassBuilder {

    protected:

        String m_name;

        Value m_constructor;

        Value m_proto;

        Value m_parent;

    public:

        ClassBuilder(
        ) = delete;

        ~ClassBuilder(
        ) = default;

        ClassBuilder(
            ClassBuilder&& other
        ) = delete;

        auto operator =(
            ClassBuilder&& other
        ) -> ClassBuilder& = delete;

        ClassBuilder(
            const ClassBuilder& other
        ) = delete;

        auto operator =(
            const ClassBuilder& other
        ) = delete;

        explicit ClassBuilder(
            const String& name,
            const Value& parent
        ) : m_name{name}, m_constructor{}, m_proto{}, m_parent{Value::new_ref(parent._context(), parent.value())} {
            thiz.m_proto = thiz.m_parent.new_object();
            thiz.m_parent.context().runtime().template register_class<T>(name);
            thiz.m_parent.context().set_class_proto(Detail::class_id<T>, Value::new_ref(thiz.m_proto._context(), thiz.m_proto.value()));
        }

        template <auto function> requires is_global_function_v<function> &&
            std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
            std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType>
        auto add_constructor (
        ) -> ClassBuilder& {
            thiz.m_constructor = Value::new_value(thiz.m_proto._context());
            thiz.m_constructor.template set_function<function, true>(thiz.m_name);
            Subprojects::quickjs::JS_SetConstructor(thiz.m_constructor._context(), thiz.m_constructor.value(), thiz.m_proto.value());
            thiz.m_parent.define_property(thiz.m_name, Value::new_ref(thiz.m_constructor._context(), thiz.m_constructor.value()));
            return thiz;
        }

        template <auto function>
        auto add_constructor_by_proxy (
        ) -> ClassBuilder& {
            return thiz.template add_constructor<&Javascript::proxy_native_function_wrapper<function>>();
        }

        template<auto function> requires is_global_function_v<function> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
        std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType>
        auto add_member_function (
            const String& name
        ) -> ClassBuilder& {
            thiz.m_proto.template add_function<function, false>(name);
            return thiz;
        }

        template<auto function>
        auto add_member_function_by_proxy (
            const String& name
        ) -> ClassBuilder& {
            return thiz.template add_member_function<&Javascript::proxy_native_function_wrapper<function>>(name);
        }

        template <typename U>
        auto add_member_variable (
            const String& name,
            U&& value
        ) -> ClassBuilder& {
            auto new_value = thiz.m_proto.new_value();
            new_value.template set<U>(std::forward<U>(value));
            thiz.m_proto.define_property(name, new_value.release());
            return thiz;
        }

        template<auto function> requires is_global_function_v<function> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
        std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType>
        auto add_static_function (
            const String& name
        ) -> ClassBuilder& {
            thiz.m_constructor.template add_function<function, false>(name);
            return thiz;
        }

        template<auto function>
        auto add_static_function_by_proxy (
            const String& name
        ) -> ClassBuilder& {
            return thiz.template add_member_function<&Javascript::proxy_native_function_wrapper<function>>(name);
        }

        template <typename U>
        auto add_static_variable (
            const String& name,
            U&& value
        ) -> ClassBuilder& {
            auto new_value = thiz.m_proto.new_value();
            new_value.template set<U>(std::forward<U>(value));
            thiz.m_constructor.define_property(name, new_value.release());
            return thiz;
        }

        template <auto getter, auto setter> requires is_global_function_v<getter> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<getter>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
            std::is_void_v<typename is_global_function<std::decay_t<type_of<getter>>>::ReturnType> && is_global_function_v<setter> &&
            std::is_same_v<typename is_global_function<std::decay_t<type_of<setter>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
            std::is_void_v<typename is_global_function<std::decay_t<type_of<setter>>>::ReturnType>
        auto add_getter_setter (
            const String& name
        ) -> ClassBuilder& {
            auto generator = [this, &name]<auto function> requires is_global_function_v<function> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
            std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType> () -> Value {
                auto func = thiz.m_proto.new_value();
                func.template set_function<function>(name);
                return func;
            };
            thiz.m_proto.define_property(name, generator.template operator()<getter>(), generator.template operator()<setter>());
            return thiz;
        }

    };

    class NamespaceBuilder {

    protected:

        Value m_object;

        explicit NamespaceBuilder(
            const Value& other
        ) : m_object{other.new_reference()} {

        }

    public:

        explicit NamespaceBuilder(
            const String& name,
            const Value& parent
        ) : m_object{} {
            thiz.m_object = parent.new_object();
            parent.define_property(name, thiz.m_object.new_reference());
        }

        auto operator = (
            NamespaceBuilder const & that
        ) -> NamespaceBuilder & = delete;

        auto operator = (
            NamespaceBuilder && that
        ) -> NamespaceBuilder & = default;

        ~NamespaceBuilder (
        ) = default;

        NamespaceBuilder (
        ) = delete;

        NamespaceBuilder (
            NamespaceBuilder const & that
        ) = delete;

        NamespaceBuilder (
            NamespaceBuilder && that
        ) = default;

        auto add_space (
            const String& name
        ) const -> NamespaceBuilder {
            return NamespaceBuilder{name, thiz.m_object};
        }

        auto add_variable (
            const String& name,
            Value && value
        ) -> NamespaceBuilder & {
            thiz.m_object.define_property(name, value.release());
            return thiz;
        }

        template<auto function> requires is_global_function_v<function> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
        std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType>
        auto add_function (
            const String& name
        ) -> NamespaceBuilder & {
            thiz.m_object.add_function<function, false>(name);
            return thiz;
        }

        template <typename T> requires std::is_class_v<T>
        auto add_class (
            const String& name
        ) -> ClassBuilder<T> {
            return ClassBuilder<T>{name, thiz.m_object};
        }

        template<auto function>
        auto add_function_by_proxy (
            const String& name
        ) -> NamespaceBuilder & {
            return thiz.template add_function<&Javascript::proxy_native_function_wrapper<function>>(name);
        }

        static auto get (
            const Value& value
        ) -> NamespaceBuilder {
            return NamespaceBuilder{value};
        }

    };

    inline auto get_namespace (
        const Value& value,
        const String& name
    ) -> NamespaceBuilder {
        assert_conditional(value.has_property(name), "Property does not exist", "get_namespace");
        return NamespaceBuilder::get(value.get_property(name));
    }

}