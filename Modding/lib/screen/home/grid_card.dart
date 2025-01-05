part of 'home_screen.dart';

@immutable
class GridCard extends StatelessWidget {
  final Item item;
  final bool isValid;
  final String invalidMessage;
  final void Function()? onSetting;

  const GridCard({
    required this.item,
    required this.isValid,
    required this.invalidMessage,
    this.onSetting,
    super.key,
  });

  void _onClick(BuildContext context) {
    Navigator.push(
      context,
      PageTransition(
        duration: const Duration(milliseconds: 300),
        type: PageTransitionType.rightToLeft,
        child: item.onWidget(),
      ),
    );
  }

  Widget _settingsSymbol(
    BuildContext context,
  ) {
    return Positioned(
      top: 8,
      right: 8,
      child: IconButton(
        icon: const Icon(Symbols.settings),
        onPressed: onSetting,
        tooltip: AppLocalizations.of(context)!.settings,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Tooltip(
      message: item.title,
      child: Stack(
        children: [
          Card(
            clipBehavior: Clip.none,
            child: InkWell(
              splashColor: Colors.blue.withAlpha(30),
              onTap: isValid ? () => _onClick(context) : null,
              child: Container(
                width: 250.0,
                margin: const EdgeInsets.symmetric(horizontal: 8.0),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    AnimatedFloating(child: item.icon),
                    const SizedBox(height: 8),
                    Text(
                      item.title,
                      textAlign: TextAlign.center,
                      maxLines: 4,
                      style: const TextStyle(fontWeight: FontWeight.bold),
                    ),
                    const SizedBox(height: 4),
                    Text(
                      item.description,
                      textAlign: TextAlign.center,
                      maxLines: 4,
                      overflow: TextOverflow.ellipsis,
                    ),
                    const SizedBox(height: 15),
                    isValid
                        ? Container()
                        : Text(
                            invalidMessage,
                            textAlign: TextAlign.center,
                          ),
                  ],
                ),
              ),
            ),
          ),
          onSetting != null ? _settingsSymbol(context) : const SizedBox.shrink(),
        ],
      ),
    );
  }
}
