#pragma once

#include "kernel/utility/algorithm/image/max_rects/max_rects_bin.hpp"

namespace Sen::Kernel::Image::MaxRects {

    class MaxRectsPacker : Common {

    protected:

        using Option = Option;

        using Node = ExtendedImage;

        template <typename T>
        using List = CList<T>;

    protected:

        u32 width;

        u32 height;

        List<MaxRectsBin> bins;

    public:

        explicit MaxRectsPacker(
            const u32& width,
            const u32& height
        ) : width{width}, height{height}, bins{256_size}
        {

        }

        explicit MaxRectsPacker(
        ) = delete;

        MaxRectsPacker(
            MaxRectsPacker&& other
        ) = delete;

        MaxRectsPacker(
            const MaxRectsPacker& other
        ) = delete;

        auto operator =(
            MaxRectsPacker&& other
        ) -> MaxRectsPacker& = delete;

        auto operator =(
            const MaxRectsPacker& other
        ) -> MaxRectsPacker& = delete;

        ~MaxRectsPacker(
        ) = default;

    protected:

        static auto sort(
            List<Node> &rects
        ) -> void
        {
            if constexpr (option.logic == LOGIC::MAX_EDGE)
            {
                std::ranges::sort(
                    rects,
                    [](auto &a, auto &b) {
                        return Math::max(b.width, b.height) < Math::max(a.width, a.height);
                    });
            }
            else
            {
                std::ranges::sort(
                    rects,
                    [](auto &a, auto &b) {
                        return b.width * b.height < a.width * a.height;
                    });
            }
        }

    public:

        auto clear (
        ) -> void {
            thiz.bins.clear();
        }

        auto add(
            Node &rect
        )  -> bool
        {
            assert_conditional(rect.width <= thiz.width && rect.height <= thiz.height, "rect size greater than packer size", "add");
            for (auto &bin : bins)
            {
                if (bin.add(rect)) {
                    return true;
                }
            }
            auto make_bin = [&]() -> MaxRectsBin {
                auto bin = MaxRectsBin{width, height};
                bin.add(rect);
                return bin;
            };
            thiz.bins.append(make_bin());
            return rect.rect_used;
        }

        auto add_list(
            List<Node> &rects
        ) -> void
        {
            MaxRectsPacker::sort(rects);
            for (auto &rect : rects)
            {
                thiz.add(rect);
            }
        }


    };

}