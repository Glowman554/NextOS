#include <fe/fe_runner.h>

extern "C" void run_fe(char* code) {
	fe_Code code_obj;
	code_obj.code = code;
	code_obj.code_length = strlen(code);
	code_obj.index = 0;

	run_fe(&code_obj);
}

void run_fe(fe_Code* code_ptr) {

	char* buf = (char*) pmm_alloc();

	fe_Context *ctx = fe_open(buf, 4096);

	int gc = fe_savegc(ctx);

	while (true) {
		fe_Object *obj = read_data(ctx, [](fe_Context* ctx, void* udata) {

			fe_Code* code = (fe_Code*) udata;

			char tmp = code->code[code->index];

			code->index++;
			if(code->index > (int) code->code_length) {
				return '\0';
			} else {
				return tmp;
			}
		}, code_ptr);

		if (!obj) {
			break;
		}

		fe_eval(ctx, obj);
		fe_restoregc(ctx, gc);
	}

	fe_close(ctx);
	pmm_free(buf);
}