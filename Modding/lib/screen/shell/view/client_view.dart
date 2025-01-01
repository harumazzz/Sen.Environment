import 'package:flutter/material.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import 'package:sen/model/message.dart';
import 'package:sen/screen/shell/view/message_card.dart';

class ClientView extends StatelessWidget {
  final bool finished;
  final List<Message> messages;
  final ScrollController scrollController;
  final Widget Function() makeStage;
  final String stage;

  const ClientView({
    super.key,
    required this.finished,
    required this.messages,
    required this.scrollController,
    required this.makeStage,
    required this.stage,
  });

  Widget? _leading(BuildContext context) {
    if (finished) {
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

  Widget _containerWrap({required Widget child}) {
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

  Widget _makeUserStage(BuildContext context) {
    if (finished) {
      return _containerWrap(child: makeStage());
    }
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

  Widget _loadingState(BuildContext context) {
    if (!finished) {
      return const Padding(
        padding: EdgeInsets.symmetric(horizontal: 8.0),
        child: LinearProgressIndicator(),
      );
    }
    return const SizedBox.shrink();
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Expanded(
          child: ListView.builder(
            controller: scrollController,
            itemCount: messages.length,
            itemBuilder: (context, index) {
              return MessageCard(
                message: messages[index],
              );
            },
          ),
        ),
        _makeUserStage(context),
      ],
    );
  }
}
