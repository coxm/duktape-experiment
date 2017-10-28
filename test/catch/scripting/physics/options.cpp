#include <catch.hpp>

#include <Box2D/Common/b2Math.h>

#include <duktape.h>

#include "dukdemo/scripting/physics.h"

#include "./test_utils.h"


namespace ds = dukdemo::scripting;


constexpr char const* const pPropName = "prop";


duk_idx_t
prepContextForOptionLoad(duk_context* pContext)
{
	duk_push_number(pContext, 123);
	duk_push_number(pContext, 456);
	auto const ownerIdx = duk_push_object(pContext);
	duk_push_number(pContext, 789);
	return ownerIdx;
}


using ValueInserter = void(duk_context*, duk_idx_t);


template <typename Result>
using ResultLoader = bool(duk_context*, duk_idx_t, char const*, Result*);


template <typename Result>
using ResultCreator = Result();


template <typename Result>
using ResultChecker = void(Result);


template <typename Result>
void
testOptionGetter(
	ValueInserter insertValidJSValue,
	ResultLoader<Result> loadResultFromJS,
	ResultCreator<Result> createInitialResult,
	ResultChecker<Result> checkResultUnchanged,
	ResultChecker<Result> checkChangedResultIsValid
)
{
	testutils::duk_context_ptr pContext{duk_create_heap_default()};
	auto const ownerIdx = prepContextForOptionLoad(pContext.get());

	auto const arrayIdx = duk_push_array(pContext.get());
	duk_put_prop_index(pContext.get(), -1, 0);
	auto result = createInitialResult();

	WHEN("the property doesn't exist")
	{
		auto const success = loadResultFromJS(
			pContext.get(), ownerIdx, pPropName, &result);

		THEN("the property is considered valid (returns true)")
		{
			CHECK(success == true);
		}
		THEN("the result is unchanged")
		{
			checkResultUnchanged(result);
		}
	}

	WHEN("the property exists but is invalid")
	{
		duk_push_undefined(pContext.get());
		duk_put_prop_string(pContext.get(), ownerIdx, pPropName);
		auto const success = loadResultFromJS(
			pContext.get(), ownerIdx, pPropName, &result);

		THEN("the property is considered invalid (returns false)")
		{
			CHECK(success == false);
		}
		THEN("the result is unchanged")
		{
			checkResultUnchanged(result);
		}
	}

	WHEN("the property exists and is valid")
	{
		insertValidJSValue(pContext.get(), ownerIdx);
		auto const success = loadResultFromJS(
			pContext.get(), ownerIdx, pPropName, &result);

		THEN("the result is correct")
		{
			checkChangedResultIsValid(result);
		}
	}
}


namespace b2Vec2_test {
	constexpr float originalX = 4.5f;
	constexpr float originalY = 6.7f;
	constexpr float newX = 1.2f;
	constexpr float newY = 3.4f;

	void insertValidJSValue(duk_context* pCtx, duk_idx_t ownerIdx)
	{
		ownerIdx = duk_normalize_index(pCtx, ownerIdx);
		auto const arrayIdx = duk_push_array(pCtx);
		duk_push_number(pCtx, newX);
		duk_put_prop_index(pCtx, arrayIdx, 0);
		duk_push_number(pCtx, newY);
		duk_put_prop_index(pCtx, arrayIdx, 1);
		duk_put_prop_string(pCtx, ownerIdx, pPropName);
	}

	b2Vec2 createInitialResult()
	{
		return b2Vec2{originalX, originalY};
	}

	void checkResultUnchanged(b2Vec2 result)
	{
		CHECK(result.x == Approx(originalX));
		CHECK(result.y == Approx(originalY));
	}

	void checkChangedResultIsValid(b2Vec2 result)
	{
		CHECK(result.x == Approx(newX));
		CHECK(result.y == Approx(newY));
	}
} // namespace b2Vec2_test


SCENARIO("Loading optional b2Vec2 props", "[loadOptionalVec2Prop]")
{
	GIVEN("a duktape context")
	{
		testOptionGetter<b2Vec2>(
			b2Vec2_test::insertValidJSValue,
			ds::loadOptionalVec2Prop,
			b2Vec2_test::createInitialResult,
			b2Vec2_test::checkResultUnchanged,
			b2Vec2_test::checkChangedResultIsValid
		);
	}
}
