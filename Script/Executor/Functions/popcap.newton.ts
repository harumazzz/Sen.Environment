namespace Sen.Script.Executor.Functions.PopCap.Newton {
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

	export function forward(): void {
		inject<
			Functions.PopCap.Newton.Decode.Argument,
			Functions.PopCap.Newton.Decode.BatchArgument,
			Functions.PopCap.Newton.Decode.Configuration
		>({
			id: 'popcap.newton.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.newton.decode.json'),
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
				Kernel.Support.PopCap.NewTypeObjectNotation.decode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.newton$/i],
			option: 20n,
		});
		inject<
			Functions.PopCap.Newton.Encode.Argument,
			Functions.PopCap.Newton.Encode.BatchArgument,
			Functions.PopCap.Newton.Encode.Configuration
		>({
			id: 'popcap.newton.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.newton.encode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.newton`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.NewTypeObjectNotation.encode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 21n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.Newton.forward();
