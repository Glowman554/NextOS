#include <fe/fe.h>

fe_Object* checktype(fe_Context *ctx, fe_Object *obj, int type) {
	char buf[64];
	if (type(obj) != type) {
		sprintf(buf, "expected %s, got %s", typenames[type], typenames[type(obj)]);
		fe_error(ctx, buf);
	}
	return obj;
}

int fe_type(fe_Context *ctx, fe_Object *obj) {
	unused(ctx);
	return type(obj);
}


int fe_isnil(fe_Context *ctx, fe_Object *obj) {
	unused(ctx);
	return isnil(obj);
}

int equal(fe_Object *a, fe_Object *b) {
	if (a == b) {
		return 1;
	}
	if (type(a) != type(b)) {
		return 0;
	}
	if (type(a) == FE_TNUMBER) {
		return number(a) == number(b);
	}
	if (type(a) == FE_TSTRING) {
		for (; !isnil(a); a = cdr(a), b = cdr(b)) {
			if (car(a) != car(b)) { return 0; }
		}
		return a == b;
	}
	return 0;
}


int streq(fe_Object *obj, const char *str) {
	while (!isnil(obj)) {
		int i;
		for (i = 0; i < STRBUFSIZE; i++) {
			if (strbuf(obj)[i] != *str) {
				return 0;
			}
			if (*str) {
				str++;
			}
		}
		obj = cdr(obj);
	}
	return *str == '\0';
}


fe_Object* object(fe_Context *ctx) {
	fe_Object *obj;
	if (isnil(ctx->freelist)) {
		collectgarbage(ctx);
		if (isnil(ctx->freelist)) {
			fe_error(ctx, "out of memory");
		}
	}
	obj = ctx->freelist;
	ctx->freelist = cdr(obj);
	fe_pushgc(ctx, obj);
	return obj;
}


fe_Object* fe_cons(fe_Context *ctx, fe_Object *car, fe_Object *cdr) {
	fe_Object *obj = object(ctx);
	car(obj) = car;
	cdr(obj) = cdr;
	return obj;
}


fe_Object* fe_bool(fe_Context *ctx, int b) {
	return b ? ctx->t : &nil;
}


fe_Object* fe_number(fe_Context *ctx, fe_Number n) {
	fe_Object *obj = object(ctx);
	settype(obj, FE_TNUMBER);
	number(obj) = n;
	return obj;
}


fe_Object* buildstring(fe_Context *ctx, fe_Object *tail, int chr) {
	if (!tail || strbuf(tail)[STRBUFSIZE - 1] != '\0') {
		fe_Object *obj = fe_cons(ctx, NULL, &nil);
		settype(obj, FE_TSTRING);
		if (tail) {
			cdr(tail) = obj;
			ctx->gcstack_idx--;
		}
		tail = obj;
	}
	strbuf(tail)[strlen(strbuf(tail))] = chr;
	return tail;
}


fe_Object* fe_string(fe_Context *ctx, const char *str) {
	fe_Object *obj = buildstring(ctx, NULL, '\0');
	fe_Object *tail = obj;
	while (*str) {
		tail = buildstring(ctx, tail, *str++);
	}
	return obj;
}


fe_Object* fe_symbol(fe_Context *ctx, const char *name) {
	fe_Object *obj;

	for (obj = ctx->symlist; !isnil(obj); obj = cdr(obj)) {
		if (streq(car(cdr(car(obj))), name)) {
			return car(obj);
		}
	}

	obj = object(ctx);
	settype(obj, FE_TSYMBOL);
	cdr(obj) = fe_cons(ctx, fe_string(ctx, name), &nil);
	ctx->symlist = fe_cons(ctx, obj, ctx->symlist);
	return obj;
}


fe_Object* fe_cfunc(fe_Context *ctx, fe_CFunc fn) {
	fe_Object *obj = object(ctx);
	settype(obj, FE_TCFUNC);
	cfunc(obj) = fn;
	return obj;
}


fe_Object* fe_ptr(fe_Context *ctx, void *ptr) {
	fe_Object *obj = object(ctx);
	settype(obj, FE_TPTR);
	cdr(obj) = (fe_Object*) ptr;
	return obj;
}


fe_Object* fe_list(fe_Context *ctx, fe_Object **objs, int n) {
	fe_Object *res = &nil;
	while (n--) {
		res = fe_cons(ctx, objs[n], res);
	}
	return res;
}


fe_Object* fe_car(fe_Context *ctx, fe_Object *obj) {
	if (isnil(obj)) {
		return obj;
	}
	return car(checktype(ctx, obj, FE_TPAIR));
}


fe_Object* fe_cdr(fe_Context *ctx, fe_Object *obj) {
	if (isnil(obj)) {
		return obj;
	}
	return cdr(checktype(ctx, obj, FE_TPAIR));
}

int fe_tostring(fe_Context *ctx, fe_Object *obj, char *dst, int size) {
	CharPtrInt x;
	x.p = dst;
	x.n = size - 1;
	fe_write(ctx, obj, writebuf, &x, 0);
	*x.p = '\0';
	return size - x.n - 1;
}


fe_Number fe_tonumber(fe_Context *ctx, fe_Object *obj) {
	return number(checktype(ctx, obj, FE_TNUMBER));
}


void* fe_toptr(fe_Context *ctx, fe_Object *obj) {
	return cdr(checktype(ctx, obj, FE_TPTR));
}
