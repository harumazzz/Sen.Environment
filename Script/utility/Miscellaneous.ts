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
		if (typeof value === 'object') return Console.send(Kernel.JSON.serialize(value, 1n, false));
		Console.send(value);
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
}
