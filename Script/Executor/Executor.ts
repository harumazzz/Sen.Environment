namespace Sen.Script.Executor {
	// The base need to be an object that is declared later

	export type Base = {
		[x: string]: unknown;
	} & {
		source: unknown;
	};

	// Base Configuration need to be inherited

	export interface Configuration {
		[x: string]: unknown;
	}

	export type MethodType = 'file' | 'directory' | 'any' | 'files';

	// Method Executor should implement direct forward, batch forward and async forward

	export interface MethodExecutor<
		Argument extends Executor.Base,
		BatchArgument extends Executor.Base,
		Configuration extends Executor.Configuration,
	> {
		id: string;
		configuration_file: string;
		direct_forward: (argument: Argument) => void;
		batch_forward?: (argument: BatchArgument) => void;
		configuration: Configuration;
		filter: [MethodType, RegExp] | [MethodType, ...Array<RegExp>];
		option: bigint;
	}

	/**
	 * Forwarder typical type
	 */

	export enum Forward {
		DIRECT,
		BATCH,
	}

	/**
	 * Clock need to be initialized during the runtime.
	 * Clock will calculate everything
	 */

	export const clock: Kernel.Clock = new Kernel.Clock();

	/**
	 *
	 * All methods are assigned here as key | value
	 * Key: must be the id of the typical module
	 * Value: the worker
	 *
	 */

	const methods: Map<
		string,
		Executor.MethodExecutor<Executor.Base, Executor.Base, Executor.Configuration>
	> = new Map();

	/**
	 * ----------------------------------------------------------
	 * JavaScript Implementation of Executor
	 * @param worker - Here, we assign worker with the typical
	 * object that has been declared
	 * @returns - and so, the typical method will be assign
	 * if the key is not found
	 * ----------------------------------------------------------
	 */

	export function inject<
		Argument extends Executor.Base,
		BatchArgument extends Executor.Base,
		Configuration extends Executor.Configuration,
	>(worker: MethodExecutor<Argument, BatchArgument, Configuration>): void {
		const id: string = worker.id!;
		delete (worker as any).id;
		assert(
			methods.get(id) === undefined,
			format(Kernel.Language.get('js.id_already_existed'), id),
		);
		methods.set(id, worker as MethodExecutor<Base, Base, Configuration>);
	}

	/**
	 * ----------------------------------------------------------
	 * JavaScript Executor Implement
	 * @param argument - Argument to query
	 * @param key - Key
	 * @param defined_value - If not, this val will assign to it
	 * @returns
	 * ----------------------------------------------------------
	 */

	export function defined_or_default<Argument extends Executor.Base, T>(
		argument: Argument,
		key: string,
		defined_value: T,
	): void {
		if ((argument as any & Argument)[key] === undefined) {
			(argument as any & Argument)[key] = defined_value;
		}
	}

	export type IntegerRule = Array<[bigint, string | bigint, string]>;

	export function delete_invalid_value<
		Argument extends Executor.Base,
		Configuration extends Executor.Configuration,
		Return,
	>(
		argument: Argument,
		key: keyof Argument & keyof Configuration,
		callback: () => Return,
	): Return {
		Console.warning(format(Kernel.Language.get('script.invalid_input_data'), argument[key]));
		delete (argument as any & Argument)[key];
		return callback();
	}

	export function invalid_argument_then<
		Argument extends Executor.Base,
		Configuration extends Executor.Configuration,
		Return,
	>(
		configuration: Configuration,
		key: keyof Argument & keyof Configuration,
		callback: () => Return,
	): Return {
		Console.display(
			format(Kernel.Language.get('invalid.argument'), configuration[key]),

			'red',
		);
		(configuration as any)[key] = '?';
		return callback();
	}

	export function load_bigint<
		Argument extends Executor.Base,
		Configuration extends Executor.Configuration,
	>(
		argument: Argument,
		key: keyof Argument & keyof Configuration,
		configuration: Configuration,
		rule: Array<bigint> | Array<[bigint, string | bigint, string]> | Array<string>,
		title: string,
	): void {
		Console.argument(title);
		if ((argument as any & Argument)[key] !== undefined) {
			if ((rule as IntegerRule).some((e) => e[1] === (argument as any)[key])) {
				print_argument(argument[key] as string);
			} else {
				return delete_invalid_value(argument, key as any, () =>
					load_bigint(argument, key, configuration, rule, title),
				);
			}
		} else if ((configuration as any)[key] === '?') {
			return configurate_or_input(argument, key as string, rule as IntegerRule);
		} else {
			const rof = (value: string) => {
				print_argument(value);
				(argument as any & Argument)[key] = configuration[key];
			};
			if (rule.includes(configuration[key] as unknown as bigint & string)) {
				rof(configuration[key] as string);
			} else if ((rule as IntegerRule).map((e) => e[1]).includes(configuration[key] as any)) {
				rof((rule as any)[Number((configuration[key] as bigint) - 1n)][2] as string);
			} else {
				invalid_argument_then(configuration, key as any, () =>
					load_bigint(argument, key, configuration, rule, title),
				);
			}
		}
	}

	/**
	 * ----------------------------------------------------------
	 * JavaScript Executor Implement
	 * @param argument - Argument to query
	 * @param key - Key
	 * @param defined_value - If not, this val will assign to it
	 * @returns
	 * ----------------------------------------------------------
	 */

	export function input_range<
		Argument extends Executor.Base,
		Configuration extends Executor.Configuration,
	>(
		argument: Argument,
		key: keyof Argument & keyof Configuration,
		configuration: Configuration,
		rule: [bigint, bigint],
		title: string,
	): void {
		Console.argument(title);
		if ((argument as any & Argument)[key] !== undefined) {
			if (
				((argument as any & Argument)[key] as bigint) <= rule[1] &&
				((argument as any & Argument)[key] as bigint) >= rule[0]
			) {
				print_argument(argument[key] as string);
			} else {
				return delete_invalid_value(argument, key as any, () =>
					input_range(argument, key, configuration, rule, title),
				);
			}
		} else if ((configuration as any)[key] === '?') {
			let input: string = undefined!;
			while (true) {
				input = readline();
				if (/\d+/.test(input) && rule[0] <= BigInt(input) && rule[1] >= BigInt(input)) {
					break;
				}
				Console.display(format(Kernel.Language.get('invalid.argument'), input), 'red');
			}
			(argument as any)[key] = BigInt(input);
		} else {
			const value = BigInt(configuration[key] as string);
			if (/\d+/.test(configuration[key] as string) && rule[0] <= value && rule[1] >= value) {
				print_argument(configuration[key] as string);
				(argument as any & Argument)[key] = value;
			} else {
				invalid_argument_then(configuration, key as any, () =>
					load_bigint(argument, key, configuration, rule, title),
				);
			}
		}
	}

	/**
	 * ----------------------------------------------------------
	 * JavaScript Executor Implement
	 * @param argument - Argument to query
	 * @param key - Key
	 * @param defined_value - If not, this val will assign to it
	 * @returns
	 * ----------------------------------------------------------
	 */

	export function load_string<
		Argument extends Executor.Base,
		Configuration extends Executor.Configuration,
	>(
		argument: Argument,
		key: keyof Argument & keyof Configuration,
		configuration: Configuration,
		title: string,
		rule?: Array<string>,
	): void {
		Console.argument(title);
		if ((argument as any & Argument)[key] !== undefined) {
			if (rule !== undefined && !rule.includes(argument[key] as string)) {
				return delete_invalid_value(argument, key as any, () =>
					load_string(argument, key, configuration, title, rule),
				);
			}
			return print_argument(argument[key] as string);
		} else if ((configuration as any)[key] === '?') {
			(argument as any)[key] = readline();
		} else {
			if (rule === undefined) {
				print_argument(configuration[key] as string);
				(argument as any & Argument)[key] = configuration[key];
			} else if (rule.includes(configuration[key] as string)) {
				print_argument(configuration[key] as string);
				(argument as any & Argument)[key] = configuration[key];
			} else {
				return invalid_argument_then(configuration, key as any, () =>
					load_string(argument, key, configuration, title, rule),
				);
			}
		}
	}

	/**
	 * ----------------------------------------------------------
	 * JavaScript Executor Implement
	 * @param argument - Argument to query
	 * @param key - Key
	 * @param defined_value - If not, this val will assign to it
	 * @returns
	 * ----------------------------------------------------------
	 */

	export function load_boolean<
		Argument extends Executor.Base,
		Configuration extends Executor.Configuration,
	>(
		argument: Argument,
		key: keyof Argument & keyof Configuration,
		configuration: Configuration,
		title: string,
	): void {
		Console.argument(title);
		if ((argument as any & Argument)[key] !== undefined) {
			if (!(typeof argument[key] === 'boolean')) {
				return delete_invalid_value(argument, key as any, () =>
					load_boolean(argument, key, configuration, title),
				);
			}
			return print_argument(argument[key] as any);
		}
		if ((configuration as any)[key] === '?') {
			(argument as any)[key] = input_boolean();
		} else {
			if (/^(true|false)$/.test(configuration[key] as string)) {
				print_argument(`${configuration[key]}`);
				(argument as any & Argument)[key] = Boolean(configuration[key]);
				return;
			}
			return invalid_argument_then(configuration, key as any, () =>
				load_boolean(argument, key, configuration, title),
			);
		}
	}

	export const k_boolean: Array<[string, bigint]> = [
		[Kernel.Language.get('input.set_argument_to_true'), 1n],
		[Kernel.Language.get('input.set_argument_to_false'), 2n],
	];

	export function input_boolean(): boolean {
		if (is_gui) {
			const result = Shell.callback('input_boolean')[0];
			return result === '1';
		} else {
			k_boolean.forEach((e) => print_statement(e[0], e[1]));
			const result = input_integer([1n, 2n], []);
			return result === 1n;
		}
	}

	export function make_enumeration(
		rule: Array<bigint>,
		functions_name: Array<string>,
	): Array<string> {
		const result: Array<string> = [];
		for (let i = 0; i < rule.length; ++i) {
			result.push(`${rule[i]}|${functions_name[i]}`);
		}
		return result;
	}

	/**
	 *
	 * @param rule - Rule to filter
	 * @returns Input in rule
	 */

	export function input_integer(rule: Array<bigint>, functions_name: Array<string>): bigint {
		let input: string = undefined!;
		if (is_gui) {
			input = Shell.callback(
				'input_enumeration',
				...make_enumeration(rule, functions_name),
			)[0];
		} else {
			while (true) {
				input = readline();
				if (/^\d+$/.test(input) && (rule as Array<bigint>).includes(BigInt(input))) {
					break;
				}
				Console.warning(Kernel.Language.get('js.invalid_input_value'));
			}
		}
		return BigInt(input);
	}

	export function is_valid_rule(rule: Array<bigint> | IntegerRule): number {
		const new_rule: Array<bigint> = [];
		rule.forEach((e: [bigint, any, string] & any) => {
			print_statement(e[2], e[0]);
			new_rule.push(e[0]);
		});
		return Number(
			input_integer(
				new_rule,
				is_gui_or([], () => rule.map((e: any) => e[2])),
			) - 1n,
		);
	}

	/**
	 * ----------------------------------------------------------
	 * JavaScript Executor Implement
	 * @param argument - Argument to query
	 * @param key - Key
	 * @param defined_value - If not, this val will assign to it
	 * @returns
	 * ----------------------------------------------------------
	 */

	export function configurate_or_input<Argument extends Executor.Base>(
		argument: Argument,
		key: keyof Argument,
		rule: Array<bigint> | IntegerRule,
	): void {
		if ((argument as any & Argument)[key] === undefined) {
			switch (typeof rule[0]) {
				case 'object':
					(argument as any)[key] = (rule as IntegerRule)[is_valid_rule(rule)]![1];
					break;
				case 'string':
					(argument as any)[key] = readline();
					break;
				case 'bigint':
					(argument as any)[key] = input_integer(
						rule as Array<bigint>,
						is_gui_or<Array<string>>(
							[],
							() => rule.map((e: any) => e[2]) as Array<string>,
						),
					);
					break;
			}
		}
	}

	export function is_gui_or<T>(not_gui: T, if_gui: () => T): T {
		return is_gui ? if_gui() : not_gui;
	}

	/**
	 * JS Filter
	 * @param param0 - Type
	 * @param source - Source file to test
	 * @returns Filter
	 */

	export function test_of([type, pattern]: [MethodType, RegExp], source: string): boolean {
		let is_valid: boolean = undefined!;
		switch (type) {
			case 'file':
				is_valid = Kernel.FileSystem.is_file(source);
				break;
			case 'directory':
				is_valid = Kernel.FileSystem.is_directory(source);
				break;
			case 'any':
				is_valid = true;
				break;
		}
		is_valid &&= pattern.test(source);
		return is_valid;
	}

	export function test_array(
		[type, ...method]: [MethodType, ...Array<RegExp>],
		source: Array<string>,
	): boolean {
		return source.every((e, i) => test_of([type, method[i]], e));
	}

	/**
	 * ----------------------------------------------------------
	 * JavaScript Implementation of Runner
	 * @param id - Here, we call the id. If the id is assigned
	 * as a method, the method will be called instantly
	 * @param argument - Provide arguments
	 * @param forward_type - Forwarder type
	 * @returns - Launch if the method found.
	 * If an error is thrown, it mean the id is not assigned
	 * ----------------------------------------------------------
	 */

	export function run_as_module<Argument extends Executor.Base>(
		id: string,
		argument: Argument,
		forward_type: Executor.Forward,
	): void {
		const worker:
			| Executor.MethodExecutor<Executor.Base, Executor.Base, Executor.Configuration>
			| undefined = methods.get(id);
		assert(worker !== undefined, format(Kernel.Language.get('js.method_not_found'), id));
		worker.configuration = Kernel.JSON.deserialize_fs<Configuration>(worker.configuration_file);
		Console.display(
			`${Kernel.Language.get('method_loaded')}:`,
			'green',
			`${Kernel.Language.get(id)} | ${id}`,
		);
		switch (forward_type) {
			case Forward.BATCH:
				assert(
					worker.batch_forward !== undefined,
					format(Kernel.Language.get('method_does_not_support_batch_implementation'), id),
				);
				worker.batch_forward(argument);
				break;
			case Forward.DIRECT:
				worker.direct_forward(argument);
				break;
			default:
				assert(false, format(Kernel.Language.get('js.method_does_not_execute')));
		}
		if (is_gui) {
			Console.finished(
				Kernel.Language.get('js.command_line_has_finished'),
				`${Kernel.Language.get('execution_time')}: ${clock
					.duration_as_seconds()
					.toFixed(3)}s`,
			);
		} else {
			Console.display(
				`${Kernel.Language.get('execution_time')}: ${clock
					.duration_as_seconds()
					.toFixed(3)}s`,
				'green',
			);
		}
	}

	export function display_argument(argument: string | string[]): void {
		const title = `${Kernel.Language.get('execution_argument')}:`;
		const color: Kernel.Color = 'cyan';
		if (typeof argument === 'string') {
			Console.display(title, color, argument);
		} else {
			Console.display(title, color);
			argument.forEach(print_argument);
		}
	}

	export function execute<Argument extends Base>(
		argument: Argument,
		id: string,
		forward: Forward,
		load: ExecuteType,
	): void {
		try {
			switch (load) {
				case 'simple':
					run_as_module<Argument>(id, argument, forward);
					break;
				case 'whole':
					assert(
						Array.isArray(argument.source),
						Kernel.Language.get('argument_must_be_list_of_string'),
					);
					(argument.source as Array<string>).forEach((e) =>
						run_as_module<Argument>(id, { ...argument, source: e }, forward),
					);
			}
		} catch (e: any) {
			let result = Exception.make_exception(e);
			if (result !== undefined) {
				Console.error(e.name, result);
			}
		}
		return;
	}

	export type ExecuteType = 'simple' | 'whole';

	export function print_statement(name: string, num: bigint | number): void {
		return print_argument(`${num}. ${Kernel.Language.get(name)}`);
	}

	export function print_argument(e: string): void {
		if (is_gui) return print(e.toString());
		else return print(`    ${e}`);
	}

	export function load_module<Argument extends Base>(
		argument: Argument,
		load: ExecuteType,
	): void {
		let modules: Map<bigint, string> = new Map<bigint, string>();
		const query = (
			callback:
				| (([type, method]: [MethodType, RegExp], source: string) => boolean)
				| ((
					[type, method]: [MethodType, ...Array<RegExp>],
					source: Array<string>,
				) => boolean),
			filter: [MethodType, RegExp | Array<RegExp>],
			source: string | string[],
			[method_name, option_number]: [method_name: string, option_number: bigint],
		) => {
			if (callback(filter as [MethodType, RegExp], source as string & string[])) {
				modules.set(option_number, method_name);
			}
		};
		methods.forEach((worker, method_name) => {
			if (typeof argument.source === 'string') {
				query(test_of, worker.filter as [MethodType, RegExp], argument.source as string, [
					method_name,
					worker.option,
				]);
			}
			if (Array.isArray(argument.source)) {
				query(
					test_array,
					worker.filter as [MethodType, RegExp],
					argument.source as string[],
					[method_name, worker.option],
				);
			}
		});
		display_argument(argument.source as string | string[]);
		Console.argument(Kernel.Language.get('js.input_an_method_to_start'));
		if (!is_gui) {
			print_statement(Kernel.Language.get('js.skip_argument_input'), 0n);
			modules.forEach(print_statement);
		}
		const view: Array<bigint> = Array.from(modules.keys());
		switch (view.length) {
			case 0:
				Console.display(Kernel.Language.get('js.argument_ignored'), 'red');
				break;
			case 1:
				execute<Argument>(argument, modules.get(view[0])!, Forward.DIRECT, load);
				break;
			default:
				const input_value: bigint = input_integer(
					[0n, ...view],
					[
						Kernel.Language.get('js.skip_argument_input'),
						...modules.values().map((e) => Kernel.Language.get(e)),
					],
				);
				if (input_value === 0n) return;
				execute<Argument>(argument, modules.get(input_value)!, Forward.DIRECT, load);
		}
	}

	export type ModuleLoader = Record<string, unknown> & { method: string };

	export function is_valid_source<Argument extends Base>(
		argument: Argument,
		is_directory: boolean,
	): void {
		if (argument.source === undefined) {
			argument.source = Console.path(
				Kernel.Language.get('input_argument'),
				is_directory ? 'directory' : 'file',
			);
		} else if (typeof argument.source !== 'string') {
			delete argument.source;
			return is_valid_source(argument, is_directory);
		} else if (is_directory && Kernel.FileSystem.is_file(argument.source as string)) {
			delete argument.source;
			return is_valid_source(argument, is_directory);
		} else if (!is_directory && Kernel.FileSystem.is_directory(argument.source)) {
			delete argument.source;
			return is_valid_source(argument, is_directory);
		}
	}

	export function exchange_argument_value<T>(value: string): T {
		if (/^\d+[fn]$/.test(value)) {
			return (
				value.endsWith('f') ? Number(value.slice(0, -1)) : BigInt(value.slice(0, -1))
			) as T;
		}
		if (/^(true|false)$/i.test(value)) {
			return (value === 'true') as T;
		}
		return value as T;
	}

	export function parse_argument<Argument extends Base & { source: Array<string> }>(
		argument: Argument,
		temporary: ModuleLoader,
	): void {
		const raw = argument.source;
		for (let i = 0; i < raw.length; i++) {
			if (raw[i].startsWith('-')) {
				temporary[raw[i].slice(1)] = exchange_argument_value(raw[++i]);
			}
		}
	}

	export function maybe_contains_atlas<Argument extends Base & { source: Array<string> }>(
		argument: Argument,
		temporary: ModuleLoader,
	): boolean {
		const pattern = /^popcap\.atlas\.split_by_(resource_group|res_info)$/;
		if (argument.source.some((e) => pattern.test(e))) {
			temporary.source = [];
			return true;
		}
		return false;
	}

	export function parse_atlas<Argument extends Base & { source: Array<string> }>(
		argument: Argument,
		temporary: ModuleLoader & { source: Array<string> },
	): void {
		const raw = argument.source;
		for (let i = 0; i < raw.length; i++) {
			if (raw[i] === '-source') {
				while (++i < raw.length && !raw[i].startsWith('-')) {
					temporary.source.push(exchange_argument_value(raw[i]));
				}
				i--;
			} else if (raw[i].startsWith('-')) {
				temporary[raw[i].slice(1)] = exchange_argument_value(raw[++i]);
			}
		}
	}

	export const k_type: Array<string> = [
		Kernel.Language.get('file'),
		Kernel.Language.get('directory'),
	];

	export function input_path<Argument extends Base & { source: Array<string> }>(
		argument: Argument,
	): void {
		Console.argument(
			Kernel.Language.get(
				'script.input_any_path_to_continue_or_provide_an_empty_string_to_exit',
			),
		);
		while (true) {
			let value = readline();
			if (value.length === 0) break;
			if (value === ':p') {
				Console.argument(Kernel.Language.get('input_type'));
				k_type.forEach((e, i) => print_statement(e, i + 1));
				const result = input_integer([1n, 2n], k_type);
				value = Shell.callback(result === 1n ? 'pick_file' : 'pick_directory')[0];
				if (value.length === 0) continue;
			}
			if (value === ':b') {
				Console.argument(Kernel.Language.get('input_number_to_process_batch_function'));
				const modules: Map<bigint, string> = new Map<bigint, string>();
				let counter: bigint = 0n;
				methods.forEach((value, key) => {
					if (value.batch_forward !== undefined) {
						modules.set(++counter, key);
					}
				});
				let option: bigint = undefined!;
				if (!is_gui) {
					print_statement(Kernel.Language.get('js.skip_argument_input'), counter);
					modules.forEach((v, k) => print_statement(Kernel.Language.get(v), k));
				}
				option = input_integer(
					[0n, ...modules.keys()],
					!is_gui
						? []
						: [Kernel.Language.get('js.skip_argument_input'), ...modules.values()],
				);
				if (option === 0n) break;
				(argument as any).directory = Console.path(
					Kernel.Language.get('input_directory'),
					'directory',
				);
				execute(argument, modules.get(option)!, Forward.BATCH, 'simple');
				break;
			}
			if (/^["'].*["']$/.test(value)) {
				value = value.slice(1, -1);
			}
			if (Kernel.FileSystem.is_file(value) || Kernel.FileSystem.is_directory(value)) {
				Console.argument(value);
				argument.source.push(value);
				continue;
			}
			Console.warning(
				format(Kernel.Language.get('js.input_is_not_a_file_or_directory'), value),
			);
		}
	}

	export let k_argument = (() => {
		return [
			[Kernel.Language.get('js.skip_argument_input'), 0n],
			[Kernel.Language.get('js.process_whole'), 1n],
			[Kernel.Language.get('js.process_in_queue'), 2n],
			[Kernel.Language.get('popcap.atlas.split_by_resource_group'), 3n],
			[Kernel.Language.get('popcap.atlas.split_by_res_info'), 4n],
		] as Array<[string, bigint]>
	});

	export function process_module<Argument extends Base>(argument: Argument): void {
		return (argument.source as Array<string>).forEach((e) =>
			load_module({ source: e }, 'simple'),
		);
	}

	export function forward<Argument extends Base>(argument: Argument): void {
		const loader: ModuleLoader = { method: undefined! };
		const has_atlas = maybe_contains_atlas(
			argument as Argument & { source: Array<string> },
			loader,
		);
		if (has_atlas) {
			parse_atlas(argument as Argument & { source: Array<string> }, loader as any);
		} else {
			parse_argument(argument as Argument & { source: Array<string> }, loader);
		}
		if (loader.method !== undefined) {
			const method = loader.method;
			delete (loader as any).method;
			execute(loader as Argument, method, Forward.DIRECT, 'simple');
			return;
		}
		if ((argument.source as Array<string>).length === 0) {
			input_path(argument as any);
		}
		argument.source = (argument.source as Array<string>).map((e: string) =>
			Kernel.Path.normalize(e),
		);
		if ((argument.source as Array<string>).length > 1) {
			if (is_gui) {
				Console.display(
					`${Kernel.Language.get('js.make_host.argument_obtained')}:`,
					'cyan',
					(argument.source as Array<string>).map((e, i) => `${i + 1}. ${e}`).join('\n'),
				);
			} else {
				Console.display(
					`${Kernel.Language.get('js.make_host.argument_obtained')}:`,
					'cyan',
				);
				(argument.source as Array<string>).forEach((e, i) => print_statement(e, i + 1));
			}
			Console.argument(
				format(
					`${Kernel.Language.get('js.obtained_argument')}`,
					(argument.source as string).length,
				),
			);
			if (!is_gui) k_argument().forEach((e) => print_statement(e[0], e[1]));
			const input: bigint = input_integer(
				[0n, 1n, 2n, 3n, 4n],
				is_gui_or<Array<string>>([], () => k_argument().map((e) => e[0])),
			);
			switch (input) {
				case 1n:
					load_module({ source: argument.source }, 'whole');
					Console.finished(
						format(
							Kernel.Language.get('total_n_files_are_executed'),
							(argument.source as Array<string>).length,
						),
					);
					break;

				case 2n:
					process_module(argument);
					break;
				case 3n:
				case 4n:
					execute(
						argument,
						input === 3n
							? 'popcap.atlas.split_by_resource_group'
							: 'popcap.atlas.split_by_res_info',
						Forward.DIRECT,
						'simple',
					);
					break;
			}
		} else {
			process_module(argument);
		}
	}

	export function check_overwrite<
		Argument extends {
			destination: string;
		},
	>(argument: Argument, type: 'file' | 'directory'): void {
		let message = null as string | null;
		if (type === 'file' && Kernel.FileSystem.is_file(argument.destination)) {
			message = format(Kernel.Language.get('js.file_already_exists'), argument.destination);
		} else if (type === 'directory' && Kernel.FileSystem.is_directory(argument.destination)) {
			message = format(
				Kernel.Language.get('js.directory_already_exists'),
				argument.destination,
			);
		}
		if (message !== null) Console.warning(message);
	}

	export function basic_batch<
		Argument extends Executor.Base,
		BatchArgument extends Executor.Base & { directory: string },
		Configuration extends Executor.Configuration,
	>(
		thiz: MethodExecutor<Argument, BatchArgument, Configuration>,
		argument: BatchArgument,
		is_directory: boolean,
		other?: Record<string, unknown>,
	): void {
		const callback: (source: string) => boolean = is_directory
			? Kernel.FileSystem.is_directory
			: Kernel.FileSystem.is_file;
		const files: Array<string> = Kernel.FileSystem.read_directory(argument.directory).filter(
			(path: string) => callback(path) && thiz.filter[1].test(path),
		);
		files.forEach((e) => thiz.direct_forward({ source: e as string, ...other } as any));
		Console.finished(format(Kernel.Language.get('batch.process.count'), files.length));
	}
}
