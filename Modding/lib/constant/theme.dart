import 'dart:io';
import 'package:flutter/material.dart' as material;

class MaterialDesign {
  static String _exchangeFont() {
    if (Platform.isWindows) {
      return 'SegoeUI';
    }
    return 'GoogleSans';
  }

  static final material.ThemeData darkTheme = material.ThemeData(
    brightness: material.Brightness.dark,
    fontFamily: _exchangeFont(),
    useMaterial3: true,
    colorScheme: material.ColorScheme.fromSeed(
      seedColor: material.Colors.deepPurpleAccent,
      brightness: material.Brightness.dark,
    ),
    progressIndicatorTheme: material.ProgressIndicatorThemeData(
      color: material.Colors.pinkAccent,
      linearTrackColor: material.Colors.grey.shade800,
      linearMinHeight: 4.0,
      circularTrackColor: material.Colors.grey.shade700,
      refreshBackgroundColor: material.Colors.grey.shade900,
    ),
    sliderTheme: material.SliderThemeData(
      trackHeight: 4.0,
      thumbShape: const material.RoundSliderThumbShape(),
      overlayShape: const material.RoundSliderOverlayShape(overlayRadius: 20.0),
      tickMarkShape: const material.RoundSliderTickMarkShape(),
      activeTrackColor: material.Colors.pinkAccent,
      inactiveTrackColor: material.Colors.pinkAccent.withValues(alpha: 0.5),
      thumbColor: material.Colors.pinkAccent,
      overlayColor: material.Colors.pinkAccent.withValues(alpha: 0.2),
      valueIndicatorColor: material.Colors.pinkAccent,
      valueIndicatorTextStyle: const material.TextStyle(
        color: material.Colors.white,
        fontWeight: material.FontWeight.bold,
      ),
    ),
    appBarTheme: const material.AppBarTheme(
      backgroundColor: material.Colors.transparent,
      elevation: 0,
      surfaceTintColor: material.Colors.pinkAccent,
      titleTextStyle: material.TextStyle(
        fontSize: 20,
        fontWeight: material.FontWeight.bold,
        color: material.Colors.white,
      ),
      iconTheme: material.IconThemeData(color: material.Colors.white),
    ),
    cardTheme: material.CardTheme(
      elevation: 3,
      shape: material.RoundedRectangleBorder(
        borderRadius: material.BorderRadius.circular(12),
      ),
    ),
    dialogTheme: material.DialogTheme(
      backgroundColor: material.Colors.grey.shade900,
    ),
    pageTransitionsTheme: material.PageTransitionsTheme(
      builders: Map<
        material.TargetPlatform,
        material.PageTransitionsBuilder
      >.fromIterable(
        material.TargetPlatform.values,
        value: (_) => const material.FadeForwardsPageTransitionsBuilder(),
      ),
    ),
  );

  static final material.ThemeData lightTheme = material.ThemeData(
    brightness: material.Brightness.light,
    fontFamily: _exchangeFont(),
    useMaterial3: true,
    colorScheme: material.ColorScheme.fromSeed(
      seedColor: material.Colors.pinkAccent,
    ),
    progressIndicatorTheme: material.ProgressIndicatorThemeData(
      color: material.Colors.pinkAccent,
      linearTrackColor: material.Colors.grey.shade300,
      linearMinHeight: 4.0,
      circularTrackColor: material.Colors.grey.shade200,
      refreshBackgroundColor: material.Colors.grey.shade100,
    ),
    sliderTheme: material.SliderThemeData(
      trackHeight: 4.0,
      thumbShape: const material.RoundSliderThumbShape(),
      overlayShape: const material.RoundSliderOverlayShape(overlayRadius: 20.0),
      tickMarkShape: const material.RoundSliderTickMarkShape(),
      activeTrackColor: material.Colors.pinkAccent,
      inactiveTrackColor: material.Colors.pinkAccent.withValues(alpha: 0.5),
      thumbColor: material.Colors.pinkAccent,
      overlayColor: material.Colors.pinkAccent.withValues(alpha: 0.2),
      valueIndicatorColor: material.Colors.pinkAccent,
      valueIndicatorTextStyle: const material.TextStyle(
        color: material.Colors.white,
        fontWeight: material.FontWeight.bold,
      ),
    ),
    appBarTheme: const material.AppBarTheme(
      backgroundColor: material.Colors.transparent,
      elevation: 0,
      surfaceTintColor: material.Colors.deepPurpleAccent,
      titleTextStyle: material.TextStyle(
        fontSize: 20,
        fontWeight: material.FontWeight.bold,
        color: material.Colors.black,
      ),
      iconTheme: material.IconThemeData(color: material.Colors.black),
    ),
    cardTheme: material.CardTheme(
      elevation: 3,
      shape: material.RoundedRectangleBorder(
        borderRadius: material.BorderRadius.circular(12),
      ),
    ),
    pageTransitionsTheme: material.PageTransitionsTheme(
      builders: Map<
        material.TargetPlatform,
        material.PageTransitionsBuilder
      >.fromIterable(
        material.TargetPlatform.values,
        value: (_) => const material.FadeForwardsPageTransitionsBuilder(),
      ),
    ),
  );
}
