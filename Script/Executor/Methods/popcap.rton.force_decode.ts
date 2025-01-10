namespace Sen.Script.Executor.Methods.PopCap.RTON.ForceDecode {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.RTON.ForceDecode.Argument,
			Methods.PopCap.RTON.ForceDecode.BatchArgument,
			Methods.PopCap.RTON.ForceDecode.Configuration
		>({
			id: 'popcap.rton.force_decode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rton.force_decode.json',
			),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
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
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)(\.rton|pp\.dat)$/i],
			option: 70n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.RTON.ForceDecode.forward();
