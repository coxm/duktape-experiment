#include <catch.hpp>

#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2EdgeShape.h>
#include <Box2D/Collision/Shapes/b2ChainShape.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <duktape.h>

#include "dukdemo/scripting/loaders.h"

#include "./test_utils.h"


namespace ds = dukdemo::scripting;


constexpr char const* const pFullFixtureDefJSON = R"JSON({
	"friction": 1.23,
	"restitution": 4.56,
	"density": 7.89,
	"categoryBits": 123,
	"maskBits": 456,
	"groupIndex": 789
})JSON";


auto const checkFullyWrittenFixtureDef = [](b2FixtureDef const& fixtureDef) {
	CHECK(fixtureDef.friction == Approx(1.23f));
	CHECK(fixtureDef.restitution == Approx(4.56));
	CHECK(fixtureDef.density == Approx(7.89));
	CHECK(fixtureDef.filter.categoryBits == 123);
	CHECK(fixtureDef.filter.maskBits == 456);
	CHECK(fixtureDef.filter.groupIndex == 789);
};


SCENARIO("Loading fixture defs from duktape", "[loadFixtureDefWithoutShape]")
{
	GIVEN("a duktape context")
	{
		testutils::duk_context_ptr pContext{duk_create_heap_default()};
		// Push some junk to the value stack.
		duk_push_number(pContext.get(), 123);
		duk_push_string(pContext.get(), "dummy test data");

		auto const pushFixtureDefObject = [pCtx = pContext.get()]() {
			duk_push_string(pCtx, pFullFixtureDefJSON);
			duk_json_decode(pCtx, -1);
		};

		b2FixtureDef fixtureDef;

		WHEN("loading a full b2FixtureDef")
		{
			THEN("true is returned on success")
			{
				pushFixtureDefObject();
				bool const result = ds::loadFixtureDefWithoutShape(
					pContext.get(), -1, &fixtureDef);
				REQUIRE(result);
			}

			THEN("false is returned if given a non-object")
			{
				bool const result = ds::loadFixtureDefWithoutShape(
					pContext.get(), -1, &fixtureDef);
				CHECK(!result);
			}

			THEN("all values are loaded")
			{
				pushFixtureDefObject();
				ds::loadFixtureDefWithoutShape(
					pContext.get(), -1, &fixtureDef);
				checkFullyWrittenFixtureDef(fixtureDef);
			}
		}

		WHEN("loading from the middle of the stack")
		{
			duk_push_int(pContext.get(), 0xdead);
			duk_push_int(pContext.get(), 0xbeef);
			pushFixtureDefObject();
			auto const objectIdx = duk_normalize_index(pContext.get(), -1);
			duk_push_int(pContext.get(), 0xdead);
			duk_push_int(pContext.get(), 0xbeef);

			THEN("using a negative index loads correctly")
			{
				auto const success = ds::loadFixtureDefWithoutShape(
					pContext.get(), -3, &fixtureDef);
				REQUIRE(success);
				checkFullyWrittenFixtureDef(fixtureDef);
			}
			THEN("using a non-negative index loads correctly")
			{
				auto const success = ds::loadFixtureDefWithoutShape(
					pContext.get(), objectIdx, &fixtureDef);
				REQUIRE(success);
				checkFullyWrittenFixtureDef(fixtureDef);
			}
		}

		WHEN("using a partial definition")
		{
			fixtureDef.friction = 0.7f;
			fixtureDef.isSensor = true;
			fixtureDef.filter.maskBits = 0xeeee;
			fixtureDef.filter.groupIndex = 16;
			duk_push_string(pContext.get(), R"JSON({
				"restitution": 4.56,
				"density": 7.89,
				"categoryBits": 123
			})JSON");
			duk_json_decode(pContext.get(), -1);
			ds::loadFixtureDefWithoutShape(pContext.get(), -1, &fixtureDef);

			THEN("the defined properties are used")
			{
				CHECK(fixtureDef.restitution == Approx(4.56));
				CHECK(fixtureDef.density == Approx(7.89));
				CHECK(fixtureDef.filter.categoryBits == 123);
			}

			THEN("the existing properties remain")
			{
				// Check defaults are still there.
				CHECK(fixtureDef.friction == Approx(0.7f));
				CHECK(fixtureDef.isSensor == true);
				CHECK(fixtureDef.filter.maskBits == 0xeeee);
				CHECK(fixtureDef.filter.groupIndex == 16);
			}
		}
	}
}
