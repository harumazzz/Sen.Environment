namespace Sen.Script.Executor.Functions.Project.RSB {
	export type Generic = Support.Project.ResourceStreamBundle.Configuration.Generic;

	export type Setting = Support.Project.ResourceStreamBundle.Configuration.Setting;

	export namespace Init {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			layout?: string;
			generic?: Generic;
			only_high_resolution?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			generic: 1n | 2n | 3n | '?';
		}
	}

	export namespace Build {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			generic?: Generic;
			key?: string;
			iv?: string;
			manifest?: boolean;
			packages?: boolean;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {
			layout: string | '?';
			generic: string | '?';
			packages_setting: Support.Project.ResourceStreamBundle.Configuration.PackagesSetting;
		}

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
	}

	export namespace Detail {
		export function generic(): Array<[bigint, bigint, string]> {
			return [
				[1n, 0n, Kernel.Language.get('android')],
				[2n, 1n, Kernel.Language.get('ios')],
				[3n, 2n, Kernel.Language.get('pvz2_android_cn')],
			];
		}
	}

	export function forward(): void {
		inject<
			Functions.Project.RSB.Init.Argument,
			Functions.Project.RSB.Init.BatchArgument,
			Functions.Project.RSB.Init.Configuration
		>({
			id: 'popcap.rsb.init_project',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb.init_project.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.bundle`);
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
				Kernel.Support.Project.ResourceStreamBundle.unpack_fs(
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
			filter: ['file', /(.*)\.(rsb|obb)$/i],
			option: 68n,
		});
		inject<
			Functions.Project.RSB.Build.Argument,
			Functions.Project.RSB.Build.BatchArgument,
			Functions.Project.RSB.Build.Configuration
		>({
			id: 'popcap.rsb.build_project',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.rsb.build_project.json',
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
				load_bigint(
					argument,
					'generic',
					this.configuration,
					Detail.generic(),
					Kernel.Language.get('popcap.rsb.custom.generic'),
				);
				const packages_info: Build.PackagesInfo | null = (
					Kernel.JSON.deserialize_fs(`${argument.source}/data.json`) as any
				).packages_info;
				const packages_setting: Support.Project.ResourceStreamBundle.Configuration.PackagesSetting =
					Build.load_packages(argument.source, packages_info);
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

			configuration: undefined!,
			filter: ['directory', /(.*)\.bundle$/i],
			option: 69n,
		});
	}
}

Sen.Script.Executor.Functions.Project.RSB.forward();
