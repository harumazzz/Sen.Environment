namespace Sen.Script.Helper.WWise.HashID {
	export function execute() {
		let input: string = undefined!;
		while (input !== '') {
			Console.argument(Kernel.Language.get('wwise.helper.hash_id.input_id_to_hash'));
			Executor.clock.stop_safe();
			input = readline();
			Executor.clock.start_safe();
			if (input.length === 0) break;
			Console.obtained(`${input}`);
			Console.finished(`${Kernel.Support.WWise.SoundBank.hash(input!)}`);
		}
	}
}
Sen.Script.Helper.WWise.HashID.execute();
