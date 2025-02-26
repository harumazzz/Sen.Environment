namespace Sen.Script.Helper.PVZ2.UnpackCustom.RebuildRSB {
	export type Generic = Support.Project.ResourceStreamBundle.Configuration.Generic;

	export function process(
		setting: any,
		texture_format_category: bigint,
		resolution_list: Array<bigint>,
		source: string,
	) {
		const rsb_setting = {
			texture_format_category,
			only_high_resolution: false,
			packages_setting: {
				rton_count: 0n,
				json_count: 0n,
				key: '65bd1b2305f46eb2806b935aab7630bb',
				iv: '1b2305f46eb2806b935aab76',
			},
			compression_setting: {
				manifest: true,
				packages: true,
			},
			unpack_packages: true,
		} as Support.Project.ResourceStreamBundle.Configuration.Setting;

		const scg_setting = {
			decode_method: setting.decode_method,
			animation_split_label: false,
		} as Support.Project.StreamCompressedGroup.Configuration.Setting;
		if (setting.rebuild_rsb_by_loose_constraints_first) {
			Kernel.Support.PopCap.ResourceStreamBundle.unpack_cipher(
				source,
				`${source}.temp_bundle`,
			);
			Kernel.Support.PopCap.ResourceStreamBundle.pack_resource(
				`${source}.temp_bundle`,
				`${source}.temp.rsb`,
			);
			source = `${source}.temp.rsb`;
		}
		Kernel.Support.Project.ResourceStreamBundle.unpack_fs(
			source,
			`${source}.bundle`,
			rsb_setting,
		);
		let error_log = '';
		for (const element of Kernel.FileSystem.read_directory_only_file(
			`${source}.bundle/packet`,
		)) {
			Console.display(`${Kernel.Language.get('unpack')}: ${Kernel.Path.basename(element)}`);
			try {
				const composite = Kernel.Support.Project.StreamCompressedGroup.test_scg(element);
				const scg_dest = `${source}.bundle/packet/${Kernel.Path.base_without_extension(
					element,
				)}.package`;
				Kernel.Support.Project.StreamCompressedGroup.decode_fs(
					element,
					scg_dest,
					scg_setting,
				);
				if (composite) {
					const data_info = Kernel.JSON.deserialize_fs(`${scg_dest}/data.json`) as any;
					for (let resolution of resolution_list) {
						if (
							!(data_info['category']['resolution'] as Array<bigint>).includes(
								resolution,
							)
						) {
							(data_info['category']['resolution'] as Array<bigint>).push(resolution);
						}
					}
					Kernel.JSON.serialize_fs(`${scg_dest}/data.json`, data_info);
				}
			} catch (e: any) {
				Console.warning(e);
				error_log += `● ${Kernel.Language.get('unpack')}: ${Kernel.Path.basename(
					element,
				)} | Catch: ${e}\n`;
			}
		}
		for (const element of Kernel.FileSystem.read_directory_only_directory(
			`${source}.bundle/packet`,
		)) {
			try {
				Console.display(`${Kernel.Language.get('pack')}: ${Kernel.Path.basename(element)}`);
				Kernel.Support.Project.StreamCompressedGroup.encode_fs(
					element,
					`${source}.bundle/packet/${Kernel.Path.base_without_extension(element)}.scg`,
					scg_setting,
				);
			} catch (e: any) {
				Console.warning(e);
				error_log += `● ${Kernel.Language.get('pack')}: ${Kernel.Path.basename(
					element,
				)} | Catch: ${e}\n`;
			}
		}
		const packages_info = (Kernel.JSON.deserialize_fs(`${source}.bundle/data.json`) as any)
			.packages_info;
		const packages_setting = Executor.Functions.Project.RSB.Build.load_packages(
			`${source}.bundle`,
			packages_info,
		);
		rsb_setting.packages_setting.json_count = packages_setting.json_count;
		rsb_setting.packages_setting.rton_count = packages_setting.rton_count;
		const destination = source.replace(
			Kernel.Path.extname(source),
			`_rebuild${Kernel.Path.extname(source)}`,
		);
		Kernel.Support.Project.ResourceStreamBundle.pack_fs(
			`${source}.bundle`,
			destination,
			rsb_setting,
		);
		if (error_log.length !== 0) {
			Kernel.FileSystem.write_file(`${Kernel.Path.dirname(source)}/error_log.txt`, error_log);
		}
	}

	export function execute() {
		const setting = {
			rebuild_rsb_by_loose_constraints_first: false,
			decode_method: 1n,
		};
		const source: string = Console.path(
			Kernel.Language.get('script.unpack_custom.rebuild_rsb.source_file'),
			'file',
		);
		const generic = Executor.Functions.Project.RSB.Detail.generic();
		Console.argument(Kernel.Language.get('popcap.rsb.custom.generic'));
		const input_generic = generic[Executor.is_valid_rule(generic)][1];
		const resolution_list: Array<bigint> = [];
		if (input_generic != 2n) {
			const resolution = [
				[1n, 0n, Kernel.Language.get('script.unpack_custom.resolution.finish')],
				[2n, 768n, Kernel.Language.get('script.unpack_custom.resolution.768n')],
				[3n, 384n, Kernel.Language.get('script.unpack_custom.resolution.384n')],
			] as any;
			while (resolution.length > 1) {
				Console.argument(Kernel.Language.get('script.rsb.unpack_custom.resolution'));
				const input_resolution = resolution[Executor.is_valid_rule(resolution)][1];
				if (input_resolution === 0n) {
					break;
				}
				resolution_list.push(input_resolution);
				for (let i = 1; i < resolution.length; i++) {
					if (resolution[i][1] === input_resolution) {
						resolution.splice(i, 1);
					}
					if (i < resolution.length) {
						--resolution[i][0];
					}
				}
			}
		}
		if (resolution_list.length > 0) {
			Console.output(
				`${Kernel.Language.get(
					'script.rsb.unpack_custom.resolution_additional',
				)}: ${resolution_list.toString()}`,
			);
		}
		Console.warning(Kernel.Language.get('script.unpack_custom.rebuild_rsb.warning'));
		Executor.clock.start_safe();
		process(setting, input_generic, resolution_list, source);
		Executor.clock.stop_safe();
	}
}
Sen.Script.Helper.PVZ2.UnpackCustom.RebuildRSB.execute();
