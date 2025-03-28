import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/load_script_bloc/load_script_bloc.dart';
import '../../bloc/settings_bloc/settings_bloc.dart';
import '../../extension/context.dart';
import 'js_card.dart';
import '../../i18n/app_localizations.dart';
import 'search_script.dart';
import '../../service/ui_helper.dart';
import '../../widget/hotkey.dart';

class JsExecute extends StatelessWidget {
  const JsExecute({super.key});

  void _showErrorDialog(BuildContext context, String message) async {
    final los = AppLocalizations.of(context)!;
    await UIHelper.showSimpleDialog(
      context: context,
      title: los.error,
      content: message,
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;

    return BlocProvider(
      create: (context) {
        return LoadScriptBloc(settingsBloc: context.read<SettingsBloc>())
          ..add(LoadScripts(localizations: los));
      },
      child: HotkeyBuilder(
        child: Scaffold(
          body: UIHelper.applyScrollView(
            builder: (context, innerBoxIsScrolled) {
              return UIHelper.buildAppBar(
                context,
                innerBoxIsScrolled,
                title: Text(
                  los.js_execute,
                  style: const TextStyle(fontWeight: FontWeight.bold),
                ),
              );
            },
            child: Padding(
              padding: const EdgeInsets.all(8.0),
              child: BlocConsumer<LoadScriptBloc, LoadScriptState>(
                listener: (context, state) {
                  if (state is LoadScriptFailed) {
                    _showErrorDialog(context, state.message);
                  }
                },
                builder: (context, state) {
                  if (state is LoadScriptLoaded) {
                    return CustomScriptList(data: state.filteredData);
                  } else if (state is LoadScriptFailed) {
                    return const CustomErrorPage();
                  }
                  return const Center(
                    child: CircularProgressIndicator.adaptive(),
                  );
                },
              ),
            ),
          ),
        ),
      ),
    );
  }
}

class CustomScriptList extends StatelessWidget {
  const CustomScriptList({super.key, required this.data});

  final List<Script> data;

  Future<void> _showContextMenu(
    TapDownDetails details,
    BuildContext context,
  ) async {
    final overlay = Overlay.of(context).context.findRenderObject() as RenderBox;
    await showMenu(
      context: context,
      position: RelativeRect.fromRect(
        details.globalPosition & const Size(40, 40),
        Offset.zero & overlay.size,
      ),
      items: [
        PopupMenuItem(
          onTap: () {
            BlocProvider.of<LoadScriptBloc>(
              context,
            ).add(ReloadScripts(localizations: AppLocalizations.of(context)!));
            ContextMenuController.removeAny();
          },
          child: Text(context.los.reload_script),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onSecondaryTapDown:
          (details) async => await _showContextMenu(details, context),
      child: Column(
        children: [
          const SearchScript(),
          Expanded(
            child: ListView.builder(
              itemCount: data.length,
              itemBuilder: (context, index) {
                return JsCard(
                  item: data[index],
                  toolChain: context.read<SettingsBloc>().state.toolChain,
                );
              },
            ),
          ),
        ],
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(IterableProperty<Script>('data', data));
  }
}

class CustomErrorPage extends StatelessWidget {
  const CustomErrorPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: TextButton(
        child: Padding(
          padding: const EdgeInsets.symmetric(vertical: 8.0, horizontal: 16.0),
          child: Text(context.los.reload_script),
        ),
        onPressed: () {
          BlocProvider.of<LoadScriptBloc>(
            context,
          ).add(ReloadScripts(localizations: AppLocalizations.of(context)!));
        },
      ),
    );
  }
}
