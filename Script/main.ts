namespace Sen.Script {
	/**
	 * --------------------------------------------------
	 * JavaScript custom formatter
	 * @param str - string to format
	 * @param args - arguments
	 * @returns formatted string
	 * --------------------------------------------------
	 */

	export function format(str: string, ...args: Array<any>): string {
		for (const arg of args) {
			str = str.replace('{}', arg.toString());
		}
		return str;
	}

	/**
	 *
	 * @param value - Any JS Value
	 * @returns
	 */

	export function debug(value: unknown): void {
		if (typeof value === 'object') return Console.display(Kernel.JSON.serialize(value));
		Console.display(value as string);
	}

	/**
	 *
	 * @param condition - Conditional to assert
	 * @param message - Message if assert failed
	 * @returns
	 */

	export function assert(condition: boolean, message?: string): asserts condition {
		if (!condition) {
			throw new Error(message);
		}
	}

	export const is_gui: boolean = Shell.callback('is_gui')[0] === '1';

	/**
	 *
	 * @param args - Arguments to print
	 */

	export function print(...args: Array<string>): void {
		Shell.callback('display', ...args);
	}

	/**
	 *
	 * @returns Result of the readline
	 */

	export function readline(): string {
		if (is_gui) return Shell.callback('input_string')[0];
		else return Shell.callback('input')[0];
	}

	/**
	 * Console namespace of Script
	 */

	export namespace Console {
		/**
		 * --------------------------------------------------
		 * JavaScript send output message
		 * @param title - title to send
		 * @param message - message to send
		 * @param color - color to input
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function display(
			title: string,
			color: Kernel.Color = 'default',
			message?: string,
		): void {
			const prefix = is_gui ? '' : '● ';
			const new_tille = `${prefix}${title}`;
			let msg = message ? message : '';
			if (!is_gui && msg !== '') msg = `    ${msg}`;
			return print(new_tille, color, msg);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript send error message
		 * @param str - string to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function error(name: string, str: string): void {
			return Console.display(`${Kernel.Language.get('runtime_error')}: ${name}`, 'red', str);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript send argument message
		 * @param str - string to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function argument(str: any): void {
			const title = is_gui
				? `${Kernel.Language.get('execution_argument')}:`
				: `${Kernel.Language.get('execution_argument')}: ${str}`;
			const message = is_gui ? str : '';
			return display(title, 'cyan', message);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript send finished message
		 * @returns The console output
		 * --------------------------------------------------
		 * @param subtitle
		 * @param message
		 */

		export function finished(subtitle: string, message?: string): void {
			return display(
				`${Kernel.Language.get('execution_finished')}: ${subtitle}`,

				'green',
				message,
			);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript notify
		 * @param source - source to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function obtained(source: string): void {
			return display(`${Kernel.Language.get('input_argument')}:`, 'cyan', source);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript notify
		 * @param source - source to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function output(source: string): void {
			return display(`${Kernel.Language.get('output_argument')}:`, 'green', source);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript notify
		 * @param source - source to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function warning(source: string): void {
			return Console.display(
				`${Kernel.Language.get('execution_warning')}:`,

				'yellow',
				source,
			);
		}

		// Path type

		export type Path = 'file' | 'directory' | 'any';

		/**
		 * Path input
		 * @param source - Message
		 * @param type - Type of input
		 * @returns
		 */

		export function path(source: string, type: Path): string {
			Console.argument(source);
			let destination: string = undefined!;
			while (true) {
				destination = readline();
				if (destination.length === 0) {
					Console.finished(Kernel.Language.get('argument_got'), destination);
					break;
				}
				if (destination === 'p') {
					destination = Shell.callback(
						type === 'file' ? 'pick_file' : 'pick_directory',
					)[0];
					Console.obtained(destination);
					break;
				} else {
					if (/^["'].*["']$/.test(destination)) {
						destination = destination.slice(1, -1);
					}
					if (type === 'any') break;
					if (type === 'file' && Kernel.FileSystem.is_file(destination)) break;
					if (type === 'directory' && Kernel.FileSystem.is_directory(destination)) break;
				}
			}
			if (destination.length !== 0) {
				Console.finished(Kernel.Language.get('argument_got'), destination);
			}
			return destination;
		}
	}

	/**
	 * JavaScript Home
	 */

	export namespace Home {
		/**
		 * --------------------------------------------------
		 * JS default path
		 * --------------------------------------------------
		 */

		export let participant: string = undefined!;

		/**
		 * --------------------------------------------------
		 * JavaScript setup current home
		 * @returns Setup home
		 * --------------------------------------------------
		 */

		export function setup(script: string): void {
			participant = Kernel.Path.dirname(script);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript query current home
		 * @param path - Path to query
		 * @returns The resolved path
		 * --------------------------------------------------
		 */

		export function query(path: string): string {
			return path.replace(/^~(?=(\/|$))/, participant);
		}
	}

	/**
	 * JS exception
	 */

	export namespace Exception {
		/**
		 * --------------------------------------------------
		 * JS make stack, it can catch c++ code too
		 * @param stack - current stack
		 * @returns newly stack
		 * --------------------------------------------------
		 */

		export function make_stack(stack: string): string {
			let base_stack = stack
				.replace(/^\s*at/gm, `    ${Kernel.Language.get('at')}`)
				.replaceAll('\\', '/')
				.split('\n')
				.map((line) => line.replace(/\((.*)(?=(Kernel|Script))/g, '('))
				.filter((line) => !/\s<eval>\s/.test(line));
			return is_gui
				? base_stack.map((line) => line.trim().replaceAll('../', '')).join('\n')
				: base_stack.join('\n');
		}

		/**
		 * JS Make exception for error
		 * @param e - Error
		 * @returns error with stack
		 */

		export function make_exception_cli(e: Error): string {
			let result: string = `${e.message}\n● ${Kernel.Language.get('stack')}:\n`;
			result += make_stack(e.stack!);
			result = result.replace(/\n$/, '');
			return result;
		}

		/**
		 *
		 * @param e - Error
		 * @returns
		 */

		export function make_exception(e: Error): string {
			if (is_gui) {
				Console.error(e.name, e.message);
				Shell.callback(
					'display_stack',
					`${Kernel.Language.get('stack')}:`,
					make_stack(e.stack!).replace(/\n$/, ''),
				);
				return undefined!;
			}
			return make_exception_cli(e);
		}
	}

	/**
	 * --------------------------------------------------
	 * Script version
	 * --------------------------------------------------
	 */

	export const version = 15 as const;

	/**
	 * --------------------------------------------------
	 * Main function
	 * @returns
	 * --------------------------------------------------
	 */

	export function main(data: { arguments: Array<string>; home: string }): void {
		const result: RuntimeException | undefined = launch(data.arguments);
		if (result !== undefined) {
			Console.error(result.name, result.message);
		}
		if (is_gui) {
			Console.finished(
				Kernel.Language.get('method_are_succeeded'),
				Kernel.Language.get('js.relaunch_tool'),
			);
		} else {
			Console.finished(Kernel.Language.get('method_are_succeeded'));
			Shell.callback('finish');
		}
	}

	export type RuntimeException = { name: string; message: string };

	/**
	 * --------------------------------------------------
	 * Main thread
	 * @returns - result after execute
	 * --------------------------------------------------
	 */

	export function launch(args: Array<string>): RuntimeException | undefined {
		let result: RuntimeException | undefined = undefined;
		try {
			Home.setup(args[2]);
			Console.display(
				`Sen ~ Shell: ${
					Shell.callback('version')[0]
				} & Kernel: ${Kernel.version()} & Script: ${version}`,
				'default',
				args[0],
			);
			Module.load_module();
			args.splice(0, 3);
			Setting.load();
			Console.finished(
				Kernel.Language.get('current_status'),
				format(
					Kernel.Language.get('js.environment_has_been_loaded'),
					1n,
					1n,
					Module.modules.length + Module.entries.length + 1,
				),
			);
			Module.load_entries();
			Executor.forward({ source: args });
		} catch (e: any) {
			result = {
				name: e.name,
				message: Exception.make_exception(e),
			};
		}
		return result;
	}

	/**
	 * JavaScript Modules
	 */

	export namespace Module {
		/**
		 * --------------------------------------------------
		 * All JS modules need to be initialized
		 * @returns
		 * --------------------------------------------------
		 */

		export function load(scripts: Array<string>): void {
			for (const script of scripts) {
				Kernel.JavaScript.evaluate_fs(Home.query(script));
			}
		}

		export const load_module = () => load(modules);

		export const load_entries = () => load(entries);

		/**
		 * Modules in queue await to be execute
		 */

		export const modules: Array<string> = [
			'~/Third/maxrects-packer/maxrects-packer.js',
			'~/Setting/Setting.js',
			'~/Support/Texture/Format.js',
			'~/Support/PopCap/ResourceGroup/Convert.js',
			'~/Support/PopCap/Atlas/Split.js',
			'~/Support/PopCap/Atlas/Pack.js',
			'~/Support/PopCap/Atlas/MultiResolution.js',
			'~/Support/PopCap/Animation/Miscellaneous/GenerateAnimation.js',
			'~/Support/PopCap/Animation/Miscellaneous/AddLibrary.js',
			'~/Support/PopCap/ResourceStreamBundle/Miscellaneous/PlatformConverter.js',
			'~/Support/PopCap/ReflectionObjectNotation/DecodeByLooseConstraints.js',
			'~/Support/Wwise/Media/Common.js',
			'~/Support/Wwise/Media/Decode.js',
			'~/Support/Wwise/Media/Encode.js',
			'~/Executor/Executor.js',
		];

		export const entries: Array<string> = [
			'~/Executor/Functions/js.js',
			'~/Executor/Functions/marmalade.dzip.js',
			'~/Executor/Functions/popcap.animation.js',
			'~/Executor/Functions/popcap.atlas.js',
			'~/Executor/Functions/popcap.cfw2.js',
			'~/Executor/Functions/popcap.compiled_text.js',
			'~/Executor/Functions/popcap.crypt_data.js',
			'~/Executor/Functions/popcap.newton.js',
			'~/Executor/Functions/popcap.pak.js',
			'~/Executor/Functions/popcap.particles.js',
			'~/Executor/Functions/popcap.player_info.js',
			'~/Executor/Functions/popcap.ptx.js',
			'~/Executor/Functions/popcap.pvz2.lawnstrings.js',
			'~/Executor/Functions/popcap.reanim.js',
			'~/Executor/Functions/popcap.render_effects.js',
			'~/Executor/Functions/popcap.res_info.js',
			'~/Executor/Functions/popcap.resource_group.js',
			'~/Executor/Functions/popcap.rsb_patch.js',
			'~/Executor/Functions/popcap.rsb.js',
			'~/Executor/Functions/popcap.rsg.js',
			'~/Executor/Functions/popcap.rton.js',
			'~/Executor/Functions/popcap.zlib.js',
			'~/Executor/Functions/project.rsb.js',
			'~/Executor/Functions/project.scg.js',
			'~/Executor/Functions/wwise.media.js',
			'~/Executor/Functions/wwise.soundbank.js',
		];
	}
}

Sen.Script.main;
