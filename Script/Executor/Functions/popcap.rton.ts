namespace Sen.Script.Executor.Functions.PopCap.RTON {
	export namespace Decode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace ForceDecode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace DecryptAndDecode {
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

	export namespace Decrypt {
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

	export namespace EncodeAndEncrypt {
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
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Encrypt {
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

	export namespace Detail {
		export function prepare<T extends Base, U extends Configuration>(
			argument: T,
			configuration: U,
		): void {
			load_string(
				argument,
				'key',
				configuration,
				Kernel.Language.get('popcap.rton.decrypt.key'),
			);
			load_string(
				argument,
				'iv',
				configuration,
				Kernel.Language.get('popcap.rton.decrypt.iv'),
			);
		}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.RTON.Decode.Argument,
			Functions.PopCap.RTON.Decode.BatchArgument,
			Functions.PopCap.RTON.Decode.Configuration
		>({
			id: 'popcap.rton.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rton.decode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ReflectionObjectNotation.decode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.rton|pp\.dat)$/i],
			option: 59n,
		});
		inject<
			Functions.PopCap.RTON.Encode.Argument,
			Functions.PopCap.RTON.Encode.BatchArgument,
			Functions.PopCap.RTON.Encode.Configuration
		>({
			id: 'popcap.rton.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rton.encode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.rton`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ReflectionObjectNotation.encode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 60n,
		});
		inject<
			Functions.PopCap.RTON.Decrypt.Argument,
			Functions.PopCap.RTON.Decrypt.BatchArgument,
			Functions.PopCap.RTON.Decrypt.Configuration
		>({
			id: 'popcap.rton.decrypt',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rton.decrypt.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bin`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				Detail.prepare(argument, this.configuration);
				clock.start_safe();
				Kernel.Support.PopCap.ReflectionObjectNotation.decrypt_fs(
					argument.source,
					argument.destination!,
					argument.key!,
					argument.iv!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.rton|pp\.dat)$/i],
			option: 61n,
		});
		inject<
			Functions.PopCap.RTON.Encrypt.Argument,
			Functions.PopCap.RTON.Encrypt.BatchArgument,
			Functions.PopCap.RTON.Encrypt.Configuration
		>({
			id: 'popcap.rton.encrypt',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rton.encrypt.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bin`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				Detail.prepare(argument, this.configuration);
				clock.start_safe();
				Kernel.Support.PopCap.ReflectionObjectNotation.encrypt_fs(
					argument.source,
					argument.destination!,
					argument.key!,
					argument.iv!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.rton|pp\.dat)$/i],
			option: 62n,
		});
		inject<
			Functions.PopCap.RTON.DecryptAndDecode.Argument,
			Functions.PopCap.RTON.DecryptAndDecode.BatchArgument,
			Functions.PopCap.RTON.DecryptAndDecode.Configuration
		>({
			id: 'popcap.rton.decrypt_and_decode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rton.decrypt_and_decode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				Detail.prepare(argument, this.configuration);
				clock.start_safe();
				Kernel.Support.PopCap.ReflectionObjectNotation.decrypt_and_decode_fs(
					argument.source,
					argument.destination!,
					argument.key!,
					argument.iv!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.rton|pp\.dat)$/i],
			option: 63n,
		});
		inject<
			Functions.PopCap.RTON.EncodeAndEncrypt.Argument,
			Functions.PopCap.RTON.EncodeAndEncrypt.BatchArgument,
			Functions.PopCap.RTON.EncodeAndEncrypt.Configuration
		>({
			id: 'popcap.rton.encode_and_encrypt',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rton.encode_and_encrypt.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.rton`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				Detail.prepare(argument, this.configuration);
				clock.start_safe();
				Kernel.Support.PopCap.ReflectionObjectNotation.encode_and_encrypt_fs(
					argument.source,
					argument.destination!,
					argument.key!,
					argument.iv!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 64n,
		});
		inject<
			Functions.PopCap.RTON.ForceDecode.Argument,
			Functions.PopCap.RTON.ForceDecode.BatchArgument,
			Functions.PopCap.RTON.ForceDecode.Configuration
		>({
			id: 'popcap.rton.force_decode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rton.force_decode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Support.PopCap.ReflectionObjectNotation.DecodeByLooseConstraints.process_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.rton|pp\.dat)$/i],
			option: 65n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.RTON.forward();
