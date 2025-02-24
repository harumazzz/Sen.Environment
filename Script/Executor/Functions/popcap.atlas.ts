namespace Sen.Script.Executor.Functions.PopCap.Atlas {
	export namespace PackByResInfo {
		export interface Argument extends Executor.Base {
			source: string;
			size: Support.PopCap.Atlas.Pack.Detail.SizeRange<number>;
			detail: Support.PopCap.Atlas.Pack.Detail.Data;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {}

		export interface Configuration extends Executor.Configuration {
			size: Support.PopCap.Atlas.Pack.Detail.SizeRange<number>;
			detail: Support.PopCap.Atlas.Pack.Detail.Data;
		}
	}

	export namespace PackByResourceGroup {
		export interface Argument extends Base {
			source: string;
			size: Support.PopCap.Atlas.Pack.Detail.SizeRange<number>;
			detail: Support.PopCap.Atlas.Pack.Detail.Data;
			destination?: string;
		}

		export interface BatchArgument extends Base {}

		export interface Configuration extends Executor.Configuration {
			size: Support.PopCap.Atlas.Pack.Detail.SizeRange<number>;
			detail: Support.PopCap.Atlas.Pack.Detail.Data;
		}
	}

	export namespace SplitByResInfo {
		export interface Argument extends Executor.Base {
			source: [string, ...Array<string>];
			destination?: string;
			split_method?: Support.PopCap.Atlas.Structure.TypicalMethod;
			style?: Support.PopCap.Atlas.Structure.PathStyle;
		}

		export interface BatchArgument extends Executor.Base {}

		export interface Configuration extends Executor.Configuration {
			split_method: '?' | 'path' | 'id';
			style: '?' | 'string' | 'array';
		}
	}

	export namespace SplitByResourceGroup {
		export interface Argument extends Executor.Base {
			source: [string, ...Array<string>];
			destination?: string;
			split_method?: Support.PopCap.Atlas.Structure.TypicalMethod;
			style?: Support.PopCap.Atlas.Structure.PathStyle;
		}

		export interface BatchArgument extends Executor.Base {}

		export interface Configuration extends Executor.Configuration {
			split_method: '?' | 'path' | 'id';
			style: '?' | 'string' | 'array';
		}
	}

	export namespace Detail {
		export const dimension: Array<bigint> = [
			128n,
			256n,
			512n,
			1024n,
			2048n,
			4096n,
			8192n,
			16384n,
		];

		export const padding: Array<bigint> = [1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n, 10n];

		export function method(): Array<[bigint, string, string]> {
			return [
				[1n, 'id', Kernel.Language.get('popcap.atlas.split.method.id')],
				[2n, 'path', Kernel.Language.get('popcap.atlas.split.method.path')],
			];
		}

		export function style(): Array<[bigint, string, string]> {
			return [
				[1n, 'string', Kernel.Language.get('popcap.atlas.split.style.string')],
				[2n, 'array', Kernel.Language.get('popcap.atlas.split.style.array')],
			];
		}

		export function prepare<
			T extends Base,
			U extends Configuration & { size: V },
			V extends Support.PopCap.Atlas.Pack.Detail.SizeRange<number>,
		>(argument: T, configuration: U): void {
			input_range(
				argument.size as any,
				'width',
				configuration.size,
				[64n, 16384n],
				Kernel.Language.get('popcap.atlas.pack.width'),
			);
			input_range(
				argument.size as any,
				'height',
				configuration.size,
				[64n, 16384n],
				Kernel.Language.get('popcap.atlas.pack.height'),
			);
			input_range(
				argument.size as any,
				'padding',
				configuration.size,
				[1n, 10n],
				Kernel.Language.get('popcap.atlas.pack.padding'),
			);
			load_boolean(
				argument.detail as any,
				'smart',
				configuration.detail as any,
				Kernel.Language.get('popcap.atlas.pack.smart'),
			);
			load_boolean(
				argument.detail as any,
				'pot',
				configuration.detail as any,
				Kernel.Language.get('popcap.atlas.pack.pot'),
			);
			load_boolean(
				argument.detail as any,
				'allowRotation',
				configuration.detail as any,
				Kernel.Language.get('popcap.atlas.pack.allow_rotation'),
			);
			load_boolean(
				argument.detail as any,
				'square',
				configuration.detail as any,
				Kernel.Language.get('popcap.atlas.pack.square'),
			);
		}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.Atlas.PackByResInfo.Argument,
			Functions.PopCap.Atlas.PackByResInfo.BatchArgument,
			Functions.PopCap.Atlas.PackByResInfo.Configuration
		>({
			id: 'popcap.atlas.pack_by_res_info',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.atlas.pack_by_res_info.json',
			),
			configuration: undefined!,
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.dirname(argument.source)}`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				argument.size = {} as any;
				argument.detail = {} as any;
				Detail.prepare(argument, this.configuration);
				argument.size.width = Number(argument.size.width);
				argument.size.height = Number(argument.size.height);
				argument.size.padding = Number(argument.size.padding);
				clock.start_safe();
				Support.PopCap.Atlas.Pack.ResInfo.process_fs(
					argument.source,
					argument.size,
					argument.detail,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,

			filter: ['directory', /(.+)\.sprite$/i],
			option: 12n,
		});
		inject<
			Functions.PopCap.Atlas.PackByResourceGroup.Argument,
			Functions.PopCap.Atlas.PackByResourceGroup.BatchArgument,
			Functions.PopCap.Atlas.PackByResourceGroup.Configuration
		>({
			id: 'popcap.atlas.pack_by_resource_group',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.atlas.pack_by_resource_group.json',
			),
			configuration: undefined!,
			direct_forward(argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.dirname(argument.source)}`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				argument.size = {} as any;
				argument.detail = {} as any;
				Detail.prepare(argument, this.configuration);
				argument.size.width = Number(argument.size.width);
				argument.size.height = Number(argument.size.height);
				argument.size.padding = Number(argument.size.padding);
				clock.start_safe();
				Support.PopCap.Atlas.Pack.ResourceGroup.process_fs(
					argument.source,
					argument.size,
					argument.detail,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,

			filter: ['directory', /(.+)\.sprite$/i],
			option: 13n,
		});
		inject<
			Functions.PopCap.Atlas.SplitByResInfo.Argument,
			Functions.PopCap.Atlas.SplitByResInfo.BatchArgument,
			Functions.PopCap.Atlas.SplitByResInfo.Configuration
		>({
			id: 'popcap.atlas.split_by_res_info',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.atlas.split_by_res_info.json',
			),
			configuration: undefined!,
			direct_forward(argument): void {
				const jsons: Array<string> = argument.source.filter((e) => /\.json$/gi.test(e));
				assert(
					jsons.length > 0,
					Kernel.Language.get('popcap.atlas.split_by_res_info.source_file_must_be_json'),
				);
				const pngs: Array<string> = argument.source.filter((e) => /\.png$/gi.test(e));
				jsons.forEach((json: string) => {
					const category: [string, ...Array<string>] = [json, ...pngs];
					category.forEach((e: string) => Console.obtained(e));
					defined_or_default(
						argument,
						'destination',
						`${Kernel.Path.except_extension(json)}.sprite`,
					);
					Console.output(argument.destination!);
					load_bigint(
						argument,
						'split_method',
						this.configuration,
						Detail.method(),
						Kernel.Language.get('popcap.atlas.split.method'),
					);
					clock.start_safe();
					Support.PopCap.Atlas.Split.ResInfo.process_fs(
						category,
						argument.destination!,
						argument.split_method!,
					);
					clock.stop_safe();
				});
			},
			batch_forward: undefined!,

			filter: ['files', /(.+)\.json$/i, /(.+)\.png$/i],
			option: undefined!,
		});
		inject<
			Functions.PopCap.Atlas.SplitByResourceGroup.Argument,
			Functions.PopCap.Atlas.SplitByResourceGroup.BatchArgument,
			Functions.PopCap.Atlas.SplitByResourceGroup.Configuration
		>({
			id: 'popcap.atlas.split_by_resource_group',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.atlas.split_by_resource_group.json',
			),
			configuration: undefined!,
			direct_forward(argument): void {
				const jsons: Array<string> = argument.source.filter((e) => /\.json$/gi.test(e));
				assert(
					jsons.length > 0,
					Kernel.Language.get(
						'popcap.atlas.split_by_resource_group.source_file_must_be_json',
					),
				);
				const pngs: Array<string> = argument.source.filter((e) => /\.png$/gi.test(e));
				jsons.forEach((json: string) => {
					const category: [string, ...Array<string>] = [json, ...pngs];
					category.forEach((e: string) => Console.obtained(e));
					defined_or_default(
						argument,
						'destination',
						`${Kernel.Path.except_extension(json)}.sprite`,
					);
					Console.output(argument.destination!);
					load_bigint(
						argument,
						'split_method',
						this.configuration,
						Detail.method(),
						Kernel.Language.get('popcap.atlas.split.method'),
					);
					load_bigint(
						argument,
						'style',
						this.configuration,
						Detail.style(),
						Kernel.Language.get('popcap.atlas.split.style'),
					);
					clock.start_safe();
					Support.PopCap.Atlas.Split.ResourceGroup.process_fs(
						category,
						argument.destination!,
						argument.split_method!,
						argument.style!,
					);
					clock.stop_safe();
				});
			},
			batch_forward: undefined!,
			filter: ['files', /(.+)\.json$/gi, /(.+)\.png$/gi],
			option: undefined!,
		});
	}
}
Sen.Script.Executor.Functions.PopCap.Atlas.forward();
