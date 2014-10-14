#apt-get install libcurl3 libcurl3-dev libasound2-dev

#-lwiringPi
CC       = g++
#For cygwin you will also need -lpthread -ldsound -lwinmm and remove -lasound.
#Your linker can't find the symbols in gettext's library - make sure you're linking to it with the proper -l options. Read the info pages for info :)
#CFLAGS	 =-g -Wall -O3 -lasound -lcurl -lxml2 `xml2-config --cflags` \
#CFLAGS	 =-g -Wall -O3 -lintl -lpthread -ldsound -lwinmm -lcurl -lxml2 `xml2-config --cflags` \
CFLAGS	 =-g -Wall -O3 -lxml2 'xml2-config --cflags' \
	-I~/NVR/boost -Ibower_components/tinyxml_2_6_2 -I~/NVR/bower_components/tinyxpath_1_3_1 \
	-L~/NVR/bower_components/tinyxml_2_6_2 -L~/NVR/bower_components/tinyxpath_1_3_1 \
	~/NVR/boost/stage/lib/libboost_regex.a
TARGET	 =nvr
DESTDIR ?=
LOCDIR   = ./locale
CONFDIR  = ./

OBJ =  display.o i18n.o main.o menu.o osd.o remote.o tools.o \
	~/NVR/bower_components/tinyxml_2_6_2/tinyxml.o \
	~/NVR/bower_components/tinyxml_2_6_2/tinystr.o \
	~/NVR/bower_components/tinyxml_2_6_2/tinyxmlparser.o \
	~/NVR/bower_components/tinyxml_2_6_2/tinyxmlerror.o \
	~/NVR/bower_components/tinyxpath_1_3_1/htmlutil.o \
	~/NVR/bower_components/tinyxpath_1_3_1/lex_util.o \
	~/NVR/bower_components/tinyxpath_1_3_1/action_store.o \
	~/NVR/bower_components/tinyxpath_1_3_1/tokenlist.o \
	~/NVR/bower_components/tinyxpath_1_3_1/xpath_stack.o \
	~/NVR/bower_components/tinyxpath_1_3_1/xml_util.o \
	~/NVR/bower_components/tinyxpath_1_3_1/xpath_expression.o \
	~/NVR/bower_components/tinyxpath_1_3_1/xpath_static.o \
	~/NVR/bower_components/tinyxpath_1_3_1/xpath_stream.o \
	~/NVR/bower_components/tinyxpath_1_3_1/xpath_syntax.o \
	~/NVR/bower_components/tinyxpath_1_3_1/xpath_processor.o \
	~/NVR/bower_components/tinyxpath_1_3_1/node_set.o

DEFINES += -DCONFDIR=\"$(CONFDIR)\"
DEFINES += -DLOCDIR=\"$(LOCDIR)\"
DEFINES += -DSIMULATE

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

clean:
	-rm -rf $(LOCALEDIR) $(PODIR)/*.mo $(PODIR)/*.pot
	rm -f *.o nvr