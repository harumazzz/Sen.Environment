#pragma once

#include "kernel/utility/stream/output/output_stream.hpp"

namespace Sen::Kernel {

    class BufferedOutputStream final : public std::ostream {

    protected:

        OutputStream* buffer;

    public:

        explicit BufferedOutputStream(OutputStream* buffer) : std::ostream{ buffer }, buffer{ buffer } {
        }

        auto flush(
        ) const -> void {
            buffer->flush();
        }
    };

    template <typename T> requires is_implemented_stream<T>
    inline auto debug_file (
        const std::string_view& filename,
        const T& value
    ) -> void {
        auto output_stream = OutputStream{ filename };
        auto stream = BufferedOutputStream{ &output_stream };
        stream << value;
        stream.flush();
    }

}