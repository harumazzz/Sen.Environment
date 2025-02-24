namespace Sen.Script.Executor.Functions.PopCap.PlayerInfo {
	export namespace Encode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Decode {
		export interface Argument extends Executor.Base {
			source: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {
			directory: string;
		}

		export interface Configuration extends Executor.Configuration {}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.PlayerInfo.Encode.Argument,
			Functions.PopCap.PlayerInfo.Encode.BatchArgument,
			Functions.PopCap.PlayerInfo.Encode.Configuration
		>({
			id: 'popcap.player_info.encode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.player_info.encode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.PlayerInfo.encode_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.dat\.json$/i],
			option: 28n,
		});
		inject<
			Functions.PopCap.PlayerInfo.Decode.Argument,
			Functions.PopCap.PlayerInfo.Decode.BatchArgument,
			Functions.PopCap.PlayerInfo.Decode.Configuration
		>({
			id: 'popcap.player_info.decode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.player_info.decode.json',
			),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default(argument, 'destination', `${argument.source}.json`);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.PlayerInfo.decode_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},

			configuration: undefined!,
			filter: ['file', /(.+)\.dat$/i],
			option: 29n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.PlayerInfo.forward();
