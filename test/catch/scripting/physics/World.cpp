#include <catch.hpp>

#include <Box2D/Dynamics/b2World.h>

#include <duktape.h>

#include "./test_utils.h"

#include "dukdemo/scripting/util.h"
#include "dukdemo/scripting/World.h"


SCENARIO("Creating a Box2D world", "[scripting::world]")
{
	GIVEN("a duktape context initialised by the world module")
	{
		testutils::duk_context_ptr pContext{duk_create_heap_default()};
		dukdemo::scripting::world::init(pContext.get());

		THEN("the World constructor exists on the global object")
		{
			auto const exists = duk_get_global_string(pContext.get(), "World");
			auto const pFunc = duk_get_c_function(pContext.get(), -1);
			CHECK(pFunc == &dukdemo::scripting::world::constructor);
		}

		THEN("the World constructor has its own prototype")
		{
			auto const exists = duk_get_global_string(pContext.get(), "World");
			REQUIRE(exists);
			auto const hasPrototype = duk_get_prop_string(
				pContext.get(), -1, "prototype");
			REQUIRE(hasPrototype);
		}

		WHEN("we create a world in JS")
		{
			duk_eval_string_noresult(
				pContext.get(), "world = new World([1, 2]);");

			THEN("the gravity is correctly set")
			{
				duk_get_global_string(pContext.get(), "world");
				duk_get_prop_string(
					pContext.get(), -1, dukdemo::scripting::g_ownWorldPtrSym);
				auto* const pWorld = static_cast<b2World*>(
					duk_get_pointer(pContext.get(), -1));
				REQUIRE(bool(pWorld));
				auto const gravity{pWorld->GetGravity()};
				CHECK(gravity.x == Approx(1.0f));
				CHECK(gravity.y == Approx(2.0f));
			}

			THEN("the gravity can be obtained via getGravity()")
			{
				constexpr duk_idx_t vecIdx = 0;
				duk_eval_string(pContext.get(), "world.getGravity([]);");
				REQUIRE(duk_get_top(pContext.get()) == 1);

				// Ensure the result is an appropriate array.
				REQUIRE(duk_is_array(pContext.get(), vecIdx));
				REQUIRE(duk_get_length(pContext.get(), 0) == 2);

				duk_get_prop_index(pContext.get(), vecIdx, 0);
				duk_get_prop_index(pContext.get(), vecIdx, 1);

				auto const x = float(duk_get_number(pContext.get(), -2));
				auto const y = float(duk_get_number(pContext.get(), -1));
				CHECK(x == Approx(1.0f));
				CHECK(y == Approx(2.0f));
			}
		}
	}
}
