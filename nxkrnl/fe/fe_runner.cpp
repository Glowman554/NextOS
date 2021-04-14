#include <fe/fe_runner.h>

extern "C" void run_fe(char* code) {
	fe_Code code_obj;
	code_obj.code = code;
	code_obj.code_length = strlen(code);
	code_obj.index = 0;

	run_fe(&code_obj);
}

extern "C" bool write_file(char* driver_name, char* file_name, char* file_content);

static fe_Object* f_write_file(fe_Context *ctx, fe_Object *arg) {
	char* driver = (char*) pmm_alloc();
	char* file = (char*) pmm_alloc();
	char* content = (char*) pmm_alloc();
	fe_tostring(ctx, fe_nextarg(ctx, &arg), driver, 4096);
	fe_tostring(ctx, fe_nextarg(ctx, &arg), file, 4096);
	fe_tostring(ctx, fe_nextarg(ctx, &arg), content, 4096);

	bool res = write_file(driver, file, content);

	pmm_free(driver);
	pmm_free(file);
	pmm_free(content);

	return fe_number(ctx, res);
}

void run_fe(fe_Code* code_ptr) {

	char* buf = (char*) pmm_alloc();

	fe_Context *ctx = fe_open(buf, 4096);

	fe_set(ctx, fe_symbol(ctx, "write_file"), fe_cfunc(ctx, f_write_file));

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