import 'dart:io';

import 'package:flutter/material.dart';
import 'package:sen/model/translator.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/setting/translator_page.dart';
import 'package:sen/service/android_service.dart';
import 'package:sen/service/file_service.dart';
import 'package:provider/provider.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class SettingScreen extends StatefulWidget {
  const SettingScreen({super.key});

  @override
  State<SettingScreen> createState() => _SettingScreenState();
}

class _SettingScreenState extends State<SettingScreen> {
  bool _hasPermission = false;

  @override
  void initState() {
    super.initState();
    Future.sync(() async {
      var hasPermission = await _checkDefaultPermission();
      setState(() {
        _hasPermission = hasPermission;
      });
    });
  }

  void _onChangeTheme() async {
    final settingProvider = Provider.of<SettingProvider>(
      context,
      listen: false,
    );
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.theme),
        content: Column(
          crossAxisAlignment: CrossAxisAlignment.end,
          mainAxisSize: MainAxisSize.min,
          children: [
            _buildThemeOptionTile(
              settingProvider: settingProvider,
              title: los.system,
              value: 'system',
            ),
            _buildThemeOptionTile(
              settingProvider: settingProvider,
              title: los.light,
              value: 'light',
            ),
            _buildThemeOptionTile(
              settingProvider: settingProvider,
              title: los.dark,
              value: 'dark',
            ),
            const SizedBox(height: 10),
            _onCloseButton(),
          ],
        ),
      ),
    );
  }

  Widget _onCloseButton() {
    final los = AppLocalizations.of(context)!;
    return TextButton(
      onPressed: () {
        Navigator.of(context).pop();
      },
      child: Text(los.okay),
    );
  }

  Widget _buildThemeOptionTile({
    required SettingProvider settingProvider,
    required String title,
    required String value,
  }) {
    return ListTile(
      title: Text(title),
      leading: Radio<String>(
        value: value,
        groupValue: settingProvider.theme,
        onChanged: (String? value) {
          if (value == null) return;
          settingProvider.setTheme(value);
        },
      ),
    );
  }

  Widget _buildLocaleOptionTitle({
    required SettingProvider settingProvider,
    required String title,
    required String value,
  }) {
    return ListTile(
      title: Text(title),
      leading: Radio<String>(
        value: value,
        groupValue: settingProvider.locale,
        onChanged: (String? value) {
          if (value == null) return;
          settingProvider.setLocale(value);
        },
      ),
    );
  }

  Widget _buildNotificationOption({
    required SettingProvider settingProvider,
    required String title,
    required bool value,
  }) {
    return ListTile(
      title: Text(title),
      leading: Radio<bool>(
        value: value,
        groupValue: settingProvider.sendNotification,
        onChanged: (bool? value) {
          if (value == null) {
            return;
          }
          settingProvider.setNotification(value);
        },
      ),
    );
  }

  void _onChangeNotification() async {
    final settingProvider = Provider.of<SettingProvider>(
      context,
      listen: false,
    );
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.send_notification),
        actions: [
          _buildNotificationOption(
            settingProvider: settingProvider,
            title: los.enable,
            value: true,
          ),
          _buildNotificationOption(
            settingProvider: settingProvider,
            title: los.disable,
            value: false,
          ),
          const SizedBox(height: 10),
          _onCloseButton(),
        ],
      ),
    );
  }

  void _onCheckValidator() {
    final settingProvider = Provider.of<SettingProvider>(
      context,
      listen: false,
    );
    if (settingProvider.toolChain.isNotEmpty) {
      settingProvider.setIsValid(
        _existKernel(settingProvider.toolChain) &&
            _existScript(settingProvider.toolChain),
      );
    }
  }

  bool _existKernel(String path) {
    if (Platform.isAndroid) {
      return true;
    }
    if (Platform.isWindows) {
      return FileService.isFile('$path/Kernel.dll');
    }
    if (Platform.isLinux) {
      return FileService.isFile('$path/libKernel.so');
    }
    if (Platform.isIOS || Platform.isMacOS) {
      return FileService.isFile('$path/libKernel.dylib');
    }
    return false;
  }

  Future<bool> _checkDefaultPermission() async {
    if (Platform.isAndroid) {
      return await AndroidService.checkStoragePermission();
    }
    return true;
  }

  bool _existScript(String path) {
    return FileService.isFile('$path/Script/main.js');
  }

  void _onChangeToolChain() async {
    final settingProvider = Provider.of<SettingProvider>(
      context,
      listen: false,
    );
    final los = AppLocalizations.of(context)!;
    final controller = TextEditingController(text: settingProvider.toolChain);
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.toolchain),
        actions: [
          Row(
            children: [
              Expanded(
                child: TextField(
                  controller: controller,
                  onChanged: (String value) {
                    controller.text = value;
                    settingProvider.setToolChain(controller.text);
                    _onCheckValidator();
                  },
                ),
              ),
              Tooltip(
                message: los.upload_directory,
                child: IconButton(
                  onPressed: () async {
                    final directory = await FileService.uploadDirectory();
                    if (directory == null) {
                      return;
                    }
                    controller.text = directory;
                    settingProvider.setToolChain(directory);
                    _onCheckValidator();
                  },
                  icon: const Icon(Icons.drive_folder_upload_outlined),
                ),
              ),
            ],
          ),
          const SizedBox(height: 10),
          _onCloseButton(),
        ],
      ),
    );
    controller.dispose();
  }

  Translator _exchangeTranslator(String name) {
    return switch (name) {
      'haruma' => Translator(
          name: 'Haruma',
          discord: 'harumaluvcat',
          contacts: [
            ['GitHub', 'https://github.com/Haruma-VN'],
            ['Youtube', 'https://www.youtube.com/@harumavn'],
          ],
          imageCover: 'assets/images/translator/haruma.png',
        ),
      'jnr' => Translator(
          name: 'JNR',
          discord: 'jnr1809',
          contacts: [
            ['Youtube', 'https://www.youtube.com/@jnr1809'],
          ],
          imageCover: 'assets/images/translator/jnr.png',
        ),
      'ppp' => Translator(
          name: 'PPP',
          discord: 'theprimalpea',
          contacts: [
            ['Facebook', 'https://www.facebook.com/ThePrimalPea'],
          ],
          imageCover: 'assets/images/translator/ppp.png',
        ),
      'vi' => Translator(
          name: 'Vi',
          discord: 'vi_i_guess',
          contacts: [
            ['GitHub', 'https://github.com/viiguess']
          ],
          imageCover: 'assets/images/translator/vi.png',
        ),
      String() => throw UnimplementedError(),
    };
  }

  void _onViewTranslator() async {
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        content: TranslatorPage(
          translator: _exchangeTranslator(
            AppLocalizations.of(context)!.author_of_this_locale.toLowerCase(),
          ),
        ),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(AppLocalizations.of(context)!.okay),
          ),
        ],
      ),
    );
  }

  void _onChangeLocale() async {
    final los = AppLocalizations.of(context)!;
    final settingProvider = Provider.of<SettingProvider>(
      context,
      listen: false,
    );
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.language),
        content: Column(
          crossAxisAlignment: CrossAxisAlignment.end,
          mainAxisSize: MainAxisSize.min,
          children: [
            _buildLocaleOptionTitle(
              settingProvider: settingProvider,
              title: los.en,
              value: 'en',
            ),
            _buildLocaleOptionTitle(
              settingProvider: settingProvider,
              title: los.vi,
              value: 'vi',
            ),
            _buildLocaleOptionTitle(
              settingProvider: settingProvider,
              title: los.es,
              value: 'es',
            ),
            _buildLocaleOptionTitle(
              settingProvider: settingProvider,
              title: los.ru,
              value: 'ru',
            ),
            const SizedBox(height: 10),
            _onCloseButton(),
          ],
        ),
      ),
    );
  }

  String _exchangeLocale(String key) {
    final localization = AppLocalizations.of(context)!;
    final Map<String, String> data = {
      'en': localization.en,
      'vi': localization.vi,
      'es': localization.es,
      'ru': localization.ru,
    };
    return data[key] ?? key;
  }

  void _requestPermission() async {
    await AndroidService.requestStoragePermission();
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    final settingProvider = Provider.of<SettingProvider>(context);
    toolchainPath() => settingProvider.toolChain == ''
        ? los.not_specified
        : settingProvider.toolChain;
    return Container(
      margin: const EdgeInsets.symmetric(
        horizontal: 16.0,
        vertical: 8.0,
      ),
      child: SingleChildScrollView(
        child: Container(
          constraints: BoxConstraints(
            minHeight: MediaQuery.of(context).size.height,
          ),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(los.default_setting),
              const SizedBox(height: 15),
              ListTile(
                leading: const Icon(Icons.dark_mode_outlined),
                title: Text(los.theme),
                onTap: _onChangeTheme,
              ),
              const SizedBox(height: 10),
              ListTile(
                leading: const Icon(Icons.translate_outlined),
                title: Text(los.language),
                subtitle: Text(_exchangeLocale(settingProvider.locale)),
                onTap: _onChangeLocale,
              ),
              const SizedBox(height: 10),
              ListTile(
                leading: const Icon(Icons.person_2_outlined),
                title: Text(los.author),
                subtitle: Text(los.author_of_this_locale),
                onTap: _onViewTranslator,
              ),
              const Divider(),
              Text(los.application_setting),
              const SizedBox(height: 15),
              ListTile(
                leading: const Icon(Icons.notifications_outlined),
                title: Text(los.send_notification),
                onTap: _onChangeNotification,
              ),
              const SizedBox(height: 10),
              ListTile(
                leading: const Icon(Icons.storage_outlined),
                title: Text(los.storage_permission),
                subtitle: _hasPermission ? Text(los.granted) : Text(los.denied),
                onTap: !_hasPermission ? _requestPermission : null,
              ),
              const SizedBox(height: 10),
              ListTile(
                leading: const Icon(Icons.build_outlined),
                title: Text(los.toolchain),
                subtitle: Text(toolchainPath()),
                onTap: _onChangeToolChain,
              ),
            ],
          ),
        ),
      ),
    );
  }
}
