namespace Sen.Script.Executor.Methods.Data.MD5.Hash {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
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
			Methods.Data.MD5.Hash.Argument,
			Methods.Data.MD5.Hash.BatchArgument,
			Methods.Data.MD5.Hash.Configuration
		>({
			id: 'data.md5.hash',
			configuration_file: Home.query('~/Executor/Configuration/data.md5.hash.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				clock.start_safe();
				Console.output(Kernel.Encryption.MD5.hash_fs(argument.source));
				clock.stop_safe();
				return;
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: false,
			configuration: undefined!,
			filter: ['file', /.*/],
			option: 2n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.Data.MD5.Hash.forward();
