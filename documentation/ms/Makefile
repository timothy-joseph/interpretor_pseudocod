build:
	groff -ms implementare.ms -p -s -T ps > implementare.ps
	ps2pdf implementare.ps implementare.pdf

clean:
	rm implementare.ps implementare.pdf

default: build
	mupdf implementare.pdf

