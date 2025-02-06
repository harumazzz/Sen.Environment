#pragma once

#include "kernel/utility/algorithm/image/max_rects/extended_image.hpp"
#include "kernel/utility/algorithm/math.hpp"
#include "kernel/utility/algorithm/pointer.hpp"
#include "kernel/utility/algorithm/image/max_rects/common.hpp"

namespace Sen::Kernel::Image::MaxRects {

    struct MaxRectsBin : Common {

    private:

        template <typename T>
        using List = CList<T>;

        using Node = ExtendedImage;

    protected:

        using Common = Common;

        u32 width;

        u32 height;

        u32 max_width;

        u32 max_height;

        bool vertical_expand;

        List<std::shared_ptr<Node>> rects;

        List<std::shared_ptr<Node>> free_rects;

        Node stage;

    public:

        explicit MaxRectsBin(
            const u32& width,
            const u32& height
        ) : width{option.smart ? 1 : width}, height{option.smart ? 1 : height}, max_width{width}, max_height{height},
        vertical_expand{false}, rects{1024_size}, free_rects{1024_size}, stage{width, height}
        {

        }

        explicit MaxRectsBin(
        ) = default;

        ~MaxRectsBin() = default;

        MaxRectsBin(
            MaxRectsBin&& other
        ) noexcept : width{other.width}, height{other.height}, max_width{other.max_width}, max_height{other.max_height}, vertical_expand{other.vertical_expand} {
            thiz.free_rects.take_ownership(other.free_rects);
            thiz.rects.take_ownership(other.rects);
            other.width = 0;
            other.height = 0;
            other.max_width = 0;
            other.max_height = 0;
            other.vertical_expand = false;
        }

        MaxRectsBin(
            const MaxRectsBin& other
        ) = delete;

        auto operator =(
            MaxRectsBin&& other
        ) noexcept -> MaxRectsBin& {
            thiz.width = other.width;
            thiz.height = other.height;
            thiz.max_width = other.max_width;
            thiz.max_height = other.max_height;
            thiz.vertical_expand = other.vertical_expand;
            thiz.free_rects.take_ownership(other.free_rects);
            thiz.rects.take_ownership(other.rects);
            other.width = 0;
            other.height = 0;
            other.max_width = 0;
            other.max_height = 0;
            other.vertical_expand = false;
            return thiz;
        }

        auto operator =(
            const MaxRectsBin& other
        ) -> MaxRectsBin& = delete;

        auto find_node(
            const u32& width,
            const u32& height,
            Node& node
        ) -> void
        {
            auto score = k_score;
            auto area_fit = double{0};
            for (auto &rect : free_rects)
            {
                if (rect->width >= width && rect->height >= height)
                {
                    if constexpr (option.logic == LOGIC::MAX_AREA) {
                        area_fit = rect->width * rect->height - width * height;
                    }
                    else {
                        area_fit = Math::min(rect->width - width, rect->height - height);
                    }
                    if (area_fit < score)
                    {
                        node.width = width;
                        node.height = height;
                        node.x = rect->x;
                        node.y = rect->y;
                        node.source = rect->source;
                        score = area_fit;
                    }
                }
            }
        }

        auto prune_free_list(
        ) -> void
        {
            auto i = 0_size;
            auto j = 0_size;
            auto size = thiz.free_rects.size();
            while (i < size)
            {
                j = i + 1_size;
                auto& temporary_rect1 = thiz.free_rects[i];
                while (j < size)
                {
                    auto& temporary_rect2 = thiz.free_rects[j];
                    if (temporary_rect2->contains(temporary_rect1.operator*()))
                    {
                        thiz.free_rects.pop(i);
                        --i;
                        --size;
                        break;
                    }
                    if (temporary_rect1->contains(temporary_rect2.operator*()))
                    {
                        thiz.free_rects.pop(j);
                        --j;
                        --size;
                    }
                    j++;
                }
                i++;
            }
        }

        auto expand_free_rects(
            const u32& r_width,
            const u32& r_height
        ) -> void
        {
            for (auto &rect : thiz.free_rects)
            {
                if (rect->x + rect->width >= Math::min(width + option.padding - option.border, r_width))
                {
                    rect->width = r_width - rect->x - option.border;
                }
                if (rect->y + rect->height >= Math::min(height + option.padding - option.border, r_height))
                {
                    rect->height = r_height - rect->y - option.border;
                }
            }
            thiz.free_rects.append(make_node(r_width - width - option.padding, r_height - option.border * 2, width + option.padding - option.border, option.border));
            thiz.free_rects.append(make_node(r_width - option.border * 2, r_height - height - option.padding, option.border, height + option.padding - option.border));
            for (auto i : Range{thiz.free_rects.size()})
            {
                auto &rect = rects[i];
                if (rect->width <= 0 || rect->height <= 0 || rect->x < option.border || rect->y < option.border)
                {
                    thiz.rects.pop(i);
                }
            }
            thiz.prune_free_list();
        }

        auto update_bin_size(
            const Node &node
        ) -> bool
        {
            if constexpr (!option.smart)
            {
                return false;
            }
            if (stage.contains(node))
            {
                return false;
            }
            auto temporary_width = Math::max(width, node.x + node.width - option.padding + option.border);
            auto temporary_height = Math::max(height, node.y + node.height - option.padding + option.border);
            if constexpr (option.pot)
            {
                temporary_width = Math::pow(2, Math::ceil(Math::log(temporary_width) * k_log_2e));
                temporary_height = Math::pow(2, Math::ceil(Math::log(temporary_height) * k_log_2e));
            }
            if constexpr (option.square)
            {
                temporary_width = temporary_height = Math::max(temporary_width, temporary_height);
            }
            if (temporary_width > thiz.max_width + option.padding || temporary_height > thiz.max_height + option.padding)
            {
                return false;
            }
            thiz.expand_free_rects(temporary_width + option.padding, temporary_height + option.padding);
            thiz.width = stage.width = temporary_width;
            thiz.height = stage.height = temporary_height;
            return true;
        }

        constexpr static auto can_do_vertical_pack (
            const Node & free_rect,
            const Node & used_node
        ) -> bool {
            return used_node.x < free_rect.x + free_rect.width && used_node.x + used_node.width > free_rect.x;
        }

        constexpr static auto can_do_horizontal_pack (
            const Node & free_rect,
            const Node & used_node
        ) -> bool {
            return used_node.y < free_rect.y + free_rect.height && used_node.y + used_node.height > free_rect.y;
        }

        template <typename... Args>
        auto make_node (
            Args&&... args
        ) -> std::shared_ptr<Node> {
            return std::make_shared<Node>(std::forward<Args>(args)...);
        }

        auto split_node(
            const Node & free_rect,
            const Node & used_node
        ) -> bool
        {
            if (!free_rect.collide(used_node)) {
                return false;
            }
            if (thiz.can_do_vertical_pack(free_rect, used_node)) {
                if (used_node.y > free_rect.y && used_node.y < free_rect.y + free_rect.height) {
                    thiz.free_rects.append(thiz.make_node(free_rect.width, used_node.y - free_rect.y, free_rect.x, free_rect.y, free_rect.source));
                }
                if (used_node.y + used_node.height < free_rect.y + free_rect.height) {
                    thiz.free_rects.append(thiz.make_node(free_rect.width, free_rect.y + free_rect.height - (used_node.y + used_node.height), free_rect.x, used_node.y + used_node.height, free_rect.source));
                }
            }
            if (thiz.can_do_horizontal_pack(free_rect, used_node)) {
                if (used_node.x > free_rect.x && used_node.x < free_rect.x + free_rect.width) {
                    thiz.free_rects.append(thiz.make_node(used_node.x - free_rect.x, free_rect.height, free_rect.x, free_rect.y, free_rect.source));
                }
                if (used_node.x + used_node.width < free_rect.x + free_rect.width) {
                    thiz.free_rects.append(thiz.make_node(free_rect.x + free_rect.width - (used_node.x + used_node.width), free_rect.height, used_node.x + used_node.width, free_rect.y, free_rect.source));
                }
            }
            return true;
        }

        constexpr auto can_expand_vertical (
        ) -> void {
            thiz.vertical_expand = thiz.width > thiz.height;
        }

        auto place(
            Node* rect,
            std::shared_ptr<Node>& place_node
        ) -> void
            {
                auto node = make_node();
                thiz.find_node(rect->width + option.padding, rect->height + option.padding, node.operator*());
                if (node->rect_used) {
                    thiz.update_bin_size(node.operator*());
                    auto total_rects = thiz.free_rects.size();
                    auto i = 0_size;
                    while (i < total_rects)
                    {
                        if (thiz.split_node(thiz.free_rects[i].operator*(), node.operator*()))
                        {
                            thiz.free_rects.pop(i);
                            --total_rects;
                            --i;
                        }
                        ++i;
                    }
                    thiz.prune_free_list();
                    thiz.can_expand_vertical();
                    rect->x = node->x;
                    rect->y = node->y;
                    place_node.reset(rect, [](Node* _) {});
                    return;
                }
                auto node1 = Node{rect->width + option.padding, rect->height + option.padding, width + option.padding - option.border, option.border, rect->source};
                auto node2 = Node{rect->width + option.padding, rect->height + option.padding, option.border, height + option.padding - option.border, rect->source};
                if (thiz.update_bin_size(node1) || thiz.update_bin_size(node2)) {
                    thiz.place(rect, place_node);
                }
            }

            auto add (
                Node &rect
            ) -> bool
            {
                auto place_node = make_node();
                thiz.place(&rect, place_node);
                if (place_node->rect_used)
                {
                    thiz.rects.append(as_move(place_node));
                }
                return place_node->rect_used;
            }

    };

}