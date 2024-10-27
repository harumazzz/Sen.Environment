namespace Sen.Script.Executor.Methods.PopCap.Animation.AddLibrary {
    /**
     * Argument for the current method
     */

    export interface Argument extends Sen.Script.Executor.Base {
        source: string;
        image_posix?: string;
        additional?: Array<Support.PopCap.Animation.Miscellaenous.AddLibrary.Additional>;
    }

    /**
     * Argument for batch method
     */

    export interface BatchArgument extends Sen.Script.Executor.Base {
        directory: string;
    }

    /**
     * Configuration file if needed
     */

    export interface Configuration extends Sen.Script.Executor.Configuration {}

    /**
     * ----------------------------------------------
     * JavaScript forward method, this method need
     * to be evaluated during script loading time
     * ----------------------------------------------
     */

    export function forward(): void {
        Sen.Script.Executor.push_as_module<
            Sen.Script.Executor.Methods.PopCap.Animation.AddLibrary.Argument,
            Sen.Script.Executor.Methods.PopCap.Animation.AddLibrary.BatchArgument,
            Sen.Script.Executor.Methods.PopCap.Animation.AddLibrary.Configuration
        >({
            id: "popcap.animation.add_library",
            configuration_file: Home.query("~/Executor/Configuration/popcap.animation.add_library.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, true);
                Console.obtained(argument.source);
                Console.output(argument.source);
                let image_source: string | Array<string> = Console.path(Kernel.Language.get("popcap.animation.add_library.input_png_path"), "any");
                if (Kernel.FileSystem.is_directory(image_source)) {
                    image_source = Kernel.FileSystem.read_directory(image_source);
                }
                Console.argument(Kernel.Language.get("popcap.animation.add_library.image_posix_for_id"));
                argument.image_posix = Kernel.Console.readline();
                Console.argument(Kernel.Language.get("popcap.animation.add_library.generate_sprite"));
                const has_sprite = Executor.input_boolean();
                const input_sprite = (media: string) => {
                    let sprite: string | undefined = undefined;
                    if (has_sprite) {
                        Console.argument(`${Kernel.Language.get("popcap.animation.add_library.sprite_name")} ${media}`);
                        sprite = has_sprite ? Kernel.Console.readline() : undefined;
                    }
                    return sprite;
                };
                if (Array.isArray(image_source)) {
                    argument.additional = image_source.map(function process_source(e, i) {
                        const media = Kernel.Path.base_without_extension(e);
                        const sprite = input_sprite(media);
                        return {
                            original_path: e,
                            image: media,
                            media: media,
                            sprite: sprite,
                        };
                    });
                } else {
                    const media = Kernel.Path.base_without_extension(image_source);
                    const sprite = input_sprite(media);
                    argument.additional = [
                        {
                            original_path: image_source,
                            image: media,
                            media: media,
                            sprite: sprite,
                        },
                    ];
                }
                clock.start_safe();
                Support.PopCap.Animation.Miscellaenous.AddLibrary.process_fs(argument.source, argument.image_posix!, argument.additional);
                clock.stop_safe();
                return;
            },
            batch_forward(argument: BatchArgument): void {
                return basic_batch(this, argument, false);
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["directory", /.*/g],
            option: 79n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.PopCap.Animation.AddLibrary.forward();
