#include <fe/fe.h>

void fe_pushgc(fe_Context *ctx, fe_Object *obj) {
	if (ctx->gcstack_idx == GCSTACKSIZE) {
		fe_error(ctx, "gc stack overflow");
	}
	ctx->gcstack[ctx->gcstack_idx++] = obj;
}


void fe_restoregc(fe_Context *ctx, int idx) {
	ctx->gcstack_idx = idx;
}


int fe_savegc(fe_Context *ctx) {
	return ctx->gcstack_idx;
}

void fe_mark(fe_Context *ctx, fe_Object *obj) {
	fe_Object *car;
begin:
	if (tag(obj) & GCMARKBIT) {
		return;
	}
	car = car(obj);
	tag(obj) |= GCMARKBIT;

	switch (type(obj)) {
		case FE_TPAIR:
			fe_mark(ctx, car);
		case FE_TFUNC: case FE_TMACRO: case FE_TSYMBOL: case FE_TSTRING:
			obj = cdr(obj);
			goto begin;

		case FE_TPTR:
			if (ctx->handlers.mark) {
				ctx->handlers.mark(ctx, obj);
			}
			break;
	}
}


void collectgarbage(fe_Context *ctx) {
	int i;

	for (i = 0; i < ctx->gcstack_idx; i++) {
		fe_mark(ctx, ctx->gcstack[i]);
	}
	fe_mark(ctx, ctx->symlist);

	for (i = 0; i < ctx->object_count; i++) {
		fe_Object *obj = &ctx->objects[i];
		if (type(obj) == FE_TFREE) {
			continue;
		}
		if (~tag(obj) & GCMARKBIT) {
			if (type(obj) == FE_TPTR && ctx->handlers.gc) {
				ctx->handlers.gc(ctx, obj);
			}
			settype(obj, FE_TFREE);
			cdr(obj) = ctx->freelist;
			ctx->freelist = obj;
		} else {
			tag(obj) &= ~GCMARKBIT;
		}
	}
}