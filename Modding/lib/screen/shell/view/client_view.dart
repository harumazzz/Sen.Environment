import 'package:flutter/material.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import 'package:sen/model/message.dart';
import 'package:sen/screen/shell/model/running_state.dart';
import 'package:sen/screen/shell/view/message_card.dart';
import 'package:sen/i18n/app_localizations.dart';

class ClientView extends StatelessWidget {
  final RunningState state;
  final List<Message> messages;
  final ScrollController scrollController;
  final Widget Function() makeStage;
  final String stage;
  final void Function() onLaunch;

  const ClientView({
    super.key,
    required this.state,
    required this.messages,
    required this.scrollController,
    required this.makeStage,
    required this.stage,
    required this.onLaunch,
  });

  Widget? _leading(
    BuildContext context,
  ) {
    if (state == RunningState.finished) {
      return null;
    }
    return switch (stage) {
      'input_string' => LoadingAnimationWidget.waveDots(
          color: Theme.of(context).colorScheme.primary,
          size: 20.0,
        ),
      'input_enumeration' => LoadingAnimationWidget.fourRotatingDots(
          color: Theme.of(context).colorScheme.primary,
          size: 20.0,
        ),
      'input_boolean' => LoadingAnimationWidget.beat(
          color: Theme.of(context).colorScheme.primary,
          size: 20.0,
        ),
      _ => const SizedBox.shrink(),
    };
  }

  Widget _containerWrap({
    required Widget child,
  }) {
    return Container(
      margin: const EdgeInsets.only(
        left: 16.0,
        right: 16.0,
        bottom: 8.0,
        top: 8.0,
      ),
      child: child,
    );
  }

  Widget _launchButton(
    BuildContext context,
  ) {
    final los = AppLocalizations.of(context)!;
    return ElevatedButton(
      onPressed: onLaunch,
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: SizedBox(
          width: double.infinity,
          child: Center(
            child: Text(los.launch_immediately),
          ),
        ),
      ),
    );
  }

  Widget _makeUserStage(
    BuildContext context,
  ) {
    if (state == RunningState.finished) {
      return _containerWrap(child: makeStage());
    } else if (state == RunningState.idle) {
      return _containerWrap(
        child: _launchButton(context),
      );
    } else {
      return _containerWrap(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const SizedBox(height: 4),
            _loadingState(context),
            const SizedBox(height: 4),
            Card(
              child: ListTile(
                leading: _leading(context),
                title: makeStage(),
              ),
            ),
          ],
        ),
      );
    }
  }

  Widget _loadingState(
    BuildContext context,
  ) {
    if (state == RunningState.running) {
      return const Padding(
        padding: EdgeInsets.symmetric(horizontal: 8.0),
        child: LinearProgressIndicator(),
      );
    }
    return const SizedBox.shrink();
  }

  Widget _buildMainScene(
    BuildContext context,
  ) {
    if (state == RunningState.running || state == RunningState.finished) {
      return Expanded(
        child: ListView.builder(
          controller: scrollController,
          itemCount: messages.length,
          itemBuilder: (context, index) {
            return MessageCard(
              message: messages[index],
            );
          },
        ),
      );
    } else {
      final los = AppLocalizations.of(context)!;
      return Expanded(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            LoadingAnimationWidget.hexagonDots(
              color: Theme.of(context).brightness == Brightness.dark ? Colors.white : Colors.black,
              size: 150,
            ),
            const SizedBox(height: 25.0),
            Text('${los.waiting_for_user}...'),
          ],
        ),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        _buildMainScene(context),
        _makeUserStage(context),
      ],
    );
  }
}
