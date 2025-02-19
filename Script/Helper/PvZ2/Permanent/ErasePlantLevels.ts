namespace Sen.Script.Helper.PvZ2.Permanent.ErasePlantLevels {
	/**
	 * @TODO Update this for official
	 */
	export interface PlantLevels extends Record<string, unknown> {
		objects: Array<{
			aliases: [string];
			objdata: {
				UsesLeveling?: boolean;
				LevelCoins?: Array<bigint>;
				LevelXP?: Array<bigint>;
				LevelCap?: bigint;
				PlantTier?: Array<bigint>;
				FloatStats: any;
				StringStats: any;
			};
		}>;
	}

	/**
	 *
	 * @param source - input
	 * @param ripe - output
	 * @returns Erased
	 */
	export function process(source: string, ripe: string): void {
		const json: PlantLevels = Kernel.JSON.deserialize_fs(source);
		for (let obj of json.objects) {
			for (let stat of obj.objdata.FloatStats) {
				if (stat.Values.length > 1) {
					stat.Values.splice(1);
				}
			}
			if (obj.objdata.StringStats) {
				for (let stat of obj.objdata.StringStats) {
					if (stat.Values.length > 1) {
						stat.Values.splice(1);
					}
				}
			}
			obj.objdata.LevelCap = 1n;
			obj.objdata.LevelCoins = [];
			obj.objdata.LevelXP = [];
			obj.objdata.UsesLeveling = false;
			if ('PlantTier' in obj.objdata && obj.objdata.PlantTier!.length > 1) {
				obj.objdata.PlantTier!.length = 1;
			}
			Console.output(`Leveling removed for: ${obj.aliases[0]}`);
		}
		Kernel.JSON.serialize_fs<PlantLevels>(ripe, json);
	}

	/**
	 *
	 * @returns Evaluate
	 */

	export function execute(): void {
		const source: string = Console.path(
			Kernel.Language.get('script.erase_plant_level.source_file'),
			'file',
		);
		return process(source, source.replace(/((\.json))?$/i, '.patch.json'));
	}
}

Sen.Script.Helper.PvZ2.Permanent.ErasePlantLevels.execute();
