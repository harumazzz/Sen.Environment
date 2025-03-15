import 'package:equatable/equatable.dart';
import 'assets.dart';
import 'author.dart';

class GitHub extends Equatable {
  const GitHub({
    this.url,
    this.assetsUrl,
    this.uploadUrl,
    this.htmlUrl,
    this.id,
    this.author,
    this.nodeId,
    this.tagName,
    this.targetCommitish,
    this.name,
    this.draft,
    this.prerelease,
    this.createdAt,
    this.publishedAt,
    this.assets,
    this.tarballUrl,
    this.zipballUrl,
    this.body,
  });

  factory GitHub.fromJson(Map<String, dynamic> json) {
    return GitHub(
      url: json['url'],
      assetsUrl: json['assets_url'],
      uploadUrl: json['upload_url'],
      htmlUrl: json['html_url'],
      id: json['id'],
      author: json['author'] != null ? Author.fromJson(json['author']) : null,
      nodeId: json['node_id'],
      tagName: json['tag_name'],
      targetCommitish: json['target_commitish'],
      name: json['name'],
      draft: json['draft'],
      prerelease: json['prerelease'],
      createdAt: json['created_at'],
      publishedAt: json['published_at'],
      assets:
          json['assets'] != null
              ? (json['assets'] as List).map((v) => Assets.fromJson(v)).toList()
              : null,
      tarballUrl: json['tarball_url'],
      zipballUrl: json['zipball_url'],
      body: json['body'],
    );
  }
  final String? url;
  final String? assetsUrl;
  final String? uploadUrl;
  final String? htmlUrl;
  final int? id;
  final Author? author;
  final String? nodeId;
  final String? tagName;
  final String? targetCommitish;
  final String? name;
  final bool? draft;
  final bool? prerelease;
  final String? createdAt;
  final String? publishedAt;
  final List<Assets>? assets;
  final String? tarballUrl;
  final String? zipballUrl;
  final String? body;

  @override
  List<Object?> get props => [
    url,
    assetsUrl,
    uploadUrl,
    htmlUrl,
    id,
    author,
    nodeId,
    tagName,
    targetCommitish,
    name,
    draft,
    prerelease,
    createdAt,
    publishedAt,
    assets,
    tarballUrl,
    zipballUrl,
    body,
  ];
}
