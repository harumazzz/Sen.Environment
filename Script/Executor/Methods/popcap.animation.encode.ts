namespace Sen.Script.Executor.Methods.PopCap.Animation.Encode {
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
			Methods.PopCap.Animation.Encode.Argument,
			Methods.PopCap.Animation.Encode.BatchArgument,
			Methods.PopCap.Animation.Encode.Configuration
		>({
			id: 'popcap.animation.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.animation.encode.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.Animation.encode_fs(argument.source, argument.destination!);
				clock.stop_safe();
				return;
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /.*\.pam\.json$/i],
			option: 9n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.PopCap.Animation.Encode.forward();
