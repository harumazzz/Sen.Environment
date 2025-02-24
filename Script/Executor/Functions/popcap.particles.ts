namespace Sen.Script.Executor.Functions.PopCap.Particles {
	export type Platform = Kernel.Support.PopCap.Particles.Platform;
	export namespace Decode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			platform?: Platform;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			platform?: Platform;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Encode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			platform?: Platform;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			platform?: Platform;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace ToXML {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace FromXML {
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
		export const _platform: Array<Kernel.Support.PopCap.Particles.Platform> = [
			'pc',
			'game-console',
			'phone-32',
			'phone-64',
			'tv',
		];

		export function platform(): Array<[bigint, string, string]> {
			return _platform.map((e, i) => [
				BigInt(i + 1),
				e as string,
				Kernel.Language.get(`popcap.reanim.platform.${e}`),
			]);
		}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.Particles.Decode.Argument,
			Functions.PopCap.Particles.Decode.BatchArgument,
			Functions.PopCap.Particles.Decode.Configuration
		>({
			id: 'popcap.particles.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.particles.decode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.json`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				Console.argument(Kernel.Language.get('popcap.particles.decode.generic'));
				configurate_or_input(argument, 'platform', Detail.platform());
				clock.start_safe();
				Kernel.Support.PopCap.Particles.decode_fs(
					argument.source,
					argument.destination!,
					argument.platform!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.xml|\.xml\.compiled)$/i],
			option: 24n,
		});
		inject<
			Functions.PopCap.Particles.Encode.Argument,
			Functions.PopCap.Particles.Encode.BatchArgument,
			Functions.PopCap.Particles.Encode.Configuration
		>({
			id: 'popcap.particles.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.particles.encode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.compiled`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				Console.argument(Kernel.Language.get('popcap.particles.encode.generic'));
				configurate_or_input(argument, 'platform', Detail.platform());
				clock.start_safe();
				Kernel.Support.PopCap.Particles.encode_fs(
					argument.source,
					argument.destination!,
					argument.platform!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.xml)\.json$/i],
			option: 25n,
		});
		inject<
			Functions.PopCap.Particles.ToXML.Argument,
			Functions.PopCap.Particles.ToXML.BatchArgument,
			Functions.PopCap.Particles.ToXML.Configuration
		>({
			id: 'popcap.particles.to_xml',
			configuration_file: Home.query('~/Executor/Configuration/popcap.particles.to_xml.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.Particles.to_xml(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.xml)\.json$/i],
			option: 26n,
		});
		inject<
			Functions.PopCap.Particles.FromXML.Argument,
			Functions.PopCap.Particles.FromXML.BatchArgument,
			Functions.PopCap.Particles.FromXML.Configuration
		>({
			id: 'popcap.particles.from_xml',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.particles.from_xml.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.xml.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.Particles.from_xml(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.xml$/i],
			option: 27n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.Particles.forward();
