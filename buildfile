./: {*/ -build/} doc{README.md} legal{LICENSE} manifest

# Don't install tests.
#
tests/: install = false
