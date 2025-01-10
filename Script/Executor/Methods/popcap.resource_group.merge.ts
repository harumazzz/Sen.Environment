namespace Sen.Script.Executor.Methods.PopCap.ResourceGroup.Merge {
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
			Methods.PopCap.ResourceGroup.Merge.Argument,
			Methods.PopCap.ResourceGroup.Merge.BatchArgument,
			Methods.PopCap.ResourceGroup.Merge.Configuration
		>({
			id: 'popcap.resource_group.merge',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.resource_group.merge.json',
			),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					Kernel.Path.except_extension(argument.source),
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceGroup.merge_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['directory', /.*\.info$/i],
			option: 50n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.ResourceGroup.Merge.forward();
