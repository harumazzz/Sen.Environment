namespace Sen.Script.Executor.Functions.PopCap.PvZ2.LawnStrings {
	export namespace Convert {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			source_type?: Support.PopCap.LawnStrings.Convert.Conversion;
			destination_type?: Support.PopCap.LawnStrings.Convert.Conversion;
		}

		export interface BatchArgument extends Executor.Base {}

		export interface Configuration extends Executor.Configuration {
			source_type: Support.PopCap.LawnStrings.Convert.Conversion | '?';
			destination_type: Support.PopCap.LawnStrings.Convert.Conversion | '?';
		}
	}

	export namespace Detail {
		export function type(): Array<[bigint, string, string]> {
			return [
				[1n, 'text', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.text')],
				[2n, 'array', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.array')],
				[3n, 'map', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.map')],
				[4n, 'cn-text', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.cn_text')],
			];
		}

		export const rule: Array<Support.PopCap.LawnStrings.Convert.Conversion> = [
			'text',
			'array',
			'map',
			'cn-text',
		];

		export function extension(
			destination: Support.PopCap.LawnStrings.Convert.Conversion,
		): string {
			switch (destination) {
				case 'array':
				case 'map':
					return 'json';

				case 'cn-text':
				case 'text':
					return 'txt';

				default:
					return 'unknown';
			}
		}
	}

	export function forward(): void {
		return inject<
			Functions.PopCap.PvZ2.LawnStrings.Convert.Argument,
			Functions.PopCap.PvZ2.LawnStrings.Convert.BatchArgument,
			Functions.PopCap.PvZ2.LawnStrings.Convert.Configuration
		>({
			id: 'popcap.pvz2.lawnstrings.convert',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.pvz2.lawnstrings.convert.json',
			),
			configuration: undefined!,
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				load_bigint(
					argument,
					'source_type',
					this.configuration,
					Detail.type(),
					Kernel.Language.get('popcap.pvz2.lawnstrings.convert.source_type'),
				);
				load_bigint(
					argument,
					'destination_type',
					this.configuration,
					Detail.type(),
					Kernel.Language.get('popcap.pvz2.lawnstrings.destination.destination_type'),
				);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.${Detail.extension(
						argument.destination_type!,
					)}`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Support.PopCap.LawnStrings.Convert.process_fs(
					argument.source,
					argument.destination!,
					argument.source_type!,
					argument.destination_type!,
				);
				Executor.clock.stop_safe();
			},
			batch_forward: undefined!,
			is_enabled: true,
			filter: ['file', /(.+)(\.json|\.txt)$/i],
			option: 35n,
		});
	}
}
Sen.Script.Executor.Functions.PopCap.PvZ2.LawnStrings.forward();
