import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/model/worldmap.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/screen/map_editor/app/l10n/l10n.dart';
import 'package:sen/screen/map_editor/bloc/history/history_bloc.dart';
import 'package:sen/screen/map_editor/bloc/item/item_bloc.dart';
import 'package:sen/screen/map_editor/bloc/stage/stage_bloc.dart';
import 'package:sen/screen/map_editor/bloc/suggestion/suggestion_bloc.dart';
import 'package:sen/screen/map_editor/include/check_box_field.dart';
import 'package:sen/screen/map_editor/include/downdrop_search_field.dart';
import 'package:sen/screen/map_editor/include/dropdown_button_field.dart';
import 'package:sen/screen/map_editor/include/number_text_field.dart';
import 'package:sen/screen/map_editor/include/readonly_box_field.dart';
import 'package:sen/screen/map_editor/include/text_string_field.dart';
import 'package:sen/screen/map_editor/models/action_service.dart';
import 'package:sen/screen/map_editor/models/game_resource.dart';
import 'package:sen/screen/map_editor/models/map_const.dart';
import 'package:sen/screen/map_editor/models/action_model.dart';

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
          data: {
            ActionModelType.oneEvent: event.clone(),
            ActionModelType.id: id
          },
          change: (data) {
            final eventId = data![ActionModelType.id];
            stageBloc.state.events[eventId] =
                (data[ActionModelType.oneEvent] as MapEventItem).clone();
            itemBloc.add(const ItemStoreUpdated());
            historyBloc.add(const UpdateIndexEvent());
          });
      historyBloc.add(CaptureState(state: actionService));
    }

    final los = context.los;
    final gameResource =
        context.read<MapEditorConfigurationCubit>().state.gameResource;
    return Column(
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
            )),
          ),
        ),
        Expanded(
          child: Padding(
            padding:
                const EdgeInsets.only(top: 4, left: 8, right: 8, bottom: 16),
            child: SingleChildScrollView(
              child: Column(
                children: [
                  Row(
                    children: [
                      Padding(
                        padding: const EdgeInsets.all(12),
                        child: SizedBox(
                            width: 120,
                            child: ReadonlyBoxField(
                              label: los.type,
                              value: event.eventType.name,
                            )),
                      ),
                      Padding(
                          padding: const EdgeInsets.all(12),
                          child: SizedBox(
                            width: 60,
                            child: BlocBuilder<ItemBloc, ItemState>(
                                buildWhen: (prev, state) =>
                                    prev.itemStore != state.itemStore,
                                builder: (context, state) {
                                  return NumberTextField(
                                    label: los.id,
                                    controller: NumberEditingController(
                                        value: event.eventID, isDouble: false),
                                    range: const Range(
                                        begin: 0, end: MapConst.intMaxValue),
                                    useChangeButton: true,
                                    onFieldSubmitted: (value) {
                                      event.eventID = value as int;
                                      captureEvent(ActionType.eventChangeID);
                                      itemBloc.add(const ItemStoreUpdated());
                                    },
                                    autovalidateMode: AutovalidateMode.always,
                                    validator: (value) {
                                      final events = context
                                          .read<StageBloc>()
                                          .state
                                          .events;
                                      var counter = 0;
                                      events.forEach((_, eventValue) {
                                        if (eventValue.eventID ==
                                            event.eventID) {
                                          counter++;
                                        }
                                        if (counter > 1) {
                                          return;
                                        }
                                      });
                                      return counter > 1 ? 'duplicated' : null;
                                    },
                                  );
                                }),
                          ))
                    ],
                  ),
                  BlocBuilder<ItemBloc, ItemState>(
                      buildWhen: (prev, state) =>
                          prev.itemStore != state.itemStore,
                      builder: (context, state) {
                        return Row(children: [
                          Padding(
                              padding: const EdgeInsets.all(12),
                              child: SizedBox(
                                width: 90,
                                child: NumberTextField(
                                  label: los.position_x,
                                  controller: NumberEditingController(
                                      value: event.position.x, isDouble: true),
                                  range: const Range(
                                      begin: MapConst.doubleMinValue,
                                      end: MapConst.doubleMaxValue),
                                  useChangeButton: true,
                                  onFieldSubmitted: (value) {
                                    event.position.x = value as double;
                                    captureEvent(ActionType.moveItem);
                                    itemBloc.add(const ItemStoreUpdated());
                                  },
                                ),
                              )),
                          Padding(
                              padding: const EdgeInsets.all(12),
                              child: SizedBox(
                                width: 90,
                                child: NumberTextField(
                                  label: los.position_y,
                                  controller: NumberEditingController(
                                      value: event.position.y, isDouble: true),
                                  range: const Range(
                                      begin: MapConst.doubleMinValue,
                                      end: MapConst.doubleMaxValue),
                                  useChangeButton: true,
                                  onFieldSubmitted: (value) {
                                    event.position.y = value as double;
                                    captureEvent(ActionType.moveItem);
                                    itemBloc.add(const ItemStoreUpdated());
                                  },
                                ),
                              )),
                        ]);
                      }),
                  if (event.eventType != EventType.doodad)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                            width: double.infinity,
                            child: BlocBuilder<ItemBloc, ItemState>(
                                buildWhen: (prev, state) =>
                                    prev.itemStore != state.itemStore,
                                builder: (context, state) {
                                  return TextStringField(
                                      label: los.name,
                                      value: event.name,
                                      onFieldSubmitted: (value) {
                                        event.name = value;
                                        captureEvent(
                                            ActionType.eventChangeName);
                                        itemBloc.add(const ItemStoreUpdated());
                                      },
                                      autovalidateMode: AutovalidateMode.always,
                                      validator: (value) {
                                        if (value != event.name) {
                                          return 'Enter to save'; //TODO: add locale
                                        }
                                        return null;
                                      });
                                }))),
                  if (event.eventType != EventType.doodad)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
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
                                    .getSuggestion(eventNameList, pattern,
                                        expect: event.name);
                              },
                              itemBuilder: (context, String suggestion) {
                                return ListTile(
                                  title: Text(suggestion,
                                      style: Theme.of(context)
                                          .textTheme
                                          .titleMedium),
                                );
                              },
                              onSuggestionSelected: (String suggestion) {
                                event.parentEvent = suggestion;
                                captureEvent(ActionType.eventChangeParent);
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
                                  return 'Parent must same name'; //TODO: add locale
                                }
                              },
                            ))),
                  if (event.eventType != EventType.doodad)
                    Padding(
                      padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
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
                                  .getSuggestion(eventNameList, pattern,
                                      expect: event.name);
                            },
                            itemBuilder: (context, String suggestion) {
                              return ListTile(
                                title: Text(suggestion,
                                    style: Theme.of(context)
                                        .textTheme
                                        .titleMedium),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.unlockedFrom = suggestion;
                              captureEvent(ActionType.eventChangeUnlockedFrom);
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
                                return 'Unlocked must same name'; //TODO: add locale
                              }
                            },
                          )),
                    ),
                  if (event.eventType != EventType.doodad)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: 'Visible From', //TODO: add locale
                            value: event.visibleFrom,
                            suggestionsCallback: (pattern) {
                              final eventNameList =
                                  context.read<StageBloc>().getLevelName();
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(eventNameList, pattern,
                                      expect: event.name);
                            },
                            itemBuilder: (context, String suggestion) {
                              return ListTile(
                                title: Text(suggestion,
                                    style: Theme.of(context)
                                        .textTheme
                                        .titleMedium),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.visibleFrom = suggestion;
                              captureEvent(ActionType.eventChangeVisibleFrom);
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
                                return 'Visible must same name'; //TODO: add locale
                              }
                            },
                          ),
                        )),
                  if (event.eventType != EventType.doodad)
                    BlocBuilder<ItemBloc, ItemState>(
                        buildWhen: (prev, state) =>
                            prev.itemStore != state.itemStore,
                        builder: (context, state) {
                          return CheckBoxField(
                            label: los.auto_visible,
                            value: event.autoVisible,
                            height: 40,
                            onChanged: (value) {
                              event.autoVisible = value ?? false;
                              captureEvent(ActionType.eventChangeAutoVisible);
                              itemBloc.add(const ItemStoreUpdated());
                            },
                          );
                        }),
                  if (event.eventType == EventType.level)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: ReadonlyBoxField(
                          label: 'Level Type', // TODO : Localize
                          value: levelNodeType!.name,
                        )),
                  if (event.eventType == EventType.level)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                            width: double.infinity,
                            child: BlocBuilder<ItemBloc, ItemState>(
                                buildWhen: (prev, state) =>
                                    prev.itemStore != state.itemStore,
                                builder: (context, state) {
                                  return TextStringField(
                                      label: 'Level Data', //TODO: add locale
                                      value: event.dataString,
                                      onFieldSubmitted: (value) {
                                        event.dataString = value;
                                        captureEvent(
                                            ActionType.eventChangeLevelData);
                                        itemBloc.add(const ItemStoreUpdated());
                                      },
                                      autovalidateMode: AutovalidateMode.always,
                                      validator: (value) {
                                        if (value != '' &&
                                            value != event.dataString) {
                                          return 'Enter to save'; //TODO: add locale
                                        }
                                        return null;
                                      });
                                }))),
                  if (event.eventType == EventType.level)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                            width: double.infinity,
                            child: BlocBuilder<ItemBloc, ItemState>(
                                buildWhen: (prev, state) =>
                                    prev.itemStore != state.itemStore,
                                builder: (context, state) {
                                  return TextStringField(
                                      label: 'Display Text', //TODO: add locale
                                      value: event.displayText,
                                      onFieldSubmitted: (value) {
                                        event.displayText = value;
                                        captureEvent(
                                            ActionType.eventChangeDisplayText);
                                        itemBloc.add(const ItemStoreUpdated());
                                      },
                                      autovalidateMode: AutovalidateMode.always,
                                      validator: (value) {
                                        if (value != '' &&
                                            value != event.displayText) {
                                          return 'Enter to save'; //TODO: add locale
                                        }
                                        return null;
                                      });
                                }))),
                  if (event.eventType == EventType.level)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                            width: double.infinity,
                            child: DowndropSearchField(
                              label: 'Narration Unlocked', // TODO : Localize
                              value: event.unlockedNarrationID ?? '',
                              suggestionsCallback: (pattern) {
                                final narrationList = context
                                    .read<SuggestionBloc>()
                                    .state
                                    .narrationList;
                                return context
                                    .read<SuggestionBloc>()
                                    .getSuggestion(narrationList, pattern);
                              },
                              itemBuilder: (context, String suggestion) {
                                return ListTile(
                                  title: Text(suggestion,
                                      style: Theme.of(context)
                                          .textTheme
                                          .titleMedium),
                                );
                              },
                              onSuggestionSelected: (String suggestion) {
                                event.unlockedNarrationID = suggestion;
                                captureEvent(ActionType.eventChangeNarUnlocked);
                                // MapManager.mapUpdate();
                              },
                              autovalidateMode: AutovalidateMode.always,
                              validator: (value) {
                                event.unlockedNarrationID = value;
                                final narrationList = context
                                    .read<SuggestionBloc>()
                                    .state
                                    .narrationList;
                                if (event.unlockedNarrationID == null ||
                                    event.unlockedNarrationID == '' ||
                                    narrationList
                                        .contains(event.unlockedNarrationID)) {
                                  return null;
                                } else {
                                  return '';
                                }
                              },
                            ))),
                  if (event.eventType == EventType.level)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: 'Narration Completed', // TODO : Localize
                            value: event.completedNarrationID ?? '',
                            suggestionsCallback: (pattern) {
                              final narrationList = context
                                  .read<SuggestionBloc>()
                                  .state
                                  .narrationList;
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(narrationList, pattern);
                            },
                            itemBuilder: (context, String suggestion) {
                              return ListTile(
                                title: Text(suggestion,
                                    style: Theme.of(context)
                                        .textTheme
                                        .titleMedium),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.completedNarrationID = suggestion;
                              captureEvent(ActionType.eventChangeNarCompleted);
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              event.completedNarrationID = value;
                              final narrationList = context
                                  .read<SuggestionBloc>()
                                  .state
                                  .narrationList;
                              if (event.completedNarrationID == null ||
                                  event.completedNarrationID == '' ||
                                  narrationList
                                      .contains(event.completedNarrationID)) {
                                return null;
                              } else {
                                return '';
                              }
                            },
                          ),
                        )),
                  if (event.eventType == EventType.level)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                          width: double.infinity,
                          child: DropdownButtonField<WorldMapEventStatus>(
                            label: 'Level Tutorial When', // TODO : Localize
                            value: event.worldMapTutorialVisibleWhen ??
                                WorldMapEventStatus.none,
                            items: WorldMapEventStatus.values
                                .map((e) =>
                                    DropdownMenuItem<WorldMapEventStatus>(
                                      value: e,
                                      child: Text(
                                        e.name,
                                        maxLines: 1,
                                        overflow: TextOverflow.ellipsis,
                                        softWrap: false,
                                      ),
                                    ))
                                .toList(),
                            onChanged: (value) {
                              event.worldMapTutorialVisibleWhen = value;
                              captureEvent(
                                  ActionType.eventChangeTutorialUnlocked);
                            },
                          ),
                        )),
                  if (event.eventType == EventType.level)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                          width: double.infinity,
                          child: BlocBuilder<ItemBloc, ItemState>(
                              buildWhen: (prev, state) =>
                                  prev.itemStore != state.itemStore,
                              builder: (context, state) {
                                return TextStringField(
                                    label: 'Level Toggle', // TODO : Localize
                                    value: event.toggleName,
                                    onFieldSubmitted: (value) {
                                      event.toggleName = value;
                                      captureEvent(
                                          ActionType.eventChangeLevelToggle);
                                      itemBloc.add(const ItemStoreUpdated());
                                    },
                                    autovalidateMode: AutovalidateMode.always,
                                    validator: (value) {
                                      if (value != '' &&
                                          value != event.toggleName) {
                                        return 'Enter to save'; //TODO: add locale
                                      }
                                      return null;
                                    });
                              }),
                        )),
                  if (event.eventType == EventType.level)
                    const Padding(
                        padding: EdgeInsets.all(12),
                        child: SizedBox(
                          width: double.infinity,
                          height: 360,
                        )),
                  if (event.eventType == EventType.plant ||
                      event.eventType == EventType.plantbox)
                    Padding(
                        padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                        child: SizedBox(
                          width: double.infinity,
                          child: DowndropSearchField(
                            label: 'Plant Type', // TODO : Localize
                            value: event.dataString ?? '',
                            suggestionsCallback: (pattern) {
                              final plantList = context
                                  .read<SuggestionBloc>()
                                  .state
                                  .plantList;
                              return context
                                  .read<SuggestionBloc>()
                                  .getSuggestion(plantList, pattern);
                            },
                            itemBuilder: (context, String suggestion) {
                              final plant = gameResource.plant[suggestion] ??
                                  gameResource
                                      .commonImage[ImageCommonType.readyPlant];
                              return ListTile(
                                title: Row(children: [
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
                                      child: Text(suggestion,
                                          overflow: TextOverflow.ellipsis,
                                          style: Theme.of(context)
                                              .textTheme
                                              .titleMedium))
                                ]),
                              );
                            },
                            onSuggestionSelected: (String suggestion) {
                              event.dataString = suggestion;
                              captureEvent(ActionType.eventChangePlantType);
                              itemBloc.add(const ItemStoreUpdated());
                            },
                            autovalidateMode: AutovalidateMode.always,
                            validator: (value) {
                              event.dataString = value;
                              final plantList = context
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
                        )),
                  if (event.eventType == EventType.upgrade)
                    Padding(
                      padding: const EdgeInsets.fromLTRB(12, 12, 20, 12),
                      child: SizedBox(
                        width: double.infinity,
                        child: DowndropSearchField(
                          label: 'Upgrade Type', // TODO : Localize
                          value: event.dataString ?? '',
                          suggestionsCallback: (pattern) {
                            final upgradeList = context
                                .read<SuggestionBloc>()
                                .state
                                .upgradeList;
                            return context
                                .read<SuggestionBloc>()
                                .getSuggestion(upgradeList, pattern);
                          },
                          itemBuilder: (context, String suggestion) {
                            final upgrade = gameResource.upgrade[suggestion] ??
                                gameResource.commonImage[
                                    ImageCommonType.missingArtPiece];

                            return ListTile(
                              title: Row(children: [
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
                                    child: Text(suggestion,
                                        overflow: TextOverflow.ellipsis,
                                        style: Theme.of(context)
                                            .textTheme
                                            .titleMedium))
                              ]),
                            );
                          },
                          onSuggestionSelected: (String suggestion) {
                            event.dataString = suggestion;
                            captureEvent(ActionType.eventChangeUpgradeType);
                            itemBloc.add(const ItemStoreUpdated());
                          },
                          autovalidateMode: AutovalidateMode.always,
                          validator: (value) {
                            event.dataString = value;
                            final upgradeList = context
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
                          label: 'Star Cost', //TODO: add locale
                          controller: NumberEditingController(
                              value: event.cost ?? 0, isDouble: false),
                          range:
                              const Range(begin: 0, end: MapConst.intMaxValue),
                          useChangeButton: true,
                          onFieldSubmitted: (value) {
                            event.cost = value as int;
                            captureEvent(ActionType.eventChangeStarCost);
                            itemBloc.add(const ItemStoreUpdated());
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
                          label: 'Key Cost', //TODO: add locale
                          controller: NumberEditingController(
                              value: event.cost ?? 0, isDouble: false),
                          range:
                              const Range(begin: 0, end: MapConst.intMaxValue),
                          useChangeButton: true,
                          onFieldSubmitted: (value) {
                            event.cost = value as int;
                            captureEvent(ActionType.eventChangeKeyCost);
                            itemBloc.add(const ItemStoreUpdated());
                          },
                        ),
                      ),
                    ),
                  if (event.eventType == EventType.star_gate ||
                      event.eventType == EventType.key_gate)
                    BlocBuilder<ItemBloc, ItemState>(
                        buildWhen: (prev, state) =>
                            prev.itemStore != state.itemStore,
                        builder: (context, state) {
                          return CheckBoxField(
                            label: los.piece_art_flip,
                            value: event.isArtFlipped ?? false,
                            height: 40,
                            onChanged: (value) {
                              event.isArtFlipped = value ?? false;
                              captureEvent(ActionType.eventArtFlip);
                              itemBloc.add(const ItemStoreUpdated());
                              // capturePiece(ActionType.pieceArtFlip);
                            },
                          );
                        }),
                ],
              ),
            ),
          ),
        )
      ],
    );
  }
}
