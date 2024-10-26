namespace Sen.Script.Helper.PvZ2.Chinese.ToInternational {
    export type Dummy = Record<string, Array<string>>;

    export function execute(): void {
        const international_bundle: string = Console.path(Kernel.Language.get("script.helper.pvz2.chinese.to_international.input_international_bundle"), "directory");
        const chinese_bundle: string = Console.path(Kernel.Language.get("script.helper.pvz2.chinese.to_international.input_chinese_bundle"), "directory");
        Console.argument(Kernel.Language.get("script.helper.pvz2.chinese.to_international.parsing_international_manifest"));
        const international_data = Kernel.JSON.deserialize_fs<any>(`${international_bundle}/data.json`);
        Console.finished(Kernel.Language.get("script.helper.pvz2.chinese.to_international.done"));
        Console.argument(Kernel.Language.get("script.helper.pvz2.chinese.to_international.parsing_chinese_manifest"));
        const chinese_data = Kernel.JSON.deserialize_fs<any>(`${chinese_bundle}/data.json`);
        Console.finished(Kernel.Language.get("script.helper.pvz2.chinese.to_international.done"));
        const dummy = chinese_data["packet"] as Array<string>;
        const scg_setting: Script.Support.Miscellaneous.Custom.StreamCompressedGroup.Configuration.Setting = {
            decode_method: 0n,
            animation_split_label: false,
        };
        const _handle = () => {
            const ripe_file = `${chinese_bundle}/group.json`;
            Kernel.JSON.serialize_fs<Array<string>>(ripe_file, dummy, 1, false);
            Console.finished(`${Kernel.Language.get("script.helper.pvz2.chinese.to_international.dumped_file")}: ${ripe_file}`);
            const international_ripe_file: string = `${international_bundle}/group.json`;
            Kernel.JSON.serialize_fs<Array<string>>(international_ripe_file, [], 1, false);
            Console.finished(`${Kernel.Language.get("script.helper.pvz2.chinese.to_international.to_port_2c_content_edit_this")}: ${ripe_file}`);
            Console.argument(Kernel.Language.get("script.helper.pvz2.chinese.to_international.waiting"));
            Kernel.Console.readline();
            return Kernel.JSON.deserialize_fs<Array<string>>(international_ripe_file);
        };
        const list = _handle();
        for (const e of list) {
            if (international_data.packet.includes(e)) {
                Console.error(format(Kernel.Language.get("already_exists"), e));
                continue;
            }
            Console.send(e);
            const source_directory = `${international_bundle}/packet/${e}.scg.package`;
            Kernel.Support.Miscellaneous.Custom.StreamCompressedGroup.decode_fs(`${chinese_bundle}/packet/${e}.scg`, source_directory, scg_setting);
            const scg_data = Kernel.JSON.deserialize_fs<any>(`${international_bundle}/packet/${e}.scg.package/data.json`);
            scg_data["texture_format_category"] = 0n;
            if (scg_data["composite"] && scg_data["category"] !== null) {
                scg_data["category"]["format"] = 0n;
            }
            for (const [_, value] of Object.entries(scg_data.subgroup)) {
                (value as any).category.compression = 3n;
            }
            Kernel.JSON.serialize_fs(`${source_directory}/data.json`, scg_data, 1, false);
            Kernel.Support.Miscellaneous.Custom.StreamCompressedGroup.encode_fs(source_directory, `${international_bundle}/packet/${e}.scg`, scg_setting);
            international_data.packet.push(e);
            Kernel.FileSystem.Operation.remove_all(source_directory);
        }
        Console.finished(Kernel.Language.get("script.helper.pvz2.chinese.to_international.all_process_has_been_finished"));
        Kernel.JSON.serialize_fs(`${international_bundle}/data.json`, international_data, 1, false);
    }
}

Sen.Script.Helper.PvZ2.Chinese.ToInternational.execute();
