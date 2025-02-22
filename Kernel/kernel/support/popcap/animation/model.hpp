#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::Animation {

    using namespace jsoncons;

    using AnimationColor = Array<double, 4>;

    using SourceRectangle = Array<double, 4>;

    using AnimationTransform = List<double>;

    struct AnimationPosition {
        double x;
        double y;

        constexpr explicit AnimationPosition() = default;

        ~AnimationPosition(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationPosition& other
        ) -> std::ostream& {
            os << "AnimationPosition(" << other.x << ", " << other.y << ")";
            return os;
        }
    };

    struct Dimension {
        u32 width;
        u32 height;

        constexpr explicit Dimension() = default;

        ~Dimension(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const Dimension& other
        ) -> std::ostream& {
            os << "Dimension(" << other.width << ", " << other.height << ")";
            return os;
        }
    };

    struct AnimationSize {
        double width;
        double height;

        constexpr explicit AnimationSize() = default;

        ~AnimationSize(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationSize& other
        ) -> std::ostream& {
            os << "AnimationSize(" << other.width << ", " << other.height << ")";
            return os;
        }
    };

    struct AnimationImage {
        String path;
        String id;
        Dimension dimension;
        AnimationTransform transform;

        constexpr explicit AnimationImage() = default;

        ~AnimationImage(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationImage& other
        ) -> std::ostream& {
            os << "AnimationImage(" << other.path << ", " << other.id << ", " << other.dimension << ", " << other.transform << ")";
            return os;
        }
    };

    struct AnimationWorkArea {
        u16 start;
        u16 duration;

        constexpr explicit AnimationWorkArea() = default;

        ~AnimationWorkArea(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationWorkArea& other
        ) -> std::ostream& {
            os << "AnimationWorkArea(" << other.start << ", " << other.duration << ")";
            return os;
        }
    };

    struct AnimationCommand {
        String command;
        String argument;

        constexpr explicit AnimationCommand() = default;

        ~AnimationCommand(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationCommand& other
        ) -> std::ostream& {
            os << "AnimationCommand(" << other.command << ", " << other.argument << ")";
            return os;
        }
    };

    struct AnimationAppend {
        u32 index;
        String name;
        u16 resource;
        bool sprite;
        bool additive;
        u16 preload_frame;
        double time_scale{1.0};

        constexpr explicit AnimationAppend() = default;

        ~AnimationAppend(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationAppend& other
        ) -> std::ostream& {
            os << "AnimationAppend(" << other.index << ", " << other.name << ", " << other.resource << ", " <<
                other.sprite << ", " << other.additive << ", " << other.preload_frame << ", " << other.time_scale << ")";
            return os;
        }
    };

    struct AnimationChange {
        u32 index;
        AnimationTransform transform;
        Optional<AnimationColor> color; // This make null
        Optional<SourceRectangle> source_rectangle;
        u16 sprite_frame_number;

        constexpr explicit AnimationChange() = default;

        ~AnimationChange(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationChange& other
        ) -> std::ostream& {
            os << "AnimationChange(" << other.index << ", " << other.transform << ", " << other.color << ", " <<
                other.source_rectangle << ", " << other.sprite_frame_number << ")";
            return os;
        }
    };

    struct AnimationFrame {
        String label;
        bool stop;
        List<u32> remove;
        List<AnimationCommand> command;
        List<AnimationAppend> append;
        List<AnimationChange> change;

        constexpr explicit AnimationFrame() = default;

        ~AnimationFrame(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationFrame& other
        ) -> std::ostream& {
            os << "AnimationFrame(" << other.label << ", " << other.stop << ", " << other.remove << ", " <<
                other.command << ", " << other.append << ", " << other.change << ")";
            return os;
        }
    };

    struct AnimationSprite {
        String name;
        AnimationWorkArea work_area;
        List<AnimationFrame> frame;

        constexpr explicit AnimationSprite() = default;

        ~AnimationSprite(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const AnimationSprite& other
        ) -> std::ostream& {
            os << "AnimationSprite(" << other.name << ", " << other.work_area << ", " << other.frame << ")";
            return os;
        }
    };

    struct SexyAnimation {
        u32 version;
        u8 frame_rate;
        AnimationPosition position;
        AnimationSize size;
        List<AnimationImage> image;
        List<AnimationSprite> sprite;
        AnimationSprite main_sprite;

        constexpr explicit SexyAnimation() = default;

        ~SexyAnimation(

        ) = default;

        friend auto operator << (
            std::ostream& os,
            const SexyAnimation& other
        ) -> std::ostream& {
            os << "SexyAnimation(" << other.version << ", " << other.frame_rate << ", " << other.position
            << ", " << other.size << ", " << other.image << ", " << other.sprite << ", " << other.main_sprite << ")";
            return os;
        }

    };
}

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationPosition, x, y);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationSize, width, height);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::Dimension, width, height);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationImage, path, id, dimension, transform);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationWorkArea, start, duration);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationCommand, command, argument);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationChange, index, transform, color, source_rectangle, sprite_frame_number);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationAppend, index, name, resource, sprite, additive, preload_frame, time_scale);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationFrame, label, stop, remove, command, append, change);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::AnimationSprite, name, work_area, frame);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::Animation::SexyAnimation, version, frame_rate, position, size, image, sprite, main_sprite);


