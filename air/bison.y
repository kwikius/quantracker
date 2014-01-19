%{
/* simple parser */
%}

%token   NAME INTEGER FLOAT EQUALS BECOMES TRUE FALSE

%%

script :
   lexprlist
;

lexprlist :
   lexpr 
|  lexprlist lexpr
;

lexpr :
   assignment
;

assignment :
  NAME op rexpr ';'
;

op :
  EQUALS
| BECOMES
;

number:
  FLOAT
| INTEGER
;

rexpr:
  NAME
| number
| boolean
;

boolean :
  TRUE
| FALSE
;

%%