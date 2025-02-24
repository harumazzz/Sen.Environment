namespace Sen.Script.Executor.Functions.PopCap.CryptData {
	export namespace Decrypt {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			key?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			key?: string;
		}

		export interface Configuration extends Executor.Configuration {
			key: '?' | string;
		}
	}

	export namespace Encrypt {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
			key?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
			key?: string;
		}

		export interface Configuration extends Executor.Configuration {
			key: '?' | string;
		}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.CryptData.Decrypt.Argument,
			Functions.PopCap.CryptData.Decrypt.BatchArgument,
			Functions.PopCap.CryptData.Decrypt.Configuration
		>({
			id: 'popcap.crypt_data.decrypt',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.crypt_data.decrypt.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
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
					Kernel.Language.get('popcap.crypt_data.decrypt.key'),
				);
				clock.start_safe();
				Kernel.Support.PopCap.CryptData.decrypt_fs(
					argument.source,
					argument.destination!,
					argument.key!,
				);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.bin$/i],
			option: 18n,
		});
		inject<
			Functions.PopCap.CryptData.Encrypt.Argument,
			Functions.PopCap.CryptData.Encrypt.BatchArgument,
			Functions.PopCap.CryptData.Encrypt.Configuration
		>({
			id: 'popcap.crypt_data.encrypt',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.crypt_data.encrypt.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
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
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.bin$/i],
			option: 19n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.CryptData.forward();
