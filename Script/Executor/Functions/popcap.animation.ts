namespace Sen.Script.Executor.Functions.PopCap.Animation {
	export namespace Flash.Resize {
		export interface Argument extends Executor.Base {
			source: string;
			resolution?: bigint;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			resolution?: bigint;
		}

		export interface Configuration extends Executor.Configuration {
			resolution: bigint;
		}
	}

	export namespace DecodeAndToFlash {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			resolution?: bigint;
			has_label?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			resolution: bigint;
			has_label: boolean;
		}
	}

	export namespace AddLibrary {
		export interface Argument extends Executor.Base {
			source: string;
			image_posix?: string;
			additional?: Array<Support.PopCap.Animation.Miscellaenous.AddLibrary.Additional>;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration { }
	}

	export namespace ToFlash {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			resolution?: bigint;
			has_label?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			resolution?: bigint;
		}

		export interface Configuration extends Executor.Configuration {
			resolution: bigint;
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

		export interface Configuration extends Executor.Configuration { }
	}

	export namespace ToAPNG {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			media?: string;
		}

		export interface BatchArgument extends Executor.Base { }

		export interface Configuration extends Executor.Configuration {
			resolution: bigint;
		}
	}

	export namespace FromFlash {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			has_label?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration { }
	}

	export namespace Encode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration { }
	}

	export namespace FromFlashAndEncode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			has_label?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			resolution: bigint;
		}
	}

	export namespace Detail {
		export const resolution = (): Array<[bigint, bigint, string]> => {
			return [
				[1n, 1536n, Kernel.Language.get('popcap.animation.to_flash.resolution.1536n')],
				[2n, 768n, Kernel.Language.get('popcap.animation.to_flash.resolution.768n')],
				[3n, 384n, Kernel.Language.get('popcap.animation.to_flash.resolution.384n')],
				[4n, 1200n, Kernel.Language.get('popcap.animation.to_flash.resolution.1200n')],
				[5n, 640n, Kernel.Language.get('popcap.animation.to_flash.resolution.640n')],
			];
		};
	}

	export function forward(): void {
		inject<
			Functions.PopCap.Animation.Flash.Resize.Argument,
			Functions.PopCap.Animation.Flash.Resize.BatchArgument,
			Functions.PopCap.Animation.Flash.Resize.Configuration
		>({
			id: 'animation.flash.resize',
			configuration_file: Home.query('~/Executor/Configuration/animation.flash.resize.json'),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				Console.output(argument.source!);
				load_bigint(
					argument,
					'resolution',
					this.configuration,
					Detail.resolution(),
					Kernel.Language.get('popcap.animation.to_flash.resolution'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.Animation.Miscellaneous.resize_fs(
					argument.source,
					BigInt(argument.resolution!),
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['directory', /.*(\.xfl|\.pam\.xfl)$/i],
			option: 4n,
		});
		inject<
			Functions.PopCap.Animation.Decode.Argument,
			Functions.PopCap.Animation.Decode.BatchArgument,
			Functions.PopCap.Animation.Decode.Configuration
		>({
			id: 'popcap.animation.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.animation.decode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.json`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				Executor.clock.start_safe();
				Kernel.Support.PopCap.Animation.decode_fs(argument.source, argument.destination!);
				Executor.clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.pam$/i],
			option: 5n,
		});
		inject<
			Functions.PopCap.Animation.Encode.Argument,
			Functions.PopCap.Animation.Encode.BatchArgument,
			Functions.PopCap.Animation.Encode.Configuration
		>({
			id: 'popcap.animation.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.animation.encode.json'),
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
				Kernel.Support.PopCap.Animation.encode_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /.*\.pam\.json$/i],
			option: 6n,
		});
		inject<
			Functions.PopCap.Animation.AddLibrary.Argument,
			Functions.PopCap.Animation.AddLibrary.BatchArgument,
			Functions.PopCap.Animation.AddLibrary.Configuration
		>({
			id: 'popcap.animation.add_library',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.animation.add_library.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				Console.output(argument.source);
				let image_source: string | Array<string> = Console.path(
					Kernel.Language.get('popcap.animation.add_library.input_png_path'),
					'any',
				);
				if (Kernel.FileSystem.is_directory(image_source)) {
					image_source = Kernel.FileSystem.read_directory(image_source);
				}
				Console.argument(
					Kernel.Language.get('popcap.animation.add_library.image_posix_for_id'),
				);
				while (argument.image_posix === undefined) {
					const value = readline();
					if (value.length === 0) {
						Console.warning(
							Kernel.Language.get(
								'popcap.animation.add_library.image_posix_cannot_be_empty',
							),
						);
						continue;
					}
					argument.image_posix = value;
					break;
				}
				Console.argument(
					Kernel.Language.get('popcap.animation.add_library.generate_sprite'),
				);
				const has_sprite = Executor.input_boolean();
				const input_sprite = (media: string) => {
					let sprite: string | undefined = undefined;
					if (has_sprite) {
						Console.argument(
							`${Kernel.Language.get(
								'popcap.animation.add_library.sprite_name',
							)} ${media}`,
						);
						sprite = has_sprite ? readline() : undefined;
					}
					return sprite;
				};
				const as_callable = (image_source: string) => {
					const media = Kernel.Path.base_without_extension(image_source);
					return {
						original_path: image_source,
						image: media,
						media: media,
						sprite: input_sprite(media),
					};
				};
				if (Array.isArray(image_source)) {
					argument.additional = image_source.map((e, _) => as_callable(e));
				} else {
					argument.additional = [as_callable(image_source)];
				}
				clock.start_safe();
				Support.PopCap.Animation.Miscellaenous.AddLibrary.process_fs(
					argument.source,
					argument.image_posix!,
					argument.additional,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['directory', /.*/g],
			option: 7n,
		});
		inject<
			Functions.PopCap.Animation.FromFlash.Argument,
			Functions.PopCap.Animation.FromFlash.BatchArgument,
			Functions.PopCap.Animation.FromFlash.Configuration
		>({
			id: 'popcap.animation.from_flash',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.animation.from_flash.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				if (/(.+)\.xfl$/i.test(argument.source)) {
					defined_or_default(
						argument,
						'destination',
						`${Kernel.Path.except_extension(argument.source)}.json`,
					);
				} else {
					defined_or_default(argument, 'destination', `${argument.source}.pam.json`);
				}
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_boolean(
					argument,
					'has_label',
					this.configuration,
					Kernel.Language.get('popcap.animation.extract_label'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.Animation.from_flash(
					argument.source,
					argument.destination!,
					argument.has_label!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, true);
			},

			configuration: undefined!,
			filter: ['directory', /.*/g],
			option: 8n,
		});
		inject<
			Functions.PopCap.Animation.ToFlash.Argument,
			Functions.PopCap.Animation.ToFlash.BatchArgument,
			Functions.PopCap.Animation.ToFlash.Configuration
		>({
			id: 'popcap.animation.to_flash',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.animation.to_flash.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				if (/(.+)\.xfl$/i.test(argument.source)) {
					defined_or_default(
						argument,
						'destination',
						`${Kernel.Path.except_extension(argument.source)}.json`,
					);
				} else {
					defined_or_default(argument, 'destination', `${argument.source}.pam.json`);
				}
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'resolution',
					this.configuration,
					Detail.resolution(),
					Kernel.Language.get('popcap.animation.to_flash.resolution'),
				);
				load_boolean(
					argument,
					'has_label',
					this.configuration,
					Kernel.Language.get('popcap.animation.extract_label'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.Animation.to_flash(
					argument.source,
					argument.destination!,
					argument.resolution!,
					argument.has_label!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, true);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.pam\.json$/i],
			option: 9n,
		});
		inject<
			Functions.PopCap.Animation.DecodeAndToFlash.Argument,
			Functions.PopCap.Animation.DecodeAndToFlash.BatchArgument,
			Functions.PopCap.Animation.DecodeAndToFlash.Configuration
		>({
			id: 'popcap.animation.decode_and_to_flash',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.animation.decode_and_to_flash.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.xfl`);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'resolution',
					this.configuration,
					Detail.resolution(),
					Kernel.Language.get('popcap.animation.to_flash.resolution'),
				);
				load_boolean(
					argument,
					'has_label',
					this.configuration,
					Kernel.Language.get('popcap.animation.extract_label'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.Animation.decode_and_to_flash(
					argument.source,
					argument.destination!,
					argument.resolution!,
					argument.has_label!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.pam$/i],
			option: 10n,
		});
		inject<
			Functions.PopCap.Animation.FromFlashAndEncode.Argument,
			Functions.PopCap.Animation.FromFlashAndEncode.BatchArgument,
			Functions.PopCap.Animation.FromFlashAndEncode.Configuration
		>({
			id: 'popcap.animation.from_flash_and_encode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.animation.from_flash_and_encode.json',
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
				load_boolean(
					argument,
					'has_label',
					this.configuration,
					Kernel.Language.get('popcap.animation.extract_label'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.Animation.from_flash_and_encode(
					argument.source,
					argument.destination!,
					argument.has_label!,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, true);
			},

			configuration: undefined!,
			filter: ['directory', /(.+)\.xfl$/i],
			option: 11n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.Animation.forward();
