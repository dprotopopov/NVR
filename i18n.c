#include "i18n.h"

void I18nInitialize(const char *LocaleDir)
{
	bindtextdomain("nvr", LocaleDir);
	textdomain("nvr");
}