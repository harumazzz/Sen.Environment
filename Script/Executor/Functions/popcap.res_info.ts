namespace Sen.Script.Executor.Functions.PopCap.ResInfo {
	export namespace Convert {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			layout?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Split {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Merge {
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
			Functions.PopCap.ResInfo.Convert.Argument,
			Functions.PopCap.ResInfo.Convert.BatchArgument,
			Functions.PopCap.ResInfo.Convert.Configuration
		>({
			id: 'popcap.res_info.convert',
			configuration_file: Home.query('~/Executor/Configuration/popcap.res_info.convert.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.dirname(argument.source)}/resources.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResInfo.convert_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 43n,
		});
		inject<
			Functions.PopCap.ResInfo.Split.Argument,
			Functions.PopCap.ResInfo.Split.BatchArgument,
			Functions.PopCap.ResInfo.Split.Configuration
		>({
			id: 'popcap.res_info.split',
			configuration_file: Home.query('~/Executor/Configuration/popcap.res_info.split.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.info`);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResInfo.split_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 44n,
		});
		inject<
			Functions.PopCap.ResInfo.Merge.Argument,
			Functions.PopCap.ResInfo.Merge.BatchArgument,
			Functions.PopCap.ResInfo.Merge.Configuration
		>({
			id: 'popcap.res_info.merge',
			configuration_file: Home.query('~/Executor/Configuration/popcap.res_info.merge.json'),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					Kernel.Path.except_extension(argument.source),
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResInfo.merge_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			configuration: undefined!,
			filter: ['directory', /.*\.info$/i],
			option: 45n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.ResInfo.forward();
