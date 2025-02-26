namespace Sen.Script.Test.XML {
	export function Test() {
		return function (_: any, propertyKey: string, descriptor: PropertyDescriptor) {
			const originalMethod = descriptor.value;
			descriptor.value = function (...args: any[]) {
				Console.display(
					`Execution argument: Running ${propertyKey}`,
					'cyan',
					`arguments: [${args}]`,
				);
				try {
					originalMethod.apply(this, args);
					Console.finished(`Finished ${propertyKey}`);
					return true;
				} catch (e) {
					Console.error((e as Error).name, Exception.make_exception(e as any));
					return false;
				}
			};
			return descriptor;
		};
	}

	export class Tester {
		constructor() {}

		@Test()
		public test1(): boolean {
			debug(
				Kernel.test(`<DOMDocument xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://ns.adobe.com/xfl/2008/" backgroundColor="#999999" frameRate="30" width="390.000000" height="390.000000" xflVersion="2.971">
    <folders>
        <DOMFolderItem name="media" isExpanded="true"/>
        <DOMFolderItem name="image" isExpanded="true"/>
        <DOMFolderItem name="sprite" isExpanded="true"/>
        <DOMFolderItem name="label" isExpanded="true"/>
    </folders>
    <media>
        <DOMBitmapItem name="media/hypno_zombie_effect_149x149" href="media/hypno_zombie_effect_149x149.png"/>
        <DOMBitmapItem name="media/hypno_zombie_effect_149x149_2" href="media/hypno_zombie_effect_149x149_2.png"/>
        <DOMBitmapItem name="media/hypno_zombie_effect_67x67" href="media/hypno_zombie_effect_67x67.png"/>
        <DOMBitmapItem name="media/hypno_zombie_effect_34x34" href="media/hypno_zombie_effect_34x34.png"/>
        <DOMBitmapItem name="media/hypno_zombie_effect_149x149_3" href="media/hypno_zombie_effect_149x149_3.png"/>
        <DOMBitmapItem name="media/hypno_zombie_effect_51x51" href="media/hypno_zombie_effect_51x51.png"/>
        <DOMBitmapItem name="media/hypno_zombie_effect_149x149_4" href="media/hypno_zombie_effect_149x149_4.png"/>
        <DOMBitmapItem name="media/hypno_zombie_effect_149x149_5" href="media/hypno_zombie_effect_149x149_5.png"/>
    </media>
    <symbols>
        <Include href="image/hypno_zombie_effect_149x149.xml"/>
        <Include href="image/hypno_zombie_effect_149x149_2.xml"/>
        <Include href="image/hypno_zombie_effect_67x67.xml"/>
        <Include href="image/hypno_zombie_effect_34x34.xml"/>
        <Include href="image/hypno_zombie_effect_149x149_3.xml"/>
        <Include href="image/hypno_zombie_effect_51x51.xml"/>
        <Include href="image/hypno_zombie_effect_149x149_4.xml"/>
        <Include href="image/hypno_zombie_effect_149x149_5.xml"/>
        <Include href="label/animation.xml"/>
    </symbols>
    <timelines>
        <DOMTimeline name="animation">
            <layers>
                <DOMLayer name="label">
                    <frames>
                        <DOMFrame index="0" duration="66" name="animation" labelType="name">
                            <elements/>
                        </DOMFrame>
                    </frames>
                </DOMLayer>
                <DOMLayer name="action">
                    <frames>
                        <DOMFrame index="0" duration="65">
                            <elements/>
                        </DOMFrame>
                        <DOMFrame index="65">
                            <Actionscript>
                                <script><![CDATA[stop();]]></script>
                            </Actionscript>
                            <elements/>
                        </DOMFrame>
                    </frames>
                </DOMLayer>
                <DOMLayer name="instance">
                    <frames>
                        <DOMFrame index="0" duration="66">
                            <elements>
                                <DOMSymbolInstance libraryItemName="label/animation" symbolType="graphic" loop="loop"/>
                            </elements>
                        </DOMFrame>
                    </frames>
                </DOMLayer>
            </layers>
        </DOMTimeline>
    </timelines>
</DOMDocument>
`),
			);
			return true;
		}

		@Test()
		public test2(): boolean {
			return true;
		}

		@Test()
		public test3(): boolean {
			return true;
		}
	}

	export function main() {
		const tester = new Tester();
		const results = new Array();
		results.push(tester.test1());
		results.push(tester.test2());
		results.push(tester.test3());
		Console.finished(
			`All test case has been executed!`,
			`Total: ${results.length} tasks. Success: ${results.filter((e) => e).length}, Error: ${
				results.filter((e) => !e).length
			}`,
		);
	}
}

Sen.Script.Test.XML.main();
