#apt-get install libcurl3 libcurl3-dev libasound2-dev

#-lwiringPi
CC       = g++
CFLAGS	 = -g -Wall -O3 -lintl -lboost_regex
CFLAGS	+= $(LIBXML2_CFLAGS) $(LIBXML2_LIBS)
CFLAGS	+= $(LIBXMLXX_CFLAGS) $(LIBXMLXX_LIBS)
CFLAGS	+= $(LIBGLIB_CFLAGS) $(LIBGLIB_LIBS)
CFLAGS	+= $(LIBGLIBMM_CFLAGS) $(LIBGLIBMM_LIBS)

TARGET	 =nvr
DESTDIR ?=
LOCDIR   = ./locale
CONFDIR  = ./

OBJ	 =  display.o i18n.o main.o menu.o osd.o remote.o tools.o 

DEFINES = -DSIMULATE
DEFINES += -DCONFDIR=\"$(CONFDIR)\"
DEFINES += -DLOCDIR=\"$(LOCDIR)\"

all: nvr i18n

nvr: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

.c.o:
	$(CC) -c $(DEFINES) $(CFLAGS) $< -o $@


# Internationalization (I18N):
PODIR     = po
LOCALEDIR = locale
I18Npo    = $(wildcard $(PODIR)/*.po)
I18Nmsgs  = $(addprefix $(LOCALEDIR)/, $(addsuffix /LC_MESSAGES/nvr.mo, $(notdir $(foreach file, $(I18Npo), $(basename $(file))))))
I18Npot   = $(PODIR)/nvr.pot

%.mo: %.po
	msgfmt -c -o $@ $<

$(I18Npot): $(wildcard *.c)
	xgettext -C -cTRANSLATORS --no-wrap --no-location -k -ktr -ktrNOOP --package-name=nvr -o $@ `ls $^`

%.po: $(I18Npot)
	msgmerge -U --no-fuzzy-matching --no-wrap --no-location --backup=none -q $@ $<
	@touch $@

$(I18Nmsgs): $(LOCALEDIR)/%/LC_MESSAGES/nvr.mo: $(PODIR)/%.mo
	@mkdir -p $(dir $@)
	cp $< $@

.PHONY: i18n
i18n: $(I18Nmsgs)

clean-local:
	-rm -rf $(LOCALEDIR) $(PODIR)/*.mo $(PODIR)/*.pot
	rm -f *.o nvr