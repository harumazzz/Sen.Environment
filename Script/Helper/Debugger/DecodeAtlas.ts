namespace Sen.Script.Helper.Debugger.DecodeAtlas {
	export function execute() {
		const source: string = Console.path('input_folder', 'directory');
		const resource_folder = `${source}/resource`;
		const data_info: any = Kernel.JSON.deserialize_fs(`${source}/data.json`);
		for (let [_, group_value] of Object.entries(data_info.group) as any) {
			if (group_value.composite) {
				for (let [_, subgroup_value] of Object.entries(group_value.subgroup) as any) {
					if (subgroup_value.category.resolution !== 0) {
						for (let resource of subgroup_value.resource) {
							if (resource.path.endsWith('.PTX')) {
								Console.display(resource.path);
								const texture_info = resource.texture_info;
								if (texture_info.format === 0) {
									Console.display('RGBA_8888');
									Kernel.Support.Texture.decode_fs(
										`${resource_folder}/${resource.path}`,
										`${resource_folder}/${resource.path.replace(
											'.PTX',
											'.PNG',
										)}`,
										BigInt(texture_info.dimension.width),
										BigInt(texture_info.dimension.height),
										BigInt(Support.Texture.Format.RGBA_8888),
									);
								}
								if (texture_info.format === 147) {
									Console.display('RGB_ETC1_A_8');
									Kernel.Support.Texture.decode_fs(
										`${resource_folder}/${resource.path}`,
										`${resource_folder}/${resource.path.replace(
											'.PTX',
											'.PNG',
										)}`,
										BigInt(texture_info.dimension.width),
										BigInt(texture_info.dimension.height),
										BigInt(Support.Texture.Format.RGB_ETC1_A_8),
									);
								}
							}
						}
					}
				}
			}
		}
	}
}
Sen.Script.Helper.Debugger.DecodeAtlas.execute();
