namespace Sen.Script.Support.PopCap.Animation {
    export type Transform = [number, number, number, number, number, number];

    export type Color = [number, number, number, number];

    export interface FrameNode {
        index: bigint;
        duration: bigint;
        resource: number;
        sprite: boolean;
        first_frame?: bigint;
        transform: Transform;
        color: Color;
    }

    export type FrameNodeList = Array<FrameNode>;

    export type FrameNodeStructure = Record<number, FrameNodeList>;

    export enum State {
        state_null,
        state_false,
        state_true
    }

    export interface Model {
        state: State;
        resource: number;
        sprite: boolean;
        transform: Transform;
        color: Color;
        frame_start: bigint;
        frame_duration: bigint;
        index?: bigint;
    }

    // animation
    export interface SexyAnimation {
        version: bigint;
        frame_rate: bigint;
        position: AnimationPosition;
        size: AnimationSize;
        image: Array<AnimationImage>;
        sprite: Array<AnimationSprite>;
        main_sprite: AnimationSprite;
    }

    export interface AnimationPosition {
        x: number;
        y: number;
    }

    export interface AnimationDimension {
        width: bigint;
        height: bigint;
    }

    export interface AnimationSize {
        width: number;
        height: number;
    }

    export interface AnimationImage {
        path: string;
        id: string;
        dimension: AnimationDimension;
        transform: number[];
    }

    export interface AnimationSprite {
        name: string;
        work_area: AnimationWorkArea;
        frame: AnimationFrame[];
    }

    export interface AnimationWorkArea {
        start: bigint;
        duration: bigint;
    }

    export interface AnimationFrame {
        label: string;
        stop: boolean;
        command: AnimationCommand[];
        remove: bigint[];
        append: AnimationAppend[];
        change: AnimationMove[];
    }

    export interface AnimationCommand {
        command: string;
        parameter: string;
    }

    export interface AnimationAppend {
        index: bigint;
        name: string;
        resource: bigint;
        sprite: boolean;
        additive: boolean;
        preload_frame: bigint;
        time_scale: number;
    }

    export interface AnimationMove {
        index: bigint;
        transform: Array<number>;
        color: Color | null;
        source_rectangle: Array<number> | null;
        sprite_frame_number: bigint;
    }

    export namespace DOM {
        // sprite_document
        export interface SpriteDocument extends Kernel.XML.XMLDocument {
            DOMSymbolItem: {
                "@attributes": Attributes;
                timeline: {
                    DOMTimeline: SpriteDomTimeline;
                };
            };
        }

        export interface PureInstance {
            "@text": {
                value: string;
                is_cdata: boolean;
            };
        }

        export interface Matrix {
            "@attributes": {
                a: string;
                b: string;
                c: string;
                d: string;
                tx: string;
                ty: string;
            };
        }

        export interface DomColor {
            "@attributes": {
                redMultiplier?: string;
                greenMultiplier?: string;
                blueMultiplier?: string;
                alphaMultiplier?: string;
                redOffset?: string;
                greenOffset?: string;
                blueOffset?: string;
                alphaOffset?: string;
                tintMultiplier?: string;
                tintColor?: string;
            };
        }

        export interface DOMSymbolInstanceAttributes {
            libraryItemName: string;
            firstFrame?: string;
            symbolType: string;
            loop: string;
        }

        export interface DOMSymbolInstance {
            "@attributes": DOMSymbolInstanceAttributes;
            matrix: {
                Matrix: Matrix;
            };
            color?: {
                Color: Color;
            };
        }

        export interface SpriteDomFrame {
            "@attributes": {
                index: string;
                duration: string;
            };
            elements: ElementsDOMSymbolInstance | PureInstance | null;
        }

        export interface ElementsDOMSymbolInstance {
            DOMSymbolInstance: DOMSymbolInstance;
        }

        export type SpriteDomFrameProperty = SpriteDomFrame | SpriteDomFrame[];

        export interface SpriteFrame {
            DOMFrame: SpriteDomFrameProperty;
        }

        export interface SpriteDomLayer {
            "@attributes": {
                name: string;
            };
            frames: SpriteFrame;
        }

        export type SpriteDomLayerProperty = SpriteDomLayer | SpriteDomLayer[];

        export interface SpriteLayers {
            DOMLayer: SpriteDomLayerProperty;
        }

        export interface SpriteDomTimeline {
            "@attributes": {
                name: string;
            };
            layers: SpriteLayers;
        }

        // image_document
        export interface ImageDocument extends Kernel.XML.XMLDocument {
            DOMSymbolItem: {
                "@attributes": Attributes;
                timeline: {
                    DOMTimeline: DomTimeline;
                };
            };
        }

        export interface DOMBitmapInstance {
            "@attributes": {
                libraryItemName: string;
            };
            matrix: {
                Matrix: Matrix;
            };
        }

        export interface DomFrame {
            "@attributes": {
                index: string;
            };
            elements: {
                DOMBitmapInstance: DOMBitmapInstance;
            };
        }

        export interface DomLayer {
            frames: {
                DOMFrame: DomFrame;
            };
        }

        export interface DomTimeline {
            "@attributes": {
                name: string;
            };
            layers: {
                DOMLayer: DomLayer;
            };
        }

        export type xmlns_xsi = "http://www.w3.org/2001/XMLSchema-instance";

        export type xmlns = "http://ns.adobe.com/xfl/2008/";

        export interface Attributes {
            "xmlns:xsi": xmlns_xsi;
            xmlns: xmlns;
            name: string;
            symbolType: string;
        }

        // dom_document

        export interface DOMDocument extends Kernel.XML.XMLDocument {
            DOMDocument: {
                "@attributes": DocumentAttributes;
                folders: DomFolderItem;
                media: DomBitmapItem;
                symbols: {
                    Include: AttributesSymbolsItem[];
                };
                timelines: {
                    DOMTimeline: DocumentDOMTimeline;
                };
            };
        }

        export interface DocumentDOMTimeline {
            "@attributes": {
                name: string;
            };
            layers: {
                DOMLayer: DocumentDomLayer[];
            };
        }

        export interface DocumentDomLayer {
            "@attributes": {
                name: string;
            };
            frames: {
                DOMFrame: DocumentDomFrameProperty;
            };
        }

        export interface DocumentDomSymbolInstance {
            "@attributes": {
                libraryItemName: string;
                symbolType: string;
                loop: string;
            };
            matrix?: {
                Matrix: Matrix;
            } | null;
            color?: {
                Color: Color;
            } | null;
        }

        export type Actionscript = {
            "@text": {
                value: string;
                is_cdata: boolean;
            };
        };

        export interface ActionscriptProperty {
            script: Actionscript;
        }

        export interface DocumentElements {
            DOMSymbolInstance: DocumentDomSymbolInstance;
        }

        export interface DocumentDomFrame {
            "@attributes": {
                index: string;
                duration?: string;
                name?: string;
                labelType?: string;
            };
            Actionscript?: ActionscriptProperty;
            elements?: DocumentElements | null | PureInstance;
        }

        export type DocumentDomFrameProperty = DocumentDomFrame | DocumentDomFrame[];

        export type XflVersion = "2.971";

        export interface DocumentAttributes {
            "xmlns:xsi": xmlns_xsi;
            xmlns: xmlns;
            frameRate: string;
            width: string;
            height: string;
            xflVersion: XflVersion;
        }

        export interface AttributesFolderItem {
            "@attributes": {
                name: string;
                isExpanded: string;
            };
        }

        export interface DomFolderItem {
            DOMFolderItem: AttributesFolderItem[];
        }

        export interface AttributesBitmapItem {
            "@attributes": {
                name: string;
                href: string;
            };
        }

        export interface DomBitmapItem {
            DOMBitmapItem: AttributesBitmapItem[];
        }

        export interface AttributesSymbolsItem {
            "@attributes": {
                href: string;
            };
        }
    }


}
