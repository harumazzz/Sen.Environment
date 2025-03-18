import 'package:equatable/equatable.dart';

class Extension extends Equatable {
  const Extension({
    required this.extensionId,
    required this.extensionTitle,
    required this.extensionDescription,
    required this.downloadLink,
    required this.extensionAuthor,
  });
  factory Extension.fromJson(Map<String, dynamic> map) {
    return Extension(
      extensionId: map['extension_id'] as int,
      extensionTitle: map['extension_title'] as String,
      extensionDescription: map['extension_description'] as String,
      downloadLink: map['download_link'] as String,
      extensionAuthor: map['extension_author'] as String,
    );
  }

  final int extensionId;
  final String extensionTitle;
  final String extensionDescription;
  final String extensionAuthor;
  final String downloadLink;

  @override
  List<Object> get props => [
    extensionId,
    extensionTitle,
    extensionDescription,
    extensionAuthor,
    downloadLink,
  ];

  Map<String, dynamic> toJson() {
    return <String, dynamic>{
      'extensionId': extensionId,
      'extensionTitle': extensionTitle,
      'extensionDescription': extensionDescription,
      'extensionAuthor': extensionAuthor,
      'downloadLink': downloadLink,
    };
  }
}
