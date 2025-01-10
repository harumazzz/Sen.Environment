namespace Sen.Script.Executor.Methods.PopCap.RSBPatch.Encode {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		after_file?: string;
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
			Methods.PopCap.RSBPatch.Encode.Argument,
			Methods.PopCap.RSBPatch.Encode.BatchArgument,
			Methods.PopCap.RSBPatch.Encode.Configuration
		>({
			id: 'popcap.rsb_patch.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb_patch.encode.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				argument.after_file = Console.path(
					Kernel.Language.get('popcap.rsb_patch.encode.after_file'),
					'file',
				);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.after_file!)}.rsbpatch`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RSBPatch.encode_fs(
					argument.source,
					argument.after_file!,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)(\.rsb|\.obb)$/i],
			option: 53n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.RSBPatch.Encode.forward();
