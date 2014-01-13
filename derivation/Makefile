.PHONY: all clean cleanxx

all: derivation.pdf
%.pdf: %.dvi
	@dvipdf $^
derivation.dvi: derivation.tex
	@latex $^
	@latex $^
	@latex $^
clean:
	@rm -rf *.aux *.bbl *.blg *.log *.synctex *.tex.bak *.dvi *.out *.toc
cleanxx: clean
	@rm -rf *.pdf
