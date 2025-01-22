namespace Sen.Script.Helper.Windows.AssignWindowsRegistry {
	export function execute() {
		assert(
			Kernel.OperatingSystem.current() === 'Windows',
			'This method only support Windows machine',
		);
		const entry = Kernel.Path.dirname(Home.participant).replaceAll('/', '\\\\');
		let command = `Windows Registry Editor Version 5.00\n`;
		command += `[HKEY_CLASSES_ROOT\\Directory\\shell\\Open With Sen]\n`;
		command += `@="Open with Sen"\n`;
		command += `"Icon"="${entry}\\\\Launcher.exe\\"\n`;
		command += `[HKEY_CLASSES_ROOT\\Directory\\shell\\Open With Sen\\command]\n`;
		command += `@="\\"${entry}\\\\Launcher.exe\\" \\"%1\\""\n`;
		command += `[HKEY_CLASSES_ROOT\\Directory\\shell\\PopCap RSB: Build Project]\n`;
		command += `@="PopCap RSB: Build Project"\n`;
		command += `"Icon"="${entry}\\\\Launcher.exe\\"\n`;
		command += `[HKEY_CLASSES_ROOT\\Directory\\shell\\PopCap RSB: Build Project\\command]\n`;
		command += `@="\\"${entry}\\\\Launcher.exe\\" -source \\"%1\\" -method popcap.rsb.build_project"\n`;
		command += `[HKEY_CLASSES_ROOT\\Directory\\shell\\PvZ2 SCG: Encode]\n`;
		command += `@="PvZ2 SCG: Encode"\n`;
		command += `"Icon"="${entry}\\\\Launcher.exe\\"\n`;
		command += `[HKEY_CLASSES_ROOT\\Directory\\shell\\PvZ2 SCG: Encode\\command]\n`;
		command += `@="\\"${entry}\\\\Launcher.exe\\" -source \\"%1\\" -method project.scg.encode"\n`;
		const extensions = [
			'.rsb',
			'.rsg',
			'.pam',
			'.rton',
			'.json',
			'.png',
			'.ptx',
			'.js',
			'.obb',
			'.bnk',
			'.wem',
			'.scg',
			'.popfx',
			'.newton',
		];
		for (const ext of extensions) {
			command += `[HKEY_CURRENT_USER\\Software\\Classes\\${ext}]\n`;
			command += `@="Sen.Environment"\n`;
		}
		command += `[HKEY_CURRENT_USER\\Software\\Classes\\Sen.Environment\\DefaultIcon]\n`;
		command += `@="${entry}\\\\Launcher.exe\\"\n`;
		command += `[HKEY_CURRENT_USER\\Software\\Classes\\Sen.Environment\\shell\\open\\command]\n`;
		command += `@="\\"${entry}\\\\Launcher.exe\\" \\"%1\\""\n`;
		command += `[HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\*\\shell\\Open With Sen]\n`;
		command += `@="Open with Sen"\n`;
		command += `"Icon"="${entry}\\\\Launcher.exe\\"\n`;
		command += `[HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\*\\shell\\Open With Sen\\command]\n`;
		command += `@="\\"${entry}\\\\Launcher.exe\\" \\"%1\\""\n`;
		const destination = `${entry}/sen.reg`;
		Kernel.FileSystem.write_file(destination, command);
		Kernel.Process.run(`regedit /s ${destination}`);
		Kernel.FileSystem.remove(destination);
	}
}

Sen.Script.Helper.Windows.AssignWindowsRegistry.execute();
