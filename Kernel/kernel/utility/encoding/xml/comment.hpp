#pragma once

#include "kernel/utility/encoding/xml/common.hpp"

namespace Sen::Kernel::Encoding::XML {

    struct Comment {

    protected:

        String m_value{};

    public:

        auto has_value (
        ) const -> bool {
            return !thiz.m_value.empty();
        }

        auto value (
        ) -> String& {
            return thiz.m_value;
        }

    };

}