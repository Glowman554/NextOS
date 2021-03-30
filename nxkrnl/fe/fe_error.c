#include <fe/fe.h>

void fe_error(fe_Context *ctx, const char *msg) {
	fe_Object *cl = ctx->calllist;

	ctx->calllist = &nil;

	if (ctx->handlers.error) {
		ctx->handlers.error(ctx, msg, cl);
	}
	
	printf_k("error: %s\n", msg);
	for (; !isnil(cl); cl = cdr(cl)) {
		char buf[64];
		fe_tostring(ctx, car(cl), buf, sizeof(buf));
		printf_k("=> %s\n", buf);
	}
	fe_exit(-0xf00dbabe);
}