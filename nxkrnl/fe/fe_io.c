#include <fe/fe.h>

void writestr(fe_Context *ctx, fe_WriteFn fn, void *udata, const char *s) {
	while (*s) {
		fn(ctx, udata, *s++);
	}
}

void fe_write(fe_Context *ctx, fe_Object *obj, fe_WriteFn fn, void *udata, int qt) {
	char buf[32];

	switch (type(obj)) {
		case FE_TNIL:
			writestr(ctx, fn, udata, "nil");
			break;

		case FE_TNUMBER:
			sprintf(buf, "%.7g", number(obj));
			writestr(ctx, fn, udata, buf);
			break;

		case FE_TPAIR:
			fn(ctx, udata, '(');
			for (;;) {
				fe_write(ctx, car(obj), fn, udata, 1);
				obj = cdr(obj);
				if (type(obj) != FE_TPAIR) {
					break;
				}
				fn(ctx, udata, ' ');
			}
			if (!isnil(obj)) {
				writestr(ctx, fn, udata, " . ");
				fe_write(ctx, obj, fn, udata, 1);
			}
			fn(ctx, udata, ')');
			break;

		case FE_TSYMBOL:
			fe_write(ctx, car(cdr(obj)), fn, udata, 0);
			break;

		case FE_TSTRING:
			if (qt) { fn(ctx, udata, '"'); }
			while (!isnil(obj)) {
				int i;
				for (i = 0; i < STRBUFSIZE && strbuf(obj)[i]; i++) {
					if (qt && strbuf(obj)[i] == '"') {
						fn(ctx, udata, '\\');
					}
					fn(ctx, udata, strbuf(obj)[i]);
				}
				obj = cdr(obj);
			}
			if (qt) {
				fn(ctx, udata, '"');
			}
			break;

		default:
			sprintf(buf, "[%s %p]", typenames[type(obj)], (void*) obj);
			writestr(ctx, fn, udata, buf);
			break;
	}
}

void write_screen(fe_Context *ctx, void* udata, char chr) {
	unused(ctx);
	kprintf("%c", chr);
}

void writebuf(fe_Context *ctx, void *udata, char chr) {
	CharPtrInt *x = (CharPtrInt*) udata;
	unused(ctx);
	if (x->n) {
		*x->p++ = chr; x->n--;
	}
}

fe_Object* getbound(fe_Object *sym, fe_Object *env) {
	for (; !isnil(env); env = cdr(env)) {
		fe_Object *x = car(env);
		if (car(x) == sym) {
			return x;
		}
	}
	return cdr(sym);
}


void fe_set(fe_Context *ctx, fe_Object *sym, fe_Object *v) {
	unused(ctx);
	cdr(getbound(sym, &nil)) = v;
}

fe_Object rparen;

fe_Object* read_data(fe_Context *ctx, fe_ReadFn fn, void *udata) {
	const char *delimiter = " \n\t\r();";
	fe_Object *v, *res, **tail;
	fe_Number n;
	int chr, gc;
	char buf[64], *p;

	chr = ctx->nextchr ? ctx->nextchr : fn(ctx, udata);
	ctx->nextchr = '\0';

	while (chr && strchr(" \n\t\r", chr)) {
		chr = fn(ctx, udata);
	}

	switch (chr) {
		case '\0':
			return NULL;

		case ';':
			while (chr && chr != '\n') { chr = fn(ctx, udata); }
			return read_data(ctx, fn, udata);

		case ')':
			return &rparen;

		case '(':
			res = &nil;
			tail = &res;
			gc = fe_savegc(ctx);
			fe_pushgc(ctx, res);
			while ( (v = read_data(ctx, fn, udata)) != &rparen ) {
				if (v == NULL) { fe_error(ctx, "unclosed list"); }
				if (type(v) == FE_TSYMBOL && streq(car(cdr(v)), ".")) {
					*tail = fe_read(ctx, fn, udata);
				} else {
					*tail = fe_cons(ctx, v, &nil);
					tail = &cdr(*tail);
				}
				fe_restoregc(ctx, gc);
				fe_pushgc(ctx, res);
			}
			return res;

		case '\'':
			v = fe_read(ctx, fn, udata);
			if (!v) { fe_error(ctx, "stray '''"); }
			return fe_cons(ctx, fe_symbol(ctx, "quote"), fe_cons(ctx, v, &nil));

		case '"':
			res = buildstring(ctx, NULL, '\0');
			v = res;
			chr = fn(ctx, udata);
			while (chr != '"') {
				if (chr == '\0') { fe_error(ctx, "unclosed string"); }
				if (chr == '\\') {
					chr = fn(ctx, udata);
					if (strchr("nrt", chr)) { chr = strchr("n\nr\rt\t", chr)[1]; }
				}
				v = buildstring(ctx, v, chr);
				chr = fn(ctx, udata);
			}
			return res;

		default:
			p = buf;
			do {
				if (p == buf + sizeof(buf) - 1) {
					fe_error(ctx, "symbol too long");
				}
				*p++ = chr;
				chr = fn(ctx, udata);
			} while (chr && !strchr(delimiter, chr));
			*p = '\0';
			ctx->nextchr = chr;
			n = strtod(buf, &p);
			if (p != buf && strchr(delimiter, *p)) {
				return fe_number(ctx, n);
			}
			if (!strcmp(buf, (char*) "nil")) {
				return &nil;
			}
			return fe_symbol(ctx, buf);
	}
}


fe_Object* fe_read(fe_Context *ctx, fe_ReadFn fn, void *udata) {
	fe_Object* obj = read_data(ctx, fn, udata);
	if (obj == &rparen) {
		fe_error(ctx, "stray ')'");
	}
	return obj;
}

fe_Context* fe_open(void *ptr, int size) {
	int i, save;
	fe_Context *ctx;

	ctx = (fe_Context*) ptr;
	memset(ctx, 0, sizeof(fe_Context));
	ptr = (char*) ptr + sizeof(fe_Context);
	size -= sizeof(fe_Context);

	ctx->objects = (fe_Object*) ptr;
	ctx->object_count = size / sizeof(fe_Object);

	ctx->calllist = &nil;
	ctx->freelist = &nil;
	ctx->symlist = &nil;

	for (i = 0; i < ctx->object_count; i++) {
		fe_Object *obj = &ctx->objects[i];
		settype(obj, FE_TFREE);
		cdr(obj) = ctx->freelist;
		ctx->freelist = obj;
	}

	ctx->t = fe_symbol(ctx, "t");
	fe_set(ctx, ctx->t, ctx->t);

	save = fe_savegc(ctx);
	for (i = 0; i < P_MAX; i++) {
		fe_Object *v = object(ctx);
		settype(v, FE_TPRIM);
		prim(v) = i;
		fe_set(ctx, fe_symbol(ctx, primnames[i]), v);
		fe_restoregc(ctx, save);
	}

	return ctx;
}


void fe_close(fe_Context *ctx) {
	ctx->gcstack_idx = 0;
	ctx->symlist = &nil;
	collectgarbage(ctx);
}