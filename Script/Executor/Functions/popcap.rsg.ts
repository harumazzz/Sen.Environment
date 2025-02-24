namespace Sen.Script.Executor.Functions.PopCap.RSG {
	export namespace Pack {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Unpack {
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
			Functions.PopCap.RSG.Pack.Argument,
			Functions.PopCap.RSG.Pack.BatchArgument,
			Functions.PopCap.RSG.Pack.Configuration
		>({
			id: 'popcap.rsg.pack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsg.pack.json'),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.rsg`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceStreamGroup.pack_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, true);
			},

			configuration: undefined!,
			filter: ['directory', /(.*)\.packet$/i],
			option: 57n,
		});
		inject<
			Functions.PopCap.RSG.Unpack.Argument,
			Functions.PopCap.RSG.Unpack.BatchArgument,
			Functions.PopCap.RSG.Unpack.Configuration
		>({
			id: 'popcap.rsg.unpack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsg.unpack.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.packet`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceStreamGroup.unpack_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.*)\.rsg$/i],
			option: 58n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.RSG.forward();
