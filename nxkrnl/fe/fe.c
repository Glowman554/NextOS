#include <fe/fe.h>

const char *primnames[] = {
	"let",
	"=",
	"if",
	"fn",
	"mac",
	"while",
	"quote",
	"and",
	"or",
	"do",
	"cons",
	"car",
	"cdr",
	"setcar",
	"setcdr",
	"list",
	"not",
	"is",
	"atom",
	"print",
	"<",
	"<=",
	"+",
	"-",
	"*",
	"/"
};

const char *typenames[] = {
	"pair",
	"free",
	"nil",
	"number",
	"symbol",
	"string",
	"func",
	"macro",
	"prim",
	"cfunc",
	"ptr"
};

fe_Object nil = {{ (fe_Object*) (FE_TNIL << 2 | 1) }, { NULL }};


fe_Handlers* fe_handlers(fe_Context *ctx) {
	return &ctx->handlers;
}