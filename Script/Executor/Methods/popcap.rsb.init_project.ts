namespace Sen.Script.Executor.Methods.PopCap.RSB.InitProject {
	// Generic

	export type Generic = Support.Miscellaneous.Custom.ResourceStreamBundle.Configuration.Generic;

	// Setting

	export type Setting = Support.Miscellaneous.Custom.ResourceStreamBundle.Configuration.Setting;

	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		layout?: string;
		generic?: Generic;
		only_high_resolution?: boolean;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
	}

	/**
	 * Async support
	 */

	export interface AsyncArgument extends Executor.Base {
		parameter: Array<[string, string]>;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {
		generic: 1n | 2n | 3n | '?';
	}

	/**
	 * Detail namespace
	 */

	export namespace Detail {
		export function generic(): Array<[bigint, bigint, string]> {
			return [
				[1n, 0n, Kernel.Language.get('android')],
				[2n, 1n, Kernel.Language.get('ios')],
				[3n, 2n, Kernel.Language.get('pvz2_android_cn')],
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
			Methods.PopCap.RSB.InitProject.Argument,
			Methods.PopCap.RSB.InitProject.BatchArgument,
			Methods.PopCap.RSB.InitProject.Configuration
		>({
			id: 'popcap.rsb.init_project',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb.init_project.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${argument.source}.bundle`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'generic',
					this.configuration,
					Detail.generic(),
					Kernel.Language.get('popcap.rsb.custom.generic'),
				);
				load_boolean(
					argument,
					'only_high_resolution',
					this.configuration,
					Kernel.Language.get('popcap.rsb.init_project.only_high_resolution'),
				);
				const setting: Setting = {
					texture_format_category: argument.generic!,
					only_high_resolution: argument.only_high_resolution!,
					packages_setting: {
						rton_count: 0n,
						json_count: 0n,
						key: '',
						iv: '',
					},
					unpack_packages: true,
				};
				clock.start_safe();
				Kernel.Support.Miscellaneous.Custom.ResourceStreamBundle.unpack_fs(
					argument.source,
					argument.destination!,
					setting,
				);
				clock.stop_safe();
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.*)\.(rsb|obb)$/i],
			option: 58n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.RSB.InitProject.forward();
