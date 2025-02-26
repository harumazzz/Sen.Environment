namespace Sen.Script.Executor.Functions.JS {
	export namespace Evaluate {
		export interface Argument extends Executor.Base {
			source: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export function forward(): void {
		inject<
			Functions.JS.Evaluate.Argument,
			Functions.JS.Evaluate.BatchArgument,
			Functions.JS.Evaluate.Configuration
		>({
			id: 'js.evaluate',
			configuration_file: Home.query('~/Executor/Configuration/js.evaluate.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				clock.start_safe();
				const result: string | undefined = Kernel.JavaScript.evaluate_fs(
					argument.source,
				) as unknown & string;
				Console.display(Kernel.Language.get('js.process.done'), 'green', result);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.js$/i],
			option: 1n,
		});
	}
}

Sen.Script.Executor.Functions.JS.forward();
