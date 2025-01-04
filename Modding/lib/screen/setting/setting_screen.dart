import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/build_distribution.dart';
import 'package:sen/model/translator.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/setting/locale_option.dart';
import 'package:sen/screen/setting/notification_option.dart';
import 'package:sen/screen/setting/theme_option.dart';
import 'package:sen/screen/setting/translator_page.dart';
import 'package:sen/service/android_service.dart';
import 'package:sen/service/file_service.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/widget/hyperlink.dart';

class SettingScreen extends ConsumerStatefulWidget {
  const SettingScreen({super.key});

  @override
  ConsumerState<SettingScreen> createState() => _SettingScreenState();
}

class _SettingScreenState extends ConsumerState<SettingScreen> {
  bool _hasPermission = false;

  late TextEditingController _controller;

  @override
  void initState() {
    super.initState();
    Future.sync(() async {
      var hasPermission = await _checkDefaultPermission();
      setState(() {
        _hasPermission = hasPermission;
      });
    });
    _controller = TextEditingController(
      text: ref.read(settingProvider).toolChain,
    );
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  void _onChangeTheme() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => Consumer(
        builder: (context, ref, child) => AlertDialog(
          title: Text(los.theme),
          content: Column(
            crossAxisAlignment: CrossAxisAlignment.end,
            mainAxisSize: MainAxisSize.min,
            children: [
              ThemeOption(
                title: los.system,
                value: 'system',
              ),
              ThemeOption(
                title: los.light,
                value: 'light',
              ),
              ThemeOption(
                title: los.dark,
                value: 'dark',
              ),
              const SizedBox(height: 10),
              _onCloseButton(),
            ],
          ),
        ),
      ),
    );
  }

  Widget _onCloseButton() {
    final los = AppLocalizations.of(context)!;
    return TextButton(
      onPressed: () {
        _onCheckValidator();
        Navigator.of(context).pop();
      },
      child: Text(los.okay),
    );
  }

  void _onChangeNotification() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => Consumer(
        builder: (context, ref, child) => AlertDialog(
          title: Text(los.send_notification),
          actions: [
            NotificationOption(
              title: los.enable,
              value: true,
            ),
            NotificationOption(
              title: los.disable,
              value: false,
            ),
            const SizedBox(height: 10),
            _onCloseButton(),
          ],
        ),
      ),
    );
  }

  void _onCheckValidator() {
    if (ref.watch(settingProvider).toolChain.isNotEmpty) {
      final state = _existKernel(ref.watch(settingProvider).toolChain) &&
          _existScript(
            ref.watch(settingProvider).toolChain,
          );
      ref.watch(settingProvider.notifier).setIsValid(state);
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
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => Consumer(
        builder: (context, ref, child) => AlertDialog(
          title: Text(los.toolchain),
          actions: [
            Row(
              children: [
                Expanded(
                  child: TextField(
                    controller: _controller,
                    onChanged: (String value) {
                      _controller.text = value;
                      ref.watch(settingProvider.notifier).setToolChain(_controller.text);
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
                      _controller.text = directory;
                      ref.watch(settingProvider.notifier).setToolChain(directory);
                      _onCheckValidator();
                    },
                    icon: const Icon(Symbols.drive_folder_upload),
                  ),
                ),
              ],
            ),
            const SizedBox(height: 10),
            _onCloseButton(),
          ],
        ),
      ),
    );
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
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.language),
        content: Column(
          crossAxisAlignment: CrossAxisAlignment.end,
          mainAxisSize: MainAxisSize.min,
          children: [
            LocaleOption(
              title: los.en,
              value: 'en',
            ),
            LocaleOption(
              title: los.vi,
              value: 'vi',
            ),
            LocaleOption(
              title: los.es,
              value: 'es',
            ),
            LocaleOption(
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

  Widget _makeCustomizeRow({
    required String title,
    required String description,
    required String link,
  }) {
    return Row(
      children: [
        Text('$title: '),
        Hyperlink(
          title: description,
          link: link,
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    toolchainPath() =>
        ref.watch(settingProvider).toolChain == '' ? los.not_specified : ref.read(settingProvider).toolChain;
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
                leading: const Icon(Symbols.dark_mode),
                title: Text(los.theme),
                onTap: _onChangeTheme,
              ),
              const SizedBox(height: 10),
              ListTile(
                leading: const Icon(Symbols.translate),
                title: Text(los.language),
                subtitle: Text(_exchangeLocale(ref.read(settingProvider).locale)),
                onTap: _onChangeLocale,
              ),
              const SizedBox(height: 10),
              ListTile(
                leading: const Icon(Symbols.person_2),
                title: Text(los.author),
                subtitle: Text(los.author_of_this_locale),
                onTap: _onViewTranslator,
              ),
              const Divider(),
              Text(los.application_setting),
              const SizedBox(height: 15),
              ListTile(
                leading: const Icon(Symbols.notifications),
                title: Text(los.send_notification),
                onTap: _onChangeNotification,
              ),
              const SizedBox(height: 10),
              ListTile(
                leading: const Icon(Symbols.storage),
                title: Text(los.storage_permission),
                subtitle: _hasPermission ? Text(los.granted) : Text(los.denied),
                onTap: !_hasPermission ? _requestPermission : null,
              ),
              const SizedBox(height: 10),
              ListTile(
                leading: const Icon(Symbols.build),
                title: Text(los.toolchain),
                subtitle: Text(toolchainPath()),
                onTap: _onChangeToolChain,
                enabled: !Platform.isAndroid,
              ),
              AboutListTile(
                icon: const Icon(Symbols.info),
                applicationIcon: Image.asset(
                  'assets/images/logo.png',
                  width: 50,
                  height: 50,
                ),
                applicationName: 'Sen',
                aboutBoxChildren: [
                  Text('${los.version}: ${BuildDistribution.version}'),
                  const SizedBox(height: 10),
                  Text(
                    'Copyright © ${DateTime.now().year} ${BuildDistribution.kApplicationName}. All Rights Reserved.',
                    style: const TextStyle(fontStyle: FontStyle.italic),
                  ),
                  const SizedBox(height: 10),
                  const Text(
                    'Project is under GPLv3 License.',
                    style: TextStyle(fontStyle: FontStyle.italic),
                  ),
                  const SizedBox(height: 10),
                  _makeCustomizeRow(
                    title: 'Official Website',
                    description: 'Website',
                    link: 'https://haruma-vn.github.io/Sen.Environment/',
                  ),
                  const SizedBox(height: 10),
                  _makeCustomizeRow(
                    title: 'Repo',
                    description: 'GitHub',
                    link: 'https://github.com/Haruma-VN/Sen.Environment',
                  ),
                  const SizedBox(height: 10),
                  _makeCustomizeRow(
                    title: 'Discord',
                    description: 'Server',
                    link: 'https://discord.com/invite/C2Xr2kaBYJ',
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}
