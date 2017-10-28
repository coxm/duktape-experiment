#include <memory>

#include <duktape.h>

#include "dukdemo/util/deleters.h"


namespace testutils {


using duk_context_ptr =
	std::unique_ptr<duk_context, dukdemo::util::DukContextDeleter>;


} // namespace testutils
