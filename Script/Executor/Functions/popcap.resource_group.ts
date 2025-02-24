namespace Sen.Script.Executor.Functions.PopCap.ResourceGroup {
	export namespace Convert {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			layout?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			layout?: string;
		}

		export interface Configuration extends Executor.Configuration {
			layout: string;
		}
	}

	export namespace Merge {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Split {
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
		export const m_layout = {
			string: Support.PopCap.ResourceGroup.PathStyle.WindowStyle,
			array: Support.PopCap.ResourceGroup.PathStyle.ArrayStyle,
		};

		export function exchange_layout(layout: string): Support.PopCap.ResourceGroup.PathStyle {
			const value = m_layout[layout];
			if (value !== undefined) return value!;
			assert(
				false,
				format(
					Kernel.Language.get('popcap.resource_group.convert.cannot_exchange_layout'),
					layout,
				),
			);
		}

		export function style(): Array<[bigint, string, string]> {
			return [
				[1n, 'string', Kernel.Language.get('popcap.resource_group.convert.layout.string')],
				[2n, 'array', Kernel.Language.get('popcap.resource_group.convert.layout.array')],
			];
		}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.ResourceGroup.Convert.Argument,
			Functions.PopCap.ResourceGroup.Convert.BatchArgument,
			Functions.PopCap.ResourceGroup.Convert.Configuration
		>({
			id: 'popcap.resource_group.convert',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.resource_group.convert.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.dirname(argument.source)}/res.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_bigint(
					argument,
					'layout',
					this.configuration,
					Detail.style(),
					Kernel.Language.get('popcap.atlas.split.style'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceGroup.convert_fs(
					argument.source,
					argument.destination!,
					BigInt(Detail.exchange_layout(argument.layout!)),
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 46n,
		});
		inject<
			Functions.PopCap.ResourceGroup.Split.Argument,
			Functions.PopCap.ResourceGroup.Split.BatchArgument,
			Functions.PopCap.ResourceGroup.Split.Configuration
		>({
			id: 'popcap.resource_group.split',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.resource_group.split.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.info`);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceGroup.split_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 47n,
		});
		inject<
			Functions.PopCap.ResourceGroup.Merge.Argument,
			Functions.PopCap.ResourceGroup.Merge.BatchArgument,
			Functions.PopCap.ResourceGroup.Merge.Configuration
		>({
			id: 'popcap.resource_group.merge',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.resource_group.merge.json',
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
				clock.start_safe();
				Kernel.Support.PopCap.ResourceGroup.merge_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['directory', /.*\.info$/i],
			option: 48n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.ResourceGroup.forward();
