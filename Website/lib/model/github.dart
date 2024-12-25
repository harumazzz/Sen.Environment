import 'package:website/model/assets.dart';
import 'package:website/model/author.dart';

class GitHub {
  String? url;
  String? assetsUrl;
  String? uploadUrl;
  String? htmlUrl;
  int? id;
  Author? author;
  String? nodeId;
  String? tagName;
  String? targetCommitish;
  String? name;
  bool? draft;
  bool? prerelease;
  String? createdAt;
  String? publishedAt;
  List<Assets>? assets;
  String? tarballUrl;
  String? zipballUrl;
  String? body;

  GitHub({
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

  GitHub.fromJson(Map<String, dynamic> json) {
    url = json['url'];
    assetsUrl = json['assets_url'];
    uploadUrl = json['upload_url'];
    htmlUrl = json['html_url'];
    id = json['id'];
    author = json['author'] != null ? Author.fromJson(json['author']) : null;
    nodeId = json['node_id'];
    tagName = json['tag_name'];
    targetCommitish = json['target_commitish'];
    name = json['name'];
    draft = json['draft'];
    prerelease = json['prerelease'];
    createdAt = json['created_at'];
    publishedAt = json['published_at'];
    if (json['assets'] != null) {
      assets = <Assets>[];
      json['assets'].forEach((v) {
        assets!.add(Assets.fromJson(v));
      });
    }
    tarballUrl = json['tarball_url'];
    zipballUrl = json['zipball_url'];
    body = json['body'];
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = <String, dynamic>{};
    data['url'] = url;
    data['assets_url'] = assetsUrl;
    data['upload_url'] = uploadUrl;
    data['html_url'] = htmlUrl;
    data['id'] = id;
    if (author != null) {
      data['author'] = author!.toJson();
    }
    data['node_id'] = nodeId;
    data['tag_name'] = tagName;
    data['target_commitish'] = targetCommitish;
    data['name'] = name;
    data['draft'] = draft;
    data['prerelease'] = prerelease;
    data['created_at'] = createdAt;
    data['published_at'] = publishedAt;
    if (assets != null) {
      data['assets'] = assets!.map((v) => v.toJson()).toList();
    }
    data['tarball_url'] = tarballUrl;
    data['zipball_url'] = zipballUrl;
    data['body'] = body;
    return data;
  }
}
