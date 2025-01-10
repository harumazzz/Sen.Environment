namespace Sen.Script.Executor.Methods.WWise.Media.Encode {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		format?: string;
		destination?: string;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {
		format: Support.Texture.Format | '?';
	}

	export namespace Detail {
		/**
		 *
		 * Typical Style
		 *
		 */

		export function format(): Array<[bigint, string, string]> {
			return [
				[1n, 'pcm', '16-bit Little Endian PCM (pcm)'],
				[2n, 'adpcm', 'Platinum 4-bit ADPCM (adpcm)'],
				[3n, 'vorbis', 'Custom Vorbis (vorbis)'],
				[4n, 'wemopus', 'libopus Opus (wemopus)'],
				// [5n, "acc", "AAC (Advanced Audio Coding) (acc)"]
			];
		}
	}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.WWise.Media.Encode.Argument,
			Methods.WWise.Media.Encode.BatchArgument,
			Methods.WWise.Media.Encode.Configuration
		>({
			id: 'wwise.media.encode',
			configuration_file: Home.query('~/Executor/Configuration/wwise.media.encode.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
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
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.wav$/i],
			option: 76n,
		});
	}
}

Sen.Script.Executor.Methods.WWise.Media.Encode.forward();
