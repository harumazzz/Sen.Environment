import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/load_script_bloc/load_script_bloc.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
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

  Widget _buildErrorScreen(BuildContext context) {
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
    final los = AppLocalizations.of(context)!;

    return BlocProvider(
      create:
          (context) =>
              LoadScriptBloc(settingsCubit: context.read<SettingsCubit>())
                ..add(LoadScripts(localizations: los)),
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
                    return _buildScriptList(context, state.filteredData);
                  } else if (state is LoadScriptFailed) {
                    return _buildErrorScreen(context);
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

  Widget _buildScriptList(BuildContext context, List<Script> data) {
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
                  toolChain: context.read<SettingsCubit>().state.toolChain,
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}
