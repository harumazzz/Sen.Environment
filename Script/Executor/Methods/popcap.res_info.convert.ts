namespace Sen.Script.Executor.Methods.PopCap.ResInfo.Convert {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		layout?: string;
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
			Methods.PopCap.ResInfo.Convert.Argument,
			Methods.PopCap.ResInfo.Convert.BatchArgument,
			Methods.PopCap.ResInfo.Convert.Configuration
		>({
			id: 'popcap.res_info.convert',
			configuration_file: Home.query('~/Executor/Configuration/popcap.res_info.convert.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.dirname(argument.source)}/resources.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResInfo.convert_fs(argument.source, argument.destination!);
				clock.stop_safe();
				return;
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 46n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.PopCap.ResInfo.Convert.forward();
