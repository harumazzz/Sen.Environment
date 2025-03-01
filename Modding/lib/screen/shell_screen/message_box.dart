import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:screenshot/screenshot.dart';
import 'package:sen/bloc/add_option_bloc/add_option_bloc.dart';
import 'package:sen/bloc/launch_status_bloc/launch_status_bloc.dart';
import 'package:sen/bloc/message_bloc/message_bloc.dart';
import 'package:sen/screen/shell_screen/message_card.dart';
import 'package:sen/screen/shell_screen/shimmer_card.dart';

class MessageBox extends StatefulWidget {
  const MessageBox({super.key});

  @override
  State<MessageBox> createState() => _MessageBoxState();
}

class _MessageBoxState extends State<MessageBox> {
  late final ScrollController _scrollController;

  @override
  void initState() {
    _scrollController = ScrollController();
    super.initState();
  }

  @override
  void dispose() {
    _scrollController.dispose();
    super.dispose();
  }

  Widget _buildLauncher() {
    return BlocConsumer<MessageBloc, MessageState>(
      listener: (context, state) async {
        if (state is MessageScrollState) {
          final shouldScroll = _scrollController.position.pixels == _scrollController.position.maxScrollExtent;
          await WidgetsBinding.instance.endOfFrame;
          if (shouldScroll) {
            await _scrollController.position.animateTo(
              _scrollController.position.maxScrollExtent,
              duration: const Duration(milliseconds: 100),
              curve: Curves.easeOut,
            );
          }
        }
      },
      builder: (context, state) {
        return Screenshot(
          controller: context.read<AddOptionBloc>().screenshotController,
          child: ListView.builder(
            itemCount: state.size,
            controller: _scrollController,
            itemBuilder: (context, index) {
              return MessageCard(
                message: state[index],
              );
            },
          ),
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
        return state is LaunchStatusLoading ? _buildShimmer() : _buildLauncher();
      },
    );
  }
}
