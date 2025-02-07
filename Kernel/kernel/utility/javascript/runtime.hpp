#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/context.hpp"

namespace Sen::Kernel::Javascript {

    struct Runtime {

        protected:

            Pointer<Subprojects::quickjs::JSRuntime> m_runtime;

            bool m_owner;

            explicit Runtime(
                const Pointer<Subprojects::quickjs::JSRuntime>& runtime,
                const bool owner
            ) : m_runtime{runtime}, m_owner{owner} {

            }

        public:

            ~Runtime(

            ) {
                if (thiz.m_owner) {
                    Subprojects::quickjs::JS_FreeRuntime(thiz.m_runtime);
                }
            }

            Runtime (
                Runtime const & that
            ) = delete;

            Runtime (
                Runtime && other
            ) noexcept : m_runtime{other.m_runtime}, m_owner{other.m_owner} {
                other.m_runtime = nullptr;
                other.m_owner = false;
            }

            auto operator = (
                const Runtime& other
            ) -> Runtime & = delete;

            auto operator=(
                Runtime && other
            ) noexcept -> Runtime & {
                thiz.m_runtime = other.m_runtime;
                thiz.m_owner = other.m_owner;
                other.m_runtime = nullptr;
                other.m_owner = false;
                return thiz;
            }

            auto runtime (

            ) -> Pointer<Subprojects::quickjs::JSRuntime>& {
                return thiz.m_runtime;
            }

            auto context (
            ) const -> Context {
                return Context::new_context(thiz.m_runtime);
            }

            inline static auto new_ref (
                const Pointer<Subprojects::quickjs::JSRuntime>& runtime
            ) -> Runtime {
                return Runtime{runtime, false};
            }

            inline static auto new_instance (
                const Pointer<Subprojects::quickjs::JSRuntime>& runtime
            ) -> Runtime {
                return Runtime{runtime, true};
            }

        inline auto has_promise(

        ) const -> bool {
            return static_cast<bool>(Subprojects::quickjs::JS_IsJobPending(thiz.m_runtime));
        }

        inline auto dump_memory_usage(
            std::FILE* fp
        ) const -> void {
            auto mem_usage = Subprojects::quickjs::JSMemoryUsage{};
            Subprojects::quickjs::JS_ComputeMemoryUsage(thiz.m_runtime, &mem_usage);
            Subprojects::quickjs::JS_DumpMemoryUsage(fp, &mem_usage, thiz.m_runtime);
        }

        inline auto execute_pending_job(

        ) const -> void {
            auto job_context = std::add_pointer_t<Subprojects::quickjs::JSContext>{nullptr};
            auto count = Subprojects::quickjs::JS_ExecutePendingJob(thiz.m_runtime, &job_context);
            // TODO : Add context to catch exception
            assert_conditional(count != 0, "Unhandled promise: Promise still not finished", "execute_pending_job");
        }

        inline static auto custom_module_loader(
            Subprojects::quickjs::JSContext* context,
            char const* module_name,
            void* opaque
        ) -> Subprojects::quickjs::JSModuleDef* {
            auto source = String{module_name};
            if (!FileSystem::is_file(source)) {
                Subprojects::quickjs::JS_ThrowInternalError(context, "Cannot read module, import file is missing, path: %s", module_name);
                return nullptr;
            }
            auto file = String{};
            FileSystem::read_file(source, file);
            auto value = Subprojects::quickjs::JS_Eval(context, file.data(), file.size(), module_name, Subprojects::quickjs::$JS_EVAL_TYPE_MODULE | Subprojects::quickjs::$JS_EVAL_FLAG_COMPILE_ONLY | Subprojects::quickjs::$JS_EVAL_FLAG_STRICT);
            if (Subprojects::quickjs::JS_IsException(value)) {
                Subprojects::quickjs::JS_ThrowInternalError(context, "Cannot read module: %s", module_name);
                return nullptr;
            }
            auto definition = static_cast<Subprojects::quickjs::JSModuleDef*>(Subprojects::quickjs::JS_VALUE_GET_POINTER(value));
            Subprojects::quickjs::JS_FreeValue(context, value);
            auto meta = JS_GetImportMeta(context, definition);
            if (Subprojects::quickjs::JS_IsException(meta)) {
                Subprojects::quickjs::JS_ThrowReferenceError(context, "Module cannot be loaded '%s': Failed to get import meta", module_name);
                return nullptr;
            }
            auto atom = Subprojects::quickjs::JS_NewAtomLen(context, "name", 4_size);
            Subprojects::quickjs::JS_DefinePropertyValue(context, meta, atom, JS_NewStringLen(context, module_name, std::strlen(module_name)), Subprojects::quickjs::$JS_PROP_C_W_E);
            Subprojects::quickjs::JS_FreeValue(context, meta);
            Subprojects::quickjs::JS_FreeAtom(context, atom);
            return definition;
        }

    };

    inline auto Context::runtime(
    ) const -> Runtime {
        return Runtime::new_ref(Subprojects::quickjs::JS_GetRuntime(m_context));
    }
}
