#include <catch.hpp>

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>

#include <duktape.h>

#include "./test_utils.h"

#include "dukdemo/scripting/util.h"
#include "dukdemo/scripting/World.h"
#include "dukdemo/scripting/Body.h"


void checkIsBodyInstance(duk_context* pContext, duk_idx_t index)
{
	REQUIRE(duk_is_object(pContext, index));
	duk_get_prototype(pContext, -1);
	CHECK(duk_strict_equals(pContext, index, -1));
}


constexpr char const* const pCreateBodyScript = R"JS(
bodyDef = {
	type: 'static',
	position: [0, 0],
};
body = world.createBody(bodyDef);
)JS";


SCENARIO("Creating Box2D bodies", "[createBody]")
{
	GIVEN("a duktape context and world")
	{
		testutils::duk_context_ptr pContext{duk_create_heap_default()};
		dukdemo::scripting::world::init(pContext.get());
		dukdemo::scripting::body::init(pContext.get());

		b2World world{b2Vec2{0.0f, 0.0f}};
		dukdemo::scripting::world::pushWorldWithoutFinalizer(
			pContext.get(), &world);

		// Put the world on the global object.
		duk_put_global_string(pContext.get(), "world");

		WHEN("we create a body in a script")
		{
			using dukdemo::scripting::g_ownBodyPtrSym;

			// THEN("the object is an instanceof Body")
			// {
			// 	duk_eval_string(pContext.get(), pCreateBodyScript);
			// 	checkIsBodyInstance(pContext.get(), -1);
			// }
			THEN("the body pointer is accessible")
			{
				duk_eval_string(pContext.get(), pCreateBodyScript);
				duk_get_prop_string(pContext.get(), -1, g_ownBodyPtrSym);
				auto const* const pBody = static_cast<b2Body*>(
					duk_get_pointer(pContext.get(), -1));
				REQUIRE(bool(pBody));
			}
			// THEN("the body is inserted into the world")
			// {
			// 	duk_get_prop_string(pContext.get(), -1, g_ownBodyPtrSym);
			// 	auto const* const pBody = static_cast<b2Body*>(
			// 		duk_get_pointer(pContext.get(), -1));
			// 	REQUIRE(bool(pBody));
			// 	CHECK(pBody->GetWorld() == &world);
			// 	CHECK(world.GetBodyList() == pBody);
			// }
		}
	}
}
