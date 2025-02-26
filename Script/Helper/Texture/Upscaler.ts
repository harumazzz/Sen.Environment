namespace Sen.Script.Support.Texture.Upscaler {
	/**
	 * Detail namespace
	 */

	export namespace Detail {
		export function sprite_generic(): Array<[bigint, bigint, string]> {
			return [
				[1n, 0n, Kernel.Language.get('popcap.animation.miscellaneous.to_apng.enable_all')],
				[2n, 1n, Kernel.Language.get('popcap.animation.miscellaneous.to_apng.disable_all')],
				[
					3n,
					2n,
					Kernel.Language.get('popcap.animation.miscellaneous.to_apng.select_to_disable'),
				],
			];
		}
	}

	export function process(source: string, destination: string) {
		const operating_system = Kernel.OperatingSystem.current();
		assert(
			operating_system === 'Windows' ||
				operating_system === 'macOS' ||
				operating_system === 'Linux',
			Kernel.Language.get('image.upscaler.unsupported_operating_system'),
		);
		const real_esrgan_program_third_path = Home.query('~/../thirdapp/real_esrgan');
		const real_esrgan_program_file_path = `${real_esrgan_program_third_path}/realesrgan-ncnn-vulkan${
			operating_system === 'Windows' ? '.exe' : ''
		}`;
		assert(
			Kernel.FileSystem.is_file(real_esrgan_program_file_path),
			Kernel.Language.get('image.upscaler.cannot_find_third'),
		);
		assert(
			Kernel.FileSystem.is_directory(`${real_esrgan_program_third_path}/models`),
			Kernel.Language.get('image.upscaler.cannot_find_real_esrgan_models_folder'),
		);
		const model_list: Array<string> = Kernel.FileSystem.read_directory_only_file(
			`${real_esrgan_program_third_path}/models`,
		).filter((e) => /(\.param)?$/i.test(e));
		assert(model_list.length > 0, 'real_esrgan_model_folder_is_empty');
		const model_rule: Array<[bigint, string, string]> = model_list.map((e, i) => [
			BigInt(i + 1),
			e,
			Kernel.Path.base_without_extension(e).toLowerCase(),
		]);
		Console.argument(Kernel.Language.get('image.upscaler.enter_model'));
		const model_selected: string = model_rule[Executor.is_valid_rule(model_rule)][2];
		const command = `${real_esrgan_program_file_path} -i "${source}" -o "${destination}" -n "${model_selected}" > nul 2>&1`;
		Kernel.FileSystem.remove(destination);
		Kernel.Process.execute(command);
		assert(
			Kernel.FileSystem.is_file(destination),
			Kernel.Language.get('failed_to_upscale_image'),
		);
	}

	export function process_fs(source: string, destination: string) {
		return process(source, destination);
	}
}
