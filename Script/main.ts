namespace Sen.Script {
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
			message: any,
			color: Definition.Console.Color = Definition.Console.Color.DEFAULT,
		): void {
			const is_gui = Shell.is_gui();
			const prefix = is_gui ? '' : '● ';
			const new_tille = `${prefix}${title}`;
			let msg = message ? message : '';
			if (!is_gui && msg !== '') msg = `    ${msg}`;
			return Kernel.Console.print(new_tille, msg, color);
		}

		export function send(
			message: any,
			color: Definition.Console.Color = Definition.Console.Color.DEFAULT,
		): void {
			return display(message, '', color);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript send error message
		 * @param str - string to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function error(str: string | undefined): void {
			if (str === undefined) return;
			return Console.display(
				`${Kernel.Language.get('runtime_error')}:`,
				str,
				Definition.Console.Color.RED,
			);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript send argument message
		 * @param str - string to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function argument(str: any): void {
			const title = Shell.is_gui()
				? `${Kernel.Language.get('execution_argument')}:`
				: `${Kernel.Language.get('execution_argument')}: ${str}`;
			const message = Shell.is_gui() ? str : '';
			return display(title, message, Definition.Console.Color.CYAN);
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
				Definition.Console.Color.GREEN,
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
			return display(
				`${Kernel.Language.get('input_argument')}:`,
				source,
				Definition.Console.Color.CYAN,
			);
		}

		/**
		 * --------------------------------------------------
		 * JavaScript notify
		 * @param source - source to send
		 * @returns The console output
		 * --------------------------------------------------
		 */

		export function output(source: string): void {
			return display(
				`${Kernel.Language.get('output_argument')}:`,
				source,
				Definition.Console.Color.GREEN,
			);
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
				Kernel.Language.get('execution_warning'),
				source,
				Definition.Console.Color.YELLOW,
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
				destination = Kernel.Console.readline();
				switch (destination) {
					case ':p':
						if (type === 'file') {
							destination = Shell.callback(['pick_file']);
						}
						if (type === 'directory') {
							destination = Shell.callback(['pick_directory']);
						}
						Console.obtained(destination);
						break loop;

					default:
						if (destination.startsWith('"') && destination.endsWith('"')) {
							destination = destination.slice(1, destination.length - 1);
						}
						if (type === 'file') {
							if (Kernel.FileSystem.is_file(destination)) {
								break loop;
							}
							Console.warning(
								format(Kernel.Language.get('file_not_found'), destination),
							);
						}
						if (type === 'directory') {
							if (Kernel.FileSystem.is_directory(destination)) {
								break loop;
							}
							Console.warning(
								format(Kernel.Language.get('directory_not_found'), destination),
							);
						}
						if (type === 'any') {
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

		export function setup(): void {
			participant = Kernel.Path.dirname(Sen.Kernel.Home.script());
			return;
		}

		/**
		 * --------------------------------------------------
		 * JavaScript query current home
		 * @param path - Path to query
		 * @returns The resolved path
		 * --------------------------------------------------
		 */

		export function query(path: string): string {
			return Kernel.Path.resolve(path.replace(/^~(?=(\/|$))/gm, participant));
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
				.replaceAll('at', `${Kernel.Language.get('at')}`)
				.replaceAll('\\', '/')
				.split('\n')
				.map((e) => e.replace(/(?<=\()(.*)(?=(Kernel|Script))/, ''))
				.filter((e: string) => !/(\s)<eval>(\s)/m.test(e));
			if (Shell.is_gui()) {
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
			let result: string = `${e.message}`;
			result += `\n● ${Kernel.Language.get('stack')}:\n`;
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
			if (Shell.is_gui()) {
				Console.error(e.message);
				Console.display(
					`${Kernel.Language.get('stack')}`,
					`${make_stack(e.stack!).replace(/\n$/, '')}`,
					Definition.Console.Color.RED,
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

	export const version = 8 as const;

	/**
	 * --------------------------------------------------
	 * Main function
	 * @returns
	 * --------------------------------------------------
	 */

	export function main(): void {
		const result: string = launch();
		Console.error(result);
		Console.finished(Kernel.Language.get('method_are_succeeded'));
		Shell.callback(['finish']);
		return;
	}

	/**
	 * --------------------------------------------------
	 * Main thread
	 * @returns - result after execute
	 * --------------------------------------------------
	 */

	export function launch(): string {
		let result: string = undefined!;
		try {
			const args = Kernel.arguments();
			args.splice(0, 3);
			Home.setup();
			Module.load();
			Console.send(
				`Sen ~ Shell: ${Shell.version()} & Kernel: ${Kernel.version()} & Script: ${version} ~ ${Kernel.OperatingSystem.current()} & ${Kernel.OperatingSystem.architecture()}`,
			);
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
				Sen.Kernel.JavaScript.evaluate_fs(Home.query(script));
			}
			return;
		}

		/**
		 * Modules in queue await to be execute
		 */

		export const script_list: Array<string> = [
			'~/Third/maxrects-packer/maxrects-packer.js',
			'~/utility/Miscellaneous.js',
			'~/Setting/Setting.js',
			'~/utility/Definition.js',
			'~/Support/Texture/Format.js',
			'~/Support/PopCap/ResourceGroup/Convert.js',
			'~/Support/PopCap/Atlas/Split.js',
			'~/Support/PopCap/Atlas/Pack.js',
			'~/Support/PopCap/Atlas/MultiResolution.js',
			'~/Support/PopCap/Animation/Miscellaneous/GenerateAnimation.js',
			'~/Support/PopCap/Animation/Miscellaneous/AddLibrary.js',
			'~/Support/PopCap/LawnStrings/Convert.js',
			'~/Support/PopCap/ResourceStreamBundle/Miscellaneous/Obfuscate.js',
			'~/Support/PopCap/ResourceStreamBundle/Miscellaneous/PlatformConverter.js',
			'~/Support/PopCap/ReflectionObjectNotation/DecodeByLooseConstraints.js',
			'~/Support/Wwise/Media/Common.js',
			'~/Support/Wwise/Media/Decode.js',
			'~/Support/Wwise/Media/Encode.js',
			'~/Executor/Executor.js',
			'~/Executor/Methods/js.evaluate.js',
			'~/Executor/Methods/data.md5.hash.js',
			'~/Executor/Methods/data.base64.encode.js',
			'~/Executor/Methods/data.base64.decode.js',
			'~/Executor/Methods/popcap.rton.decode.js',
			'~/Executor/Methods/popcap.rton.force_decode.js',
			'~/Executor/Methods/popcap.rton.encode.js',
			'~/Executor/Methods/popcap.rton.decrypt.js',
			'~/Executor/Methods/popcap.rton.encrypt.js',
			'~/Executor/Methods/popcap.rton.decrypt_and_decode.js',
			'~/Executor/Methods/popcap.rton.encode_and_encrypt.js',
			'~/Executor/Methods/popcap.resource_group.split.js',
			'~/Executor/Methods/popcap.resource_group.merge.js',
			'~/Executor/Methods/popcap.resource_group.convert.js',
			'~/Executor/Methods/popcap.res_info.split.js',
			'~/Executor/Methods/popcap.res_info.merge.js',
			'~/Executor/Methods/popcap.res_info.convert.js',
			'~/Executor/Methods/popcap.newton.decode.js',
			'~/Executor/Methods/popcap.newton.encode.js',
			'~/Executor/Methods/popcap.crypt_data.decrypt.js',
			'~/Executor/Methods/popcap.crypt_data.encrypt.js',
			'~/Executor/Methods/popcap.animation.decode.js',
			'~/Executor/Methods/popcap.animation.encode.js',
			'~/Executor/Methods/popcap.animation.to_flash.js',
			'~/Executor/Methods/popcap.animation.from_flash.js',
			'~/Executor/Methods/popcap.animation.decode_and_to_flash.js',
			'~/Executor/Methods/popcap.animation.from_flash_and_encode.js',
			'~/Executor/Methods/animation.flash.resize.js',
			'~/Executor/Methods/popcap.animation.to_apng.js',
			'~/Executor/Methods/popcap.cfw2.decode.js',
			'~/Executor/Methods/popcap.cfw2.encode.js',
			'~/Executor/Methods/popcap.compiled_text.decode.js',
			'~/Executor/Methods/popcap.compiled_text.encode.js',
			'~/Executor/Methods/popcap.ptx.encode.js',
			'~/Executor/Methods/popcap.ptx.decode.js',
			'~/Executor/Methods/popcap.zlib.compress.js',
			'~/Executor/Methods/popcap.zlib.uncompress.js',
			'~/Executor/Methods/popcap.particles.decode.js',
			'~/Executor/Methods/popcap.particles.encode.js',
			'~/Executor/Methods/popcap.particles.to_xml.js',
			'~/Executor/Methods/popcap.particles.from_xml.js',
			'~/Executor/Methods/popcap.render_effects.decode.js',
			'~/Executor/Methods/popcap.render_effects.encode.js',
			'~/Executor/Methods/wwise.soundbank.decode.js',
			'~/Executor/Methods/wwise.soundbank.encode.js',
			'~/Executor/Methods/wwise.media.decode.js',
			'~/Executor/Methods/wwise.media.encode.js',
			'~/Executor/Methods/marmalade.dzip.unpack.js',
			'~/Executor/Methods/marmalade.dzip.pack.js',
			'~/Executor/Methods/popcap.rsg.unpack.js',
			'~/Executor/Methods/popcap.rsg.pack.js',
			'~/Executor/Methods/popcap.pak.unpack.js',
			'~/Executor/Methods/popcap.pak.pack.js',
			'~/Executor/Methods/popcap.rsb.unpack.js',
			'~/Executor/Methods/popcap.rsb.pack.js',
			'~/Executor/Methods/popcap.rsb.unpack_resource.js',
			'~/Executor/Methods/popcap.rsb.pack_resource.js',
			'~/Executor/Methods/popcap.rsb.obfuscate.js',
			'~/Executor/Methods/popcap.rsb_patch.decode.js',
			'~/Executor/Methods/popcap.rsb_patch.encode.js',
			'~/Executor/Methods/popcap.reanim.decode.js',
			'~/Executor/Methods/popcap.reanim.encode.js',
			'~/Executor/Methods/popcap.reanim.to_xml.js',
			'~/Executor/Methods/popcap.reanim.from_xml.js',
			'~/Executor/Methods/popcap.reanim.to_flash.js',
			'~/Executor/Methods/popcap.reanim.from_flash.js',
			'~/Executor/Methods/popcap.reanim.from_flash_and_encode.js',
			'~/Executor/Methods/popcap.reanim.decode_and_to_flash.js',
			'~/Executor/Methods/popcap.rsb.unpack_by_loose_constraints.js',
			'~/Executor/Methods/popcap.rsb.init_project.js',
			'~/Executor/Methods/popcap.rsb.build_project.js',
			'~/Executor/Methods/popcap.atlas.split_by_resource_group.js',
			'~/Executor/Methods/popcap.atlas.pack_by_resource_group.js',
			'~/Executor/Methods/popcap.atlas.split_by_res_info.js',
			'~/Executor/Methods/popcap.atlas.pack_by_res_info.js',
			'~/Executor/Methods/popcap.pvz2.lawnstrings.convert.js',
			'~/Executor/Methods/popcap.player_info.decode.js',
			'~/Executor/Methods/popcap.player_info.encode.js',
			'~/Executor/Methods/pvz2.scg.encode.js',
			'~/Executor/Methods/pvz2.scg.decode.js',
			'~/Executor/Methods/popcap.rsb.trace.js',
			'~/Executor/Methods/popcap.animation.add_library.js',
			'~/Executor/Methods/popcap.rsb.convert_platform.js',
		];
	}
}
