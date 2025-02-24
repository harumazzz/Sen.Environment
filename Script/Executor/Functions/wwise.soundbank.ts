namespace Sen.Script.Executor.Functions.WWise.SoundBank {
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

	export function forward(): void {
		inject<
			Functions.WWise.SoundBank.Decode.Argument,
			Functions.WWise.SoundBank.Decode.BatchArgument,
			Functions.WWise.SoundBank.Decode.Configuration
		>({
			id: 'wwise.soundbank.decode',
			configuration_file: Home.query('~/Executor/Configuration/wwise.soundbank.decode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.soundbank`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.WWise.SoundBank.decode_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.bnk$/i],
			option: 74n,
		});
		inject<
			Functions.WWise.SoundBank.Encode.Argument,
			Functions.WWise.SoundBank.Encode.BatchArgument,
			Functions.WWise.SoundBank.Encode.Configuration
		>({
			id: 'wwise.soundbank.encode',
			configuration_file: Home.query('~/Executor/Configuration/wwise.soundbank.encode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.bnk`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.WWise.SoundBank.encode_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, true);
			},

			configuration: undefined!,
			filter: ['directory', /(.+)\.soundbank$/gi],
			option: 75n,
		});
	}
}

Sen.Script.Executor.Functions.WWise.SoundBank.forward();
