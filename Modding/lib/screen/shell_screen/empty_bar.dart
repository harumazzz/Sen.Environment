import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import '../../bloc/message_bloc/message_bloc.dart';

class EmptyBar extends StatelessWidget {
  const EmptyBar({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MessageBloc, MessageState>(
      builder: (context, state) {
        if (state is MessageClearState) {
          return const CleaningBar();
        }
        return const BusyBar();
      },
    );
  }
}

class CustomLoadingBar extends StatelessWidget {
  const CustomLoadingBar({super.key, required this.child});
  final Widget child;

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: SizedBox(
        height: 46.0,
        width: double.infinity,
        child: FloatingActionButton(
          onPressed: null,
          child: Row(mainAxisSize: MainAxisSize.min, children: [child]),
        ),
      ),
    );
  }
}

class BusyBar extends StatelessWidget {
  const BusyBar({super.key});

  @override
  Widget build(BuildContext context) {
    return CustomLoadingBar(
      child: LoadingAnimationWidget.progressiveDots(
        color: Theme.of(context).colorScheme.secondary,
        size: 40,
      ),
    );
  }
}

class CleaningBar extends StatelessWidget {
  const CleaningBar({super.key});

  @override
  Widget build(BuildContext context) {
    return CustomLoadingBar(
      child: LoadingAnimationWidget.horizontalRotatingDots(
        color: Theme.of(context).colorScheme.secondary,
        size: 40,
      ),
    );
  }
}
