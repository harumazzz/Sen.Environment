namespace Sen.Script.Executor.Methods.PopCap.PvZ2.LawnStrings.Convert {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		source_type?: Support.PopCap.LawnStrings.Convert.Conversion;
		destination_type?: Support.PopCap.LawnStrings.Convert.Conversion;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {
		source_type: Support.PopCap.LawnStrings.Convert.Conversion | '?';
		destination_type: Support.PopCap.LawnStrings.Convert.Conversion | '?';
	}

	export namespace Detail {
		/**
		 *
		 * Typical Style
		 *
		 */

		export function type(): Array<[bigint, string, string]> {
			return [
				[1n, 'text', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.text')],
				[2n, 'array', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.array')],
				[3n, 'map', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.map')],
				[4n, 'cn-text', Kernel.Language.get('popcap.pvz2.lawnstrings.convert.cn_text')],
			];
		}

		/**
		 * Rule
		 */

		export const rule: Array<Support.PopCap.LawnStrings.Convert.Conversion> = [
			'text',
			'array',
			'map',
			'cn-text',
		];

		/**
		 * JS Exchange
		 * @param destination - Destination type
		 * @returns
		 */

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

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.PvZ2.LawnStrings.Convert.Argument,
			Methods.PopCap.PvZ2.LawnStrings.Convert.BatchArgument,
			Methods.PopCap.PvZ2.LawnStrings.Convert.Configuration
		>({
			id: 'popcap.pvz2.lawnstrings.convert',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.pvz2.lawnstrings.convert.json',
			),
			configuration: undefined!,
			direct_forward(argument: Argument): void {
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
				return;
			},
			batch_forward: undefined!,
			is_enabled: true,
			filter: ['file', /(.+)(\.json|\.txt)$/i],
			option: 35n,
		});
		return;
	}
}
Sen.Script.Executor.Methods.PopCap.PvZ2.LawnStrings.Convert.forward();
