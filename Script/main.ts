namespace Sen.Script {
	/**
	 *
	 * @returns Test is the current Shell is a GUI interface
	 */

	export function is_gui(): boolean {
		return Shell.callback('is_gui')[0] === '1';
	}

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
		return Shell.callback('input')[0];
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
			message?: string,
			color: Kernel.Color = 'default',
		): void {
			const is = is_gui();
			const prefix = is ? '' : '● ';
			const new_tille = `${prefix}${title}`;
			let msg = message ? message : '';
			if (!is && msg !== '') msg = `    ${msg}`;
			return print(new_tille, msg, color);
		}

		export function send(message: any, color: Kernel.Color = 'default'): void {
			return display(message, '', color);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript send error message
		 * @param str - string to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function error(str: string): void {
			return Console.display(`${Kernel.Language.get('runtime_error')}:`, str, 'red');
		}

		/**
		 * --------------------------------------------------
		 * JavaScript send argument message
		 * @param str - string to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function argument(str: any): void {
			const title = is_gui()
				? `${Kernel.Language.get('execution_argument')}:`
				: `${Kernel.Language.get('execution_argument')}: ${str}`;
			const message = is_gui() ? str : '';
			return display(title, message, 'cyan');
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
				message,
				'green',
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
			return display(`${Kernel.Language.get('input_argument')}:`, source, 'cyan');
		}

		/**
		 * --------------------------------------------------
		 * JavaScript notify
		 * @param source - source to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function output(source: string): void {
			return display(`${Kernel.Language.get('output_argument')}:`, source, 'green');
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
				source,
				'yellow',
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
			loop: do {
				destination = readline();
				switch (destination) {
					case ':p':
						if (type === 'file') destination = Shell.callback('pick_file')[0];
						else destination = Shell.callback('pick_directory')[0];

						Console.obtained(destination);
						break loop;

					default:
						if (
							(destination.startsWith('"') && destination.endsWith('"')) ||
							(destination.startsWith("'") && destination.endsWith("'"))
						) {
							destination = destination.slice(1, -1);
						}
						switch (type) {
							case 'file':
								if (Kernel.FileSystem.is_file(destination)) break loop;
								Console.warning(
									format(Kernel.Language.get('file_not_found'), destination),
								);
								break;
							case 'directory':
								if (Kernel.FileSystem.is_directory(destination)) break loop;

								Console.warning(
									format(Kernel.Language.get('directory_not_found'), destination),
								);
								break loop;
							default:
								break loop;
						}
				}
			} while (true);
			if (destination !== '') {
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
				.replaceAll('at', Kernel.Language.get('at'))
				.replaceAll('\\', '/')
				.split('\n')
				.map((e) => e.replace(/(?<=\()(.*)(?=(Kernel|Script))/, ''))
				.filter((e: string) => !/(\s)<eval>(\s)/m.test(e));
			if (is_gui()) {
				return base_stack.map((e) => e.trim().replaceAll('../', '')).join('\n');
			}
			return base_stack.join('\n');
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
			if (is_gui()) {
				Console.error(e.message);
				Console.display(
					`${Kernel.Language.get('stack')}:`,
					make_stack(e.stack!).replace(/\n$/, ''),
					'red',
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

	export const version = 14 as const;

	/**
	 * --------------------------------------------------
	 * Main function
	 * @returns
	 * --------------------------------------------------
	 */

	export function main(data: {
		arguments: Array<string>;
		home: string;
		error: string | undefined;
	}): void {
		const result: string | undefined = launch(data.arguments);
		if (result !== undefined) {
			data.error = result;
			Console.error(result);
		}
		if (is_gui()) {
			Console.finished(
				Kernel.Language.get('method_are_succeeded'),
				Kernel.Language.get('js.relaunch_tool'),
			);
		} else {
			Console.finished(Kernel.Language.get('method_are_succeeded'));
			Shell.callback('finish');
		}
	}

	/**
	 * --------------------------------------------------
	 * Main thread
	 * @returns - result after execute
	 * --------------------------------------------------
	 */

	export function launch(args: Array<string>): string | undefined {
		let result: string | undefined = undefined;
		try {
			Home.setup(args[2]);
			Console.display(
				`Sen ~ Shell: ${
					Shell.callback('version')[0]
				} & Kernel: ${Kernel.version()} & Script: ${version}`,
				args[0],
			);
			Module.load();
			args.splice(0, 3);
			Setting.load();
			Console.finished(
				Kernel.Language.get('current_status'),
				format(
					Kernel.Language.get('js.environment_has_been_loaded'),
					1n,
					1n,
					Module.script_list.length + 1,
				),
			);
			Executor.forward({ source: args });
		} catch (e: any) {
			result = Exception.make_exception(e);
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

		export function load(): void {
			for (const script of script_list) {
				Kernel.JavaScript.evaluate_fs(Home.query(script));
			}
		}

		/**
		 * Modules in queue await to be execute
		 */

		export const script_list: Array<string> = [
			'~/Third/maxrects-packer/maxrects-packer.js',
			'~/utility/Miscellaneous.js',
			'~/Setting/Setting.js',
			'~/Support/Texture/Format.js',
			'~/Support/PopCap/ResourceGroup/Convert.js',
			'~/Support/PopCap/Atlas/Split.js',
			'~/Support/PopCap/Atlas/Pack.js',
			'~/Support/PopCap/Atlas/MultiResolution.js',
			'~/Support/PopCap/Animation/Miscellaneous/GenerateAnimation.js',
			'~/Support/PopCap/Animation/Miscellaneous/AddLibrary.js',
			'~/Support/PopCap/Lawnstrings/Convert.js',
			'~/Support/PopCap/ResourceStreamBundle/Miscellaneous/PlatformConverter.js',
			'~/Support/PopCap/ReflectionObjectNotation/DecodeByLooseConstraints.js',
			'~/Support/Wwise/Media/Common.js',
			'~/Support/Wwise/Media/Decode.js',
			'~/Support/Wwise/Media/Encode.js',
			'~/Executor/Executor.js',
			'~/Executor/Functions/js.js',
			'~/Executor/Functions/popcap.rton.js',
			'~/Executor/Functions/popcap.resource_group.js',
			'~/Executor/Functions/popcap.res_info.js',
			'~/Executor/Functions/popcap.newton.js',
			'~/Executor/Functions/popcap.crypt_data.js',
			'~/Executor/Functions/popcap.animation.js',
			'~/Executor/Functions/popcap.cfw2.js',
			'~/Executor/Functions/popcap.compiled_text.js',
			'~/Executor/Functions/popcap.ptx.js',
			'~/Executor/Functions/popcap.zlib.js',
			'~/Executor/Functions/popcap.particles.js',
			'~/Executor/Functions/popcap.render_effects.js',
			'~/Executor/Functions/wwise.soundbank.js',
			'~/Executor/Functions/wwise.media.js',
			'~/Executor/Functions/marmalade.dzip.js',
			'~/Executor/Functions/popcap.rsg.js',
			'~/Executor/Functions/popcap.pak.js',
			'~/Executor/Functions/popcap.rsb.js',
			'~/Executor/Functions/popcap.rsb_patch.js',
			'~/Executor/Functions/popcap.reanim.js',
			'~/Executor/Functions/project.rsb.js',
			'~/Executor/Functions/popcap.atlas.js',
			'~/Executor/Functions/popcap.pvz2.lawnstrings.js',
			'~/Executor/Functions/popcap.player_info.js',
			'~/Executor/Functions/project.scg.js',
		];
	}
}

Sen.Script.main;
