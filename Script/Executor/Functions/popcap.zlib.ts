namespace Sen.Script.Executor.Functions.PopCap.Zlib {
	export namespace Compress {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			use_64_bit_variant?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			use_64_bit_variant?: boolean;
		}

		export interface Configuration extends Executor.Configuration {
			key?: string;
			iv?: string;
			use_64_bit_variant?: boolean;
		}
	}

	export namespace Uncompress {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			key?: string;
			iv?: string;
			use_64_bit_variant?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			key?: string;
			iv?: string;
			use_64_bit_variant?: boolean;
		}

		export interface Configuration extends Executor.Configuration {
			key?: string;
			iv?: string;
			use_64_bit_variant?: boolean;
		}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.Zlib.Compress.Argument,
			Functions.PopCap.Zlib.Compress.BatchArgument,
			Functions.PopCap.Zlib.Compress.Configuration
		>({
			id: 'popcap.zlib.compress',
			configuration_file: Home.query('~/Executor/Configuration/popcap.zlib.compress.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bin`);
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
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.(rsb|obb|bin|xml)$/i],
			option: 66n,
		});
		inject<
			Functions.PopCap.Zlib.Uncompress.Argument,
			Functions.PopCap.Zlib.Uncompress.BatchArgument,
			Functions.PopCap.Zlib.Uncompress.Configuration
		>({
			id: 'popcap.zlib.uncompress',
			configuration_file: Home.query('~/Executor/Configuration/popcap.zlib.uncompress.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bin`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_boolean(
					argument,
					'use_64_bit_variant',
					this.configuration,
					Kernel.Language.get('popcap.zlib.uncompress.use_64_bit_variant'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.Zlib.uncompress_fs(
					argument.source,
					argument.destination!,
					argument.use_64_bit_variant!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.(compiled|smf|bin)$/i],
			option: 67n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.Zlib.forward();
