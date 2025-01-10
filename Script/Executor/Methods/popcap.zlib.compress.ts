namespace Sen.Script.Executor.Methods.PopCap.Zlib.Compress {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		use_64_bit_variant?: boolean;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
		use_64_bit_variant?: boolean;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {
		key?: string;
		iv?: string;
		use_64_bit_variant?: boolean;
	}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.Zlib.Compress.Argument,
			Methods.PopCap.Zlib.Compress.BatchArgument,
			Methods.PopCap.Zlib.Compress.Configuration
		>({
			id: 'popcap.zlib.compress',
			configuration_file: Home.query('~/Executor/Configuration/popcap.zlib.compress.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${argument.source}.bin`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_boolean(
					argument,
					'use_64_bit_variant',
					this.configuration,
					Kernel.Language.get('popcap.zlib.compress.use_64_bit_variant'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.Zlib.compress_fs(
					argument.source,
					argument.destination!,
					argument.use_64_bit_variant!,
				);
				clock.stop_safe();
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.(rsb|obb|bin|xml)$/i],
			option: 71n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.Zlib.Compress.forward();
