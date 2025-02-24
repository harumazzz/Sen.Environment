namespace Sen.Script.Executor.Functions.PopCap.RSB {
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

	export namespace PackResource {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace ConvertPlatform {
		export type Generic = 0n | 1n;

		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			generic?: Generic;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			generic: 1n | 2n | '?';
		}
	}

	export namespace ForceUnpack {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace UnpackResource {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Detail {
		export function generic(): Array<[bigint, bigint, string]> {
			return [
				[1n, 0n, Kernel.Language.get('popcap.rsb.convert_platform.convert_android_to_ios')],
				[2n, 1n, Kernel.Language.get('popcap.rsb.convert_platform.convert_ios_to_android')],
			];
		}
	}

	export namespace Obfuscate {
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
			Functions.PopCap.RSB.Unpack.Argument,
			Functions.PopCap.RSB.Unpack.BatchArgument,
			Functions.PopCap.RSB.Unpack.Configuration
		>({
			id: 'popcap.rsb.unpack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb.unpack.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bundle`);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceStreamBundle.unpack_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.*)\.(rsb|obb)$/i],
			option: 51n,
		});
		inject<
			Functions.PopCap.RSB.Pack.Argument,
			Functions.PopCap.RSB.Pack.BatchArgument,
			Functions.PopCap.RSB.Pack.Configuration
		>({
			id: 'popcap.rsb.pack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb.pack.json'),
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
				Kernel.Support.PopCap.ResourceStreamBundle.pack_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,

			configuration: undefined!,
			filter: ['directory', /(.*)\.bundle$/i],
			option: 52n,
		});
		inject<
			Functions.PopCap.RSB.UnpackResource.Argument,
			Functions.PopCap.RSB.UnpackResource.BatchArgument,
			Functions.PopCap.RSB.UnpackResource.Configuration
		>({
			id: 'popcap.rsb.unpack_resource',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rsb.unpack_resource.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bundle`);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceStreamBundle.unpack_resource(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.*)\.(rsb|obb)$/i],
			option: 53n,
		});
		inject<
			Functions.PopCap.RSB.PackResource.Argument,
			Functions.PopCap.RSB.PackResource.BatchArgument,
			Functions.PopCap.RSB.PackResource.Configuration
		>({
			id: 'popcap.rsb.pack_resource',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rsb.pack_resource.json',
			),
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
				Kernel.Support.PopCap.ResourceStreamBundle.pack_resource(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,

			configuration: undefined!,
			filter: ['directory', /(.*)\.bundle$/i],
			option: 54n,
		});
		inject<
			Functions.PopCap.RSB.ConvertPlatform.Argument,
			Functions.PopCap.RSB.ConvertPlatform.BatchArgument,
			Functions.PopCap.RSB.ConvertPlatform.Configuration
		>({
			id: 'popcap.rsb.convert_platform',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rsb.convert_platform.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bundle`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'generic',
					this.configuration,
					Detail.generic(),
					Kernel.Language.get('popcap.rsb.convert_platform.platform_convert'),
				);
				clock.start_safe();
				Support.PopCap.ResourceStreamBundle.Miscellaneous.PlatformConverter.process_fs(
					argument.source,
					argument.destination!,
					argument.generic! !== 1n,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.*)\.(rsb|obb)$/i],
			option: 55n,
		});
		inject<
			Functions.PopCap.RSB.ForceUnpack.Argument,
			Functions.PopCap.RSB.ForceUnpack.BatchArgument,
			Functions.PopCap.RSB.ForceUnpack.Configuration
		>({
			id: 'popcap.rsb.force_unpack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb.force_unpack.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bundle`);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceStreamBundle.unpack_cipher(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.*)\.(rsb|obb)$/i],
			option: 56n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.RSB.forward();
