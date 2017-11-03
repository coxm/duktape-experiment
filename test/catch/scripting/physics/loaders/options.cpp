#include <catch.hpp>

#include <Box2D/Common/b2Math.h>

#include <duktape.h>

#include "dukdemo/scripting/loaders.h"

#include "../test_utils.h"


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
	ResultChecker<Result> checkChangedResultIsCorrect
)
{
	testutils::duk_context_ptr pContext{duk_create_heap_default()};
	auto const ownerIdx = duk_normalize_index(
		pContext.get(), prepContextForOptionLoad(pContext.get()));

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
			checkChangedResultIsCorrect(result);
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

	void checkChangedResultIsCorrect(b2Vec2 result)
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
			b2Vec2_test::checkChangedResultIsCorrect
		);
	}
}


namespace float_test {
	constexpr float originalValue = 4.5f;
	constexpr float newValue = 6.7f;

	void insertValidJSValue(duk_context* pCtx, duk_idx_t ownerIdx)
	{
		duk_push_number(pCtx, newValue);
		duk_put_prop_string(pCtx, ownerIdx, pPropName);
	}

	float createInitialResult()
	{
		return originalValue;
	}

	void checkResultUnchanged(float result)
	{
		CHECK(result == Approx(originalValue));
	}

	void checkChangedResultIsCorrect(float result)
	{
		CHECK(result == Approx(newValue));
	}
} // namespace f


SCENARIO("Loading optional float props", "[loadOptionalFloatProp]")
{
	GIVEN("a duktape context")
	{
		testOptionGetter<float>(
			float_test::insertValidJSValue,
			ds::loadOptionalFloatProp,
			float_test::createInitialResult,
			float_test::checkResultUnchanged,
			float_test::checkChangedResultIsCorrect
		);
	}
}


namespace int16_test {
	constexpr int16 originalValue = 1234;
	constexpr int16 newValue = 5678;

	void insertValidJSValue(duk_context* pCtx, duk_idx_t ownerIdx)
	{
		duk_push_number(pCtx, newValue);
		duk_put_prop_string(pCtx, ownerIdx, pPropName);
	}

	int16 createInitialResult()
	{
		return originalValue;
	}

	void checkResultUnchanged(int16 result)
	{
		CHECK(result == Approx(originalValue));
	}

	void checkChangedResultIsCorrect(int16 result)
	{
		CHECK(result == Approx(newValue));
	}
} // namespace f


SCENARIO("Loading optional int16 props", "[loadOptionalInt16Prop]")
{
	GIVEN("a duktape context")
	{
		testOptionGetter<int16>(
			int16_test::insertValidJSValue,
			ds::loadOptionalInt16Prop,
			int16_test::createInitialResult,
			int16_test::checkResultUnchanged,
			int16_test::checkChangedResultIsCorrect
		);
	}
}


namespace uint16_test {
	constexpr uint16 originalValue = 1234;
	constexpr uint16 newValue = 5678;

	void insertValidJSValue(duk_context* pCtx, duk_idx_t ownerIdx)
	{
		duk_push_number(pCtx, newValue);
		duk_put_prop_string(pCtx, ownerIdx, pPropName);
	}

	uint16 createInitialResult()
	{
		return originalValue;
	}

	void checkResultUnchanged(uint16 result)
	{
		CHECK(result == Approx(originalValue));
	}

	void checkChangedResultIsCorrect(uint16 result)
	{
		CHECK(result == Approx(newValue));
	}
} // namespace f


SCENARIO("Loading optional uint16 props", "[loadOptionalInt16Prop]")
{
	GIVEN("a duktape context")
	{
		testOptionGetter<uint16>(
			uint16_test::insertValidJSValue,
			ds::loadOptionalUint16Prop,
			uint16_test::createInitialResult,
			uint16_test::checkResultUnchanged,
			uint16_test::checkChangedResultIsCorrect
		);
	}
}
