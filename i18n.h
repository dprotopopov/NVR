#ifndef __I18N_H
#define __I18N_H

#include <libintl.h>
#include <locale.h>
#include <stdio.h>

void I18nInitialize(const char *LocaleDir);

#define tr(String) gettext (String)

#endif