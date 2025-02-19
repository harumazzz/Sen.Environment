namespace Sen.Script.Support.PopCap.Atlas.Split {
	// using the definition

	export type ImageRes = Kernel.Support.PopCap.ResourceGroup.ResourceContainsSprite;

	// using resource

	export type ResourceSubgroup = Kernel.Support.PopCap.ResourceGroup.ResourceSubgroup;

	/**
	 * source[0] - json file
	 * source[n] - images
	 */

	export type Source = [string, ...params: Array<string>];

	/**
	 * Support PvZ2 Resource-Group
	 */

	export namespace ResourceGroup {
		/**
		 * Make destination
		 * @param destination - atlas directory
		 * @param resource - resource
		 * @param is_path - is path
		 * @returns
		 */

		export function home(
			destination: string,
			resource: Kernel.Support.PopCap.ResourceGroup.ResourceContainsSprite,
			is_path: boolean,
		): string {
			if (is_path) {
				return `${destination}/${(resource.path as string).split('/').at(-1)!}.png`;
			}
			return `${destination}/${resource.id}.png`;
		}

		/**
		 *
		 * @param resource - Resource file
		 * @returns
		 */

		export function is_sprite_container(
			resource: Kernel.Support.PopCap.ResourceGroup.ResourceContainsImage,
		): resource is Kernel.Support.PopCap.ResourceGroup.ResourceContainsSprite {
			if (
				(resource as ImageRes).ax !== undefined &&
				(resource as ImageRes).ay !== undefined &&
				(resource as ImageRes).ah !== undefined &&
				(resource as ImageRes).aw !== undefined
			) {
				return true;
			}
			return false;
		}

		/**
		 *
		 * @param resource - Resource to convert
		 * @param method - Method
		 * @param style - Style
		 * @returns atlas
		 */

		export function make_definition(
			resource: Kernel.Support.PopCap.ResourceGroup.ResourceSubgroup,
			method: Structure.TypicalMethod,
			style: Structure.PathStyle,
		): Structure.Definition {
			const definition: Structure.Definition = {
				method: method,
				expand_path: style,
				subgroup: resource.id,
				trim: false,
				res: resource.res,
				groups: {},
			};
			for (const subgroup of resource.resources) {
				if (is_sprite_container(subgroup)) {
					const wrapper: Structure.BasicGroup = {
						path: subgroup.path as string,
						default: {
							x: subgroup.x ?? 0n,
							y: subgroup.y ?? 0n,
						},
					};
					if (subgroup.cols && subgroup.cols !== 1n) {
						wrapper.default.cols = subgroup.cols;
					}
					if (subgroup.rows && subgroup.rows !== 1n) {
						wrapper.default.rows = subgroup.rows;
					}
					definition.groups[subgroup.id] = wrapper;
				}
			}
			return definition;
		}

		/**
		 *
		 * @param resource - Resource after deserialize
		 * @param images - Images path
		 * @param method - Method to split
		 * @param style - Path style
		 * @param destination - Where to split
		 * @returns
		 */

		export function process(
			resource: Kernel.Support.PopCap.ResourceGroup.ResourceSubgroup,
			images: Array<string>,
			method: Structure.TypicalMethod,
			style: Structure.PathStyle,
			destination: string,
		): Structure.Definition {
			const resources_used: ResourceSubgroup = {
				...resource,
				resources: [],
			};
			const sprite_destination: string = `${destination}/media`;
			const by_path: boolean = method === 'path';
			const style_use_string: boolean = style === 'string';
			Kernel.FileSystem.create_directory(sprite_destination);
			const image_wrapper: Map<string, Array<Kernel.Image.RectangleFileIO>> = new Map<
				string,
				Array<Kernel.Image.RectangleFileIO>
			>();
			images.forEach((e: string) => image_wrapper.set(e, []));
			for (const current_resource of resource.resources) {
				if (is_sprite_container(current_resource)) {
					for (const png of images) {
						const current_parent: string = png.replaceAll(/\.png/gi, '').toUpperCase();
						if (
							current_resource.parent &&
							current_parent.endsWith(
								current_resource.parent.replace('ATLASIMAGE_ATLAS_', ''),
							)
						) {
							if (style_use_string) {
								current_resource.path = (
									current_resource.path as string
								).replaceAll('\\', '/');
							} else {
								current_resource.path = (
									current_resource.path as Array<string>
								).join('/');
							}
							image_wrapper.get(png)!.push({
								x: current_resource.ax,
								y: current_resource.ay,
								width: current_resource.aw,
								height: current_resource.ah,
								destination: home(sprite_destination, current_resource, by_path),
							});
							resources_used.resources.push(current_resource);
							break;
						}
					}
				}
			}
			if (by_path) {
				image_wrapper.forEach((data: Array<Kernel.Image.RectangleFileIO>, source: string) =>
					Kernel.Image.cut_multiple_fs(source, data),
				);
			} else {
				image_wrapper.forEach((data: Array<Kernel.Image.RectangleFileIO>, source: string) =>
					Kernel.Image.cut_multiple_fs_asynchronous(source, data),
				);
			}
			return make_definition(resources_used, method, style);
		}

		/**
		 *
		 * @param param0 - json file
		 * @param destination - output
		 * @param method - method to split
		 * @param style - path style
		 * @returns
		 */

		export function process_fs(
			[json, ...images]: Source,
			destination: string,
			method: Structure.TypicalMethod,
			style: Structure.PathStyle,
		): void {
			const definition = process(
				Kernel.JSON.deserialize_fs<Kernel.Support.PopCap.ResourceGroup.ResourceSubgroup>(
					json,
				),
				images,
				method,
				style,
				destination,
			);
			Kernel.JSON.serialize_fs<Structure.Definition>(`${destination}/atlas.json`, definition);
		}
	}

	export namespace ResInfo {
		/**
		 *
		 * @param resource - Resource file
		 * @returns
		 */

		export function is_sprite_container(
			resource: unknown,
		): resource is Kernel.Support.PopCap.ResInfo.Default {
			if (
				(resource as ImageRes).ax !== undefined &&
				(resource as ImageRes).ay !== undefined &&
				(resource as ImageRes).ah !== undefined &&
				(resource as ImageRes).aw !== undefined
			) {
				return true;
			}
			return false;
		}
		/**
		 * Make destination
		 * @param destination - atlas directory
		 * @param resource - resource
		 * @param is_path - is path
		 * @returns
		 */

		export function home(
			destination: string,
			resource: Kernel.Support.PopCap.ResInfo.Sprite,
			id: string | undefined,
		): string {
			if (id === undefined) {
				return `${destination}/${(resource.path as string).split('/').at(-1)!}.png`;
			}
			return `${destination}/${id}.png`;
		}

		export function make_definition(
			resource: Kernel.Support.PopCap.ResInfo.Atlas,
			method: Structure.TypicalMethod,
			id: string,
			style?: Structure.PathStyle,
		): Structure.Definition {
			const definition: Structure.Definition = {
				method: method,
				expand_path: style ?? 'array',
				subgroup: id,
				trim: false,
				res: resource.type,
				groups: {},
			};
			const parents: Array<string> = Object.keys(resource.packet);
			for (const parent of parents) {
				const sub_keys: Array<string> = Object.keys(resource.packet[parent].data);
				for (const key of sub_keys) {
					const group: Structure.BasicGroup = {
						default: {
							x: resource.packet[parent].data[key].default.x ?? 0n,
							y: resource.packet[parent].data[key].default.y ?? 0n,
						},
						path: resource.packet[parent].data[key].path,
					};
					if (
						resource.packet[parent].data[key].default.cols &&
						resource.packet[parent].data[key].default.cols !== 1n
					) {
						group.default.cols = resource.packet[parent].data[key].default.cols;
					}
					if (
						resource.packet[parent].data[key].default.rows &&
						resource.packet[parent].data[key].default.rows !== 1n
					) {
						group.default.rows = resource.packet[parent].data[key].default.rows;
					}
					definition.groups[key] = group;
				}
			}
			return definition;
		}

		export function process(
			resource: Kernel.Support.PopCap.ResInfo.Atlas,
			images: Array<string>,
			id: string,
			method: Structure.TypicalMethod,
			destination: string,
			style?: Structure.PathStyle,
		): Structure.Definition {
			const resources_used: Kernel.Support.PopCap.ResInfo.Atlas = {
				...resource,
				packet: {},
			};
			const parents: Array<string> = Object.keys(resource.packet);
			Kernel.FileSystem.create_directory(destination);
			const sprite_destination: string = `${destination}/media`;
			Kernel.FileSystem.create_directory(sprite_destination);
			const image_wrapper: Map<string, Array<Kernel.Image.RectangleFileIO>> = new Map<
				string,
				Array<Kernel.Image.RectangleFileIO>
			>();
			images.forEach((e: string) => image_wrapper.set(e, []));
			const by_path: boolean = method === 'path';
			for (const parent of parents) {
				const ids: Array<string> = Object.keys(resource.packet[parent].data);
				resources_used.packet[parent] = {
					...resource.packet[parent],
					data: {},
				};
				for (const id of ids) {
					const group: Kernel.Support.PopCap.ResInfo.Sprite =
						resource.packet[parent].data[id];
					const default_subinfo = resource.packet[parent].data[id].default;
					if (is_sprite_container(default_subinfo)) {
						for (const png of images) {
							const current_parent = png.replaceAll(/\.png$/gi, '').toUpperCase();
							if (current_parent.endsWith(parent.replace('ATLASIMAGE_ATLAS_', ''))) {
								image_wrapper.get(png)!.push({
									x: default_subinfo.ax,
									y: default_subinfo.ay,
									width: default_subinfo.aw,
									height: default_subinfo.ah,
									destination: home(
										sprite_destination,
										group,
										by_path ? undefined : id,
									),
								});
								resources_used.packet[parent].data[id] = {
									default: {
										...default_subinfo,
									},
									path: group.path,
									type: group.type,
								};
							}
						}
					}
				}
			}
			if (by_path) {
				image_wrapper.forEach((data: Array<Kernel.Image.RectangleFileIO>, source: string) =>
					Kernel.Image.cut_multiple_fs(source, data),
				);
			} else {
				image_wrapper.forEach((data: Array<Kernel.Image.RectangleFileIO>, source: string) =>
					Kernel.Image.cut_multiple_fs_asynchronous(source, data),
				);
			}
			return make_definition(resources_used, method, id, style);
		}

		/**
		 *
		 * @param param0 - json file
		 * @param destination - output
		 * @param method - method to split
		 * @param style - path style
		 * @returns
		 */

		export function process_fs(
			[json, ...images]: Source,
			destination: string,
			method: Structure.TypicalMethod,
			style?: Structure.PathStyle,
		): void {
			const definition = process(
				Kernel.JSON.deserialize_fs<Kernel.Support.PopCap.ResInfo.Atlas>(json),
				images,
				Kernel.Path.base_without_extension(json),
				method,
				destination,
				style,
			);
			Kernel.JSON.serialize_fs<Structure.Definition>(`${destination}/atlas.json`, definition);
		}
	}
}
