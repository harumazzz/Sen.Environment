import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/selected_image_bloc/selected_image_bloc.dart';
import '../../bloc/selected_label_bloc/selected_label_bloc.dart';
import '../../bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import 'main_screen.dart';
import '../../widget/hotkey.dart';

class AnimationViewer extends StatelessWidget {
  const AnimationViewer({super.key});

  @override
  Widget build(BuildContext context) {
    return HotkeyBuilder(
      child: MultiBlocProvider(
        providers: [
          BlocProvider<SelectedImageBloc>(
            create: (context) => SelectedImageBloc(),
          ),
          BlocProvider<SelectedSpriteBloc>(
            create: (context) => SelectedSpriteBloc(),
          ),
          BlocProvider<SelectedLabelBloc>(
            create: (context) => SelectedLabelBloc(),
          ),
        ],
        child: const MainScreen(),
      ),
    );
  }
}
