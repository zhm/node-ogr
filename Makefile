NODE_PATH := $(shell pwd)/lib
REPORTER  ?= list
TESTS     ?= test/*.coffee

all: build

configure:
	node-gyp configure

build:
	node-gyp build

clean:
	node-gyp clean
	rm -rf build

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

.PHONY: configure clean build test test-docs
