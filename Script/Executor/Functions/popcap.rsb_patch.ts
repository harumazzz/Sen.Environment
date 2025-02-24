namespace Sen.Script.Executor.Functions.PopCap.RSBPatch {
	export namespace Decode {
		export interface Argument extends Executor.Base {
			source: string;
			before_file?: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {}

		export interface Configuration extends Executor.Configuration {}
	}

	export namespace Encode {
		export interface Argument extends Executor.Base {
			source: string;
			after_file?: string;
			destination?: string;
		}

		export interface BatchArgument extends Executor.Base {}

		export interface Configuration extends Executor.Configuration {}
	}

	export function forward(): void {
		inject<
			Functions.PopCap.RSBPatch.Decode.Argument,
			Functions.PopCap.RSBPatch.Decode.BatchArgument,
			Functions.PopCap.RSBPatch.Decode.Configuration
		>({
			id: 'popcap.rsb_patch.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb_patch.decode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				argument.before_file = Console.path(
					Kernel.Language.get('popcap.rsb_patch.decode.before_file'),
					'file',
				);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.rsb`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceStreamBundlePatch.decode_fs(
					argument.source,
					argument.before_file!,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,
			configuration: undefined!,
			filter: ['file', /(.+)(\.rsbpatch)$/i],
			option: 49n,
		});
		inject<
			Functions.PopCap.RSBPatch.Encode.Argument,
			Functions.PopCap.RSBPatch.Encode.BatchArgument,
			Functions.PopCap.RSBPatch.Encode.Configuration
		>({
			id: 'popcap.rsb_patch.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsb_patch.encode.json'),
			direct_forward(argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				argument.after_file = Console.path(
					Kernel.Language.get('popcap.rsb_patch.encode.after_file'),
					'file',
				);
				defined_or_default(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.after_file!)}.rsbpatch`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceStreamBundlePatch.encode_fs(
					argument.source,
					argument.after_file!,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward: undefined!,
			configuration: undefined!,
			filter: ['file', /(.+)(\.rsb|\.obb)$/i],
			option: 50n,
		});
	}
}

Sen.Script.Executor.Functions.PopCap.RSBPatch.forward();
