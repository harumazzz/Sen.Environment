namespace Sen.Script.Executor.Functions.PopCap.PAK {
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

	export function forward(): void {
		inject<
			Functions.PopCap.PAK.Unpack.Argument,
			Functions.PopCap.PAK.Unpack.BatchArgument,
			Functions.PopCap.PAK.Unpack.Configuration
		>({
			id: 'popcap.pak.unpack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.pak.unpack.json'),
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
				Kernel.Support.PopCap.Package.unpack_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.*)\.pak$/i],
			option: 22n,
		});
		inject<
			Functions.PopCap.PAK.Pack.Argument,
			Functions.PopCap.PAK.Pack.BatchArgument,
			Functions.PopCap.PAK.Pack.Configuration
		>({
			id: 'popcap.pak.pack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.pak.pack.json'),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.pak`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.Package.pack_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			configuration: undefined!,
			filter: ['directory', /(.*)\.data_package$/i],
			option: 23n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.PAK.forward();
