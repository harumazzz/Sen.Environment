import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import '../../bloc/message_bloc/message_bloc.dart';

class EmptyBar extends StatelessWidget {
  const EmptyBar({super.key});

  Widget _buildLoadingBar({
    required BuildContext context,
    required Widget child,
  }) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: SizedBox(
        height: 46.0,
        width: double.infinity,
        child: FloatingActionButton(
          onPressed: null,
          child: Row(
            mainAxisSize: MainAxisSize.min,
            spacing: 10.0,
            children: [child],
          ),
        ),
      ),
    );
  }

  Widget _buildBusyBar(BuildContext context) {
    return _buildLoadingBar(
      context: context,
      child: LoadingAnimationWidget.progressiveDots(
        color: Theme.of(context).colorScheme.secondary,
        size: 40,
      ),
    );
  }

  Widget _buildCleaningBar(BuildContext context) {
    return _buildLoadingBar(
      context: context,
      child: LoadingAnimationWidget.horizontalRotatingDots(
        color: Theme.of(context).colorScheme.secondary,
        size: 40,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MessageBloc, MessageState>(
      builder: (context, state) {
        if (state is MessageClearState) {
          return _buildCleaningBar(context);
        }
        return _buildBusyBar(context);
      },
    );
  }
}
