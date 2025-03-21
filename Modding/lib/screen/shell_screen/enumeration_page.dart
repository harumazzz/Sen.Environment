import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../extension/context.dart';
import '../../model/select_option.dart';

class EnumerationPage extends StatefulWidget {
  const EnumerationPage({
    super.key,
    required this.options,
    required this.title,
    required this.onSelected,
  });

  final String title;
  final List<SelectOption>? options;
  final void Function(SelectOption selectedOption) onSelected;

  @override
  State<EnumerationPage> createState() => _EnumerationPageState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('title', title));
    properties.add(IterableProperty<SelectOption>('options', options));
    properties.add(
      ObjectFlagProperty<Function(SelectOption p1)>.has(
        'onSelected',
        onSelected,
      ),
    );
  }
}

class _EnumerationPageState extends State<EnumerationPage> {
  late List<SelectOption> _filteredOptions;

  @override
  void initState() {
    super.initState();
    _filteredOptions = widget.options ?? [];
  }

  void _filterOptions(String query) {
    setState(() {
      _filteredOptions = [
        ...(widget.options ?? []).where(
          (e) => e.regularOption.toLowerCase().contains(query.toLowerCase()),
        ),
      ];
    });
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(16.0),
      child: Column(
        mainAxisSize: MainAxisSize.min,
        crossAxisAlignment: CrossAxisAlignment.start,
        spacing: 12.0,
        children: [
          Text(
            widget.title,
            style: Theme.of(
              context,
            ).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.bold),
          ),
          _SearchEnumeration(onSearch: _filterOptions),
          Expanded(
            child: AnimatedSwitcher(
              duration: const Duration(milliseconds: 300),
              child:
                  _filteredOptions.isEmpty
                      ? _buildErrorPage()
                      : _buildOptionList(),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildOptionList() {
    return ListView.separated(
      itemCount: _filteredOptions.length,
      separatorBuilder: (_, _) => const Divider(height: 4.0),
      itemBuilder: (context, index) {
        final option = _filteredOptions[index];
        return _OptionTile(
          option: option,
          onTap: () {
            widget.onSelected(option);
            Navigator.of(context).pop();
          },
        );
      },
    );
  }

  Widget _buildErrorPage() {
    return Center(
      child: Text(
        context.los.script_not_found,
        style: Theme.of(context).textTheme.titleMedium,
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      IterableProperty<SelectOption>('filteredOptions', _filteredOptions),
    );
  }
}

class _SearchEnumeration extends StatefulWidget {
  const _SearchEnumeration({required this.onSearch});

  final Function(String value) onSearch;

  @override
  State<_SearchEnumeration> createState() => __SearchEnumerationState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<Function(String value)>.has('onSearch', onSearch),
    );
  }
}

class __SearchEnumerationState extends State<_SearchEnumeration> {
  late final TextEditingController _searchController;
  late final FocusNode _focusNode;

  @override
  void initState() {
    super.initState();
    _searchController = TextEditingController();
    _focusNode = FocusNode();
  }

  @override
  void dispose() {
    _searchController.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return SearchBar(
      controller: _searchController,
      focusNode: _focusNode,
      hintText: context.los.input_value,
      padding: const WidgetStatePropertyAll(
        EdgeInsets.symmetric(horizontal: 16.0),
      ),
      surfaceTintColor: WidgetStateProperty.all(
        Theme.of(context).colorScheme.secondary,
      ),
      leading: const Icon(Symbols.search),
      trailing: [
        if (_searchController.text.isNotEmpty)
          IconButton(
            icon: const Icon(Symbols.close),
            onPressed: () {
              _searchController.clear();
              widget.onSearch('');
              _focusNode.unfocus();
              setState(() {});
            },
          ),
      ],
      onChanged: (value) {
        widget.onSearch(value);
        setState(() {});
      },
    );
  }
}

class _OptionTile extends StatelessWidget {
  const _OptionTile({required this.option, required this.onTap});

  final SelectOption option;

  final void Function() onTap;

  @override
  Widget build(BuildContext context) {
    return ListTile(
      contentPadding: const EdgeInsets.symmetric(
        horizontal: 12.0,
        vertical: 4.0,
      ),
      tileColor: Theme.of(context).colorScheme.surface,
      onTap: onTap,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      leading: CircleAvatar(
        backgroundColor: Theme.of(context).colorScheme.primaryContainer,
        child: Text(
          option.option,
          style: Theme.of(context).textTheme.labelLarge?.copyWith(
            fontWeight: FontWeight.bold,
            color: Theme.of(context).colorScheme.onPrimaryContainer,
          ),
        ),
      ),
      title: Text(
        option.title,
        style: Theme.of(context).textTheme.labelLarge?.copyWith(
          color: Theme.of(context).colorScheme.onPrimaryContainer,
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<SelectOption>('option', option));
    properties.add(ObjectFlagProperty<void Function()>.has('onTap', onTap));
  }
}
