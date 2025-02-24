namespace Sen.Script.Executor.Functions.Project.SCG {
	export type Generic = Support.Project.StreamCompressedGroup.Configuration.Generic;

	export type Setting = Support.Project.StreamCompressedGroup.Configuration.Setting;

	export namespace Encode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			generic?: Generic;
			animation_split_label?: boolean;
			enable_debug?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			generic: 1n | 2n | 3n | '?';
		}
	}

	export namespace Decode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			generic?: Generic;
			animation_split_label?: boolean;
			enable_debug?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			generic: 1n | 2n | 3n | '?';
		}
	}

	export namespace Detail {
		export function generic(): Array<[bigint, bigint, string]> {
			return [
				[1n, 1n, Kernel.Language.get('sen.scg.regular')],
				[2n, 2n, Kernel.Language.get('sen.scg.for_modding')],
				[3n, 3n, Kernel.Language.get('debug')],
			];
		}
	}

	export interface Configuration extends Executor.Configuration {}

	export function forward(): void {
		inject<
			Functions.Project.SCG.Decode.Argument,
			Functions.Project.SCG.Decode.BatchArgument,
			Functions.Project.SCG.Decode.Configuration
		>({
			id: 'project.scg.decode',
			configuration_file: Home.query('~/Executor/Configuration/project.scg.decode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.package`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				load_boolean(
					argument,
					'enable_debug',
					this.configuration,
					Kernel.Language.get('project.scg.enable_debug'),
				);
				const generic = Detail.generic();
				if (!argument.enable_debug) {
					generic.pop();
				}
				load_bigint(
					argument,
					'generic',
					this.configuration,
					generic,
					Kernel.Language.get('project.scg.decode.generic'),
				);
				argument.generic! -= 1n;
				if (argument.generic! === 1n) {
					load_boolean(
						argument,
						'animation_split_label',
						this.configuration,
						Kernel.Language.get('project.scg.animation_split_label'),
					);
				}
				const setting: Support.Project.StreamCompressedGroup.Configuration.Setting = {
					decode_method: argument.generic!,
					animation_split_label: argument.animation_split_label! ?? false,
				};
				clock.start_safe();
				Kernel.Support.Project.StreamCompressedGroup.decode_fs(
					argument.source,
					argument.destination!,
					setting,
				);
				clock.stop_safe();
			},
			batch_forward(argument) {
				return basic_batch(this, argument, false);
			},

			configuration: undefined!,
			filter: ['file', /(.*)\.scg$/i],
			option: 70n,
		});
		inject<
			Functions.Project.SCG.Encode.Argument,
			Functions.Project.SCG.Encode.BatchArgument,
			Functions.Project.SCG.Encode.Configuration
		>({
			id: 'project.scg.encode',
			configuration_file: Home.query('~/Executor/Configuration/project.scg.encode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.scg`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_boolean(
					argument,
					'enable_debug',
					this.configuration,
					Kernel.Language.get('project.scg.enable_debug'),
				);
				const generic = Detail.generic();
				if (!argument.enable_debug) {
					generic.pop();
				}
				load_bigint(
					argument,
					'generic',
					this.configuration,
					generic,
					Kernel.Language.get('project.scg.encode.generic'),
				);
				argument.generic! -= 1n;
				if (argument.generic! === 1n) {
					load_boolean(
						argument,
						'animation_split_label',
						this.configuration,
						Kernel.Language.get('project.scg.animation_split_label'),
					);
				}
				const setting: Setting = {
					decode_method: argument.generic!,
					animation_split_label: argument.animation_split_label! ?? false,
				};
				clock.start_safe();
				Kernel.Support.Project.StreamCompressedGroup.encode_fs(
					argument.source,
					argument.destination!,
					setting,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['directory', /(.*)\.package$/i],
			option: 71n,
		});
	}
}

Sen.Script.Executor.Functions.Project.SCG.forward();
