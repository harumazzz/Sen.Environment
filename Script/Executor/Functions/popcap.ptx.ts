namespace Sen.Script.Executor.Functions.PopCap.PTX {
	export interface Dimension extends Record<string, unknown> {
		width: bigint;
		height: bigint;
	}
	export namespace Decode {
		export interface Argument extends Executor.Base {
			source: string;
			size?: Dimension;
			format?: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			size: Dimension | '?';
			format: Support.Texture.Format | '?';
		}
	}

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

	export namespace Detail {
		export function format(): Array<[bigint, string, string]> {
			return [
				[1n, 'argb_8888', 'argb_8888 (0, iOS)'],
				[2n, 'rgba_8888', 'rgba_8888 (0, Android)'],
				[3n, 'rgba_4444', 'rgba_4444 (1, Android & iOS)'],
				[4n, 'rgb_565', 'rgb_565 (2, Android)'],
				[5n, 'rgba_5551', 'rgba_5551 (3, Android & iOS)'],
				[6n, 'rgba_4444_tiled', 'rgba_4444_tiled (21, Android & iOS)'],
				[7n, 'rgb_565_tiled', 'rgb_565_tiled (22, Android)'],
				[8n, 'rgba_5551_tiled', 'rgba_5551_tiled (23, Android & iOS)'],
				[9n, 'rgba_pvrtc4', 'rgba_pvrtc4 (30, iOS)'],
				[10n, 'rgb_etc1_a_8', 'rgb_etc1_a_8 (147, Android)'],
				[
					11n,
					'rgb_etc1_a_palette',
					`rgb_etc1_a_palette (147, ${Kernel.Language.get('pvz2_android_cn')})`,
				],
				[12n, 'rgb_pvrtc4_a_8', 'rgb_pvrtc4_a_8 (148, iOS)'],
			];
		}

		const m_formats = {
			argb_8888: Support.Texture.Format.ARGB_8888,
			rgba_8888: Support.Texture.Format.RGBA_8888,
			rgba_4444: Support.Texture.Format.RGBA_4444,
			rgb_565: Support.Texture.Format.RGB_565,
			rgba_5551: Support.Texture.Format.RGBA_5551,
			rgba_4444_tiled: Support.Texture.Format.RGBA_4444_TILED,
			rgb_565_tiled: Support.Texture.Format.RGB_565_TILED,
			rgba_5551_tiled: Support.Texture.Format.RGBA_5551_TILED,
			rgba_pvrtc4: Support.Texture.Format.RGBA_PVRTC_4BPP,
			rgb_etc1_a_8: Support.Texture.Format.RGB_ETC1_A_8,
			rgb_etc1_a_palette: Support.Texture.Format.RGB_ETC1_A_PALETTE,
			rgb_pvrtc4_a_8: Support.Texture.Format.RGB_PVRTC_4BPP_A_8,
		};

		export function exchange_format(key: string): Support.Texture.Format {
			const texture_format = m_formats[key];
			if (texture_format === undefined) {
				assert(
					false,
					Script.format(Kernel.Language.get('popcap.ptx.unsupported_format'), key),
				);
			}
			return texture_format;
		}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.PTX.Decode.Argument,
			Functions.PopCap.PTX.Decode.BatchArgument,
			Functions.PopCap.PTX.Decode.Configuration
		>({
			id: 'popcap.ptx.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.ptx.decode.json'),
			configuration: undefined!,
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.png`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'format',
					this.configuration,
					Detail.format(),
					Kernel.Language.get('popcap.ptx.decode.format'),
				);
				argument.size = {} as any;
				input_range(
					argument.size as any,
					'width',
					this.configuration.size as Dimension,
					[1n, 16384n],
					Kernel.Language.get('popcap.ptx.decode.width'),
				);
				if (argument.format === 'rgb_pvrtc4_a_8') {
					(this.configuration.size as Dimension).height = argument.size!.width;
				}
				input_range(
					argument.size as any,
					'height',
					this.configuration.size as Dimension,
					[1n, 16384n],
					Kernel.Language.get('popcap.ptx.decode.height'),
				);
				clock.start_safe();
				Kernel.Support.Texture.decode_fs(
					argument.source,
					argument.destination!,
					argument!.size!.width,
					argument!.size!.height,
					BigInt(Detail.exchange_format(argument.format!)),
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			filter: ['file', /(.+)\.ptx$/i],
			option: 30n,
		});
		inject<
			Functions.PopCap.PTX.Encode.Argument,
			Functions.PopCap.PTX.Encode.BatchArgument,
			Functions.PopCap.PTX.Encode.Configuration
		>({
			id: 'popcap.ptx.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.ptx.encode.json'),
			configuration: undefined!,
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.ptx`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'format',
					this.configuration,
					Detail.format(),
					Kernel.Language.get('popcap.ptx.encode.format'),
				);
				clock.start_safe();
				Kernel.Support.Texture.encode_fs(
					argument.source,
					argument.destination!,
					BigInt(Detail.exchange_format(argument.format!)),
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				load_bigint(
					argument,
					'format',
					this.configuration,
					Detail.format(),
					Kernel.Language.get('popcap.ptx.encode.format'),
				);
				return basic_batch(this, argument, false);
			},

			filter: ['file', /(.+)\.png$/i],
			option: 31n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.PTX.forward();
