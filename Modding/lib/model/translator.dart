import 'package:equatable/equatable.dart';

class Translator extends Equatable {
  final String name;
  final String? discord;
  final String imageCover;
  final List<List<String>> contacts;

  const Translator({
    required this.name,
    required this.discord,
    required this.imageCover,
    required this.contacts,
  });

  @override
  List<Object?> get props => [name, discord, imageCover, contacts];
}
