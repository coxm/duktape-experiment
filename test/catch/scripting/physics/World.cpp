#include <catch.hpp>

#include <Box2D/Dynamics/b2World.h>

#include <duktape.h>

#include "./test_utils.h"

#include "dukdemo/scripting/util.h"
#include "dukdemo/scripting/World.h"


void checkIsWorldInstance(duk_context* pContext, duk_idx_t index)
{
	REQUIRE(duk_is_object(pContext, index));
	duk_get_global_string(pContext, "World");
	REQUIRE(duk_instanceof(pContext, index, -1));
}


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
			duk_eval_string(pContext.get(), "world = new World([1, 2]);");

			THEN("the object is an instanceof World")
			{
				REQUIRE(duk_is_object(pContext.get(), -1));
				duk_get_global_string(pContext.get(), "World");
				REQUIRE(duk_instanceof(pContext.get(), -2, -1));
			}

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
				auto const vecIdx = duk_get_top(pContext.get());
				duk_eval_string(pContext.get(), "world.getGravity([]);");

				// Ensure the result is an appropriate array.
				REQUIRE(duk_is_array(pContext.get(), vecIdx));
				REQUIRE(duk_get_length(pContext.get(), vecIdx) == 2);

				duk_get_prop_index(pContext.get(), vecIdx, 0);
				duk_get_prop_index(pContext.get(), vecIdx, 1);

				auto const x = float(duk_get_number(pContext.get(), -2));
				auto const y = float(duk_get_number(pContext.get(), -1));
				CHECK(x == Approx(1.0f));
				CHECK(y == Approx(2.0f));
			}
		}

		WHEN("we use pushWorldWithFinalizer")
		{
			using dukdemo::scripting::world::pushWorldWithFinalizer;
			auto const index = pushWorldWithFinalizer(
				pContext.get(),
				std::make_unique<b2World>(b2Vec2{0.0f, 0.0f})
			);

			THEN("a World instance is constructed")
			{
				checkIsWorldInstance(pContext.get(), index);
			}
			THEN("the world is pushed to the stack top")
			{
				CHECK(index + 1 == duk_get_top(pContext.get()));
			}
			THEN("the world has a finalizer")
			{
				dukdemo::scripting::world::pushWorldWithFinalizer(
					pContext.get(),
					std::make_unique<b2World>(b2Vec2{0.0f, 0.0f})
				);
				duk_get_finalizer(pContext.get(), index);
				auto* const pFunc = duk_get_c_function(pContext.get(), -1);
				REQUIRE(bool(pFunc));
			}
		}

		WHEN("we use pushWorldWithoutFinalizer")
		{
			using dukdemo::scripting::world::pushWorldWithoutFinalizer;
			b2World world{b2Vec2{0.0f, 0.0f}};
			auto const index = pushWorldWithoutFinalizer(
				pContext.get(), &world);

			THEN("a World instance is constructed")
			{
				REQUIRE(duk_is_object(pContext.get(), index));
				duk_get_global_string(pContext.get(), "World");
				REQUIRE(duk_instanceof(pContext.get(), index, -1));
			}
			THEN("the world is pushed to the stack top")
			{
				CHECK(index + 1 == duk_get_top(pContext.get()));
			}
			THEN("the world does not have a finalizer")
			{
				duk_get_finalizer(pContext.get(), index);
				auto* const pFunc = duk_get_c_function(pContext.get(), -1);
				REQUIRE(!bool(pFunc));
			}
		}
	}
}
