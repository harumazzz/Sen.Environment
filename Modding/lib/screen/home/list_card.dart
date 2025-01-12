part of 'home_screen.dart';

@immutable
class ListCard extends StatelessWidget {
  final Item item;
  final bool isValid;
  final String invalidMessage;
  final void Function()? onSetting;

  const ListCard({
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
        child: item.onWidget?.call(),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 4.0),
      child: Card(
        clipBehavior: Clip.hardEdge,
        child: ListTile(
          leading: item.icon,
          title: Text(
            item.title,
            maxLines: 4,
          ),
          onTap: isValid ? () => _onClick(context) : null,
          subtitle: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              const SizedBox(height: 2),
              Text(
                item.description,
                maxLines: 2,
                overflow: TextOverflow.ellipsis,
              ),
              const SizedBox(height: 8),
              isValid
                  ? Container()
                  : Text(
                      invalidMessage,
                      textAlign: TextAlign.start,
                    ),
            ],
          ),
          trailing: onSetting != null
              ? IconButton(
                  icon: const Icon(Symbols.settings),
                  onPressed: onSetting,
                  tooltip: AppLocalizations.of(context)!.settings,
                )
              : const SizedBox.shrink(),
        ),
      ),
    );
  }
}
