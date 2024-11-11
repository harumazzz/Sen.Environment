import 'package:flutter_dotenv/flutter_dotenv.dart';
import 'package:google_generative_ai/google_generative_ai.dart';

class GeminiHelper {
  late final GenerativeModel _model;

  final List<Content> _history;

  GeminiHelper()
      : _model = GenerativeModel(
          model: 'gemini-1.5-flash-latest',
          apiKey: dotenv.env['GEMINI_API']!,
        ),
        _history = [];

  Future<String?> ask({
    required String message,
  }) async {
    final content = [..._history, Content.text(message)];
    final response = await _model.generateContent(content);
    return response.text;
  }
}
