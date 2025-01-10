namespace Sen.Script.Executor.Methods.PopCap.RSBPatch.Decode {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		before_file?: string;
		destination?: string;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {}

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
			Methods.PopCap.RSBPatch.Decode.Argument,
			Methods.PopCap.RSBPatch.Decode.BatchArgument,
			Methods.PopCap.RSBPatch.Decode.Configuration
		>({
			id: 'popcap.rsb_patch.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb_patch.decode.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				argument.before_file = Console.path(
					Kernel.Language.get('popcap.rsb_patch.decode.before_file'),
					'file',
				);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.rsb`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RSBPatch.decode_fs(
					argument.source,
					argument.before_file!,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)(\.rsbpatch)$/i],
			option: 52n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.RSBPatch.Decode.forward();
