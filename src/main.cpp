#include <memory>
#include <chrono>
#include <thread>

#include <duktape.h>
extern "C" {
	#include <duk_print_alert.h>
}

#include "dukdemo/logging.h" // easylogging++ wrapper.
#include "dukdemo/render/Context.h"


INITIALIZE_EASYLOGGINGPP


void
render() noexcept
{
}


int
main(int argc, char *argv[]) {
	auto pContext = duk_create_heap_default();
	duk_print_alert_init(pContext, 0u);
	if (pContext) {
		duk_eval_string(pContext, "print('Hello world!');");
		duk_destroy_heap(pContext);
	}

	dukdemo::render::Context renderContext{"DukDemo", render};
	renderContext.render();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 0;
}
