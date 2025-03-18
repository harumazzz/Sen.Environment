import 'package:flutter/material.dart';

class DownloadError extends StatelessWidget {
  const DownloadError({super.key});

  @override
  Widget build(BuildContext context) {
    return const Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(Icons.warning, color: Colors.red, size: 80),
          SizedBox(height: 20),
          Text(
            'VPN detected! Maybe try to change the VPN application, or either turn it off if possible, and try reload the page.',
            textAlign: TextAlign.center,
            style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
          ),
        ],
      ),
    );
  }
}
