#include <memory>
#include <duktape.h>
extern "C" {
	#include <duk_print_alert.h>
}


int main(int argc, char *argv[]) {
	auto pContext = duk_create_heap_default();
	duk_print_alert_init(pContext, 0u);
	if (pContext) {
		duk_eval_string(pContext, "print('Hello world!');");
		duk_destroy_heap(pContext);
	}
	return 0;
}
