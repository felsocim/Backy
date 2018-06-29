const defaultLocale = 'en_US';

const strings = {
  en_US: {
    NAVIGATION_HOME: "Home"
  }
};

function translate(identifier, locale = defaultLocale) {
  if(locale in strings) {
    return identifier in strings[locale] ? strings[locale][identifier] : strings[defaultLocale][identifier];
  }

  return 'N/A';
}

export { defaultLocale, translate };