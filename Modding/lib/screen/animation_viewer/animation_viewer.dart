import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/bloc/selected_image_bloc/selected_image_bloc.dart';
import 'package:sen/bloc/selected_label_bloc/selected_label_bloc.dart';
import 'package:sen/bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/screen/animation_viewer/main_screen.dart';
import 'package:sen/widget/hotkey.dart';

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
        child: Builder(builder: (context) {
          return MainScreen(
            initialDirectoryCubit: context.read<InitialDirectoryCubit>(),
            selectedImageBloc: context.read<SelectedImageBloc>(),
            selectedSpriteBloc: context.read<SelectedSpriteBloc>(),
            selectedLabelBloc: context.read<SelectedLabelBloc>(),
          );
        }),
      ),
    );
  }
}
