namespace Sen.Script.Executor.Methods.JS.Evaluate {
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
	 * Detail
	 */

	export namespace Detail {
		/**
		 * If an argument is input, will be stored here
		 */

		export const argument: Array<string> = undefined!;
	}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.JS.Evaluate.Argument,
			Methods.JS.Evaluate.BatchArgument,
			Methods.JS.Evaluate.Configuration
		>({
			id: 'js.evaluate',
			configuration_file: Home.query('~/Executor/Configuration/js.evaluate.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				clock.start_safe();
				const result: string | undefined = Kernel.JavaScript.evaluate_fs(
					argument.source,
				) as unknown & string;
				Console.display(Kernel.Language.get('js.process.done'), result, Color.GREEN);
				clock.stop_safe();
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.js$/i],
			option: 1n,
		});
	}
}

Sen.Script.Executor.Methods.JS.Evaluate.forward();
