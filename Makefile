## Copyright 2015-2016 Carnë Draug
## Copyright 2015-2016 Oliver Heimlich
## Copyright 2018-2019 John Donoghue
##
## Copying and distribution of this file, with or without modification,
## are permitted in any medium without royalty provided the copyright
## notice and this notice are preserved.  This file is offered as-is,
## without any warranty.
TOPDIR := $(shell pwd)

## Some shell programs
MD5SUM    ?= md5sum
SED       ?= sed
GREP      ?= grep
TAR       ?= tar
GZIP      ?= gzip
CUT       ?= cut
TR        ?= tr
TEXI2PDF  ?= texi2pdf -q

PACKAGE := $(shell $(GREP) "^Name: " DESCRIPTION | $(CUT) -f2 -d" " | \
$(TR) '[:upper:]' '[:lower:]')
VERSION := $(shell $(GREP) "^Version: " DESCRIPTION | $(CUT) -f2 -d" ")

HG           := hg
HG_CMD        = $(HG) --config alias.$(1)=$(1) --config defaults.$(1)= $(1)
HG_ID        := $(shell $(call HG_CMD,identify) --id | sed -e 's/+//' )
HG_TIMESTAMP := $(firstword $(shell $(call HG_CMD,log) --rev $(HG_ID) --template '{date|hgdate}'))

TAR_REPRODUCIBLE_OPTIONS := --sort=name --mtime="@$(HG_TIMESTAMP)" --owner=0 --group=0 --numeric-owner
TAR_OPTIONS  := --format=ustar $(TAR_REPRODUCIBLE_OPTIONS)

TARGET_DIR      := target
RELEASE_DIR     := $(TARGET_DIR)/$(PACKAGE)-$(VERSION)
RELEASE_TARBALL := $(TARGET_DIR)/$(PACKAGE)-$(VERSION).tar.gz
HTML_DIR        := $(TARGET_DIR)/$(PACKAGE)-html
HTML_TARBALL    := $(TARGET_DIR)/$(PACKAGE)-html.tar.gz

PKG_ADD     := 

OCTAVE ?= octave --no-window-system --silent
MKOCTFILE ?= mkoctfile

.PHONY: help dist html release install all check run clean test_files

help:
	@echo "Targets:"
	@echo "   dist    - Create $(RELEASE_TARBALL) for release"
	@echo "   html    - Create $(HTML_TARBALL) for release"
	@echo "   release - Create both of the above and show md5sums"
	@echo
	@echo "   install - Install the package in GNU Octave"
	@echo "   all     - Build all oct files"
	@echo "   check   - Execute package tests (w/o install)"
	@echo "   run     - Run Octave with development in PATH (no install)"
	@echo
	@echo "   clean   - Remove releases, html documentation, and oct files"

%.tar.gz: %
	$(TAR) -cf - $(TAR_OPTIONS) -C "$(TARGET_DIR)/" "$(notdir $<)" | gzip -9n > $@
	-rm -rf $<

.PHONY: docs
docs: doc/$(PACKAGE).pdf

.PHONY: clean-docs
clean-docs:
	$(RM) -f doc/$(PACKAGE).info
	$(RM) -f doc/$(PACKAGE).pdf
	$(RM) -f doc/functions.texi

doc/$(PACKAGE).pdf: doc/$(PACKAGE).texi doc/functions.texi
	cd doc && SOURCE_DATE_EPOCH=$(HG_TIMESTAMP) $(TEXI2PDF) $(PACKAGE).texi
	# remove temp files
	cd doc && $(RM) -f $(PACKAGE).aux $(PACKAGE).cp $(PACKAGE).cps $(PACKAGE).fn  $(PACKAGE).fns $(PACKAGE).log $(PACKAGE).toc

doc/functions.texi:
	cd doc && ./mkfuncdocs.py --src-dir=../inst/ --src-dir=../src/ ../INDEX | $(SED) 's/@seealso/@xseealso/g' > functions.texi


$(RELEASE_DIR): .hg/dirstate
	@echo "Creating package version $(VERSION) release ..."
	$(RM) -r "$@"
	$(call HG_CMD,archive) --exclude ".hg*" --type files --rev $(HG_ID) "$@"
	cd "$@/src" && ./bootstrap && $(RM) -r "autom4te.cache"
	# build docs
	$(MAKE) -C "$@" docs
	chmod -R a+rX,u+w,go-w "$@"

html_options = --eval 'options = get_html_options ("octave-forge");' \
               --eval 'options.package_doc = "$(PACKAGE).texi";'
$(HTML_DIR): install
	@echo "Generating HTML documentation. This may take a while ..."
	$(RM) -r "$@"
	$(OCTAVE) --no-window-system --silent \
	  --eval "pkg load generate_html; "   \
	  --eval "pkg load $(PACKAGE);"       \
	  $(html_options)                     \
	  --eval 'generate_package_html ("${PACKAGE}", "$@", options);'
	chmod -R a+rX,u+w,go-w $@
	-rm -rf $<

dist: $(RELEASE_TARBALL)
html: $(HTML_TARBALL)

release: dist html
	md5sum $(RELEASE_TARBALL) $(HTML_TARBALL)
	@echo "Upload @ https://sourceforge.net/p/octave/package-releases/new/"
	@echo 'Execute: hg tag "release-${VERSION}"'

install: $(RELEASE_TARBALL)
	@echo "Installing package locally ..."
	$(OCTAVE) --eval 'pkg ("install", "-verbose", "${RELEASE_TARBALL}")'

all: $(CC_SOURCES)
	cd src/ && ./bootstrap && ./configure
	$(MAKE) -C src/

check: all
	$(OCTAVE) --path "$(TOPDIR)/inst/" --path "$(TOPDIR)/src/" \
	  --eval '${PKG_ADD}' \
	  --eval '__run_test_suite__ ({"$(TOPDIR)/inst"}, {})'

run: all
	$(OCTAVE) --persist --path "$(TOPDIR)/inst/" --path "$(TOPDIR)/src/" \
	  --eval '${PKG_ADD}'

doctest: all
	$(OCTAVE) --path "$(TOPDIR)/inst" --path "$(TOPDIR)/src" \
	  --eval '${PKG_ADD}' \
	  --eval 'pkg load doctest;' \
	  --eval "targets = {'$(TOPDIR)/inst', '$(TOPDIR)/src'};" \
	  --eval "doctest (targets);"

clean: clean-docs
	$(RM) -r $(TARGET_DIR) fntests.log
	test ! -e src/Makefile || $(MAKE) -C src clean

distclean: clean
	test ! -e src/Makefile || $(MAKE) -C src distclean
