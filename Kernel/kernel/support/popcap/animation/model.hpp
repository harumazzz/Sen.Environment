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
    };

    struct Dimension {
        u32 width;
        u32 height;

        constexpr explicit Dimension() = default;

        ~Dimension(

            ) = default;
    };

    struct AnimationSize {
        double width;
        double height;

        constexpr explicit AnimationSize() = default;

        ~AnimationSize(

            ) = default;
    };

    struct AnimationImage {
        String path;
        String id;
        Dimension dimension;
        AnimationTransform transform;

        constexpr explicit AnimationImage() = default;

        ~AnimationImage(

            ) = default;
    };

    struct AnimationWorkArea {
        u16 start;
        u16 duration;

        constexpr explicit AnimationWorkArea() = default;

        ~AnimationWorkArea(

            ) = default;
    };

    struct AnimationCommand {
        String command;
        String argument;

        constexpr explicit AnimationCommand() = default;

        ~AnimationCommand(

            ) = default;
    };

    struct AnimationAppend {
        u32 index;
        String name;
        u16 resource;
        bool sprite;
        bool additive;
        u16 preload_frame;
        double time_scale = 1.0;

        constexpr explicit AnimationAppend() = default;

        ~AnimationAppend(

            ) = default;
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
    };

    struct AnimationSprite {
        String name;
        AnimationWorkArea work_area;
        List<AnimationFrame> frame;

        constexpr explicit AnimationSprite() = default;

        ~AnimationSprite(

            ) = default;
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

    };
}

using namespace Sen::Kernel::Support::PopCap::Animation;

JSONCONS_ALL_MEMBER_TRAITS(AnimationPosition, x, y);

JSONCONS_ALL_MEMBER_TRAITS(AnimationSize, width, height);

JSONCONS_ALL_MEMBER_TRAITS(Dimension, width, height);

JSONCONS_ALL_MEMBER_TRAITS(AnimationImage, path, id, dimension, transform);

JSONCONS_ALL_MEMBER_TRAITS(AnimationWorkArea, start, duration);

JSONCONS_ALL_MEMBER_TRAITS(AnimationCommand, command, argument);

JSONCONS_ALL_MEMBER_TRAITS(AnimationChange, index, transform, color, source_rectangle, sprite_frame_number);

JSONCONS_ALL_MEMBER_TRAITS(AnimationAppend, index, name, resource, sprite, additive, preload_frame, time_scale);

JSONCONS_ALL_MEMBER_TRAITS(AnimationFrame, label, stop, remove, command, append, change);

JSONCONS_ALL_MEMBER_TRAITS(AnimationSprite, name, work_area, frame);

JSONCONS_ALL_MEMBER_TRAITS(SexyAnimation, version, frame_rate, position, size, image, sprite, main_sprite);


