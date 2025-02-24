namespace Sen.Script.Executor.Functions.Marmalade.DZip {
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

	export function forward() {
		inject<
			Functions.Marmalade.DZip.Unpack.Argument,
			Functions.Marmalade.DZip.Unpack.BatchArgument,
			Functions.Marmalade.DZip.Unpack.Configuration
		>({
			id: 'marmalade.dzip.unpack',
			configuration_file: Home.query('~/Executor/Configuration/marmalade.dzip.unpack.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.data_package`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.Marmalade.DZip.unpack_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.*)\.dz$/i],
			option: 2n,
		});
		inject<
			Functions.Marmalade.DZip.Pack.Argument,
			Functions.Marmalade.DZip.Pack.BatchArgument,
			Functions.Marmalade.DZip.Pack.Configuration
		>({
			id: 'marmalade.dzip.pack',
			configuration_file: Home.query('~/Executor/Configuration/marmalade.dzip.pack.json'),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default<Pack.Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.dz`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.Marmalade.DZip.pack_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['directory', /(.*)\.data_package$/i],
			option: 3n,
		});
	}
}

Sen.Script.Executor.Functions.Marmalade.DZip.forward();
