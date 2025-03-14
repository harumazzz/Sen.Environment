import '../model/translator.dart';

class Translators {
  static const haruma = Translator(
    name: 'Haruma',
    discord: 'harumaluvcat',
    contacts: [
      ['GitHub', 'https://github.com/harumazzz'],
      ['Youtube', 'https://www.youtube.com/@harumavn'],
    ],
    imageCover: 'assets/images/translator/haruma.png',
  );

  static const jnr = Translator(
    name: 'JNR',
    discord: 'jnr1809',
    contacts: [
      ['Youtube', 'https://www.youtube.com/@jnr1809'],
    ],
    imageCover: 'assets/images/translator/jnr.png',
  );

  static const ppp = Translator(
    name: 'PPP',
    discord: 'theprimalpea',
    contacts: [
      ['Facebook', 'https://www.facebook.com/ThePrimalPea'],
    ],
    imageCover: 'assets/images/translator/ppp.png',
  );

  static const vi = Translator(
    name: 'Vi',
    discord: 'vi_i_guess',
    contacts: [
      ['GitHub', 'https://github.com/viiguess'],
    ],
    imageCover: 'assets/images/translator/vi.png',
  );

  static const Map<String, Translator> translators = {
    'haruma': haruma,
    'jnr': jnr,
    'ppp': ppp,
    'vi': vi,
  };

  static const Map<String, String> languages = {
    'en': 'haruma',
    'es': 'jnr',
    'ru': 'vi',
    'vi': 'ppp',
  };
}
