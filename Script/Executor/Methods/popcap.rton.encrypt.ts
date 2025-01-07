namespace Sen.Script.Executor.Methods.PopCap.RTON.Encrypt {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		key?: string;
		iv?: string;
		use_64_bit_variant?: boolean;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
		key?: string;
		iv?: string;
		use_64_bit_variant?: boolean;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {
		key?: string;
		iv?: string;
		use_64_bit_variant?: boolean;
	}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.RTON.Encrypt.Argument,
			Methods.PopCap.RTON.Encrypt.BatchArgument,
			Methods.PopCap.RTON.Encrypt.Configuration
		>({
			id: 'popcap.rton.encrypt',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rton.encrypt.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${argument.source}.bin`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_string(
					argument,
					'key',
					this.configuration,
					Kernel.Language.get('popcap.rton.decrypt.key'),
				);
				load_string(
					argument,
					'iv',
					this.configuration,
					Kernel.Language.get('popcap.rton.decrypt.iv'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.RTON.encrypt_fs(
					argument.source,
					argument.destination!,
					argument.key!,
					argument.iv!,
				);
				clock.stop_safe();
				return;
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)(\.rton|pp\.dat)$/i],
			option: 69n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.PopCap.RTON.Encrypt.forward();
