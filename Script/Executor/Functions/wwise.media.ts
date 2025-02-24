namespace Sen.Script.Executor.Functions.WWise.Media {
	export namespace Encode {
		export interface Argument extends Executor.Base {
			source: string;
			format?: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			format: Support.Texture.Format | '?';
		}
	}

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

	export namespace Detail {
		export function format(): Array<[bigint, string, string]> {
			return [
				[1n, 'pcm', '16-bit Little Endian PCM (pcm)'],
				[2n, 'adpcm', 'Platinum 4-bit ADPCM (adpcm)'],
				[3n, 'vorbis', 'Custom Vorbis (vorbis)'],
				[4n, 'wemopus', 'libopus Opus (wemopus)'],
			];
		}
	}

	export function forward(): void {
		inject<
			Functions.WWise.Media.Encode.Argument,
			Functions.WWise.Media.Encode.BatchArgument,
			Functions.WWise.Media.Encode.Configuration
		>({
			id: 'wwise.media.encode',
			configuration_file: Home.query('~/Executor/Configuration/wwise.media.encode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.wem`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'format',
					this.configuration,
					Detail.format(),
					Kernel.Language.get('wwise.media.audio_format'),
				);
				clock.start_safe();
				Support.Wwise.Media.Encode.process_fs(
					argument.source,
					argument.destination!,
					argument.format! as Support.Wwise.Media.Common.Format,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.wav$/i],
			option: 72n,
		});
		inject<
			Functions.WWise.Media.Decode.Argument,
			Functions.WWise.Media.Decode.BatchArgument,
			Functions.WWise.Media.Decode.Configuration
		>({
			id: 'wwise.media.decode',
			configuration_file: Home.query('~/Executor/Configuration/wwise.media.decode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.wav`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Support.Wwise.Media.Decode.process_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.wem$/i],
			option: 73n,
		});
	}
}

Sen.Script.Executor.Functions.WWise.Media.forward();
