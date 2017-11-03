#include <cstring>

#include <catch.hpp>

#include <Box2D/Common/b2Math.h>

#include <duktape.h>

#include "dukdemo/scripting/loaders.h"

#include "./test_utils.h"


namespace ds = dukdemo::scripting;


SCENARIO("Loading a b2Vec2 from duktape", "[loadVec2]")
{
	GIVEN("a duktape context")
	{
		testutils::duk_context_ptr pContext{duk_create_heap_default()};

		constexpr duk_double_t value0 = 123.0;
		constexpr char const* const pValue1 = "dummy test data";
		constexpr duk_uint_t value3 = 456u;

		// Push some junk to the value stack.
		duk_push_number(pContext.get(), value0);
		duk_push_string(pContext.get(), pValue1);
		auto const arrayIdx = duk_push_array(pContext.get());
		duk_push_uint(pContext.get(), value3);

		b2Vec2 result{0.1234f, 5.6789f};

		auto const checkStackReturnedToNormalState = [pCtx = pContext.get()] {
			THEN("the stack is returned to normal state")
			{
				REQUIRE(duk_get_top(pCtx) == 4);
				auto const v0 = duk_get_number(pCtx, 0);
				CHECK(v0 == value0);

				auto const pV1 = duk_get_string(pCtx, 1);
				CHECK(std::strcmp(pV1, pValue1) == 0);

				auto const v3 = duk_get_uint(pCtx, 3);
				CHECK(v3 == value3);
			}
		};

		auto const checkFailureAt = [
			pCtx = pContext.get(),
			 &result,
			 &checkStackReturnedToNormalState
		] (
			duk_idx_t index
		) {
			auto const valid = ds::loadVec2(pCtx, index, &result);
			THEN("the return value is false")
			{
				CHECK(!valid);
			}
			THEN("the result value is not modified")
			{
				CHECK(result.x == Approx(0.1234f));
				CHECK(result.y == Approx(5.6789f));
			}
			checkStackReturnedToNormalState();
		};

		WHEN("the value is not an array")
		{
			checkFailureAt(-1);
		}

		WHEN("the value is an invalid array")
		{
			checkFailureAt(arrayIdx);
		}

		WHEN("the array contains a non-number")
		{
			// Create an array, `[1.234, null]`.
			duk_push_number(pContext.get(), 1.234f);
			duk_push_null(pContext.get());
			duk_put_prop_index(pContext.get(), arrayIdx, -1);
			duk_put_prop_index(pContext.get(), arrayIdx, -1);
			checkFailureAt(arrayIdx);
		}
	}
}
