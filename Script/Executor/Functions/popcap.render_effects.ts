namespace Sen.Script.Executor.Functions.PopCap.RenderEffects {
	export namespace Decode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Encode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.RenderEffects.Decode.Argument,
			Functions.PopCap.RenderEffects.Decode.BatchArgument,
			Functions.PopCap.RenderEffects.Decode.Configuration
		>({
			id: 'popcap.render_effects.decode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.render_effects.decode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.json`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RenderEffects.decode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.popfx$/i],
			option: 41n,
		});
		inject<
			Functions.PopCap.RenderEffects.Encode.Argument,
			Functions.PopCap.RenderEffects.Encode.BatchArgument,
			Functions.PopCap.RenderEffects.Encode.Configuration
		>({
			id: 'popcap.render_effects.encode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.render_effects.encode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.popfx`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RenderEffects.encode_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.popfx\.json$/i],
			option: 42n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.RenderEffects.forward();
