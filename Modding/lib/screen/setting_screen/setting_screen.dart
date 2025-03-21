import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import '../../constant/translators.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import '../../i18n/app_localizations.dart';
import '../../model/translator.dart';
import '../../widget/material_dialog.dart';
import 'locale_option_list.dart';
import 'notification_option_list.dart';
import 'theme_option_list.dart';
import 'translator_page.dart';
import '../../service/android_helper.dart';
import '../../service/file_helper.dart';
import '../../service/ui_helper.dart';

class SettingScreen extends StatefulWidget {
  const SettingScreen({super.key});

  @override
  State<SettingScreen> createState() => _SettingScreenState();
}

class _SettingScreenState extends State<SettingScreen> {
  late bool _hasPermission;
  late TextEditingController _controller;

  @override
  void initState() {
    super.initState();
    _hasPermission = false;
    _controller = TextEditingController();
    _controller.addListener(() async {
      await context.read<SettingsCubit>().setToolChain(_controller.text);
      await _onCheckValidator();
    });
    WidgetsBinding.instance.addPostFrameCallback((_) async {
      await _initializeSettings();
    });
  }

  Future<void> _initializeSettings() async {
    _controller.text = context.read<SettingsCubit>().state.toolChain;
    _hasPermission = await _checkDefaultPermission();
    setState(() {});
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  Future<bool> _checkDefaultPermission() async {
    return CurrentPlatform.isAndroid
        ? await AndroidHelper.checkStoragePermission()
        : true;
  }

  Future<void> _showDialog(Widget content, String title) async {
    await UIHelper.showDetailDialog(
      context: context,
      title: Text(
        title,
        style: Theme.of(
          context,
        ).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.w600),
      ),
      content: content,
    );
  }

  String _exchangeLocale(String key) {
    final localization = AppLocalizations.of(context)!;
    final Map<String, String> data = {
      'en': localization.en,
      'vi': localization.vi,
      'es': localization.es,
      'ru': localization.ru,
      'fr': localization.fr,
      'de': localization.de,
    };
    return data[key] ?? key;
  }

  String _exchangeTheme(String theme) {
    final localization = AppLocalizations.of(context)!;
    final Map<String, String> data = {
      'system': localization.system,
      'dark': localization.dark,
      'light': localization.light,
    };
    return data[theme] ?? theme;
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return BlocBuilder<SettingsCubit, SettingsState>(
      builder: (context, state) {
        return Padding(
          padding: const EdgeInsets.all(16.0),
          child: ListView(
            children: [
              _buildSection(los.default_setting, [
                _buildSettingTile(
                  icon: Symbols.dark_mode,
                  title: los.theme,
                  subtitle: _exchangeTheme(state.theme),
                  onTap:
                      () async =>
                          await _showDialog(const ThemeOptionList(), los.theme),
                ),
                _buildSettingTile(
                  icon: Symbols.translate,
                  title: los.language,
                  subtitle: _exchangeLocale(state.locale),
                  onTap:
                      () async => await _showDialog(
                        const LocaleOptionList(),
                        los.language,
                      ),
                ),
                _buildSettingTile(
                  icon: Symbols.person,
                  title: los.author,
                  subtitle: los.author_of_this_locale,
                  onTap: () async {
                    await UIHelper.showCustomDialog(
                      context: context,
                      child: TranslatorPage(
                        translator: _exchangeTranslator(state.locale),
                      ),
                    );
                  },
                ),
              ]),
              const SizedBox(height: 16),
              _buildSection(los.application_setting, [
                _buildSettingTile(
                  icon: Symbols.notifications,
                  title: los.send_notification,
                  subtitle: state.sendNotification ? los.enable : los.disable,
                  onTap:
                      () async => await _showDialog(
                        const NotificationOptionList(),
                        los.send_notification,
                      ),
                ),
                _buildSettingTile(
                  icon: Symbols.storage,
                  title: los.storage_permission,
                  subtitle: _hasPermission ? los.granted : los.denied,
                  onTap:
                      !_hasPermission
                          ? AndroidHelper.requestStoragePermission
                          : null,
                ),
                _buildSettingTile(
                  icon: Symbols.build,
                  title: los.toolchain,
                  subtitle:
                      state.toolChain.isEmpty
                          ? los.not_specified
                          : state.toolChain,
                  onTap: !CurrentPlatform.isAndroid ? _onChangeToolChain : null,
                ),
              ]),
            ],
          ),
        );
      },
    );
  }

  void _onChangeToolChain() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder:
          (context) => MaterialDialog(
            title: Text(
              los.toolchain,
              style: Theme.of(
                context,
              ).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.w600),
            ),
            content: Row(
              children: [
                Expanded(
                  child: TextField(
                    minLines: 1,
                    maxLines: null,
                    keyboardType: TextInputType.multiline,
                    controller: _controller,
                    decoration: InputDecoration(
                      labelText: los.toolchain,
                      border: const OutlineInputBorder(),
                    ),
                  ),
                ),
                IconButton(
                  tooltip: los.upload_directory,
                  onPressed: () async {
                    Future<void> setDirectory() async {
                      await BlocProvider.of<SettingsCubit>(
                        context,
                      ).setToolChain(_controller.text);
                    }

                    final directory = await FileHelper.uploadDirectory();
                    if (directory == null) {
                      return;
                    }
                    _controller.text = directory;
                    await setDirectory();
                    await _onCheckValidator();
                  },
                  icon: const Icon(Symbols.drive_folder_upload),
                ),
              ],
            ),
            actions: [_toolChainValidator()],
          ),
    );
  }

  Widget _toolChainValidator() {
    final los = AppLocalizations.of(context)!;
    return TextButton(
      onPressed: () async {
        await BlocProvider.of<SettingsCubit>(
          context,
        ).setToolChain(_controller.text);
        await _onCheckValidator();
        _onClose();
      },
      child: Text(los.okay),
    );
  }

  void _onClose() {
    Navigator.of(context).pop();
  }

  Future<void> _onCheckValidator() async {
    if (BlocProvider.of<SettingsCubit>(context).state.toolChain.isNotEmpty) {
      final state =
          _existKernel(
            BlocProvider.of<SettingsCubit>(context).state.toolChain,
          ) &&
          _existScript(BlocProvider.of<SettingsCubit>(context).state.toolChain);
      await BlocProvider.of<SettingsCubit>(context).setIsValid(isValid: state);
    }
  }

  bool _existScript(String path) {
    return FileHelper.isFile('$path/Script/main.js');
  }

  bool _existKernel(String path) {
    if (CurrentPlatform.isAndroid) {
      return true;
    }
    if (CurrentPlatform.isWindows) {
      return FileHelper.isFile('$path/Kernel.dll');
    }
    if (CurrentPlatform.isLinux) {
      return FileHelper.isFile('$path/libKernel.so');
    }
    if (CurrentPlatform.isIOS || CurrentPlatform.isMacOS) {
      return FileHelper.isFile('$path/libKernel.dylib');
    }
    return false;
  }

  Widget _buildSection(String title, List<Widget> children) {
    return Card(
      elevation: 3,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 12.0, horizontal: 24.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          spacing: 4.0,
          children: [
            Text(
              title,
              style: Theme.of(context).textTheme.labelLarge?.copyWith(
                fontWeight: FontWeight.bold,
                color: Theme.of(context).colorScheme.primary,
              ),
            ),
            const Divider(),
            ...children,
          ],
        ),
      ),
    );
  }

  Widget _buildSettingTile({
    required IconData icon,
    required String title,
    required String subtitle,
    void Function()? onTap,
  }) {
    return ListTile(
      leading: Icon(
        icon,
        color: Theme.of(context).colorScheme.primary,
        size: 28,
      ),
      title: Text(title, style: Theme.of(context).textTheme.bodyLarge),
      subtitle: Text(
        subtitle,
        style: Theme.of(context).textTheme.bodyMedium?.copyWith(
          color:
              context.isDarkMode ? Colors.grey.shade400 : Colors.grey.shade600,
        ),
      ),
      trailing:
          onTap != null ? const Icon(Symbols.arrow_forward, size: 16) : null,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12.0)),
      onTap: onTap,
    );
  }

  Translator _exchangeTranslator(String name) {
    final translator = Translators.translators[Translators.languages[name]];
    if (translator == null) {
      throw Exception('Unknown translator of locale: $name');
    }
    return translator;
  }
}
