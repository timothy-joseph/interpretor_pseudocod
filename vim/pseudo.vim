syn keyword Statement	atunci executa subprogramul face returneaza
syn keyword Conditional	daca altfel atunci si sau not
syn keyword Repeat	cat timp repeta pana cand pentru

" syn keyword pythonInclude	from import

syn match   Comment	    "#.*$" contains=Todo,@Spell
syn keyword Todo		FIXME NOTE NOTES TODO XXX contained

syn region  String      start=+[uU]\=\z(['"]\)+ end="\z1" skip="\\\\\|\\\z1"
                        \ contains=Special,@Spell
syn match   Special	    +\\[abfnrtv'"\\]+ contained

syn match   Special     "<-"

syn match   Number	    "[0-9]\+\.\?[0-9]*"

syn keyword Function    vector dimensiune floor sqrt citestetot citeste scrie tip schimbare_tip referinta cast

syn keyword Special     sursa
