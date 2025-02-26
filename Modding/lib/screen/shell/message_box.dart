import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/bloc/launch_status_bloc/launch_status_bloc.dart';
import 'package:sen/bloc/message_bloc/message_bloc.dart';
import 'package:sen/screen/shell/message_card.dart';
import 'package:sen/screen/shell/shimmer_card.dart';

class MessageBox extends StatelessWidget {
  const MessageBox({super.key});

  Widget _buildLauncher() {
    return BlocBuilder<MessageBloc, MessageState>(
      builder: (context, state) {
        return ListView.builder(
          itemCount: state.size,
          itemBuilder: (context, index) {
            return MessageCard(
              message: state[index],
            );
          },
        );
      },
    );
  }

  Widget _buildShimmer() {
    return ListView.builder(
      itemCount: 10,
      itemBuilder: (context, index) {
        return const ShimmerCard();
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return BlocConsumer<LaunchStatusBloc, LaunchStatusState>(
      listener: (context, state) {
        if (state is LaunchStatusLoading) {
          context.read<LaunchStatusBloc>().add(const LaunchStatusSleep());
        }
      },
      builder: (context, state) {
        return AnimatedSwitcher(
          duration: const Duration(milliseconds: 500),
          transitionBuilder: (Widget child, Animation<double> animation) {
            return FadeTransition(opacity: animation, child: child);
          },
          child: state is LaunchStatusLoading ? _buildShimmer() : _buildLauncher(),
        );
      },
    );
  }
}
