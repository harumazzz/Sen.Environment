import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/bloc/load_script_bloc/load_script_bloc.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/screen/javascript_category/javascript_card.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class JavaScriptCategory extends StatelessWidget {
  const JavaScriptCategory({
    super.key,
  });

  void _showErrorDialog(
    BuildContext context,
    String message,
  ) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.error),
        content: Text(message),
        actions: [
          TextButton(
            child: Text(los.ok),
            onPressed: () {
              Navigator.of(context).pop();
            },
          )
        ],
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
        bloc.add(LoadScripts());
        return bloc;
      },
      child: Scaffold(
        appBar: AppBar(
          title: Text(los.js_page),
        ),
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
                final data = state.data;
                return ListView.builder(
                  itemCount: data.length,
                  itemBuilder: (context, index) {
                    final script = data[index];
                    return JavaScriptCard(
                      item: script,
                      toolChain: BlocProvider.of<SettingsCubit>(context).state.toolChain,
                    );
                  },
                );
              } else if (state is LoadScriptFailed) {
                return Center(
                  child: TextButton(
                    child: Padding(
                      padding: const EdgeInsets.symmetric(
                        vertical: 8.0,
                        horizontal: 16.0,
                      ),
                      child: Text(los.reload_script),
                    ),
                    onPressed: () {
                      BlocProvider.of<LoadScriptBloc>(context).add(ReloadScripts());
                    },
                  ),
                );
              }
              return const Center(
                child: CircularProgressIndicator.adaptive(),
              );
            },
          ),
        ),
      ),
    );
  }
}
