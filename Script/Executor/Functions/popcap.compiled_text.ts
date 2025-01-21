namespace Sen.Script.Executor.Functions.PopCap.CompiledText {
	export namespace Decode {
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

	export namespace Encode {
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
			Functions.PopCap.CompiledText.Encode.Argument,
			Functions.PopCap.CompiledText.Encode.BatchArgument,
			Functions.PopCap.CompiledText.Encode.Configuration
		>({
			id: 'popcap.compiled_text.encode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.compiled_text.encode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bin`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_string(
					argument,
					'key',
					this.configuration,
					Kernel.Language.get('popcap.compiled_text.encode.key'),
				);
				load_string(
					argument,
					'iv',
					this.configuration,
					Kernel.Language.get('popcap.compiled_text.encode.iv'),
				);
				load_boolean(
					argument,
					'use_64_bit_variant',
					this.configuration,
					Kernel.Language.get('popcap.compiled_text.encode.use_64_bit_variant'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.CompiledText.encode_fs(
					argument.source,
					argument.destination!,
					argument.key!,
					argument.iv!,
					argument.use_64_bit_variant!,
				);
				clock.stop_safe();
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.(bin|txt)$/i],
			option: 20n,
		});
		inject<
			Functions.PopCap.CompiledText.Decode.Argument,
			Functions.PopCap.CompiledText.Decode.BatchArgument,
			Functions.PopCap.CompiledText.Decode.Configuration
		>({
			id: 'popcap.compiled_text.decode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.compiled_text.decode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bin`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_string(
					argument,
					'key',
					this.configuration,
					Kernel.Language.get('popcap.compiled_text.decode.key'),
				);
				load_string(
					argument,
					'iv',
					this.configuration,
					Kernel.Language.get('popcap.compiled_text.decode.iv'),
				);
				load_boolean(
					argument,
					'use_64_bit_variant',
					this.configuration,
					Kernel.Language.get('popcap.compiled_text.decode.key'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.CompiledText.decode_fs(
					argument.source,
					argument.destination!,
					argument.key!,
					argument.iv!,
					argument.use_64_bit_variant!,
				);
				clock.stop_safe();
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.txt$/i],
			option: 19n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.CompiledText.forward();
