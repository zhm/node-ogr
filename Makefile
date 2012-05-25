NODE_PATH := $(shell pwd)/lib
REPORTER  ?= list
TESTS     ?= test/*.coffee

all: build install

install:
	@node-waf install

configure:
	node-waf configure

build:
	node-waf build

clean:
	@node-waf clean distclean
	@rm -rf ./projects/makefiles/

uninstall:
	@node-waf uninstall

test:
	mocha \
	--reporter $(REPORTER) \
	--require should \
	--compilers coffee:coffee-script \
	$(TESTS)

test-list:
	make test REPORTER=list

test-docs:
	make test REPORTER=doc \
			| cat docs/head.html - docs/tail.html \
			> docs/test.html

lint:
	@./node_modules/.bin/jshint lib/*js test/*js

.PHONY: configure clean build install test test-docs
