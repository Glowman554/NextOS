#include <fe/fe.h>

fe_Object* fe_nextarg(fe_Context *ctx, fe_Object **arg) {
	fe_Object *a = *arg;
	if (type(a) != FE_TPAIR) {
		if (isnil(a)) {
			fe_error(ctx, "too few arguments");
		}
		fe_error(ctx, "dotted pair in argument list");
	}
	*arg = cdr(a);
	return car(a);
}