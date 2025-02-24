namespace Sen.Script.Executor.Functions.PopCap.ReAnimation {
	export type Platform = Kernel.Support.PopCap.ReAnimation.Platform;
	export namespace DecodeAndToFlash {
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

	export namespace FromFlashAndEncode {
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

	export namespace FromFlash {
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

	export namespace ToFlash {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
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

	export namespace Detail {
		export const _platform: Array<Kernel.Support.PopCap.ReAnimation.Platform> = [
			'desktop',
			'mobile32',
			'mobile64',
			'television',
		];

		export function platform(): Array<[bigint, string, string]> {
			return _platform.map((e, i) => [
				BigInt(i + 1),
				e as string,
				// TODO : rename
				Kernel.Language.get(`popcap.reanim.platform.${e}`),
			]);
		}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.ReAnimation.Decode.Argument,
			Functions.PopCap.ReAnimation.Decode.BatchArgument,
			Functions.PopCap.ReAnimation.Decode.Configuration
		>({
			id: 'popcap.reanim.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.reanim.decode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				Console.argument(Kernel.Language.get('popcap.reanim.decode.generic'));
				configurate_or_input(argument, 'platform', Detail.platform());
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.decode_fs(
					argument.source,
					argument.destination!,
					argument.platform!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.reanim|\.reanim\.compiled)$/i],
			option: 33n,
		});
		inject<
			Functions.PopCap.ReAnimation.Encode.Argument,
			Functions.PopCap.ReAnimation.Encode.BatchArgument,
			Functions.PopCap.ReAnimation.Encode.Configuration
		>({
			id: 'popcap.reanim.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.reanim.encode.json'),
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
				Console.argument(Kernel.Language.get('popcap.reanim.encode.generic'));
				configurate_or_input(argument, 'platform', Detail.platform());
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.encode_fs(
					argument.source,
					argument.destination!,
					argument.platform!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.reanim\.json)$/i],
			option: 34n,
		});
		inject<
			Functions.PopCap.ReAnimation.FromXML.Argument,
			Functions.PopCap.ReAnimation.FromXML.BatchArgument,
			Functions.PopCap.ReAnimation.FromXML.Configuration
		>({
			id: 'popcap.reanim.from_xml',
			configuration_file: Home.query('~/Executor/Configuration/popcap.reanim.from_xml.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.from_xml(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.reanim\.xml)$/i],
			option: 35n,
		});
		inject<
			Functions.PopCap.ReAnimation.ToXML.Argument,
			Functions.PopCap.ReAnimation.ToXML.BatchArgument,
			Functions.PopCap.ReAnimation.ToXML.Configuration
		>({
			id: 'popcap.reanim.to_xml',
			configuration_file: Home.query('~/Executor/Configuration/popcap.reanim.to_xml.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.xml`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.to_xml(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.reanim\.json)$/i],
			option: 36n,
		});
		inject<
			Functions.PopCap.ReAnimation.FromFlash.Argument,
			Functions.PopCap.ReAnimation.FromFlash.BatchArgument,
			Functions.PopCap.ReAnimation.FromFlash.Configuration
		>({
			id: 'popcap.reanim.from_flash',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.reanim.from_flash.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.from_flash(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['directory', /(.+)(\.reanim\.xfl)$/i],
			option: 37n,
		});
		inject<
			Functions.PopCap.ReAnimation.ToFlash.Argument,
			Functions.PopCap.ReAnimation.ToFlash.BatchArgument,
			Functions.PopCap.ReAnimation.ToFlash.Configuration
		>({
			id: 'popcap.reanim.to_flash',
			configuration_file: Home.query('~/Executor/Configuration/popcap.reanim.to_flash.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.xfl`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.to_flash(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.reanim\.json)$/i],
			option: 38n,
		});
		inject<
			Functions.PopCap.ReAnimation.DecodeAndToFlash.Argument,
			Functions.PopCap.ReAnimation.DecodeAndToFlash.BatchArgument,
			Functions.PopCap.ReAnimation.DecodeAndToFlash.Configuration
		>({
			id: 'popcap.reanim.decode_and_to_flash',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.reanim.decode_and_to_flash.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.xfl`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				Console.argument(Kernel.Language.get('popcap.reanim.decode.generic'));
				configurate_or_input(argument, 'platform', Detail.platform());
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.decode_and_to_flash(
					argument.source,
					argument.destination!,
					argument.platform!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)(\.reanim|\.reanim\.compiled)$/i],
			option: 39n,
		});
		inject<
			Functions.PopCap.ReAnimation.FromFlashAndEncode.Argument,
			Functions.PopCap.ReAnimation.FromFlashAndEncode.BatchArgument,
			Functions.PopCap.ReAnimation.FromFlashAndEncode.Configuration
		>({
			id: 'popcap.reanim.from_flash_and_encode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.reanim.from_flash_and_encode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.compiled`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				Console.argument(Kernel.Language.get('popcap.reanim.decode.generic'));
				configurate_or_input(argument, 'platform', Detail.platform());
				clock.start_safe();
				Kernel.Support.PopCap.ReAnimation.from_flash_and_encode(
					argument.source,
					argument.destination!,
					argument.platform!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['directory', /(.+)(\.reanim\.xfl)$/i],
			option: 40n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.ReAnimation.forward();
