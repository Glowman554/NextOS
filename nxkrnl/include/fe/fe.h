#ifndef FE_H
#define FE_H

#include <task.h>
#include <stdio.h>

#define unused(x)			((void) (x) )
#define car(x)				((x)->car.o)
#define cdr(x)				((x)->cdr.o)
#define tag(x)				((x)->car.c)
#define isnil(x)			((x) == &nil)
#define type(x)				(tag(x) & 0x1 ? tag(x) >> 2 : FE_TPAIR)
#define settype(x,t)		(tag(x) = (t) << 2 | 1)
#define number(x)			((x)->cdr.n)
#define prim(x)				((x)->cdr.c)
#define cfunc(x)			((x)->cdr.f)
#define strbuf(x)			(&(x)->car.c + 1)

#define STRBUFSIZE			((int) sizeof(fe_Object*) - 1)
#define GCMARKBIT			(0x2)
#define GCSTACKSIZE			(256)

#define fe_exit(code)	printf_k("EXIT: %d", code); \
						task_exit(code);


#define evalarg() eval(ctx, fe_nextarg(ctx, &arg), env, (fe_Object**) NULL)

#define arithop(op) {														\
		fe_Number x = fe_tonumber(ctx, evalarg());							\
		while (!isnil(arg)) {												\
			x = x op fe_tonumber(ctx, evalarg());			 				\
		}																	\
		res = fe_number(ctx, x);											\
	}

#define numcmpop(op) {														\
		va = checktype(ctx, evalarg(), FE_TNUMBER);							\
		vb = checktype(ctx, evalarg(), FE_TNUMBER);							\
		res = fe_bool(ctx, number(va) op number(vb));						\
	}

typedef float fe_Number;
typedef struct fe_Object fe_Object;
typedef struct fe_Context fe_Context;
typedef fe_Object* (*fe_CFunc)(fe_Context *ctx, fe_Object *args);
typedef void (*fe_ErrorFn)(fe_Context *ctx, const char *err, fe_Object *cl);
typedef void (*fe_WriteFn)(fe_Context *ctx, void *udata, char chr);
typedef char (*fe_ReadFn)(fe_Context *ctx, void *udata);
typedef struct { fe_ErrorFn error; fe_CFunc mark, gc; } fe_Handlers;
typedef union { fe_Object *o; fe_CFunc f; fe_Number n; char c; } Value;
typedef struct { char *p; int n; } CharPtrInt;

struct fe_Object { Value car, cdr; };

struct fe_Context {
	fe_Handlers handlers;
	fe_Object *gcstack[GCSTACKSIZE];
	int gcstack_idx;
	fe_Object *objects;
	int object_count;
	fe_Object *calllist;
	fe_Object *freelist;
	fe_Object *symlist;
	fe_Object *t;
	int nextchr;
};

enum {
	FE_TPAIR,
	FE_TFREE,
	FE_TNIL,
	FE_TNUMBER,
	FE_TSYMBOL,
	FE_TSTRING,
	FE_TFUNC,
	FE_TMACRO,
	FE_TPRIM,
	FE_TCFUNC,
	FE_TPTR
};

enum {
	P_LET,
	P_SET,
	P_IF,
	P_FN,
	P_MAC,
	P_WHILE,
	P_QUOTE,
	P_AND,
	P_OR,
	P_DO,
	P_CONS,
	P_CAR,
	P_CDR,
	P_SETCAR,
	P_SETCDR,
	P_LIST,
	P_NOT,
	P_IS,
	P_ATOM,
	P_PRINT,
	P_LT,
	P_LTE,
	P_ADD,
	P_SUB,
	P_MUL,
	P_DIV,
	P_MAX
};

extern const char *primnames[];
extern const char *typenames[];

extern fe_Object nil;

fe_Object* fe_nextarg(fe_Context *ctx, fe_Object **arg);
fe_Object* checktype(fe_Context *ctx, fe_Object *obj, int type);
int fe_type(fe_Context *ctx, fe_Object *obj);
int fe_isnil(fe_Context *ctx, fe_Object *obj);
int equal(fe_Object *a, fe_Object *b);
int streq(fe_Object *obj, const char *str);
fe_Object* object(fe_Context *ctx);
fe_Object* fe_cons(fe_Context *ctx, fe_Object *car, fe_Object *cdr);
fe_Object* fe_bool(fe_Context *ctx, int b);
fe_Object* fe_number(fe_Context *ctx, fe_Number n);
fe_Object* buildstring(fe_Context *ctx, fe_Object *tail, int chr);
fe_Object* fe_string(fe_Context *ctx, const char *str);
fe_Object* fe_symbol(fe_Context *ctx, const char *name);
fe_Object* fe_cfunc(fe_Context *ctx, fe_CFunc fn);
fe_Object* fe_ptr(fe_Context *ctx, void *ptr);
fe_Object* fe_list(fe_Context *ctx, fe_Object **objs, int n);
fe_Object* fe_car(fe_Context *ctx, fe_Object *obj);
fe_Object* fe_cdr(fe_Context *ctx, fe_Object *obj);
int fe_tostring(fe_Context *ctx, fe_Object *obj, char *dst, int size);
fe_Number fe_tonumber(fe_Context *ctx, fe_Object *obj);
void* fe_toptr(fe_Context *ctx, fe_Object *obj);

void fe_pushgc(fe_Context *ctx, fe_Object *obj);
void fe_restoregc(fe_Context *ctx, int idx);
int fe_savegc(fe_Context *ctx);
void fe_mark(fe_Context *ctx, fe_Object *obj);
void collectgarbage(fe_Context *ctx);
void writestr(fe_Context *ctx, fe_WriteFn fn, void *udata, const char *s);
void fe_write(fe_Context *ctx, fe_Object *obj, fe_WriteFn fn, void *udata, int qt);
void write_screen(fe_Context *ctx, void* udata, char chr);
void writebuf(fe_Context *ctx, void *udata, char chr);
fe_Object* getbound(fe_Object *sym, fe_Object *env);
void fe_set(fe_Context *ctx, fe_Object *sym, fe_Object *v);
fe_Object* read_data(fe_Context *ctx, fe_ReadFn fn, void *udata);
fe_Object* fe_read(fe_Context *ctx, fe_ReadFn fn, void *udata);

fe_Object* evallist(fe_Context *ctx, fe_Object *lst, fe_Object *env);
fe_Object* dolist(fe_Context *ctx, fe_Object *lst, fe_Object *env);
fe_Object* argstoenv(fe_Context *ctx, fe_Object *prm, fe_Object *arg, fe_Object *env);
fe_Object* eval(fe_Context *ctx, fe_Object *obj, fe_Object *env, fe_Object **newenv);
fe_Object* fe_eval(fe_Context *ctx, fe_Object *obj);

void fe_error(fe_Context *ctx, const char *msg);

fe_Handlers* fe_handlers(fe_Context *ctx);

#endif