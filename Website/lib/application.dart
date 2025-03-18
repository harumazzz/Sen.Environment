import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import 'i18n/app_localizations.dart';
import 'screen/about/about_page.dart';
import 'screen/changelog/changelog_page.dart';
import 'screen/download/download_page.dart';
import 'screen/download/thankyou_page.dart';
import 'screen/extension/extension_screen.dart';
import 'screen/home/home_page.dart';
import 'screen/root_page.dart';
import 'dart:ui' as ui;

class Application extends StatelessWidget {
  const Application({super.key});

  Locale _getInitialLocale() {
    final String browserLocale =
        ui.PlatformDispatcher.instance.locale.toLanguageTag().split('-').first;
    for (final locale in AppLocalizations.supportedLocales) {
      if (locale.languageCode == browserLocale) {
        return locale;
      }
    }
    return const Locale('en');
  }

  static int _getSelectedIndex(String location) {
    switch (location) {
      case '/':
        return 0;
      case '/download':
        return 1;
      case '/changelog':
        return 2;
      case '/about':
        return 3;
      case '/extension':
        return 4;
      default:
        return 0;
    }
  }

  static final GoRouter _router = GoRouter(
    initialLocation: '/',
    redirect: (context, state) {
      if (state.matchedLocation.isEmpty) {
        return '/';
      }
      return null;
    },
    routes: [
      ShellRoute(
        builder: (context, state, child) {
          return RootPage(
            selectedIndex: _getSelectedIndex(state.matchedLocation),
            onNavigate: (index) {
              GoRouter.of(context).go(_getPath(index));
            },
            child: child,
          );
        },
        routes: [
          GoRoute(
            path: '/',
            builder:
                (context, state) => HomePage(
                  onNavigate: (index) {
                    GoRouter.of(context).go(_getPath(index));
                  },
                ),
          ),
          GoRoute(
            path: '/download',
            builder:
                (context, state) => DownloadPage(
                  onNavigate: (index) {
                    GoRouter.of(context).go(_getPath(index));
                  },
                ),
          ),
          GoRoute(
            path: '/download/success',
            builder: (context, state) {
              final extra = state.extra as Map<String, dynamic>;
              return ThankYouPage(
                isWindows: extra['isWindows'] as bool,
                link: extra['link'] as String,
                onNavigate: extra['onNavigate'] as void Function(int),
              );
            },
          ),
          GoRoute(
            path: '/changelog',
            builder:
                (context, state) => ChangelogPage(
                  onNavigate: (index) {
                    GoRouter.of(context).go(_getPath(index));
                  },
                ),
          ),
          GoRoute(
            path: '/about',
            builder:
                (context, state) => AboutPage(
                  onNavigate: (index) {
                    GoRouter.of(context).go(_getPath(index));
                  },
                ),
          ),
          GoRoute(
            path: '/extension',
            builder:
                (context, state) => ExtensionsPage(
                  onNavigate: (index) {
                    GoRouter.of(context).go(_getPath(index));
                  },
                ),
          ),
        ],
      ),
    ],
  );

  static String _getPath(int index) {
    switch (index) {
      case 0:
        return '/';
      case 1:
        return '/download';
      case 2:
        return '/changelog';
      case 3:
        return '/about';
      case 4:
        return '/extension';
      default:
        return '/';
    }
  }

  @override
  Widget build(BuildContext context) {
    return Builder(
      builder: (context) {
        return MaterialApp.router(
          title: 'Sen: Download page',
          theme: ThemeData(
            pageTransitionsTheme: PageTransitionsTheme(
              builders:
                  Map<TargetPlatform, PageTransitionsBuilder>.fromIterable(
                    TargetPlatform.values,
                    value: (_) => const FadeForwardsPageTransitionsBuilder(),
                  ),
            ),
            fontFamily: 'GoogleSans',
            colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
            useMaterial3: true,
          ),
          debugShowCheckedModeBanner: false,
          darkTheme: ThemeData(
            pageTransitionsTheme: PageTransitionsTheme(
              builders:
                  Map<TargetPlatform, PageTransitionsBuilder>.fromIterable(
                    TargetPlatform.values,
                    value: (_) => const FadeForwardsPageTransitionsBuilder(),
                  ),
            ),
            fontFamily: 'GoogleSans',
            brightness: Brightness.dark,
            colorScheme: ColorScheme.fromSeed(
              seedColor: Colors.deepPurpleAccent,
              brightness: Brightness.dark,
            ).copyWith(surface: Colors.black, onSurface: Colors.white),
            scaffoldBackgroundColor: Colors.black,
            useMaterial3: true,
            dialogTheme: DialogTheme(backgroundColor: Colors.grey.shade900),
          ),
          localizationsDelegates: AppLocalizations.localizationsDelegates,
          supportedLocales: AppLocalizations.supportedLocales,
          locale: _getInitialLocale(),
          routerConfig: _router,
        );
      },
    );
  }
}
