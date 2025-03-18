part of 'toolbar_bloc.dart';

enum ToolType {
  openFile,
  saveFile,
  rectangleTool,
  eraseTool,
  panTool,
  resizeTool,
  clearEditor,
  configEditor,
  shortcutMenu,
}

final class ToolBarState extends Equatable {
  const ToolBarState({required this.toolStatus});
  factory ToolBarState.initailize() {
    return ToolBarState(
      toolStatus: HashMap.of({
        ToolType.openFile: false,
        ToolType.saveFile: false,
        ToolType.rectangleTool: false,
        ToolType.eraseTool: false,
        ToolType.panTool: false,
        ToolType.resizeTool: false,
        ToolType.clearEditor: false,
        ToolType.configEditor: false,
        ToolType.shortcutMenu: false,
      }),
    );
  }

  final HashMap<ToolType, bool> toolStatus;

  ToolBarState copyWith({HashMap<ToolType, bool>? toolStatus}) {
    final toolBarState = ToolBarState.initailize();
    final data = toolStatus ?? this.toolStatus;
    for (final e in data.entries) {
      toolBarState.toolStatus[e.key] = e.value;
    }
    return toolBarState;
  }

  @override
  List<Object?> get props => [toolStatus];
}
