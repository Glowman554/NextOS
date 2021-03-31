#include <fe/fe.h>

fe_Object* evallist(fe_Context *ctx, fe_Object *lst, fe_Object *env) {
	fe_Object *res = &nil;
	fe_Object **tail = &res;
	while (!isnil(lst)) {
		*tail = fe_cons(ctx, eval(ctx, fe_nextarg(ctx, &lst), env, NULL), &nil);
		tail = &cdr(*tail);
	}
	return res;
}


fe_Object* dolist(fe_Context *ctx, fe_Object *lst, fe_Object *env) {
	fe_Object *res = &nil;
	int save = fe_savegc(ctx);
	while (!isnil(lst)) {
		fe_restoregc(ctx, save);
		fe_pushgc(ctx, lst);
		fe_pushgc(ctx, env);
		res = eval(ctx, fe_nextarg(ctx, &lst), env, &env);
	}
	return res;
}


fe_Object* argstoenv(fe_Context *ctx, fe_Object *prm, fe_Object *arg, fe_Object *env) {
	while (!isnil(prm)) {
		if (type(prm) != FE_TPAIR) {
			env = fe_cons(ctx, fe_cons(ctx, prm, arg), env);
			break;
		}
		env = fe_cons(ctx, fe_cons(ctx, car(prm), fe_car(ctx, arg)), env);
		prm = cdr(prm);
		arg = fe_cdr(ctx, arg);
	}
	return env;
}

fe_Object* eval(fe_Context *ctx, fe_Object *obj, fe_Object *env, fe_Object **newenv) {
	fe_Object *fn, *arg, *res;
	fe_Object cl, *va, *vb;
	int n, gc;

	if (type(obj) == FE_TSYMBOL) {
		return cdr(getbound(obj, env));
	}
	if (type(obj) != FE_TPAIR) {
		return obj;
	}

	car(&cl) = obj, cdr(&cl) = ctx->calllist;
	ctx->calllist = &cl;

	gc = fe_savegc(ctx);
	fn = eval(ctx, car(obj), env, NULL);
	arg = cdr(obj);
	res = &nil;

	switch (type(fn)) {
		case FE_TPRIM:
			switch (prim(fn)) {
				case P_LET:
					va = checktype(ctx, fe_nextarg(ctx, &arg), FE_TSYMBOL);
					if (newenv) {
						*newenv = fe_cons(ctx, fe_cons(ctx, va, evalarg()), env);
					}
					break;

				case P_SET:
					va = checktype(ctx, fe_nextarg(ctx, &arg), FE_TSYMBOL);
					cdr(getbound(va, env)) = evalarg();
					break;

				case P_IF:
					while (!isnil(arg)) {
						va = evalarg();
						if (!isnil(va)) {
							res = isnil(arg) ? va : evalarg();
							break;
						}
						if (isnil(arg)) {
							break;
						}
						arg = cdr(arg);
					}
					break;

				case P_FN: case P_MAC:
					va = fe_cons(ctx, env, arg);
					fe_nextarg(ctx, &arg);
					res = object(ctx);
					settype(res, prim(fn) == P_FN ? FE_TFUNC : FE_TMACRO);
					cdr(res) = va;
					break;

				case P_WHILE:
					va = fe_nextarg(ctx, &arg);
					n = fe_savegc(ctx);
					while (!isnil(eval(ctx, va, env, NULL))) {
						dolist(ctx, arg, env);
						fe_restoregc(ctx, n);
					}
					break;

				case P_QUOTE:
					res = fe_nextarg(ctx, &arg);
					break;

				case P_AND:
					while (!isnil(arg) && !isnil(res = evalarg()));
					break;

				case P_OR:
					while (!isnil(arg) && isnil(res = evalarg()));
					break;

				case P_DO:
					res = dolist(ctx, arg, env);
					break;

				case P_CONS:
					va = evalarg();
					res = fe_cons(ctx, va, evalarg());
					break;

				case P_CAR:
					res = fe_car(ctx, evalarg());
					break;

				case P_CDR:
					res = fe_cdr(ctx, evalarg());
					break;

				case P_SETCAR:
					va = checktype(ctx, evalarg(), FE_TPAIR);
					car(va) = evalarg();
					break;

				case P_SETCDR:
					va = checktype(ctx, evalarg(), FE_TPAIR);
					cdr(va) = evalarg();
					break;

				case P_LIST:
					res = evallist(ctx, arg, env);
					break;

				case P_NOT:
					res = fe_bool(ctx, isnil(evalarg()));
					break;

				case P_IS:
					va = evalarg();
					res = fe_bool(ctx, equal(va, evalarg()));
					break;

				case P_ATOM:
					res = fe_bool(ctx, fe_type(ctx, evalarg()) != FE_TPAIR);
					break;

				case P_PRINT:
					while (!isnil(arg)) {
						fe_write(ctx, evalarg(), write_screen, 0, 0);
						if (!isnil(arg)) {
							kprintf(" ");
						}
					}
					kprintf("\n");
					break;

				case P_LT: numcmpop(<); break;
				case P_LTE: numcmpop(<=); break;
				case P_ADD: arithop(+); break;
				case P_SUB: arithop(-); break;
				case P_MUL: arithop(*); break;
				case P_DIV: arithop(/); break;
			}
			break;

		case FE_TCFUNC:
			res = cfunc(fn)(ctx, evallist(ctx, arg, env));
			break;

		case FE_TFUNC:
			arg = evallist(ctx, arg, env);
			va = cdr(fn);
			vb = cdr(va);
			res = dolist(ctx, cdr(vb), argstoenv(ctx, car(vb), arg, car(va)));
			break;

		case FE_TMACRO:
			va = cdr(fn);
			vb = cdr(va);
			*obj = *dolist(ctx, cdr(vb), argstoenv(ctx, car(vb), arg, car(va)));
			fe_restoregc(ctx, gc);
			ctx->calllist = cdr(&cl);
			return eval(ctx, obj, env, NULL);

		default:
			fe_error(ctx, "tried to call non-callable value");
	}

	fe_restoregc(ctx, gc);
	fe_pushgc(ctx, res);
	ctx->calllist = cdr(&cl);
	return res;
}


fe_Object* fe_eval(fe_Context *ctx, fe_Object *obj) {
	return eval(ctx, obj, &nil, NULL);
}

