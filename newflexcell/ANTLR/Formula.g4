grammar Formula;

// Лексика
NUMBER: [0-9]+ ('.' [0-9]+)?;
STRING: '"' ~["]* '"';
CELL_REF: [A-Z]+ [0-9]+;
RANGE: CELL_REF ':' CELL_REF;
TRUE: 'TRUE';
FALSE: 'FALSE';
ERROR: '#' [A-Z/!]+;
ID: [A-Za-z]+;
PLUS: '+';
MINUS: '-';
MULT: '*';
DIV: '/';
EQ: '=';
GT: '>';
LT: '<';
GTE: '>=';
LTE: '<=';
LPAREN: '(';
RPAREN: ')';
COMMA: ',';
WS: [ \t\r\n]+ -> skip;

// Основное правило
formula: EQ expr EOF;

// С выражениями по приоритету
expr
    : <assoc=right> expr EQ expr       # eqExpr
    | expr GTE expr                    # gteExpr
    | expr LTE expr                    # lteExpr
    | expr GT expr                     # gtExpr
    | expr LT expr                     # ltExpr
    | expr PLUS expr                   # plusExpr
    | expr MINUS expr                  # minusExpr
    | expr MULT expr                   # multExpr
    | expr DIV expr                    # divExpr
    | ID LPAREN expr (COMMA expr)* RPAREN # funcExpr
    | CELL_REF                         # cellRefExpr
    | RANGE                            # rangeExpr
    | NUMBER                           # numberExpr
    | STRING                           # stringExpr
    | TRUE                             # trueExpr
    | FALSE                            # falseExpr
    | ERROR                            # errorExpr
    | LPAREN expr RPAREN               # parenExpr;
