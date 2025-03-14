part of 'load_script_bloc.dart';

@immutable
sealed class LoadScriptEvent extends Equatable {
  const LoadScriptEvent({required this.localizations});
  final AppLocalizations localizations;

  @override
  List<Object?> get props => [localizations];
}

class LoadScripts extends LoadScriptEvent {
  const LoadScripts({required super.localizations});
}

class ReloadScripts extends LoadScriptEvent {
  const ReloadScripts({required super.localizations});
}

class SearchScripts extends LoadScriptEvent {
  const SearchScripts({required this.query, required super.localizations});
  final String query;

  @override
  List<Object?> get props => [localizations, query];
}
