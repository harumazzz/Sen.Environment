namespace Sen.Script.Executor.Methods.PopCap.Animation.ToAPNG {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		media?: string;
	}
	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {
		resolution: bigint;
	}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.Animation.ToAPNG.Argument,
			Methods.PopCap.Animation.ToAPNG.BatchArgument,
			Methods.PopCap.Animation.ToAPNG.Configuration
		>({
			id: 'popcap.animation.to_apng',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.animation.to_apng.json',
			),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				Console.output(argument.source!);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					Kernel.Path.resolve(`${argument.source}.animation`),
				);
				check_overwrite(argument as { destination: string }, 'directory');
				if (argument.media === undefined) {
					argument.media = Console.path(
						Kernel.Language.get('popcap.animation.to_apng.input_media'),
						'directory',
					);
				}
				const setting: Support.PopCap.Animation.Miscellaenous.GenerateAnimation.Setting = {
					image_id: false,
					frame_name: 'frame',
					sprite_disable: [], // nothing to do
					background_color: [0n, 0n, 0n, 0n], //black with fully transparent,
					rendering_size: {
						width: 0n, // if width, height = 0 use pre-calculated area
						height: 0n,
						scale: 1, // scaling of the animation, it cannot <= 0
					},
					position_additional: {
						x: 0,
						y: 0,
					},
					apng_setting: {
						make_apng: true,
						split_label: true,
						frame_rate: 0n,
						loop: 0n, // 0 - infinity
					},
				};
				const animation: Support.PopCap.Animation.SexyAnimation =
					Kernel.JSON.deserialize_fs<Support.PopCap.Animation.SexyAnimation>(
						argument.source!,
					);
				Support.PopCap.Animation.Miscellaenous.GenerateAnimation.exchange_sprite_disable(
					animation,
					setting,
				);
				Console.output(
					`${Kernel.Language.get('popcap.animation.to_apng.total_frame')}: ${
						animation.main_sprite.frame.length
					}`,
				);
				clock.start_safe();
				Support.PopCap.Animation.Miscellaenous.GenerateAnimation.process(
					animation,
					argument.media!,
					argument.destination!,
					setting,
				);
				clock.stop_safe();
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /.*(\.pam\.json)$/i],
			option: 13n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.Animation.ToAPNG.forward();
