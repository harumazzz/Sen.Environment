namespace Sen.Script.Helper.WWise.HashID {
	export function execute() {
		let input: string = undefined!;
		const destination = new Kernel.UInteger32();
		while (input !== '') {
			Console.argument(Kernel.Language.get('wwise.helper.hash_id.input_id_to_hash'));
			input = Kernel.Console.readline().trim();
			Kernel.Support.WWise.SoundBank.hash(input!, destination);
			Console.obtained(`${input}`);
			Console.finished(`${destination.value}`);
		}
	}
}
Sen.Script.Helper.WWise.HashID.execute();
