#pragma once

#include "explorer/command/method_invoke.hpp"

namespace Sen::Environment {

    namespace Detail {

        inline auto const launch = MethodInvoke{
            .name = L"Launch",
            .type = std::nullopt,
            .rule = std::nullopt,
            .method = std::nullopt,
            .argument = LR"({})",
        };

        inline auto const js_evaluate = MethodInvoke{
            .name = L"JS Evaluate",
            .type = false,
            .rule = std::wregex{LR"(.+(\.js)$)", std::wregex::icase},
            .method = L"js.evaluate",
            .argument = LR"({})",
        };

    }

    class __declspec(uuid("A918A8A8-1436-43C5-99C1-9420D0D56D40")) LaunchMethodInvokeCommand :
        public MethodInvokeCommand {
            public:
            LaunchMethodInvokeCommand () :
                MethodInvokeCommand{Detail::launch} {
            }
        };

    class __declspec(uuid("314F8A42-AC6F-4192-8F9A-D2DF1C9F0546")) JSEvaluateMethodInvokeCommand :
        public MethodInvokeCommand {
            public:
            JSEvaluateMethodInvokeCommand () :
                MethodInvokeCommand{Detail::js_evaluate} {
            }
        };

}