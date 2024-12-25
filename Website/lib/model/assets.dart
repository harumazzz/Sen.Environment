import 'package:website/model/author.dart';

class Assets {
  String? url;
  int? id;
  String? nodeId;
  String? name;
  String? label;
  Author? uploader;
  String? contentType;
  String? state;
  int? size;
  int? downloadCount;
  String? createdAt;
  String? updatedAt;
  String? browserDownloadUrl;

  Assets({
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

  Assets.fromJson(Map<String, dynamic> json) {
    url = json['url'];
    id = json['id'];
    nodeId = json['node_id'];
    name = json['name'];
    label = json['label'];
    uploader = json['uploader'] != null ? Author.fromJson(json['uploader']) : null;
    contentType = json['content_type'];
    state = json['state'];
    size = json['size'];
    downloadCount = json['download_count'];
    createdAt = json['created_at'];
    updatedAt = json['updated_at'];
    browserDownloadUrl = json['browser_download_url'];
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = <String, dynamic>{};
    data['url'] = url;
    data['id'] = id;
    data['node_id'] = nodeId;
    data['name'] = name;
    data['label'] = label;
    if (uploader != null) {
      data['uploader'] = uploader!.toJson();
    }
    data['content_type'] = contentType;
    data['state'] = state;
    data['size'] = size;
    data['download_count'] = downloadCount;
    data['created_at'] = createdAt;
    data['updated_at'] = updatedAt;
    data['browser_download_url'] = browserDownloadUrl;
    return data;
  }
}
