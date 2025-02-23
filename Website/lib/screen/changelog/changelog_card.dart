import 'package:flutter/material.dart';
import 'package:website/model/changelog.dart';

class ChangelogCard extends StatelessWidget {
  final Changelog changelog;

  const ChangelogCard({super.key, required this.changelog});

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final textColor = theme.brightness == Brightness.dark ? Colors.white : Colors.black87;
    final screenWidth = MediaQuery.of(context).size.width;
    final isSmallScreen = screenWidth < 600;

    return Card(
      margin: EdgeInsets.all(isSmallScreen ? 8.0 : 16.0),
      elevation: 4,
      shadowColor: Colors.black26,
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12),
      ),
      child: Padding(
        padding: EdgeInsets.all(isSmallScreen ? 12.0 : 20.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            _buildHeader(isSmallScreen, textColor),
            const SizedBox(height: 12),
            const Divider(),
            _buildSectionTitle('Update Changes:', textColor, isSmallScreen),
            _buildListSection(changelog.updateChanges, textColor, isSmallScreen),
            const Divider(),
            _buildSectionTitle('Special Thanks To:', textColor, isSmallScreen),
            _buildListSection(changelog.specialThanks, textColor, isSmallScreen),
          ],
        ),
      ),
    );
  }

  Widget _buildHeader(bool isSmallScreen, Color textColor) {
    return Row(
      crossAxisAlignment: CrossAxisAlignment.center,
      spacing: 12.0,
      children: [
        Container(
          width: isSmallScreen ? 30 : 40,
          height: isSmallScreen ? 30 : 40,
          decoration: const BoxDecoration(
            image: DecorationImage(
              image: AssetImage('assets/images/logo.png'),
              fit: BoxFit.cover,
            ),
          ),
        ),
        Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          spacing: 4.0,
          children: [
            Text(
              'Version: ${changelog.version}',
              style: TextStyle(
                color: textColor,
                fontSize: isSmallScreen ? 16 : 18,
                fontWeight: FontWeight.bold,
              ),
            ),
            Text(
              'Date: ${changelog.date?.toLocal().toString().split(' ')[0]}',
              style: TextStyle(
                color: Colors.grey.shade600,
                fontSize: isSmallScreen ? 12 : 14,
              ),
            ),
          ],
        ),
      ],
    );
  }

  Widget _buildSectionTitle(String title, Color textColor, bool isSmallScreen) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 8.0),
      child: Text(
        title,
        style: TextStyle(
          color: textColor,
          fontSize: isSmallScreen ? 14 : 18,
          fontWeight: FontWeight.bold,
        ),
      ),
    );
  }

  Widget _buildListSection(List<String>? items, Color textColor, bool isSmallScreen) {
    if (items == null || items.isEmpty) return const SizedBox.shrink();

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: items.map((item) {
        return Padding(
          padding: const EdgeInsets.symmetric(vertical: 4.0),
          child: Text(
            '- $item',
            style: TextStyle(color: textColor, fontSize: isSmallScreen ? 14 : 16),
          ),
        );
      }).toList(),
    );
  }
}
