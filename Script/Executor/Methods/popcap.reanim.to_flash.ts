namespace Sen.Script.Executor.Methods.PopCap.Reanim.ToFlash {
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
			Methods.PopCap.Reanim.ToFlash.Argument,
			Methods.PopCap.Reanim.ToFlash.BatchArgument,
			Methods.PopCap.Reanim.ToFlash.Configuration
		>({
			id: 'popcap.reanim.to_flash',
			configuration_file: Home.query('~/Executor/Configuration/popcap.reanim.to_flash.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.xfl`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.to_flash(argument.source, argument.destination!);
				clock.stop_safe();
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)(\.reanim\.json)$/i],
			option: 42n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.Reanim.ToFlash.forward();
