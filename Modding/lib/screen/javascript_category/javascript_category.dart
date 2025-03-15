import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/load_script_bloc/load_script_bloc.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';
import 'javascript_card.dart';
import '../../i18n/app_localizations.dart';
import 'search_script.dart';
import '../../service/ui_helper.dart';
import '../../widget/hotkey.dart';

class JavaScriptCategory extends StatelessWidget {
  const JavaScriptCategory({super.key});

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

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return BlocProvider<LoadScriptBloc>(
      create: (context) {
        final bloc = LoadScriptBloc(
          settingsCubit: BlocProvider.of<SettingsCubit>(context),
        );
        WidgetsBinding.instance.addPostFrameCallback((_) {
          bloc.add(LoadScripts(localizations: los));
        });
        return bloc;
      },
      child: HotkeyBuilder(
        child: Scaffold(
          appBar: UIHelper.appBarOr(AppBar(title: Text(los.js_execute))),
          body: Padding(
            padding: const EdgeInsets.all(8.0),
            child: BlocConsumer<LoadScriptBloc, LoadScriptState>(
              listener: (context, state) {
                if (state is LoadScriptFailed) {
                  _showErrorDialog(context, state.message);
                }
              },
              builder: (context, state) {
                if (state is LoadScriptLoaded) {
                  final data = state.filteredData;
                  return Column(
                    children: [
                      const SearchScript(),
                      Expanded(
                        child: ListView.builder(
                          itemCount: data.length,
                          itemBuilder: (context, index) {
                            final script = data[index];
                            return JavaScriptCard(
                              item: script,
                              toolChain:
                                  BlocProvider.of<SettingsCubit>(
                                    context,
                                  ).state.toolChain,
                            );
                          },
                        ),
                      ),
                    ],
                  );
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
    );
  }
}
