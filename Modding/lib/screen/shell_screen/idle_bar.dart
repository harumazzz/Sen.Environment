import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/add_option_bloc/add_option_bloc.dart';
import '../../bloc/argument_bloc/argument_bloc.dart';
import '../../bloc/message_bloc/message_bloc.dart';
import '../../extension/context.dart';
import '../../service/ui_helper.dart';
import 'attachment_page.dart';

class IdleBar extends StatelessWidget {
  const IdleBar({super.key, required this.onLaunch});

  final void Function() onLaunch;

  Future<void> _onAttach(BuildContext context) async {
    await UIHelper.showDetailDialog(
      context: context,
      title: Text(
        context.los.add,
        style: const TextStyle(fontWeight: FontWeight.w600),
      ),
      content: BlocProvider<MessageBloc>.value(
        value: context.read<MessageBloc>(),
        child: BlocProvider<AddOptionBloc>.value(
          value: context.read<AddOptionBloc>(),
          child: BlocProvider<ArgumentBloc>.value(
            value: context.read<ArgumentBloc>(),
            child: const AttachmentPage(),
          ),
        ),
      ),
    );
  }

  Future<void> _onClear(BuildContext context) async {
    context.read<ArgumentBloc>().add(const ClearArgument());
    await UIHelper.showSimpleDialog(
      context: context,
      title: context.los.clear,
      content: context.los.argument_has_been_cleared,
    );
  }

  Widget _buildIconButton({
    required IconData icon,
    required String tooltip,
    required VoidCallback onPressed,
  }) {
    return Container(
      decoration: BoxDecoration(
        shape: BoxShape.circle,
        color: Colors.white.withValues(alpha: 0.15),
      ),
      child: IconButton(
        icon: Icon(icon, size: 24),
        tooltip: tooltip,
        onPressed: onPressed,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 16.0, vertical: 10.0),
      decoration: BoxDecoration(
        color: Theme.of(context).colorScheme.surfaceContainerHighest,
        borderRadius: BorderRadius.circular(12.0),
        boxShadow: [
          BoxShadow(
            color: Colors.black.withValues(alpha: 0.1),
            blurRadius: 8,
            offset: const Offset(0, 4),
          ),
        ],
      ),
      child: Row(
        spacing: 12.0,
        children: [
          _buildIconButton(
            icon: Symbols.attachment,
            tooltip: context.los.attach,
            onPressed: () async => await _onAttach(context),
          ),
          _buildIconButton(
            icon: Symbols.clear,
            tooltip: context.los.clear,
            onPressed: () async => await _onClear(context),
          ),
          const Spacer(),
          FloatingActionButton(
            onPressed: onLaunch,
            tooltip: context.los.launch_immediately,
            child: const Icon(Symbols.play_arrow),
          ),
        ],
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<void Function()>.has('onLaunch', onLaunch),
    );
  }
}
