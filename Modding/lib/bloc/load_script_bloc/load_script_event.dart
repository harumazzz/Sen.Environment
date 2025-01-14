part of 'load_script_bloc.dart';

@immutable
sealed class LoadScriptEvent extends Equatable {
  final AppLocalizations localizations;
  const LoadScriptEvent({
    required this.localizations,
  });

  @override
  List<Object?> get props => [localizations];
}

class LoadScripts extends LoadScriptEvent {
  const LoadScripts({required super.localizations});
}

class ReloadScripts extends LoadScriptEvent {
  const ReloadScripts({required super.localizations});
}
