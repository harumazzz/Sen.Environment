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
		batch_forward?: (argument: BatchArgument) => Promise<void>;
		is_enabled: boolean;
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
		ASYNC,
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

	/**
	 * ----------------------------------------------------------
	 * JavaScript Executor Implement
	 * @param argument - Argument to query
	 * @param key - Key
	 * @param defined_value - If not, this val will assign to it
	 * @returns
	 * ----------------------------------------------------------
	 */

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
			if (
				(rule as Array<[bigint, string, string]>)
					.map((e) => e[1])
					.includes((argument as any)[key])
			) {
				print_argument(argument[key] as string);
			} else {
				Console.warning(
					format(Kernel.Language.get('script.invalid_input_data'), argument[key]),
				);
				delete (argument as any & Argument)[key];
				return load_bigint(argument, key, configuration, rule, title);
			}
		} else if ((configuration as any)[key] === '?') {
			return configurate_or_input(
				argument,
				key as string,
				rule as Array<[bigint, string, string]>,
			);
		} else {
			const setState = (value: string) => {
				print_argument(value);
				(argument as any & Argument)[key] = configuration[key];
			};
			if (rule.includes(configuration[key] as unknown as bigint & string)) {
				setState(configuration[key] as string);
			} else if (
				(rule as Array<[bigint, string, string]>)
					.map((e) => e[1])
					.includes(configuration[key] as any)
			) {
				setState((rule as any)[Number((configuration[key] as bigint) - 1n)][2] as string);
			} else {
				Console.error(format(Kernel.Language.get('invalid.argument'), configuration[key]));
				(configuration as any)[key] = '?';
				return load_bigint(argument, key, configuration, rule, title);
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
				Console.warning(
					format(Kernel.Language.get('script.invalid_input_data'), argument[key]),
				);
				delete argument[key];
				return input_range(argument, key, configuration, rule, title);
			}
		} else if ((configuration as any)[key] === '?') {
			let input: string = undefined!;
			while (true) {
				input = readline().trim();
				if (/\d+/.test(input) && rule[0] <= BigInt(input) && rule[1] >= BigInt(input)) {
					break;
				}
				Console.error(format(Kernel.Language.get('invalid.argument'), input));
			}
			(argument as any)[key] = BigInt(input);
		} else {
			if (
				/\d+/.test(configuration[key] as string) &&
				rule[0] <= BigInt(configuration[key] as string) &&
				rule[1] >= BigInt(configuration[key] as string)
			) {
				print_argument(configuration[key] as string);
				(argument as any & Argument)[key] = BigInt(configuration[key] as string);
			} else {
				Console.error(format(Kernel.Language.get('invalid.argument'), configuration[key]));
				(configuration as any)[key] = '?';
				return load_bigint(argument, key, configuration, rule, title);
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
				Console.warning(
					format(Kernel.Language.get('script.invalid_input_data'), argument[key]),
				);
				delete argument[key];
				return load_string(argument, key, configuration, title, rule);
			}
			return print_argument(argument[key] as string);
		} else if ((configuration as any)[key] === '?') {
			(argument as any)[key] = readline().trim();
		} else {
			if (rule === undefined) {
				print_argument(configuration[key] as string);
				(argument as any & Argument)[key] = configuration[key];
			} else if (rule.includes(configuration[key] as string)) {
				print_argument(configuration[key] as string);
				(argument as any & Argument)[key] = configuration[key];
			} else {
				Console.error(format(Kernel.Language.get('invalid.argument'), configuration[key]));
				(configuration as any)[key] = '?';
				return load_string(argument, key, configuration, title, rule);
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
				Console.warning(
					format(Kernel.Language.get('script.invalid_input_data'), argument[key]),
				);
				delete argument[key];
				return load_boolean(argument, key, configuration, title);
			}
			print_argument(argument[key] as any);
		}
		if ((configuration as any)[key] === '?') {
			(argument as any)[key] = input_boolean();
		} else {
			if (/^(true|false)$/.test(configuration[key] as string)) {
				print_argument(`${configuration[key]}`);
				(argument as any & Argument)[key] = Boolean(configuration[key]);
				return;
			}
			Console.error(Kernel.Language.get('input.invalid_boolean_configuration'));
			(configuration as any)[key] = '?';
			return load_boolean(argument, key, configuration, title);
		}
	}

	export function input_boolean(): boolean {
		if (is_gui()) {
			const result = Shell.callback('input_boolean')[0];
			return result === '1';
		} else {
			(
				[
					[Kernel.Language.get('input.set_argument_to_true'), 1n],
					[Kernel.Language.get('input.set_argument_to_false'), 2n],
				] as Array<[string, bigint]>
			).forEach((e) => print_statement(e[0], e[1]));
			const result = input_integer([1n, 2n]);
			return result === 1n;
		}
	}

	/**
	 *
	 * @param rule - Rule to filter
	 * @returns Input in rule
	 */

	export function input_integer(rule: Array<bigint>): bigint {
		let input: string = undefined!;
		if (is_gui()) {
			input = Shell.callback('input_enumeration', ...rule.map((e) => e.toString()))[0];
		} else {
			while (true) {
				input = readline().trim();
				if (/^\d+$/.test(input) && (rule as Array<bigint>).includes(BigInt(input))) {
					break;
				}
				Console.warning(Kernel.Language.get('js.invalid_input_value'));
			}
		}
		return BigInt(input);
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
		rule: Array<bigint> | Array<[bigint, string, string]>,
	): void {
		if ((argument as any & Argument)[key] === undefined) {
			switch (typeof rule[0]) {
				case 'object':
					const new_rule: Array<bigint> = [];
					rule.forEach((e: [bigint, string] & any) => {
						print_statement(e[2], e[0]);
						new_rule.push(e[0]);
					});
					(argument as any)[key] = (rule as Array<[bigint, string, string]>)[
						Number(input_integer(new_rule) - 1n)
					]![1];
					break;
				case 'string':
					(argument as any)[key] = readline().trim();
					break;
				case 'bigint':
					(argument as any)[key] = input_integer(rule as Array<bigint>);
					break;
			}
		}
	}

	/**
	 * JS Filter
	 * @param param0 - Type
	 * @param source - Source file to test
	 * @returns Filter
	 */

	export function test([type, pattern]: [MethodType, RegExp], source: string): boolean {
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
		let is_valid: boolean = true;
		switch (type) {
			case 'file':
				is_valid = source.every(function make_assert(e: string): boolean {
					return Kernel.FileSystem.is_file(e);
				});
				break;

			case 'directory':
				is_valid = source.every(function make_assert(e: string): boolean {
					return Kernel.FileSystem.is_directory(e);
				});
				break;

			case 'any':
				is_valid = true;
				break;
		}
		return is_valid && method.every((e: RegExp) => source.some((i: string) => e.test(i)));
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

	export async function run_as_module<Argument extends Executor.Base>(
		id: string,
		argument: Argument,
		forward_type: Executor.Forward,
	): Promise<void> {
		const worker:
			| Executor.MethodExecutor<Executor.Base, Executor.Base, Executor.Configuration>
			| undefined = methods.get(id);
		assert(worker !== undefined, format(Kernel.Language.get('js.method_not_found'), id));
		worker.configuration = Kernel.JSON.deserialize_fs<Configuration>(worker.configuration_file);
		Console.display(
			`${Kernel.Language.get('method_loaded')}:`,
			`${Kernel.Language.get(id)} | ${id}`,
			'green',
		);
		switch (forward_type) {
			case Forward.BATCH:
				assert(
					worker.batch_forward !== undefined,
					format(Kernel.Language.get('method_does_not_support_batch_implementation'), id),
				);
				await worker.batch_forward(argument);
				break;

			case Forward.DIRECT:
				worker.direct_forward(argument);
				break;

			default:
				assert(false, format(Kernel.Language.get('js.method_does_not_execute')));
		}
		if (is_gui()) {
			Console.finished(
				Kernel.Language.get('js.command_line_has_finished'),
				`${Kernel.Language.get('execution_time')}: ${clock
					.duration_as_seconds()
					.toFixed(3)}s`,
			);
		} else {
			Console.send(
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
			Console.display(title, argument, color);
		} else {
			Console.send(title, color);
			argument.forEach(print_argument);
		}
	}

	export async function execute<Argument extends Base>(
		argument: Argument,
		id: string,
		forward: Forward,
		load: ExecuteType,
	): Promise<string> {
		let result: string = undefined!;
		try {
			switch (load) {
				case 'simple':
					await run_as_module<Argument>(id, argument, forward);
					break;
				case 'whole':
					assert(
						Array.isArray(argument.source),
						Kernel.Language.get('argument_must_be_list_of_string'),
					);
					(argument.source as Array<string>).forEach(
						async (e) =>
							await run_as_module<Argument>(id, { ...argument, source: e }, forward),
					);
			}
		} catch (e: any) {
			result = Exception.make_exception(e);
			Console.error(result);
		}
		return result;
	}

	export type ExecuteType = 'simple' | 'whole';

	export function print_statement(name: string, num: bigint | number): void {
		return print_argument(`${num}. ${Kernel.Language.get(name)}`);
	}

	export function print_argument(e: string): void {
		if (is_gui()) {
			print(e.toString());
		} else {
			print(`    ${e}`);
		}
	}

	export async function load_module<Argument extends Base>(
		argument: Argument,
		load: ExecuteType,
	): Promise<void> {
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
			if (!worker.is_enabled) return;
			if (typeof argument.source === 'string') {
				query(test, worker.filter as [MethodType, RegExp], argument.source as string, [
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
		print_statement(Kernel.Language.get('js.skip_argument_input'), 0n);
		modules = new Map([...modules.entries()].sort((a, b) => Number(a[0] - b[0])));
		modules.forEach(print_statement);
		const view: Array<bigint> = Array.from(modules.keys());
		switch (view.length) {
			case 0:
				Console.error(Kernel.Language.get('js.argument_ignored'));
				break;
			case 1:
				execute<Argument>(argument, modules.get(view[0] as bigint)!, Forward.DIRECT, load);
				break;
			default:
				const input_value: bigint = input_integer([0n, ...view]);
				if (input_value === 0n) return;
				await execute<Argument>(argument, modules.get(input_value)!, Forward.DIRECT, load);
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
		if (/^(((\d+)[f|n]))$/.test(value)) {
			if (value.endsWith('f')) {
				return Number(value.substring(0, value.length - 1)) as T;
			}
			return BigInt(value.substring(0, value.length - 1)) as T;
		}
		if (/(((true|false)))/i.test(value)) {
			return (value === 'true') as T;
		}
		return value as T;
	}

	export function parse_argument<Argument extends Base & { source: Array<string> }>(
		argument: Argument,
		temporary: ModuleLoader,
	): void {
		let raw = argument.source;
		for (let i = 0; i < raw.length; ++i) {
			if (raw[i]!.startsWith('-')) {
				temporary[raw[i++]!.slice(1)] = exchange_argument_value(raw[i]!);
			}
		}
	}

	export function maybe_contains_atlas<Argument extends Base & { source: Array<string> }>(
		argument: Argument,
		temporary: ModuleLoader,
	): boolean {
		for (let e of argument.source) {
			if (
				['popcap.atlas.split_by_resource_group', 'popcap.atlas.split_by_res_info'].includes(
					e,
				)
			) {
				temporary.source = [];
				return true;
			}
		}
		return false;
	}

	export function parse_atlas<Argument extends Base & { source: Array<string> }>(
		argument: Argument,
		temporary: ModuleLoader & { source: Array<string> },
	): void {
		let raw = argument.source;
		for (let i = 0; i < raw.length; ++i) {
			if (raw[i]!.startsWith('-source')) {
				++i;
				for (; i < raw.length && !raw[i]!.startsWith('-'); ++i) {
					temporary.source.push(exchange_argument_value(raw[i]!));
				}
				--i;
			} else if (raw[i]!.startsWith('-')) {
				temporary[raw[i]!.slice(1)] = exchange_argument_value(raw[i + 1]!);
				++i;
			}
		}
	}

	export async function input_path<Argument extends Base & { source: Array<string> }>(
		argument: Argument,
	): Promise<void> {
		let input: string = undefined!;
		Console.argument(
			Kernel.Language.get(
				'script.input_any_path_to_continue_or_provide_an_empty_string_to_exit',
			),
		);
		loop: while (true) {
			input = readline().trim();
			switch (input) {
				case '':
					break loop;
				case ':p':
					Console.argument(Kernel.Language.get('input_type'));
					[Kernel.Language.get('file'), Kernel.Language.get('directory')].forEach(
						(e, i) => print_statement(e, i + 1),
					);
					const result = input_integer([1n, 2n]);
					if (result === 1n) {
						input = Shell.callback('pick_file')[0];
					} else {
						input = Shell.callback('pick_directory')[0];
					}
					if (input.length === 0) continue;
					break;
				case ':b':
					Console.argument(Kernel.Language.get('input_number_to_process_batch_function'));
					const modules: Map<bigint, string> = new Map<bigint, string>();
					let counter: bigint = 0n;
					print_statement(Kernel.Language.get('js.skip_argument_input'), 0n);
					methods.forEach((value, key) => {
						if (value.batch_forward !== undefined && value.is_enabled) {
							modules.set(++counter, key);
						}
					});
					modules.forEach((v, k) => print_statement(Kernel.Language.get(v), k));
					const option = input_integer([0n, ...modules.keys()]);
					if (option === 0n) break loop;
					(argument as any).directory = Console.path(
						Kernel.Language.get('input_directory'),
						'directory',
					);
					await execute(argument, modules.get(option)!, Forward.BATCH, 'simple');
					continue;
			}
			if (
				(input.startsWith('"') && input.endsWith('"')) ||
				(input.startsWith("'") && input.endsWith("'"))
			) {
				input = input.slice(1, -1);
			}
			if (Kernel.FileSystem.is_file(input) || Kernel.FileSystem.is_directory(input)) {
				Console.argument(input);
				argument.source.push(input);
				continue;
			}
			Console.warning(
				format(Kernel.Language.get('js.input_is_not_a_file_or_directory'), input),
			);
		}
		Console.warning(Kernel.Language.get('script.input_phase_stopped'));
	}

	export async function forward<Argument extends Base>(argument: Argument): Promise<void> {
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
			if (is_gui()) {
				Console.display(
					`${Kernel.Language.get('js.make_host.argument_obtained')}:`,
					(argument.source as Array<string>).map((e, i) => `${i + 1}. ${e}`).join('\n'),
					'cyan',
				);
			} else {
				Console.send(`${Kernel.Language.get('js.make_host.argument_obtained')}:`, 'cyan');
				(argument.source as Array<string>).forEach((e, i) => print_statement(e, i + 1));
			}
			Console.argument(
				format(
					`${Kernel.Language.get('js.obtained_argument')}`,
					(argument.source as string).length,
				),
			);
			(
				[
					[Kernel.Language.get('js.skip_argument_input'), 0n],
					[Kernel.Language.get('js.process_whole'), 1n],
					[Kernel.Language.get('js.process_in_queue'), 2n],
					[Kernel.Language.get('popcap.atlas.split_by_resource_group'), 3n],
					[Kernel.Language.get('popcap.atlas.split_by_res_info'), 4n],
				] as Array<[string, bigint]>
			).forEach((e) => print_statement(e[0], e[1]));
			const input: bigint = input_integer([0n, 1n, 2n, 3n, 4n]);
			switch (input) {
				case 1n:
					await load_module({ source: argument.source }, 'whole');
					Console.finished(
						format(
							Kernel.Language.get('total_n_files_are_executed'),
							(argument.source as Array<string>).length,
						),
					);
					break;

				case 2n:
					(argument.source as Array<string>).forEach(
						async (e) => await load_module({ source: e }, 'simple'),
					);
					break;

				case 3n:
					await execute<Argument>(
						argument,
						'popcap.atlas.split_by_resource_group',
						Forward.DIRECT,
						'simple',
					);
					break;

				case 4n:
					await execute<Argument>(
						argument,
						'popcap.atlas.split_by_res_info',
						Forward.DIRECT,
						'simple',
					);
					break;
			}
		} else {
			(argument.source as Array<string>).forEach((e: string) =>
				load_module({ source: e }, 'simple'),
			);
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

	export async function basic_batch<
		Argument extends Executor.Base,
		BatchArgument extends Executor.Base & { directory: string },
		Configuration extends Executor.Configuration,
	>(
		thiz: MethodExecutor<Argument, BatchArgument, Configuration>,
		argument: BatchArgument,
		is_directory: boolean,
		other?: Record<string, unknown>,
	): Promise<void> {
		const callback: (source: string) => boolean = is_directory
			? Kernel.FileSystem.is_directory
			: Kernel.FileSystem.is_file;
		const files: Array<string> = Kernel.FileSystem.read_directory(argument.directory).filter(
			(path: string) => callback(path) && thiz.filter[1].test(path),
		);
		const promises = files.map((source: string) => {
			return () => {
				return new Promise<void>((resolve) => {
					try {
						thiz.direct_forward({ source: source as string, ...other } as any);
					} catch (e: any) {
						Console.warning(Exception.make_exception(e));
					}
					resolve();
				});
			};
		});

		await Promise.all(promises.map((e) => e()));
		Console.finished(format(Kernel.Language.get('batch.process.count'), files.length));
	}
}
