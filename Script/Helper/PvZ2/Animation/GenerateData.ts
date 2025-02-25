namespace Sen.Script.Support.PopCap.Animation.Miscellaenous.GenerateData {
	export interface Data {
		version: bigint;
		resolution: bigint;
		position: Position;
		image: Image;
		sprite: Sprite;
	}

	export interface Position {
		x: bigint;
		y: bigint;
	}

	export type Image = Record<string, ImageInfo>;

	export interface ImageInfo {
		id: string;
		dimension: Dimension;
	}

	export interface Dimension {
		width: bigint;
		height: bigint;
	}

	export type Sprite = Record<string, Record<string, string>>;

	export function process(resolution: bigint, source: string, image_posix: string): Data {
		const document = {} as Kernel.Support.PopCap.Animation.Miscellaneous.Document;
		Kernel.Support.PopCap.Animation.Miscellaneous.dump_document(source, document);
		const result: Data = {
			version: 6n,
			position: {
				x: 0n,
				y: 0n,
			},
			resolution,
			image: {},
			sprite: {},
		};
		document.image.forEach((e) => {
			const image_name = e.replace(/image\//, '').replace(/\.xml/, '');
			const image = Kernel.Image.open(`${source}/library/media/${image_name}.png`);
			const distance: number = 1200 / Number(resolution);
			result.image[image_name] = {
				id: `${image_posix}_${image_name.toUpperCase()}`,
				dimension: {
					width: BigInt(Math.round(Number(image.width) * distance)),
					height: BigInt(Math.round(Number(image.height) * distance)),
				},
			};
		});
		return result;
	}

	export function process_fs(
		source: string,
		destination: string,
		resolution: bigint,
		image_posix: string,
	): void {
		const data = process(resolution, source, image_posix);
		Kernel.JSON.serialize_fs(destination, data);
	}

	export function execute(): void {
		const resolution_map = new Map<bigint, bigint>([
			[1n, 1536n],
			[2n, 1536n],
			[3n, 1536n],
			[4n, 1536n],
		]);
		resolution_map.forEach((v, k) => Executor.print_statement(`${v}`, k));
		const resolution = Executor.input_integer(
			[...resolution_map.keys()],
			[...resolution_map.values().map((e) => e.toString())],
		);
		const source = Console.path(Kernel.Language.get('input_directory'), 'directory');
		Console.argument(Kernel.Language.get('popcap.animation.add_library.image_posix_for_id'));
		const image_posix = readline();
		assert(
			image_posix.length === 0,
			Kernel.Language.get('popcap.animation.add_library.image_posix_cannot_be_empty'),
		);
		return process_fs(source, `${source}/data.generated.json`, resolution, image_posix);
	}
}
Sen.Script.Support.PopCap.Animation.Miscellaenous.GenerateData.execute();
