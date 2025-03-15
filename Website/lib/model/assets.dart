import 'package:equatable/equatable.dart';
import 'author.dart';

class Assets extends Equatable {
  const Assets({
    this.url,
    this.id,
    this.nodeId,
    this.name,
    this.label,
    this.uploader,
    this.contentType,
    this.state,
    this.size,
    this.downloadCount,
    this.createdAt,
    this.updatedAt,
    this.browserDownloadUrl,
  });

  factory Assets.fromJson(Map<String, dynamic> json) {
    return Assets(
      url: json['url'],
      id: json['id'],
      nodeId: json['node_id'],
      name: json['name'],
      label: json['label'],
      uploader:
          json['uploader'] != null ? Author.fromJson(json['uploader']) : null,
      contentType: json['content_type'],
      state: json['state'],
      size: json['size'],
      downloadCount: json['download_count'],
      createdAt: json['created_at'],
      updatedAt: json['updated_at'],
      browserDownloadUrl: json['browser_download_url'],
    );
  }
  final String? url;
  final int? id;
  final String? nodeId;
  final String? name;
  final String? label;
  final Author? uploader;
  final String? contentType;
  final String? state;
  final int? size;
  final int? downloadCount;
  final String? createdAt;
  final String? updatedAt;
  final String? browserDownloadUrl;

  @override
  List<Object?> get props => [
    url,
    id,
    nodeId,
    name,
    label,
    uploader,
    contentType,
    state,
    size,
    downloadCount,
    createdAt,
    updatedAt,
    browserDownloadUrl,
  ];
}
