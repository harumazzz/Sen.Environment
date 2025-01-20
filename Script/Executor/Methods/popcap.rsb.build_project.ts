namespace Sen.Script.Executor.Methods.PopCap.RSB.BuildProject {
	// Generic

	export type Generic = Support.Project.ResourceStreamBundle.Configuration.Generic;

	// Setting
	export type Setting = Support.Project.ResourceStreamBundle.Configuration.Setting;

	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		generic?: Generic;
		key?: string;
		iv?: string;
		manifest?: boolean;
		packages?: boolean;
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

	export interface Configuration extends Executor.Configuration {
		layout: string | '?';
		generic: string | '?';
		packages_setting: Support.Project.ResourceStreamBundle.Configuration.PackagesSetting;
	}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export interface PackagesFileList {
		rton_file: string[];
		json_file: string[];
	}

	export interface PackagesInfo {
		compression: 1n | 2n | 3n | 4n;
		chinese: boolean;
		encode: boolean;
	}

	export function load_packages(
		source: string,
		packages_info: null | PackagesInfo,
	): Support.Project.ResourceStreamBundle.Configuration.PackagesSetting {
		const packages_setting: Support.Project.ResourceStreamBundle.Configuration.PackagesSetting =
			{
				rton_count: 0n,
				json_count: 0n,
				key: '',
				iv: '',
			};
		if (packages_info !== null) {
			const packages_list = Kernel.FileSystem.read_directory(`${source}/packages`);
			if (packages_info.encode) {
				const unique_list = new Set<string>();
				for (let element of packages_list) {
					const currentElement = element.slice(0, element.length - 5);
					if (/(\.json)$/i.test(element)) {
						unique_list.add(currentElement);
						++packages_setting.json_count;
					}
					if (/(\.rton)$/i.test(element)) {
						const hasValue: boolean = unique_list.has(currentElement);
						if (hasValue) {
							unique_list.delete(currentElement);
						}
						++packages_setting.rton_count;
					}
				}
				packages_setting.rton_count += BigInt(unique_list.size);
			} else {
				for (let element of packages_list) {
					if (/(\.rton)$/i.test(element)) {
						++packages_setting.rton_count;
					}
				}
			}
		}
		return packages_setting;
	}

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.RSB.BuildProject.Argument,
			Methods.PopCap.RSB.BuildProject.BatchArgument,
			Methods.PopCap.RSB.BuildProject.Configuration
		>({
			id: 'popcap.rsb.build_project',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rsb.build_project.json',
			),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					Kernel.Path.except_extension(argument.source),
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'generic',
					this.configuration,
					InitProject.Detail.generic(),
					Kernel.Language.get('popcap.rsb.custom.generic'),
				);
				const packages_info: PackagesInfo | null = (
					Kernel.JSON.deserialize_fs(`${argument.source}/data.json`) as any
				).packages_info;
				const packages_setting: Support.Project.ResourceStreamBundle.Configuration.PackagesSetting =
					load_packages(argument.source, packages_info);
				if (packages_info !== null) {
					if (packages_setting.json_count !== 0n && packages_info.encode) {
						Console.output(
							`${Kernel.Language.get('popcap.rsb.build_project.total_json_count')}: ${
								packages_setting.json_count
							}`,
						);
						if (packages_setting.json_count !== 0n && packages_info.chinese) {
							load_string(
								argument,
								'key',
								this.configuration.packages_setting,
								Kernel.Language.get('popcap.rsb.build_project.key'),
							);
							load_string(
								argument,
								'iv',
								this.configuration.packages_setting,
								Kernel.Language.get('popcap.rsb.build_project.iv'),
							);
							packages_setting.key = argument.key!;
							packages_setting.iv = argument.iv!;
						}
					}
					Console.output(
						`${Kernel.Language.get('popcap.rsb.build_project.total_rton_count')}: ${
							packages_setting.rton_count
						}`,
					);
				} else {
					Console.output(
						Kernel.Language.get('popcap.rsb.build_project.packages_does_not_use'),
					);
				}
				const setting: Setting = {
					texture_format_category: argument.generic!,
					only_high_resolution: false,
					packages_setting,
					unpack_packages: true,
				};
				clock.start_safe();
				Kernel.Support.Project.ResourceStreamBundle.pack_fs(
					argument.source,
					argument.destination!,
					setting,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,
			is_enabled: true,
			configuration: undefined!,
			filter: ['directory', /(.*)\.bundle$/i],
			option: 55n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.RSB.BuildProject.forward();
