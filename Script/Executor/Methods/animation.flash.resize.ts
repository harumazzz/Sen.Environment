namespace Sen.Script.Executor.Methods.Animation.Flash.Resize {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		resolution?: bigint;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
		resolution?: bigint;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {
		resolution: bigint;
	}

	/**
	 * Detail
	 */

	export namespace Detail {
		/**
		 * Input resolution are supported covered here
		 */

		export const resolution = (): Array<[bigint, bigint, string]> => {
			return [
				[1n, 1536n, Kernel.Language.get('popcap.animation.to_flash.resolution.1536n')],
				[2n, 768n, Kernel.Language.get('popcap.animation.to_flash.resolution.768n')],
				[3n, 384n, Kernel.Language.get('popcap.animation.to_flash.resolution.384n')],
				[4n, 1200n, Kernel.Language.get('popcap.animation.to_flash.resolution.1200n')],
				[5n, 640n, Kernel.Language.get('popcap.animation.to_flash.resolution.640n')],
			];
		};
	}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.Animation.Flash.Resize.Argument,
			Methods.Animation.Flash.Resize.BatchArgument,
			Methods.Animation.Flash.Resize.Configuration
		>({
			id: 'animation.flash.resize',
			configuration_file: Home.query('~/Executor/Configuration/animation.flash.resize.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				Console.output(argument.source!);
				load_bigint(
					argument,
					'resolution',
					this.configuration,
					Detail.resolution(),
					Kernel.Language.get('popcap.animation.to_flash.resolution'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.Animation.Miscellaneous.resize_fs(
					argument.source,
					BigInt(argument.resolution!),
				);
				clock.stop_safe();
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['directory', /.*(\.xfl|\.pam\.xfl)$/i],
			option: 7n,
		});
	}
}

Sen.Script.Executor.Methods.Animation.Flash.Resize.forward();
