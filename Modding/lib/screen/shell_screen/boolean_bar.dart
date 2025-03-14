import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/interaction_bloc/interaction_bloc.dart';
import '../../extension/context.dart';

class BooleanBar extends StatelessWidget {
  const BooleanBar({super.key});

  void _onConfirm(BuildContext context) {
    context.read<InteractionBloc>().add(
      const BooleanInputCompleteEvent(value: true),
    );
  }

  void _onRefuse(BuildContext context) {
    context.read<InteractionBloc>().add(
      const BooleanInputCompleteEvent(value: false),
    );
  }

  Widget _buildButton({
    required BuildContext context,
    required void Function() onPressed,
    required Widget child,
  }) {
    return Expanded(
      child: SizedBox(
        height: 42.0,
        child: ElevatedButton(
          style: ElevatedButton.styleFrom(
            elevation: 4.0,
            animationDuration: const Duration(milliseconds: 300),
          ),
          onPressed: onPressed,
          child: child,
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        spacing: 12.0,
        children: [
          _buildButton(
            context: context,
            child: Text(context.los.yes),
            onPressed: () => _onConfirm(context),
          ),
          _buildButton(
            context: context,
            child: Text(context.los.no),
            onPressed: () => _onRefuse(context),
          ),
        ],
      ),
    );
  }
}
