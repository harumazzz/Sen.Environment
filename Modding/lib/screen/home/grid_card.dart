part of 'home_screen.dart';

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
              onTap: isValid
                  ? () {
                      Navigator.push(
                        context,
                        PageTransition(
                          duration: const Duration(milliseconds: 300),
                          type: PageTransitionType.rightToLeft,
                          child: item.onWidget(),
                        ),
                      );
                    }
                  : null,
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
          onSetting != null
              ? Positioned(
                  top: 8,
                  right: 8,
                  child: IconButton(
                    icon: const Icon(Icons.settings),
                    onPressed: onSetting,
                    tooltip: AppLocalizations.of(context)!.settings,
                  ),
                )
              : const SizedBox.shrink(),
        ],
      ),
    );
  }
}
