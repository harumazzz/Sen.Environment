import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/load_script_bloc/load_script_bloc.dart';
import '../../extension/context.dart';

class SearchScript extends StatefulWidget {
  const SearchScript({super.key});

  @override
  State<SearchScript> createState() => _SearchScriptState();
}

class _SearchScriptState extends State<SearchScript> {
  late TextEditingController _searchController;

  @override
  void initState() {
    _searchController = TextEditingController();
    super.initState();
    _searchController.addListener(() {
      BlocProvider.of<LoadScriptBloc>(context).add(
        SearchScripts(
          query: _searchController.text,
          localizations: context.los,
        ),
      );
    });
  }

  @override
  void dispose() {
    _searchController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: TextField(
        minLines: 1,
        maxLines: null,
        keyboardType: TextInputType.multiline,
        controller: _searchController,
        decoration: InputDecoration(
          labelText: '${context.los.input_value}...',
          prefixIcon: const Icon(Symbols.search),
          border: const OutlineInputBorder(),
        ),
      ),
    );
  }
}
