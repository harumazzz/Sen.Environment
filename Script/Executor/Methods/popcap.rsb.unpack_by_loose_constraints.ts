namespace Sen.Script.Executor.Methods.PopCap.RSB.UnpackByLooseConstraints {
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
			Methods.PopCap.RSB.UnpackByLooseConstraints.Argument,
			Methods.PopCap.RSB.UnpackByLooseConstraints.BatchArgument,
			Methods.PopCap.RSB.UnpackByLooseConstraints.Configuration
		>({
			id: 'popcap.rsb.unpack_by_loose_constraints',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rsb.unpack_by_loose_constraints.json',
			),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${argument.source}.bundle`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RSB.unpack_cipher(argument.source, argument.destination!);
				clock.stop_safe();
				return;
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.*)\.(rsb|obb)$/i],
			option: 57n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.PopCap.RSB.UnpackByLooseConstraints.forward();
