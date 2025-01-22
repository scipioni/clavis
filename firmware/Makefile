.PHONY: clean 

all: tangled/zmk/miryoku_layer_alternatives.h

tangled/zmk/miryoku_layer_alternatives.h: miryoku.org
	emacs --batch -Q -L org-mode/lisp -f package-initialize -eval "(require 'ob-python)" -eval "(setq org-confirm-babel-evaluate nil)" -eval "(setq python-indent-guess-indent-offset-verbose nil)" ./miryoku.org -f org-babel-tangle
	 rsync -av  tangled/zmk/ zmk/miryoku/miryoku_babel/

clean:
	rm -fR tangled
