namespace Sen.Script.Support.PopCap.ResourceStreamBundle.Miscellaneous.PlatformConverter {
	export type Manifest = Kernel.Support.PopCap.ResourceStreamBundle.Manifest;

	export const k_composite_flag_offset = 0x10n;

	export function process(source: string, destination: string, to_ios: boolean) {
		const setting: Support.Project.ResourceStreamBundle.Configuration.Setting = {
			texture_format_category: to_ios ? 0n : 1n,
			only_high_resolution: true,
			unpack_packages: false,
			packages_setting: {
				rton_count: 0n,
				json_count: 0n,
				key: '',
				iv: '',
			},
		};
		const scg_setting: Support.Project.StreamCompressedGroup.Configuration.Setting = {
			decode_method: 0n,
			animation_split_label: false,
		};
		Kernel.Support.Project.ResourceStreamBundle.unpack_fs(source, destination, setting);
		const data_info = Kernel.JSON.deserialize_fs<any>(`${destination}/data.json`);
		const packet_list: Array<string> = data_info['packet'];
		let streaming_wave_processed = false;
		for (const i in packet_list) {
			const e = packet_list[i];
			Console.display(e);
			const is_composite = Kernel.Support.Project.StreamCompressedGroup.test_scg(
				`${destination}/packet/${e}.scg`,
			);
			if (is_composite) {
				Kernel.Support.Project.StreamCompressedGroup.decode_fs(
					`${destination}/packet/${e}.scg`,
					`${destination}/packet/${e}.scg.package`,
					scg_setting,
				);
				const scg_data = Kernel.JSON.deserialize_fs<any>(
					`${destination}/packet/${e}.scg.package/data.json`,
				);
				scg_data['texture_format_category'] = to_ios ? 1n : 0n;
				if (scg_data['category'] !== null) {
					scg_data['category']['format'] = 0n;
				}
				Kernel.JSON.serialize_fs(
					`${destination}/packet/${e}.scg.package/data.json`,
					scg_data,
				);
				Kernel.Support.Project.StreamCompressedGroup.encode_fs(
					`${destination}/packet/${e}.scg.package`,
					`${destination}/packet/${e}.scg`,
					scg_setting,
				);
				Kernel.FileSystem.remove(`${destination}/packet/${e}.scg.package`);
			} else {
				const element = e.toLowerCase();
				if (element === 'streamingwave' && to_ios) {
					Kernel.Support.Project.StreamCompressedGroup.decode_fs(
						`${destination}/packet/${e}.scg`,
						`${destination}/packet/${e}.scg.package`,
						scg_setting,
					);
					const scg_data: any = Kernel.JSON.deserialize_fs<any>(
						`${destination}/packet/${e}.scg.package/data.json`,
					);
					const wave_data = {
						version: scg_data['version'],
						texture_format_category: 1n,
						composite: false,
						category: null,
						subgroup: {
							Global_Data: {
								category: {
									common_type: false,
									locale: null,
									compression:
										scg_data['subgroup'][Object.keys(scg_data['subgroup'])[0]][
											'category'
										]['compression'],
								},
								resource: {} as any,
							},
						},
					};
					const dest = `${destination}/packet/Global_Data.scg.package/resource/streamingwaves/Global_Data`;
					Kernel.FileSystem.create_directory(dest);
					for (const [_, value] of Object.entries(
						scg_data['subgroup'][Object.keys(scg_data['subgroup'])[0]]['resource'],
					) as any) {
						const file_name: string = Kernel.Path.base_without_extension(
							value['path'] as string,
						);
						const new_id: string = `RESFILE_STREAMINGWAVES_GLOBAL_DATA_${file_name}`;
						wave_data['subgroup']['Global_Data']['resource'][new_id] = {
							type: 'File',
							path: `streamingwaves/Global_Data/${file_name}.wem`,
						};
						Kernel.FileSystem.rename(
							`${destination}/packet/${e}.scg.package/resource/${value['path']}`,
							`${destination}/packet/Global_Data.scg.package/resource/streamingwaves/Global_Data/${file_name}.wem`,
						);
					}
					Kernel.JSON.serialize_fs(
						`${destination}/packet/Global_Data.scg.package/data.json`,
						wave_data,
					);
					Kernel.Support.Project.StreamCompressedGroup.encode_fs(
						`${destination}/packet/Global_Data.scg.package`,
						`${destination}/packet/Global_Data.scg`,
						scg_setting,
					);
					Kernel.FileSystem.remove(`${destination}/packet/${e}.scg.package`);
					Kernel.FileSystem.remove(`${destination}/packet/Global_Data.scg.package`);
					packet_list[i] = 'Global_Data';
					streaming_wave_processed = true;
				} else if (element === 'global_data' && !to_ios) {
					Kernel.Support.Project.StreamCompressedGroup.decode_fs(
						`${destination}/packet/${e}.scg`,
						`${destination}/packet/${e}.scg.package`,
						scg_setting,
					);
					const scg_data: any = Kernel.JSON.deserialize_fs<any>(
						`${destination}/packet/${e}.scg.package/data.json`,
					);
					const wave_data = {
						version: scg_data['version'],
						texture_format_category: 0n,
						composite: false,
						category: null,
						subgroup: {
							StreamingWave: {
								category: {
									common_type: false,
									locale: null,
									compression:
										scg_data['subgroup'][Object.keys(scg_data['subgroup'])[0]][
											'category'
										]['compression'],
								},
								resource: {} as any,
							},
						},
					};
					const dest = `${destination}/packet/StreamingWave.scg.package/resource/streamingwaves`;
					Kernel.FileSystem.create_directory(dest);
					for (const [_id, value] of Object.entries(
						scg_data['subgroup'][Object.keys(scg_data['subgroup'])[0]]['resource'],
					) as any) {
						const file_name: string = Kernel.Path.base_without_extension(
							value['path'] as string,
						);
						const new_id: string = `RESFILE_STREAMINGWAVES_${file_name}`;
						wave_data['subgroup']['StreamingWave']['resource'][new_id] = {
							type: 'File',
							path: `streamingwaves/${file_name}.wem`,
						};
						Kernel.FileSystem.rename(
							`${destination}/packet/${e}.scg.package/resource/${value['path']}`,
							`${destination}/packet/StreamingWave.scg.package/resource/streamingwaves/${file_name}.wem`,
						);
					}
					Kernel.JSON.serialize_fs(
						`${destination}/packet/StreamingWave.scg.package/data.json`,
						wave_data,
					);
					Kernel.Support.Project.StreamCompressedGroup.encode_fs(
						`${destination}/packet/StreamingWave.scg.package`,
						`${destination}/packet/StreamingWave.scg`,
						scg_setting,
					);
					Kernel.FileSystem.remove(`${destination}/packet/${e}.scg.package`);
					Kernel.FileSystem.remove(`${destination}/packet/StreamingWave.scg.package`);
					packet_list[i] = 'StreamingWave';
					streaming_wave_processed = true;
				} else {
					const scg_stream = new Kernel.DataStreamView(`${destination}/packet/${e}.scg`);
					scg_stream.write_position = k_composite_flag_offset;
					scg_stream.writeUint32(to_ios ? 4n : 2n);
					scg_stream.out_file(`${destination}/packet/${e}.scg`);
				}
			}
		}
		assert(
			streaming_wave_processed,
			Kernel.Language.get('popcap.rsb.cannot_find_streaming_wave'),
		);
		Kernel.JSON.serialize_fs(`${destination}/data.json`, data_info);
		setting['texture_format_category'] = to_ios ? 1n : 0n;
		Kernel.Support.Project.ResourceStreamBundle.pack_fs(
			destination,
			`${Kernel.Path.except_extension(source)}.${to_ios ? 'main.rsb' : 'main.obb'}`,
			setting,
		);
		Kernel.FileSystem.remove(destination);
	}

	export function process_fs(source: string, destination: string, to_ios: boolean) {
		return process(source, destination, to_ios);
	}
}
