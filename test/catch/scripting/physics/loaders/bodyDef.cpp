#include <cstring>

#include <catch.hpp>

#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>

#include <duktape.h>

#include "dukdemo/scripting/loaders.h"

#include "../test_utils.h"


namespace ds = dukdemo::scripting;


constexpr char const* const pFullBodyDefJSON = R"JSON({
	"type": 1,
	"position": [1, 2],
	"angle": 3.21,
	"linearVelocity": [8, 9],
	"angularVelocity": 1.2345,
	"linearDamping": 0.0123,
	"angularDamping": 1.4,
	"allowSleep": false,
	"awake": false,
	"fixedRotation": true,
	"bullet": true,
	"active": false,
	"gravityScale": 10
})JSON";


auto const checkFullyWrittenBodyDef = [](b2BodyDef const& bodyDef) {
	CHECK(bodyDef.type == b2_kinematicBody);
	CHECK(bodyDef.position.x == Approx(1));
	CHECK(bodyDef.position.y == Approx(2));
	CHECK(bodyDef.angle == Approx(3.21));
	CHECK(bodyDef.linearVelocity.x == Approx(8));
	CHECK(bodyDef.linearVelocity.y == Approx(9));
	CHECK(bodyDef.angularVelocity == Approx(1.2345));
	CHECK(bodyDef.linearDamping == Approx(0.0123));
	CHECK(bodyDef.angularDamping == Approx(1.4));
	CHECK(bodyDef.allowSleep == false);
	CHECK(bodyDef.awake == false);
	CHECK(bodyDef.fixedRotation == true);
	CHECK(bodyDef.bullet == true);
	CHECK(bodyDef.active == false);
	CHECK(bodyDef.gravityScale == Approx(10.0f));
};


SCENARIO("When loading body defs from duktape", "[loadBodyDef]")
{
	GIVEN("a duktape context")
	{
		testutils::duk_context_ptr pContext{duk_create_heap_default()};
		// Push some junk to the value stack.
		duk_push_number(pContext.get(), 123);
		duk_push_string(pContext.get(), "dummy test data");

		auto const pushBodyDefObject = [pCtx = pContext.get()]() {
			duk_push_string(pCtx, pFullBodyDefJSON);
			duk_json_decode(pCtx, -1);
		};

		b2BodyDef bodyDef;

		WHEN("loading a full b2BodyDef")
		{
			THEN("true is returned on success")
			{
				pushBodyDefObject();
				bool const result = ds::loadBodyDef(
					pContext.get(), -1, &bodyDef);
				REQUIRE(result);
			}

			THEN("all values are loaded")
			{
				pushBodyDefObject();
				ds::loadBodyDef(pContext.get(), -1, &bodyDef);
				checkFullyWrittenBodyDef(bodyDef);
			}
		}

		WHEN("given an invalid JS value")
		{
			THEN("false is returned if given a non-object")
			{
				duk_push_number(pContext.get(), 123);
				bool const result = ds::loadBodyDef(
					pContext.get(), -1, &bodyDef);
				CHECK(!result);
			}

			THEN("the body def is not modified")
			{
				duk_push_string(pContext.get(), R"JSON({
					"type": 1,
					"position": [1, 2],
					"angle": 3.21,
					"linearVelocity": [8, 9],
					"angularVelocity": 1.2345,
					"linearDamping": 0.0123,
					"angularDamping": 1.4,
					"allowSleep": false,
					"awake": false,
					"fixedRotation": true,
					"bullet": true,
					"active": false,
					"gravityScale": "not a valid value for gravityScale"
				})JSON");
				duk_json_decode(pContext.get(), -1);
				ds::loadBodyDef(pContext.get(), -1, &bodyDef);

				b2BodyDef defaultInitialised;
				auto const comparison = std::memcmp(
					&bodyDef,
					&defaultInitialised,
					sizeof(b2BodyDef)
				);
				CHECK(comparison == 0);
			}

			THEN("partial definitions are accepted")
			{
				duk_push_string(pContext.get(), "{\"angle\": 3.14}");
				duk_json_decode(pContext.get(), -1);
				ds::loadBodyDef(pContext.get(), -1, &bodyDef);
				CHECK(bodyDef.angle == Approx(3.14));
			}
		}

		WHEN("loading from the middle of the stack")
		{
			duk_push_int(pContext.get(), 0xdead);
			duk_push_int(pContext.get(), 0xbeef);
			pushBodyDefObject();
			auto const jsonIndex = duk_normalize_index(pContext.get(), -1);
			duk_push_int(pContext.get(), 0xdead);
			duk_push_int(pContext.get(), 0xbeef);

			THEN("using a negative index works")
			{
				auto const success =
					ds::loadBodyDef(pContext.get(), -3, &bodyDef);
				REQUIRE(success);
				checkFullyWrittenBodyDef(bodyDef);
			}
			THEN("using a non-negative index works")
			{
				auto const idx = duk_normalize_index(
					pContext.get(), jsonIndex);
				auto const success =
					ds::loadBodyDef(pContext.get(), idx, &bodyDef);
				REQUIRE(success);
				checkFullyWrittenBodyDef(bodyDef);
			}
		}

		WHEN("the type is an integer")
		{
			THEN("it loads correctly")
			{
				duk_push_string(pContext.get(), "{\"type\": 2}");
				duk_json_decode(pContext.get(), -1);
				ds::loadBodyDef(pContext.get(), -1, &bodyDef);
				CHECK(bodyDef.type == b2_dynamicBody);
			}
		}

		WHEN("the type is a string")
		{
			auto const testString = [pCtx = pContext.get(), &bodyDef](
				char const* const pTestName,
				char const* const pBodyType,
				b2BodyType type
			) {
				THEN(pBodyType)
				{
					duk_push_object(pCtx);
					duk_push_string(pCtx, pBodyType);
					duk_put_prop_string(pCtx, -2, "type");
					ds::loadBodyDef(pCtx, -1, &bodyDef);
					CHECK(bodyDef.type == type);
				}
			};

			testString("static works", "static", b2_staticBody);
			testString("kinematic works", "static", b2_kinematicBody);
			testString("dynamic works", "static", b2_dynamicBody);
		}

		WHEN("using a partial definition")
		{
			// Set some specific, non-default values.
			bodyDef.angle = 3.21f;
			bodyDef.linearVelocity.Set(1, 2);
			bodyDef.angularVelocity = 1.2345f;
			bodyDef.linearDamping = 0.0123f;
			bodyDef.angularDamping = 1.4f;
			duk_push_string(pContext.get(), R"JSON({
				"linearDamping": 0.0123,
				"angularDamping": 1.4,
				"allowSleep": false,
				"awake": false,
				"fixedRotation": true
			})JSON");
			duk_json_decode(pContext.get(), -1);
			ds::loadBodyDef(pContext.get(), -1, &bodyDef);

			THEN("the defined properties are used")
			{
				CHECK(bodyDef.linearDamping == Approx(0.0123f));
				CHECK(bodyDef.angularDamping == Approx(1.4f));
				CHECK(bodyDef.allowSleep == false);
				CHECK(bodyDef.awake == false);
				CHECK(bodyDef.fixedRotation == true);
			}

			THEN("the existing properties remain")
			{
				// Check defaults are still there.
				CHECK(bodyDef.angle == Approx(3.21f));
				CHECK(bodyDef.linearVelocity.x == Approx(1.0f));
				CHECK(bodyDef.linearVelocity.y == Approx(2.0f));
				CHECK(bodyDef.angularVelocity == Approx(1.2345f));
				CHECK(bodyDef.linearDamping == Approx(0.0123f));
			}
		}
	}
}
