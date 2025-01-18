part of 'resource_bloc.dart';

enum ResourceAnimationType { dangerNode, zombossNode, keyGate }

enum ResourceImageType { pinataSpine, pinataSpineOpen, dangerLevel }

enum ResourceStateStatus { loading, finished }

final class ResourceState extends Equatable {
  ResourceState({this.status = ResourceStateStatus.finished})
      : islandImage = HashMap(),
        islandAnimation = HashMap(),
        //  eventImage = HashMap(),
        //  eventAnimation = HashMap(),
        eventShop = {},
        eventNodeName = HashMap(),
        rasterizedInAnimation = HashMap(),
        resourceImage = HashMap(),
        resourceAnimation = HashMap();

  final ResourceStateStatus status;

  final HashMap<int, VisualImage?> islandImage;

  final HashMap<int, VisualAnimation?> islandAnimation;

  // final HashMap<EventNodeType, VisualImage> eventImage;

//  final HashMap<EventNodeType, VisualAnimation> eventAnimation;

  final Map<EventNodeType, Widget> eventShop;

  final HashMap<EventNodeType, String> eventNodeName;

  final HashMap<int, bool> rasterizedInAnimation;

  final HashMap<ResourceImageType, VisualImage?> resourceImage;

  final HashMap<ResourceAnimationType, VisualAnimation?> resourceAnimation;

  @override
  List<Object?> get props => [
        islandImage,
        islandAnimation,
        rasterizedInAnimation,
        resourceImage,
        resourceAnimation
      ];
}
