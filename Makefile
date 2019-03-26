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

PACKAGE := $(shell $(GREP) "^Name: " DESCRIPTION | $(CUT) -f2 -d" " | \
$(TR) '[:upper:]' '[:lower:]')
VERSION := $(shell $(GREP) "^Version: " DESCRIPTION | $(CUT) -f2 -d" ")

TARGET_DIR      := target
RELEASE_DIR     := $(TARGET_DIR)/$(PACKAGE)-$(VERSION)
RELEASE_TARBALL := $(TARGET_DIR)/$(PACKAGE)-$(VERSION).tar.gz
HTML_DIR        := $(TARGET_DIR)/$(PACKAGE)-html
HTML_TARBALL    := $(TARGET_DIR)/$(PACKAGE)-html.tar.gz

M_SOURCES   := $(wildcard inst/*.m)
CC_SOURCES  := $(wildcard src/*.cpp)
CC_TST_SOURCES := $(shell $(GREP) --files-with-matches '^%!' $(CC_SOURCES))
TST_SOURCES := $(patsubst src/%.cpp,inst/test/%.cpp-tst,$(CC_TST_SOURCES))
OCT_FILES   := $(patsubst %.cpp,%.oct,$(CC_SOURCES))
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
	$(TAR) -c -f - --posix -C "$(TARGET_DIR)/" "$(notdir $<)" | $(GZIP) -9n > "$@"

$(RELEASE_DIR): .hg/dirstate
	@echo "Creating package version $(VERSION) release ..."
	$(RM) -r "$@"
	hg archive --exclude ".hg*" --type files "$@"
	cd "$@/src" && ./bootstrap && $(RM) -r "autom4te.cache"
	# need to build any tests
	cd "$@" && $(MAKE) test_files
	chmod -R a+rX,u+w,go-w "$@"

$(HTML_DIR): install
	@echo "Generating HTML documentation. This may take a while ..."
	$(RM) -r "$@"
	$(OCTAVE) --no-window-system --silent \
	  --eval "pkg load generate_html; " \
	  --eval "pkg load $(PACKAGE);" \
	  --eval 'generate_package_html ("${PACKAGE}", "$@", "octave-forge");'
	chmod -R a+rX,u+w,go-w $@

# test file recipes
$(TST_SOURCES): inst/test/%.cpp-tst: src/%.cpp | inst/test
	@echo "Extracting tests from $< ..."
	@$(RM) -f "$@" "$@-t"
	@(	echo "## Generated from $<"; \
                $(GREP) '^%!' "$<") > "$@"

inst/test:
	@mkdir -p "$@"

test_files: $(TST_SOURCES)

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

check: all test_files
	$(OCTAVE) --path "$(TOPDIR)/inst/" --path "$(TOPDIR)/src/" \
	  --eval '${PKG_ADD}' \
	  --eval '__run_test_suite__ ({"$(TOPDIR)/inst"}, {})'

run: all
	$(OCTAVE) --persist --path "inst/" --path "src/" \
	  --eval '${PKG_ADD}'

clean:
	$(RM) -r $(TARGET_DIR) fntests.log
	test ! -e src/Makefile || $(MAKE) -C src clean

distclean: clean
	test ! -e src/Makefile || $(MAKE) -C src distclean
