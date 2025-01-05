part of 'load_script_bloc.dart';

@immutable
sealed class LoadScriptEvent {}

class LoadScripts extends LoadScriptEvent {}

class ReloadScripts extends LoadScriptEvent {}
