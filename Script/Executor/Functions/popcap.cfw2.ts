namespace Sen.Script.Executor.Functions.PopCap.CFW2 {
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
			Functions.PopCap.CFW2.Encode.Argument,
			Functions.PopCap.CFW2.Encode.BatchArgument,
			Functions.PopCap.CFW2.Encode.Configuration
		>({
			id: 'popcap.cfw2.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.cfw2.encode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.cfw2`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.CharacterFontWidget2.encode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /.*\.txt\.json$/i],
			option: 14n,
		});
		inject<
			Functions.PopCap.CFW2.Decode.Argument,
			Functions.PopCap.CFW2.Decode.BatchArgument,
			Functions.PopCap.CFW2.Decode.Configuration
		>({
			id: 'popcap.cfw2.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.cfw2.decode.json'),
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
				Kernel.Support.PopCap.CharacterFontWidget2.decode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.cfw2$/i],
			option: 15n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.CFW2.forward();
