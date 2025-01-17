namespace Sen.Script.Executor.Methods.PopCap.PAK.Pack {
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
			Methods.PopCap.PAK.Pack.Argument,
			Methods.PopCap.PAK.Pack.BatchArgument,
			Methods.PopCap.PAK.Pack.Configuration
		>({
			id: 'popcap.pak.pack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.pak.pack.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.pak`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.Package.pack_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['directory', /(.*)\.data_package$/i],
			option: 26n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.PAK.Pack.forward();
