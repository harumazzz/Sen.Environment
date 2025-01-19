namespace Sen.Script.Helper.PvZO.DecodeBBone {
	export const MAGIC = 0x20007856n;
	export const METADATA = 0x011e0400n;

	export interface Data {
		size: bigint;
	}

	export function process(source: string): [Kernel.DataStreamView, Data] {
		const stream = new Kernel.DataStreamView(source);
		const magic = stream.readUint32();
		const metadata = stream.readUint32();
		assert(magic === MAGIC, 'invalid magic identifier');
		assert(metadata === METADATA, 'invalid metadata identifier');
		const size = stream.readUint32();
		stream.readUint32();
		stream.readUint32();
		stream.readUint32();
		stream.readUint32();
		stream.readUint32();
		const data = Kernel.Compression.Zlib.uncompress(
			stream.getArrayBuffer(stream.read_position, stream.size()),
		);
		const value = {
			version: 1n,
			size,
		};
		stream.close();
		stream.writeArrayBuffer(data);
		return [stream, value];
	}

	export function execute() {
		const source_file = Console.path('input bbone file', 'file');
		const [view, data] = process(source_file);
		const directory = `${Kernel.Path.except_extension(source_file)}.data`;
		Kernel.FileSystem.create_directory(directory);
		Kernel.JSON.serialize_fs(`${directory}/data.json`, data, 1n, false);
		view.out_file(`${directory}/data.bin`);
	}
}
Sen.Script.Helper.PvZO.DecodeBBone.execute();
