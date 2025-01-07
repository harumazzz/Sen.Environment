namespace Sen.Script.Executor.Methods.PopCap.RenderEffects.Decode {
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
			Methods.PopCap.RenderEffects.Decode.Argument,
			Methods.PopCap.RenderEffects.Decode.BatchArgument,
			Methods.PopCap.RenderEffects.Decode.Configuration
		>({
			id: 'popcap.render_effects.decode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.render_effects.decode.json',
			),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${argument.source}.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RenderEffects.decode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
				return;
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.popfx$/i],
			option: 44n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.PopCap.RenderEffects.Decode.forward();
