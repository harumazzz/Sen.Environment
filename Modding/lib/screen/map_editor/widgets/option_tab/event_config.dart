import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../../extension/platform.dart';
import '../../../../model/worldmap.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../../../extension/context.dart';
import '../../bloc/canvas/canvas_bloc.dart';
import '../../bloc/history/history_bloc.dart';
import '../../bloc/item/item_bloc.dart';
import '../../bloc/setting/setting_bloc.dart';
import '../../bloc/stage/stage_bloc.dart';
import '../../bloc/suggestion/suggestion_bloc.dart';
import '../../include/check_box_field.dart';
import '../../include/downdrop_search_field.dart';
import '../../include/dropdown_button_field.dart';
import '../../include/number_text_field.dart';
import '../../include/readonly_box_field.dart';
import '../../include/text_string_field.dart';
import '../../models/action_service.dart';
import '../../models/game_resource.dart';
import '../../models/map_const.dart';
import '../../models/action_model.dart';

class EventConfigTab extends StatelessWidget {
  const EventConfigTab({super.key, required this.id});

  final String id;

  @override
  Widget build(BuildContext context) {
    final stageBloc = context.read<StageBloc>();
    final event = stageBloc.state.events[id]!;
    var levelNodeType = event.levelNodeType;
    if (event.dataString != null && event.dataString!.contains('danger')) {
      levelNodeType = LevelNodeType.danger;
    }
    final itemBloc = context.read<ItemBloc>();
    final historyBloc = context.read<HistoryBloc>();
    void captureEvent(ActionType actionType) {
      final actionService = ActionService<ActionModelType>(
        actionType: actionType,
        data: {ActionModelType.oneEvent: event.clone(), ActionModelType.id: id},
        change: (data) {
          final eventId = data![ActionModelType.id];
          stageBloc.state.events[eventId] =
              (data[ActionModelType.oneEvent] as MapEventItem).clone();
          itemBloc.add(const ItemStoreUpdated());
          historyBloc.add(const UpdateIndexEvent());
        },
      );
      historyBloc.add(CaptureState(state: actionService));
    }

    final los = context.los;
    final gameResource =
        context.read<MapEditorConfigurationCubit>().state.gameResource;
    final controller = context.read<CanvasBloc>().state.canvasController;
    final textColor = Theme.of(context).colorScheme.inverseSurface;
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final spacing = isDesktopPlatform ? 0.0 : 30.0;
    const edgeSpacing = EdgeInsets.fromLTRB(12, 12, 20, 12);
    return Padding(
      padding: EdgeInsets.symmetric(horizontal: spacing),
      child: Column(
        children: [
          Card(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(8),
            ),
            color: Theme.of(context).colorScheme.secondaryContainer,
            margin: const EdgeInsets.all(8),
            child: SizedBox(
              height: 50,
              width: double.infinity,
              child: Center(
                child: Text(
                  los.event_config,
                  style: const TextStyle(fontWeight: FontWeight.bold),
                ),
              ),
            ),
          ),
          Expanded(
            child: Padding(
              padding: const EdgeInsets.only(
                top: 4,
                left: 8,
                right: 8,
                bottom: 16,
              ),
              child: SingleChildScrollView(
                child: Column(
                  children: [
                    Row(
                      children: [
                        Expanded(
                          flex: 3,
                          child: Padding(
                            padding: edgeSpacing,
                            child: ReadonlyBoxField(
                              label: los.type,
                              value: event.eventType.name,
                            ),
                          ),
                        ),
                        Expanded(
                          flex: 2,
                          child: Padding(
                            padding: edgeSpacing,
                            child: BlocBuilder<ItemBloc, ItemState>(
                              buildWhen:
                                  (prev, state) =>
                                      prev.itemStore != state.itemStore,
                              builder: (context, state) {
                                return NumberTextField(
                                  label: los.id,
                                  controller: NumberEditingController(
                                    value: event.eventID,
                                    isDouble: false,
                                  ),
                                  range: const Range(
                                    begin: 0,
                                    end: MapConst.intMaxValue,
                                  ),
                                  useChangeButton: true,
                                  onFieldSubmitted: (value) {
                                    event.eventID = value as int;
                                    captureEvent(ActionType.eventChangeID);
                                    itemBloc.add(const ItemStoreUpdated());
                                    controller.requestFocus();
                                  },
                                  autovalidateMode: AutovalidateMode.always,
                                  validator: (value) {
                                    final events =
                                        context.read<StageBloc>().state.events;
                                    var counter = 0;
                                    events.forEach((_, eventValue) {
                                      if (eventValue.eventID == event.eventID) {
                                        counter++;
                                      }
                                      if (counter > 1) {
                                        return;
                                      }
                                    });
                                    return counter > 1 ? 'duplicated' : null;
                                  },
                                );
                              },
                            ),
                          ),
                        ),
                      ],
                    ),
                    BlocBuilder<ItemBloc, ItemState>(
                      buildWhen:
                          (prev, state) => prev.itemStore != state.itemStore,
                      builder: (context, state) {
                        return Row(
                          children: [
                            Expanded(
                              child: Padding(
                                padding: edgeSpacing,
                                child: NumberTextField(
                                  label: los.position_x,
                                  controller: NumberEditingController(
                                    value: event.position.x,
                                    isDouble: true,
                                  ),
                                  range: const Range(
                                    begin: MapConst.doubleMinValue,
                                    end: MapConst.doubleMaxValue,
                                  ),
                                  useChangeButton: true,
                                  onFieldSubmitted: (value) {
                                    event.position.x = value as double;
                                    captureEvent(ActionType.moveItem);
                                    itemBloc.add(const ItemStoreUpdated());
                                    controller.requestFocus();
                                  },
                                ),
                              ),
                            ),
                            Expanded(
                              child: Padding(
                                padding: edgeSpacing,
                                child: NumberTextField(
                                  label: los.position_y,
                                  controller: NumberEditingController(
                                    value: event.position.y,
                                    isDouble: true,
                                  ),
                                  range: const Range(
                                    begin: MapConst.doubleMinValue,
                                    end: MapConst.doubleMaxValue,
                                  ),
                                  useChangeButton: true,
                                  onFieldSubmitted: (value) {
                                    event.position.y = value as double;
                                    captureEvent(ActionType.moveItem);
                                    itemBloc.add(const ItemStoreUpdated());
                                    controller.requestFocus();
                                  },
                                ),
                              ),
                            ),
                          ],
                        );
                      },
                    ),
                    if (event.eventType != EventType.doodad)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: BlocBuilder<ItemBloc, ItemState>(
                            buildWhen:
                                (prev, state) =>
                                    prev.itemStore != state.itemStore,
                            builder: (context, state) {
                              return TextStringField(
                                label: los.name,
                                value: event.name,
                                onFieldSubmitted: (value) {
                                  event.name = value;
                                  captureEvent(ActionType.eventChangeName);
                                  itemBloc.add(const ItemStoreUpdated());
                                  controller.requestFocus();
                                },
                                autovalidateMode: AutovalidateMode.always,
                                validator: (value) {
                                  if (value != event.name) {
                                    return los.press_enter_to_save;
                                  }
                                  return null;
                                },
                              );
                            },
                          ),
                        ),
                      ),
                    if (event.eventType != EventType.doodad)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: los.parent,
                            value: event.parentEvent,
                            suggestionsCallback: (pattern) {
                              final eventNameList =
                                  context.read<StageBloc>().getLevelName();
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(
                                    eventNameList,
                                    pattern,
                                    expect: event.name,
                                  );
                            },
                            itemBuilder: (context, String suggestion) {
                              return ListTile(
                                title: Text(
                                  suggestion,
                                  style: TextStyle(color: textColor),
                                ),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.parentEvent = suggestion;
                              captureEvent(ActionType.eventChangeParent);
                              controller.requestFocus();
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              final eventNameList =
                                  context.read<StageBloc>().getLevelName();
                              if (value == '') {
                                event.parentEvent = '';
                                return null;
                              }
                              if (eventNameList.contains(value)) {
                                return null;
                              } else {
                                return los.parent_must_have_the_same_name;
                              }
                            },
                          ),
                        ),
                      ),
                    if (event.eventType != EventType.doodad)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: los.unlocked_from,
                            value: event.unlockedFrom,
                            suggestionsCallback: (pattern) {
                              final eventNameList =
                                  context.read<StageBloc>().getLevelName();
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(
                                    eventNameList,
                                    pattern,
                                    expect: event.name,
                                  );
                            },
                            itemBuilder: (context, String suggestion) {
                              return ListTile(
                                title: Text(
                                  suggestion,
                                  style: TextStyle(color: textColor),
                                ),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.unlockedFrom = suggestion;
                              captureEvent(ActionType.eventChangeUnlockedFrom);
                              controller.requestFocus();
                              // MapManager.mapUpdate();
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              if (value == '') {
                                event.unlockedFrom = '';
                                return null;
                              }
                              final eventNameList =
                                  context.read<StageBloc>().getLevelName();
                              if (eventNameList.contains(value)) {
                                return null;
                              } else {
                                return 'Unlocked must same name';
                              }
                            },
                          ),
                        ),
                      ),
                    if (event.eventType != EventType.doodad)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: los.visible_from,
                            value: event.visibleFrom,
                            suggestionsCallback: (pattern) {
                              final eventNameList =
                                  context.read<StageBloc>().getLevelName();
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(
                                    eventNameList,
                                    pattern,
                                    expect: event.name,
                                  );
                            },
                            itemBuilder: (context, String suggestion) {
                              return ListTile(
                                title: Text(
                                  suggestion,
                                  style: TextStyle(color: textColor),
                                ),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.visibleFrom = suggestion;
                              captureEvent(ActionType.eventChangeVisibleFrom);
                              controller.requestFocus();
                              // MapManager.mapUpdate();
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              final eventNameList =
                                  context.read<StageBloc>().getLevelName();
                              if (value == '') {
                                event.visibleFrom = '';
                                return null;
                              }
                              if (eventNameList.contains(value)) {
                                return null;
                              } else {
                                return los.visible_from_must_have_same_name;
                              }
                            },
                          ),
                        ),
                      ),
                    if (event.eventType != EventType.doodad)
                      BlocBuilder<ItemBloc, ItemState>(
                        buildWhen:
                            (prev, state) => prev.itemStore != state.itemStore,
                        builder: (context, state) {
                          return CheckBoxField(
                            label: los.auto_visible,
                            value: event.autoVisible,
                            height: 40,
                            onChanged: ({required bool? value}) {
                              event.autoVisible = value ?? false;
                              captureEvent(ActionType.eventChangeAutoVisible);
                              itemBloc.add(const ItemStoreUpdated());
                              controller.requestFocus();
                            },
                          );
                        },
                      ),
                    if (event.eventType == EventType.level)
                      Padding(
                        padding: edgeSpacing,
                        child: ReadonlyBoxField(
                          label: los.level_type,
                          value: levelNodeType!.name,
                        ),
                      ),
                    if (event.eventType == EventType.level)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: BlocBuilder<ItemBloc, ItemState>(
                            buildWhen:
                                (prev, state) =>
                                    prev.itemStore != state.itemStore,
                            builder: (context, state) {
                              return TextStringField(
                                label: los.level_data,
                                value: event.dataString,
                                onFieldSubmitted: (value) {
                                  event.dataString = value;
                                  captureEvent(ActionType.eventChangeLevelData);
                                  itemBloc.add(const ItemStoreUpdated());
                                  controller.requestFocus();
                                },
                                autovalidateMode: AutovalidateMode.always,
                                validator: (value) {
                                  if (value != '' &&
                                      value != event.dataString) {
                                    return los.press_enter_to_save;
                                  }
                                  return null;
                                },
                              );
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.level)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: BlocBuilder<ItemBloc, ItemState>(
                            buildWhen:
                                (prev, state) =>
                                    prev.itemStore != state.itemStore,
                            builder: (context, state) {
                              return TextStringField(
                                label: los.display_text,
                                value: event.displayText,
                                onFieldSubmitted: (value) {
                                  event.displayText = value;
                                  captureEvent(
                                    ActionType.eventChangeDisplayText,
                                  );
                                  itemBloc.add(const ItemStoreUpdated());
                                  controller.requestFocus();
                                },
                                autovalidateMode: AutovalidateMode.always,
                                validator: (value) {
                                  if (value != '' &&
                                      value != event.displayText) {
                                    return los.press_enter_to_save;
                                  }
                                  return null;
                                },
                              );
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.level)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: los.narration_unlocked,
                            value: event.unlockedNarrationID ?? '',
                            suggestionsCallback: (pattern) {
                              final narrationList =
                                  context
                                      .read<SuggestionBloc>()
                                      .state
                                      .narrationList;
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(narrationList, pattern);
                            },
                            itemBuilder: (context, String suggestion) {
                              return ListTile(
                                title: Text(
                                  suggestion,
                                  style: TextStyle(color: textColor),
                                ),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.unlockedNarrationID = suggestion;
                              captureEvent(ActionType.eventChangeNarUnlocked);
                              controller.requestFocus();
                              // MapManager.mapUpdate();
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              event.unlockedNarrationID = value;
                              final narrationList =
                                  context
                                      .read<SuggestionBloc>()
                                      .state
                                      .narrationList;
                              if (event.unlockedNarrationID == null ||
                                  event.unlockedNarrationID == '' ||
                                  narrationList.contains(
                                    event.unlockedNarrationID,
                                  )) {
                                return null;
                              } else {
                                return '';
                              }
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.level)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: los.narration_completed,
                            value: event.completedNarrationID ?? '',
                            suggestionsCallback: (pattern) {
                              final narrationList =
                                  context
                                      .read<SuggestionBloc>()
                                      .state
                                      .narrationList;
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(narrationList, pattern);
                            },
                            itemBuilder: (context, String suggestion) {
                              return ListTile(
                                title: Text(
                                  suggestion,
                                  style: TextStyle(color: textColor),
                                ),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.completedNarrationID = suggestion;
                              captureEvent(ActionType.eventChangeNarCompleted);
                              controller.requestFocus();
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              event.completedNarrationID = value;
                              final narrationList =
                                  context
                                      .read<SuggestionBloc>()
                                      .state
                                      .narrationList;
                              if (event.completedNarrationID == null ||
                                  event.completedNarrationID == '' ||
                                  narrationList.contains(
                                    event.completedNarrationID,
                                  )) {
                                return null;
                              } else {
                                return '';
                              }
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.level)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DropdownButtonField<WorldMapEventTutorial>(
                            label: context.los.level_tutorial,
                            value:
                                event.worldMapTutorial ??
                                WorldMapEventTutorial.none,
                            items:
                                WorldMapEventTutorial.values
                                    .map(
                                      (e) => DropdownMenuItem<
                                        WorldMapEventTutorial
                                      >(
                                        value: e,
                                        child: Text(
                                          e.name,
                                          maxLines: 1,
                                          overflow: TextOverflow.ellipsis,
                                          softWrap: false,
                                          style: Theme.of(
                                            context,
                                          ).textTheme.bodyLarge?.copyWith(
                                            color:
                                                Theme.of(
                                                  context,
                                                ).colorScheme.onSurface,
                                          ),
                                        ),
                                      ),
                                    )
                                    .toList(),
                            onChanged: (value) {
                              event.worldMapTutorial = value;
                              captureEvent(ActionType.eventChangeTutorial);
                              controller.requestFocus();
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.level)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DropdownButtonField<WorldMapEventStatus>(
                            label: los.level_tutorial_when,
                            value:
                                event.worldMapTutorialVisibleWhen ??
                                WorldMapEventStatus.none,
                            items:
                                WorldMapEventStatus.values
                                    .map(
                                      (e) =>
                                          DropdownMenuItem<WorldMapEventStatus>(
                                            value: e,
                                            child: Text(
                                              e.name,
                                              maxLines: 1,
                                              overflow: TextOverflow.ellipsis,
                                              softWrap: false,
                                              style: Theme.of(
                                                context,
                                              ).textTheme.bodyLarge?.copyWith(
                                                color:
                                                    Theme.of(
                                                      context,
                                                    ).colorScheme.onSurface,
                                              ),
                                            ),
                                          ),
                                    )
                                    .toList(),
                            onChanged: (value) {
                              event.worldMapTutorialVisibleWhen = value;
                              captureEvent(
                                ActionType.eventChangeTutorialUnlocked,
                              );
                              controller.requestFocus();
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.level)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: BlocBuilder<ItemBloc, ItemState>(
                            buildWhen:
                                (prev, state) =>
                                    prev.itemStore != state.itemStore,
                            builder: (context, state) {
                              return TextStringField(
                                label: los.level_toggle,
                                value: event.toggleName,
                                onFieldSubmitted: (value) {
                                  event.toggleName = value;
                                  captureEvent(
                                    ActionType.eventChangeLevelToggle,
                                  );
                                  itemBloc.add(const ItemStoreUpdated());
                                  controller.requestFocus();
                                },
                                autovalidateMode: AutovalidateMode.always,
                                validator: (value) {
                                  if (value != '' &&
                                      value != event.toggleName) {
                                    return los.press_enter_to_save;
                                  }
                                  return null;
                                },
                              );
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.level)
                      const Padding(
                        padding: EdgeInsets.all(12),
                        child: SizedBox(width: double.infinity, height: 360),
                      ),
                    if (event.eventType == EventType.plant ||
                        event.eventType == EventType.plantbox)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: los.plant_type,
                            value: event.dataString ?? '',
                            suggestionsCallback: (pattern) {
                              final plantList =
                                  context
                                      .read<SuggestionBloc>()
                                      .state
                                      .plantList;
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(plantList, pattern);
                            },
                            itemBuilder: (context, String suggestion) {
                              final plant =
                                  gameResource.packet[suggestion] ??
                                  gameResource.commonImage[ImageCommonType
                                      .readyPacket];
                              return ListTile(
                                title: Row(
                                  children: [
                                    Container(
                                      width: 35,
                                      height: 35,
                                      margin: const EdgeInsets.only(right: 12),
                                      decoration: BoxDecoration(
                                        borderRadius: BorderRadius.circular(8),
                                      ),
                                      clipBehavior: Clip.antiAlias,
                                      child: RawImage(image: plant),
                                    ),
                                    Flexible(
                                      child: Text(
                                        suggestion,
                                        overflow: TextOverflow.ellipsis,
                                        style: TextStyle(color: textColor),
                                      ),
                                    ),
                                  ],
                                ),
                              );
                            },
                            onSuggestionSelected: (String suggestion) async {
                              event.dataString = suggestion;
                              final plantType = event.dataString;
                              final cubit =
                                  context.read<MapEditorConfigurationCubit>();
                              captureEvent(ActionType.eventChangePlantType);
                              final settingPath = cubit.state.settingPath;
                              if (plantType != null &&
                                  !cubit.state.gameResource.plant.containsKey(
                                    plantType,
                                  )) {
                                cubit.state.gameResource.plant[plantType] =
                                    await cubit.loadPlantVisualAnimation(
                                      '$settingPath/plant/$plantType',
                                      plantType,
                                      enableCostume:
                                          context
                                              .read<SettingBloc>()
                                              .state
                                              .plantCostume,
                                    );
                              }

                              itemBloc.add(const ItemStoreUpdated());
                              controller.requestFocus();
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              event.dataString = value;
                              final plantList =
                                  context
                                      .read<SuggestionBloc>()
                                      .state
                                      .plantList;
                              if (plantList.contains(value)) {
                                return null;
                              } else {
                                return '';
                              }
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.upgrade)
                      Padding(
                        padding: edgeSpacing,
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: los.upgrade_type,
                            value: event.dataString ?? '',
                            suggestionsCallback: (pattern) {
                              final upgradeList =
                                  context
                                      .read<SuggestionBloc>()
                                      .state
                                      .upgradeList;
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(upgradeList, pattern);
                            },
                            itemBuilder: (context, String suggestion) {
                              final upgrade =
                                  gameResource.upgrade[suggestion] ??
                                  gameResource.commonImage[ImageCommonType
                                      .missingArtPiece];

                              return ListTile(
                                title: Row(
                                  children: [
                                    Container(
                                      width: 35,
                                      height: 35,
                                      margin: const EdgeInsets.only(right: 12),
                                      decoration: BoxDecoration(
                                        borderRadius: BorderRadius.circular(8),
                                      ),
                                      clipBehavior: Clip.antiAlias,
                                      child: RawImage(image: upgrade),
                                    ),
                                    Flexible(
                                      child: Text(
                                        suggestion,
                                        overflow: TextOverflow.ellipsis,
                                        style: TextStyle(color: textColor),
                                      ),
                                    ),
                                  ],
                                ),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.dataString = suggestion;
                              captureEvent(ActionType.eventChangeUpgradeType);
                              itemBloc.add(const ItemStoreUpdated());
                              controller.requestFocus();
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              event.dataString = value;
                              final upgradeList =
                                  context
                                      .read<SuggestionBloc>()
                                      .state
                                      .upgradeList;
                              if (upgradeList.contains(value)) {
                                return null;
                              } else {
                                return '';
                              }
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.star_gate)
                      Padding(
                        padding: const EdgeInsets.all(12),
                        child: SizedBox(
                          width: double.infinity,
                          child: NumberTextField(
                            label: los.star_cost,
                            controller: NumberEditingController(
                              value: event.cost ?? 0,
                              isDouble: false,
                            ),
                            range: const Range(
                              begin: 0,
                              end: MapConst.intMaxValue,
                            ),
                            useChangeButton: true,
                            onFieldSubmitted: (value) {
                              event.cost = value as int;
                              captureEvent(ActionType.eventChangeStarCost);
                              itemBloc.add(const ItemStoreUpdated());
                              controller.requestFocus();
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.key_gate)
                      Padding(
                        padding: const EdgeInsets.all(12),
                        child: SizedBox(
                          width: double.infinity,
                          child: NumberTextField(
                            label: los.key_cost,
                            controller: NumberEditingController(
                              value: event.cost ?? 0,
                              isDouble: false,
                            ),
                            range: const Range(
                              begin: 0,
                              end: MapConst.intMaxValue,
                            ),
                            useChangeButton: true,
                            onFieldSubmitted: (value) {
                              event.cost = value as int;
                              captureEvent(ActionType.eventChangeKeyCost);
                              itemBloc.add(const ItemStoreUpdated());
                              controller.requestFocus();
                            },
                          ),
                        ),
                      ),
                    if (event.eventType == EventType.star_gate ||
                        event.eventType == EventType.key_gate)
                      BlocBuilder<ItemBloc, ItemState>(
                        buildWhen:
                            (prev, state) => prev.itemStore != state.itemStore,
                        builder: (context, state) {
                          return CheckBoxField(
                            label: los.piece_art_flip,
                            value: event.isArtFlipped ?? false,
                            height: 40,
                            onChanged: ({required bool? value}) {
                              event.isArtFlipped = value ?? false;
                              captureEvent(ActionType.eventArtFlip);
                              itemBloc.add(const ItemStoreUpdated());
                              controller.requestFocus();
                            },
                          );
                        },
                      ),
                    if (event.eventType == EventType.plant ||
                        event.eventType == EventType.plantbox)
                      const Padding(
                        padding: EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(width: double.infinity, height: 150),
                      ),
                  ],
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('id', id));
  }
}
