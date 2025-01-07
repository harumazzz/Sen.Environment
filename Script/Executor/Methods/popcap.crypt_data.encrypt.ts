namespace Sen.Script.Executor.Methods.PopCap.CryptData.Encrypt {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		key?: string;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
		key?: string;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {
		key: '?' | string;
	}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.CryptData.Encrypt.Argument,
			Methods.PopCap.CryptData.Encrypt.BatchArgument,
			Methods.PopCap.CryptData.Encrypt.Configuration
		>({
			id: 'popcap.crypt_data.encrypt',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.crypt_data.encrypt.json',
			),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.bin`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				load_string(
					argument,
					'key',
					this.configuration,
					Kernel.Language.get('popcap.crypt_data.encrypt.key'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.CryptData.encrypt_fs(
					argument.source,
					argument.destination!,
					argument.key!,
				);
				clock.stop_safe();
				return;
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.bin$/i],
			option: 22n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.PopCap.CryptData.Encrypt.forward();
