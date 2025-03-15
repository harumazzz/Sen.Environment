import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:screenshot/screenshot.dart';
import '../../bloc/add_option_bloc/add_option_bloc.dart';
import '../../bloc/launch_status_bloc/launch_status_bloc.dart';
import '../../bloc/message_bloc/message_bloc.dart';
import '../../extension/context.dart';
import 'idle_screen.dart';
import 'message_card.dart';
import 'shimmer_card.dart';

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
        if (state is LaunchStatusInitial) {
          return IdleScreen(
            color: Colors.cyan.withValues(alpha: 0.6),
            text: context.los.sen_is_listening,
            child: Image.asset('assets/images/logo.png'),
          );
        }
        return state is LaunchStatusLoading
            ? _buildShimmer()
            : _MessageList(scrollController: _scrollController);
      },
    );
  }
}

class _MessageList extends StatelessWidget {
  const _MessageList({required this.scrollController});

  final ScrollController scrollController;

  @override
  Widget build(BuildContext context) {
    return BlocConsumer<MessageBloc, MessageState>(
      listener: (context, state) async {
        if (state is MessageScrollState) {
          final hasClient = scrollController.hasClients;
          if (!hasClient) {
            return;
          }
          final shouldScroll =
              scrollController.position.pixels ==
              scrollController.position.maxScrollExtent;
          await Future.delayed(Duration.zero);
          await WidgetsBinding.instance.endOfFrame;
          if (scrollController.hasClients && shouldScroll) {
            scrollController.position.jumpTo(
              scrollController.position.maxScrollExtent,
            );
          }
        }
      },
      builder: (context, state) {
        return Screenshot(
          controller: context.read<AddOptionBloc>().screenshotController,
          child: ListView.builder(
            itemCount: state.size,
            controller: scrollController,
            itemBuilder: (context, index) {
              return MessageCard(message: state[index]);
            },
          ),
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<ScrollController>(
        'scrollController',
        scrollController,
      ),
    );
  }
}
