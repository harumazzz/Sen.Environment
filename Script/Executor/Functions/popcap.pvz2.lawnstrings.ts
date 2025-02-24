namespace Sen.Script.Executor.Functions.PopCap.PvZ2.LawnStrings {
	export namespace Convert {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			source_type?: Kernel.Support.PopCap.TextTable.Type;
			destination_type?: Kernel.Support.PopCap.TextTable.Type;
		}

		export interface BatchArgument extends Executor.Base {}

		export interface Configuration extends Executor.Configuration {
			source_type: Kernel.Support.PopCap.TextTable.Type | '?';
			destination_type: Kernel.Support.PopCap.TextTable.Type | '?';
		}
	}

	export namespace Detail {
		export function type(): Array<[bigint, Kernel.Support.PopCap.TextTable.Type, string]> {
			return [
				[1n, 'utf16-text', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.text')],
				[2n, 'json-array', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.array')],
				[3n, 'json-map', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.map')],
				[4n, 'utf8-text', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.cn_text')],
			];
		}

		export const rule: Array<Kernel.Support.PopCap.TextTable.Type> = [
			'utf8-text',
			'json-array',
			'json-map',
			'utf16-text',
		];

		export function extension(destination: Kernel.Support.PopCap.TextTable.Type): string {
			switch (destination) {
				case 'json-array':
				case 'json-map':
					return 'json';
				case 'utf8-text':
				case 'utf16-text':
					return 'txt';
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
				Kernel.Support.PopCap.TextTable.convert_fs(
					argument.source,
					argument.destination!,
					argument.source_type!,
					argument.destination_type!,
				);
				Executor.clock.stop_safe();
			},
			batch_forward: undefined!,

			filter: ['file', /(.+)(\.json|\.txt)$/i],
			option: 32n,
		});
	}
}
Sen.Script.Executor.Functions.PopCap.PvZ2.LawnStrings.forward();
