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
  late bool _isFocused;
  late FocusNode _focusNode;

  @override
  void initState() {
    _searchController = TextEditingController();
    _focusNode = FocusNode();
    _isFocused = false;
    super.initState();
    _searchController.addListener(() {
      BlocProvider.of<LoadScriptBloc>(context).add(
        SearchScripts(
          query: _searchController.text,
          localizations: context.los,
        ),
      );
      if (_searchController.text.isNotEmpty) {
        _isFocused = true;
        setState(() {});
      }
    });
  }

  @override
  void dispose() {
    _searchController.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Expanded(
          child: Padding(
            padding: const EdgeInsets.symmetric(horizontal: 4.0),
            child: SearchBar(
              focusNode: _focusNode,
              controller: _searchController,
              leading: const Padding(
                padding: EdgeInsets.only(left: 8.0),
                child: Icon(Symbols.search),
              ),
              hintText: 'Search for scripts...',
              onTap: () => setState(() => _isFocused = true),
              onChanged: (value) {
                BlocProvider.of<LoadScriptBloc>(
                  context,
                ).add(SearchScripts(query: value, localizations: context.los));
              },
              trailing: [
                if (_isFocused)
                  IconButton(
                    icon: const Icon(Symbols.close),
                    onPressed: () {
                      _searchController.clear();
                      _focusNode.unfocus();
                      setState(() => _isFocused = false);
                    },
                  ),
              ],
            ),
          ),
        ),
      ],
    );
  }
}
