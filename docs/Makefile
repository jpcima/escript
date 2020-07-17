TEXI2HTML = makeinfo --html --no-split
TEXI2HTML_FLAGS =

all: index.html

clean:
	rm -f index.html

auto:
	while true; do $(TEXI2HTML) $(TEXI2HTML_FLAGS) index.texi; \
	    inotifywait -q -e close_write index.texi; done

%.html: %.texi
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) $<

.PHONY: all clean auto
