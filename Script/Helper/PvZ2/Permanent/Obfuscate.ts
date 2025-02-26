namespace Sen.Script.Helper.PvZ2.Permanent.Obfuscate {
	/**
	 * Version
	 */

	export const versions: Array<bigint> = [3n, 4n];

	/**
	 * Head
	 */

	export interface RSBHead {
		file_offset: bigint;
		file_list_length: bigint;
		file_list_begin: bigint;
		rsg_list_length: bigint;
		rsg_list_begin: bigint;
		rsg_number: bigint;
		rsg_info_begin: bigint;
		rsg_info_each_length: bigint;
		composite_number: bigint;
		composite_info_begin: bigint;
		composite_info_each_length: bigint;
		composite_list_length: bigint;
		composite_list_begin: bigint;
		autopool_number: bigint;
		autopool_info_begin: bigint;
		autopool_info_each_length: bigint;
		ptx_number: bigint;
		ptx_info_begin: bigint;
		ptx_info_each_length: bigint;
	}

	// -----------------------------------------------------------------

	export function read_head(stream: Kernel.DataStreamView): RSBHead {
		stream.read_position = 16n;
		const file_list_length = stream.readUint32();
		const file_list_begin = stream.readUint32();
		stream.read_position += 8n;
		const rsg_list_length = stream.readUint32();
		const rsg_list_begin = stream.readUint32();
		const rsg_number = stream.readUint32();
		const rsg_info_begin = stream.readUint32();
		const rsg_info_each_length = stream.readUint32();
		const composite_number = stream.readUint32();
		const composite_info_begin = stream.readUint32();
		const composite_info_each_length = stream.readUint32();
		const composite_list_length = stream.readUint32();
		const composite_list_begin = stream.readUint32();
		const autopool_number = stream.readUint32();
		const autopool_info_begin = stream.readUint32();
		const autopool_info_each_length = stream.readUint32();
		const ptx_number = stream.readUint32();
		const ptx_info_begin = stream.readUint32();
		const ptx_info_each_length = stream.readUint32();
		stream.read_position += 12n;
		const file_offset = stream.readUint32();
		return {
			file_offset,
			file_list_length,
			file_list_begin,
			rsg_list_length,
			rsg_list_begin,
			rsg_number,
			rsg_info_begin,
			rsg_info_each_length,
			composite_number,
			composite_info_begin,
			composite_info_each_length,
			composite_list_length,
			composite_list_begin,
			autopool_number,
			autopool_info_begin,
			autopool_info_each_length,
			ptx_number,
			ptx_info_begin,
			ptx_info_each_length,
		};
	}

	// -----------------------------------------------------------------

	export function make_random(): bigint {
		let result: bigint = undefined!;
		while (true) {
			result = BigInt(Math.floor(Math.random() * 0xff));
			if (!versions.includes(result)) {
				break;
			}
		}
		return result;
	}

	// -----------------------------------------------------------------

	export function disturb_header(stream: Kernel.DataStreamView): void {
		stream.write_position = 4n;
		const version: bigint = make_random();
		Console.display(
			`${Kernel.Language.get('popcap.rsb.obfuscate.version_number')}: 0x${version.toString(
				16,
			)}`,

			'green',
		);
		stream.writeUint8(version);
	}

	// -----------------------------------------------------------------

	export function process(stream: Kernel.DataStreamView): void {
		const rsb_head_info = read_head(stream);
		disturb_header(stream);
		stream.read_position = rsb_head_info.rsg_info_begin;
		const subgroup_section = new ArrayBuffer(128);
		Kernel.ArrayBuffer.random(subgroup_section);
		const packet_section = new ArrayBuffer(16);
		Kernel.ArrayBuffer.random(packet_section);
		const rsg_section = new ArrayBuffer(64);
		Kernel.ArrayBuffer.random(rsg_section);
		Console.display(
			`${Kernel.Language.get('popcap.rsb.obfuscate.modify_count')}: ${
				rsb_head_info.rsg_number
			}`,
			'green',
		);
		for (let i = 0n; i < rsb_head_info.rsg_number; ++i) {
			const start_index = stream.read_position;
			const autopool_start_index = rsb_head_info.autopool_info_begin + i * 152n;
			stream.write_position = start_index;
			stream.writeArrayBuffer(subgroup_section);
			stream.write_position = autopool_start_index;
			stream.writeArrayBuffer(subgroup_section);
			stream.write_position = start_index + 132n;
			stream.writeArrayBuffer(packet_section);
			const packet_offset = stream.readUint32(start_index + 128n);
			stream.write_position = packet_offset;
			stream.writeArrayBuffer(rsg_section);
			stream.read_position = start_index + rsb_head_info.rsg_info_each_length;
		}
	}

	// -----------------------------------------------------------------

	export function execute(): void {
		const source = Console.path('input source file', 'file');
		const destination = `${source}.bin`;
		const rsb = new Kernel.DataStreamView(source);
		process(rsb);
		rsb.out_file(destination);
	}
}
Sen.Script.Helper.PvZ2.Permanent.Obfuscate.execute();
