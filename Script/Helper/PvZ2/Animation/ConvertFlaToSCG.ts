namespace Sen.Script.Helper.PopCap.Animation.FlashToAnimation {
	const k_xmlns_xsi: Support.PopCap.Animation.DOM.xmlns_xsi =
		'http://www.w3.org/2001/XMLSchema-instance';

	const k_xmlns: Support.PopCap.Animation.DOM.xmlns = 'http://ns.adobe.com/xfl/2008/';

	class Container {
		public last_frame: number;
		public sprite_container: Record<string, string>;
		public image_container: Record<string, string>;
		public sprite_list: Array<string>;
		public symbol_list: Array<string>;
		public constructor() {
			this.last_frame = 0;
			this.sprite_container = {};
			this.image_container = {};
			this.sprite_list = [];
			this.symbol_list = [];
		}

		public generate_new_image_name() {
			return `image_${this.symbol_list.length + 1}`;
		}

		public has_sprite(name: string) {
			return this.sprite_container.hasOwnProperty(name);
		}

		public has_image(name: string) {
			return this.image_container.hasOwnProperty(name);
		}

		public check_duration(data: number) {
			if (this.last_frame < data) {
				this.last_frame = data;
			}
		}

		public clear() {
			this.last_frame = 0;
			this.sprite_container = {};
			this.image_container = {};
			this.sprite_list = [];
			this.symbol_list = [];
		}
	}

	export type T =
		| Support.PopCap.Animation.DOM.SpriteDomLayer
		| Support.PopCap.Animation.DOM.SpriteDomFrame;

	export function push_to_array(
		data: T,
	):
		| Support.PopCap.Animation.DOM.SpriteDomLayer
		| Support.PopCap.Animation.DOM.SpriteDomFrame
		| Array<T> {
		if (Array.isArray(data)) {
			return data;
		} else {
			return [data];
		}
	}

	export function exchange_image(
		container: Container,
		layer: Support.PopCap.Animation.DOM.SpriteDomLayer[] | Kernel.XML.XMLDocument,
		name: string,
	): string {
		if (!Array.isArray(layer)) {
			layer = push_to_array(
				(layer as any)['DOMSymbolItem']['timeline']['DOMTimeline']['layers']['DOMLayer'],
			) as Support.PopCap.Animation.DOM.SpriteDomLayer[];
		}
		if (!container.has_image(name)) {
			const image_name = container.generate_new_image_name();
			container.image_container[name] = image_name;
			const image_attributes: Support.PopCap.Animation.DOM.Attributes = {
				'xmlns:xsi': k_xmlns_xsi,
				xmlns: k_xmlns,
				name: `image/${image_name}`,
				symbolType: 'graphic',
			};
			const image_document: Support.PopCap.Animation.DOM.SpriteDocument = {
				DOMSymbolItem: {
					'@attributes': image_attributes,
					timeline: {
						DOMTimeline: {
							'@attributes': {
								name: image_name,
							},
							layers: {
								DOMLayer: layer,
							},
						},
					},
				},
			};
			container.symbol_list.push(Kernel.XML.serialize(image_document));
			return `image/${image_name}`;
		} else {
			return `image/${container.image_container[name]}`;
		}
	}

	export function exchange_sprite(
		source: string,
		container: Container,
		sprite: Kernel.XML.XMLDocument,
		sprite_name: string,
	): string {
		const is_sprite = sprite_name !== 'main_sprite';
		const dom = is_sprite ? (sprite as any)['DOMSymbolItem'] : (sprite as any)['DOMDocument'];
		const dom_timelines = is_sprite
			? dom['timeline']['DOMTimeline']
			: (dom['timelines']['DOMTimeline'] as any);
		const layer_list = push_to_array(
			dom_timelines['layers']['DOMLayer'],
		) as Support.PopCap.Animation.DOM.SpriteDomLayer[];
		const sprite_layer: Support.PopCap.Animation.DOM.SpriteDomLayer[] = [];
		let layer_index = layer_list.length - 1;
		let end_layer = 0;
		for (let i = 0; i < layer_list.length; i++) {
			if (layer_list[i]!['@attributes'].name.startsWith('Layer')) {
				end_layer = i;
				break;
			}
		}
		while (layer_index >= end_layer) {
			let is_contain_layer = false;
			let resource_item: any = null;
			const e_layer = layer_list[layer_index];
			e_layer!['@attributes'].name = layer_index.toString();
			const e_dom_frames = push_to_array(
				e_layer!.frames.DOMFrame as T,
			) as Support.PopCap.Animation.DOM.SpriteDomFrame[];
			const split = (index: string, start: number, duration: number) => {
				const next_frames = e_dom_frames.splice(start, duration);
				const new_layer: Support.PopCap.Animation.DOM.SpriteDomLayer = {
					'@attributes': {
						name: '0',
					},
					frames: {
						DOMFrame: [
							{
								'@attributes': {
									index: '0',
									duration: index,
								},
								elements: null,
							},
							...next_frames,
						] as any,
					},
				};
				layer_list.splice(layer_index, 0, new_layer);
				++layer_index;
			};
			for (let i = 0; i < e_dom_frames.length; i++) {
				const e_dom_frame = e_dom_frames[i];
				if (is_contain_layer && e_dom_frame!.elements === null) {
					split(e_dom_frame!['@attributes'].index, i, e_dom_frames.length - 1);
					break;
				} else {
					is_contain_layer = e_dom_frame!.elements !== null;
					if (is_contain_layer) {
						const current_duration =
							parseInt(e_dom_frame!['@attributes'].index) +
							parseInt(e_dom_frame!['@attributes'].duration ?? '0');
						container.check_duration(current_duration);
						const e_dom_symbol = (
							e_dom_frame.elements as Support.PopCap.Animation.DOM.ElementsDOMSymbolInstance
						)['DOMSymbolInstance'];
						if (
							e_dom_symbol !== undefined &&
							e_dom_symbol['@attributes']['libraryItemName'] !== undefined
						) {
							const item_name = e_dom_symbol['@attributes'].libraryItemName;
							if (resource_item !== null && resource_item !== item_name) {
								split(e_dom_frame['@attributes'].index, i, e_dom_frames.length - i);
								break;
							} else {
								resource_item = item_name;
							}
							let library_name = null as any;
							if (!container.has_sprite(item_name)) {
								const symbol: Kernel.XML.XMLDocument = Kernel.XML.deserialize_fs(
									`${source}/library/${item_name}.xml`,
								);
								let name_sprite = (symbol['DOMSymbolItem'] as any)['@attributes'][
									'linkageClassName'
								] as string;
								if (name_sprite !== undefined) {
									if (name_sprite.startsWith('holder_custom')) {
										name_sprite = name_sprite.replace(
											'holder_custom',
											'_custom',
										);
									}
									library_name = exchange_sprite(
										source,
										container,
										symbol,
										name_sprite.toLowerCase(),
									);
								} else {
									const symbol_name = (symbol['DOMSymbolItem'] as any)[
										'@attributes'
									]['name'] as string;
									library_name = exchange_image(container, symbol, symbol_name);
								}
							} else {
								library_name = `sprite/${container.sprite_container[item_name]}`;
							}
							e_dom_symbol['@attributes'].libraryItemName = library_name;
							e_dom_symbol['@attributes'].firstFrame = '0';
							e_dom_symbol['@attributes'].symbolType = 'graphic';
							e_dom_symbol['@attributes'].loop = 'loop';
							delete (e_dom_symbol['@attributes'] as any)['centerPoint3DX'];
							delete (e_dom_symbol['@attributes'] as any)['centerPoint3DY'];
						} else {
							const library_name = exchange_image(
								container,
								[e_layer],
								`${sprite_name}.${layer_index}.${i}`,
							);
							sprite_layer.push({
								'@attributes': {
									name: `${layer_index}`,
								},
								frames: {
									DOMFrame: [
										{
											'@attributes': {
												index: '0',
												duration: '1',
											},
											elements: {
												DOMSymbolInstance: {
													'@attributes': {
														libraryItemName: library_name,
														symbolType: 'graphic',
														loop: 'loop',
													},
												},
											},
										},
									] as any,
								},
							});
							break;
						}
					}
				}
			}
			sprite_layer.unshift(e_layer);
			--layer_index;
		}
		const k_name = `sprite/${sprite_name}`;
		const sprite_attributes: Support.PopCap.Animation.DOM.Attributes = {
			'xmlns:xsi': k_xmlns_xsi,
			xmlns: k_xmlns,
			name: is_sprite ? `sprite/${sprite_name}` : 'main_sprite',
			symbolType: 'graphic',
		};
		const sprite_document: Support.PopCap.Animation.DOM.SpriteDocument = {
			DOMSymbolItem: {
				'@attributes': sprite_attributes,
				timeline: {
					DOMTimeline: {
						'@attributes': {
							name: sprite_name,
						},
						layers: {
							DOMLayer: sprite_layer,
						},
					},
				},
			},
		};
		const symbol_name = dom['@attributes']['name'] as string;
		container.sprite_container[symbol_name] = sprite_name;
		container.sprite_list.push(Kernel.XML.serialize(sprite_document));
		return k_name;
	}

	export function get_name_before_last_under_score(name: string): string {
		let lastUnderscoreIndex = name.lastIndexOf('_');
		return name.substring(0, lastUnderscoreIndex);
	}

	export function exchange_document(
		dest: string,
		container: Container,
		data: Kernel.XML.XMLDocument,
	): void {
		const includes: Support.PopCap.Animation.DOM.AttributesSymbolsItem[] = [];
		Object.values(container.image_container).forEach((e, i) => {
			includes.push({
				'@attributes': {
					href: `image/${e}.xml`,
				},
			});
			Kernel.FileSystem.write_file(
				`${dest}/library/image/${e}.xml`,
				container.symbol_list[i],
			);
		});
		Object.values(container.sprite_container).forEach((e, i) => {
			if (e !== 'main_sprite') {
				includes.push({
					'@attributes': {
						href: `sprite/${e}.xml`,
					},
				});
				Kernel.FileSystem.write_file(
					`${dest}/library/sprite/${e}.xml`,
					container.sprite_list[i],
				);
			} else {
				includes.push({
					'@attributes': {
						href: `main_sprite.xml`,
					},
				});
				Kernel.FileSystem.write_file(
					`${dest}/library/main_sprite.xml`,
					container.sprite_list[i],
				);
			}
		});
		const dom: Support.PopCap.Animation.DOM.DOMDocument = {
			DOMDocument: {
				'@attributes': (data['DOMDocument'] as any)[
					'@attributes'
				] as Support.PopCap.Animation.DOM.DocumentAttributes,
				folders: {
					DOMFolderItem: [
						{
							'@attributes': {
								name: 'media',
								isExpanded: 'true',
							},
						},
						{
							'@attributes': {
								name: 'image',
								isExpanded: 'true',
							},
						},
						{
							'@attributes': {
								name: 'sprite',
								isExpanded: 'true',
							},
						},
					],
				},
				media: {
					DOMBitmapItem: [],
				},
				symbols: {
					Include: includes,
				},
				timelines: {
					DOMTimeline: {
						'@attributes': {
							name: 'animation',
						},
						layers: {
							DOMLayer: [
								{
									'@attributes': {
										name: 'label',
									},
									frames: {
										DOMFrame: [],
									},
								},
								{
									'@attributes': {
										name: 'action',
									},
									frames: {
										DOMFrame: [],
									},
								},
								{
									'@attributes': {
										name: 'instance',
									},
									frames: {
										DOMFrame: [
											{
												'@attributes': {
													index: '0',
													duration: container.last_frame.toString(),
												},
												elements: {
													DOMSymbolInstance: {
														'@attributes': {
															libraryItemName: 'main_sprite',
															symbolType: 'graphic',
															loop: 'loop',
														},
													},
												},
											},
										],
									},
								},
							],
						},
					},
				},
			},
		};
		const dom_timelines = (data as any)['DOMDocument']['timelines']['DOMTimeline'] as any;
		const layer_list = push_to_array(
			dom_timelines['layers']['DOMLayer'],
		) as Support.PopCap.Animation.DOM.SpriteDomLayer[];
		const first_layer = layer_list[0];
		const document_layers = dom.DOMDocument.timelines.DOMTimeline.layers.DOMLayer as any;
		let current_label = 'animation';
		let current_index = 0;
		const push = (duration: number) => {
			document_layers[0].frames.DOMFrame.push({
				'@attributes': {
					index: current_index.toString(),
					duration: (duration + 1).toString(),
					name: current_label,
					labelType: 'name',
				} as any,
				elements: null!,
			});
			document_layers[1].frames.DOMFrame.push({
				'@attributes': {
					index: current_index.toString(),
					duration: duration.toString(),
				} as any,
				elements: null!,
			});
			current_index += duration + 1;
			document_layers[1].frames.DOMFrame.push({
				'@attributes': {
					index: current_index.toString(),
					duration: '1',
				} as any,
				Actionscript: {
					script: {
						'@text': {
							is_cdata: true,
							value: 'stop();',
						},
					},
				} as any,
				elements: null!,
			} as any);
		};
		const label_dom_layers = (first_layer.frames as any)
			.DOMFrame as Support.PopCap.Animation.DOM.SpriteDomFrame[];
		if (first_layer['@attributes'].name === 'Labels Layer') {
			for (let i = 0; i < label_dom_layers.length; i++) {
				const e_dom_frame = label_dom_layers[i] as any;
				if (e_dom_frame['@attributes']['name'] !== undefined) {
					current_label = e_dom_frame['@attributes']['name'].toLowerCase();
				} else {
					const duration = parseInt(e_dom_frame['@attributes']['duration']);
					push(duration);
				}
			}
		}
		push(container.last_frame - current_index - 1);
		Kernel.FileSystem.write_file(`${dest}/DomDocument.xml`, Kernel.XML.serialize(dom));
	}

	export function exchange_script(
		dest: string,
		symbol_length: number,
		animation_name: string,
		resolution: number,
	): void {
		const scale_ratio = resolution / 1200;
		let image_list = [] as Array<string>;
		let script_count = 1;
		const script_string = `
            /**
            *  Copyright © 2024 Haruma
            *  Sen.Environment : Version 5.0
            *  Download: https://haruma-vn.github.io/Sen.Environment
            */
            var document = fl.getDocumentDOM();
            {0}
            if (!document) { alert("Please open or create a flashfile."); }
            function moveBitmap(item, name){
                document.library.moveToFolder("media",item.name,false);
                item.name = name;
            }
            function convert(imageName) {
                document.scaleSelection(${scale_ratio}, ${scale_ratio});
                document.convertSelectionToBitmap();
                document.scaleSelection(${1 / scale_ratio}, ${1 / scale_ratio});
                const bitmap_item = document.library.items[libraryLength];
                if (bitmap_item.itemType !== "bitmap") {alert("Fail converted");}
                moveBitmap(bitmap_item, imageName);
            }    
            function execute() {
                {1}
              //  alert("Done");
            }
            execute();
        `;
		for (let i = 0; i < symbol_length; i++) {
			const image = `document.library.editItem("image/image_${
				i + 1
			}"); document.selectAll(); convert("image_${i + 1}");`;
			image_list.push(image);
			if (image_list.length === 30) {
				if (script_count === 1) {
					Kernel.FileSystem.write_file(
						`${dest}/script_${script_count}.jsfl`,
						script_string
							.replace('{0}', 'var libraryLength = document.library.items.length;')
							.replace('{1}', image_list.join('\n')),
					);
				} else {
					Kernel.FileSystem.write_file(
						`${dest}/script_${script_count}.jsfl`,
						script_string.replace('{1}', image_list.join('\n')),
					);
				}
				image_list = [];
				++script_count;
			}
		}
		if (image_list.length > 0) {
			if (script_count === 1) {
				Kernel.FileSystem.write_file(
					`${dest}/script_${script_count}.jsfl`,
					script_string
						.replace('{0}', 'var libraryLength = document.library.items.length;')
						.replace('{1}', image_list.join('\n')),
				);
			} else {
				Kernel.FileSystem.write_file(
					`${dest}/script_${script_count}.jsfl`,
					script_string.replace('{1}', image_list.join('\n')),
				);
			}
		}
		const script_data = `
        (function(global){
        
        /**
        *  The JSON serialization and unserialization methods
        *  @class JSON
        */
        var JSON = {};
    
        JSON.prettyPrint = false;
    
        /**
        *  implement JSON.stringify serialization
        *  @method stringify
        *  @param {Object} obj The object to convert
        */
        JSON.stringify = function(obj)
        {
            return _internalStringify(obj, 0);
        };
    
        function _internalStringify(obj, depth, fromArray)
        {
            var t = typeof (obj);
            if (t != "object" || obj === null)
            {
                // simple data type
                if (t == "string") return '"'+obj+'"';
                return String(obj);
            }
            else
            {
                // recurse array or object
                var n, v, json = [], arr = (obj && obj.constructor == Array);
    
                var joinString, bracketString, firstPropString;
                if(JSON.prettyPrint)
                {
                    joinString = ",\\n";
                    bracketString = "\\n";
                    for(var i = 0; i < depth; ++i)
                    {
                        joinString += "\\t";
                        bracketString += "\\t";
                    }
                    joinString += "\\t";//one extra for the properties of this object
                    firstPropString = bracketString + "\\t";
                }
                else
                {
                    joinString = ",";
                    firstPropString = bracketString = "";
                }
                for (n in obj)
                {
                    v = obj[n]; t = typeof(v);
    
                    // Ignore functions
                    if (t == "function") continue;
    
                    if (t == "string") v = '"'+v+'"';
                    else if (t == "object" && v !== null) v = _internalStringify(v, depth + 1, arr);
    
                    json.push((arr ? "" : '"' + n + '": ') + String(v));
                }
                return (fromArray || depth === 0 ? "" : bracketString)+ (arr ? "[" : "{") + firstPropString + json.join(joinString) + bracketString + (arr ? "]" : "}");
            }
        }
    
        /**
        *  Implement JSON.parse de-serialization
        *  @method parse
        *  @param {String} str The string to de-serialize
        */
        JSON.parse = function(str)
        {
            if (str === "") str = '""';
            eval("var p=" + str + ";"); // jshint ignore:line
            return p;
        };
    
        // Assign to global space
        global.JSON = JSON;
        }(window));
        function writedata() {
        fl.showIdleMessage(false);
        const data = {
                version: 6,
                resolution: ${resolution},
                position: {
                    x: 0,
                    y: 0
                },
                image: {},
                sprite: {}
        };
        JSON.prettyPrint = true;
        const document = fl.getDocumentDOM();
        const libraryItems = document.library.items;
        var imageList = {};
        for (var i = 0; i < libraryItems.length; i++) {
            const item = libraryItems[i];
            const itemName = item.name;
            if (itemName.indexOf("media/") > -1) {
                const width = parseInt(item.hPixels * ${1200 / resolution});
                const height = parseInt(item.vPixels * ${1200 / resolution})
                var imageName = "${animation_name.toLowerCase()}_" + width.toString() + "x" + height.toString();
                if (imageList.hasOwnProperty(imageName)) {
                    ++imageList[imageName];
                    imageName = imageName + "_" + imageList[imageName].toString();
                }
                else {
                    imageList[imageName] = 1;
                }
                item.name = imageName;
                data.image[imageName] = {
                    id: ("image_${animation_name}_" + imageName).toUpperCase(),
                    dimension: {
                    width: width,
                    height: height,
                    }
                }
                const index = document.library.findItemIndex("image/" + itemName.substr(6));
				libraryItems[index].name = imageName;
            }
        }
        const pathList = document.path.split("\\\\");
        pathList.pop();
        pathList.push("data.json");
        const path = "file:///" + pathList.join("/");
        const result = FLfile.write(path, JSON.stringify(data));
        //alert("Done");
        return;
        }
        writedata();`;
		Kernel.FileSystem.write_file(`${dest}/write_data.jsfl`, script_data);
	}

	export function process(
		source: string,
		dest: string,
		animation_name: string,
		resolution: number,
	): void {
		Kernel.FileSystem.create_directory(`${dest}/library`);
		const dom_document: Kernel.XML.XMLDocument = Kernel.XML.deserialize_fs(
			`${source}/DomDocument.xml`,
		);
		const container: Container = new Container();
		exchange_sprite(source, container, dom_document, 'main_sprite');
		exchange_document(dest, container, dom_document);
		Kernel.FileSystem.write_file(`${dest}/main.xfl`, 'PROXY-CS5');
		exchange_script(`${dest}/script`, container.symbol_list.length, animation_name, resolution);
		container.clear();
	}

	export function execute(): void {
		// TODO : Add loc
		const source = Console.path('Enter Fla path', 'file');
		Console.argument('Enter animation name');
		const animation_name = readline();
		Console.argument('Enter animation type');
		const type = readline();
		const dest = Kernel.Path.dirname(source);
		const resolution = 1536;
		// TODO : Fix
		//Kernel.Compression.Zip.Uncompress.process(`${source}`, `${source}_unpack`);
		const path = `images/initial/${type}/${animation_name}`.toLowerCase();
		const data = {
			'#expand_method': 'advanced',
			version: 4n,
			texture_format_category: 0n,
			composite: true,
			category: {
				resolution: [1536n, 768n],
				format: 0n,
			},
			subgroup: {
				[`${type}${animation_name}`]: {
					category: {
						common_type: true,
						locale: null,
						compression: 3n,
					},
					resource: {
						[`popanim_${type}_${animation_name}`.toUpperCase()]: {
							type: 'PopAnim',
							path: path,
						},
					},
				},
			},
		};
		process(
			`${source}_unpack`,
			`${dest}/${type}${animation_name}.package/resource/${path}`,
			animation_name,
			resolution,
		);
		Kernel.JSON.serialize_fs(`${dest}/${type}${animation_name}.package/data.json`, data);
	}
}

Sen.Script.Helper.PopCap.Animation.FlashToAnimation.execute();
