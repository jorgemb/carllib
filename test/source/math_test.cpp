#include <catch2/catch_test_macros.hpp>

#include "carllib/util/math.hpp"

using carllib::util::wrap;

TEST_CASE("wrap leaves in-range values unchanged", "[util][wrap]")
{
  CHECK(wrap(3, 0, 10) == 3);
  CHECK(wrap(0, 0, 10) == 0);
  CHECK(wrap(10, 0, 10) == 10);
}

TEST_CASE("wrap brings values above the upper bound back into range",
          "[util][wrap]")
{
  CHECK(wrap(11, 0, 10) == 0);
  CHECK(wrap(13, 0, 10) == 2);
  CHECK(wrap(22, 0, 10) == 0);
  CHECK(wrap(100, 0, 10) == 1);
}

TEST_CASE("wrap brings values below the lower bound back into range",
          "[util][wrap]")
{
  CHECK(wrap(-1, 0, 10) == 10);
  CHECK(wrap(-11, 0, 10) == 0);
  CHECK(wrap(-12, 0, 10) == 10);
  CHECK(wrap(-23, 0, 10) == 10);
}

TEST_CASE("wrap works with fully negative ranges", "[util][wrap]")
{
  CHECK(wrap(-5, -10, -1) == -5);
  CHECK(wrap(0, -10, -1) == -10);
  CHECK(wrap(-11, -10, -1) == -1);
}

TEST_CASE("wrap of a unit range always returns the single value",
          "[util][wrap]")
{
  CHECK(wrap(0, 7, 7) == 7);
  CHECK(wrap(7, 7, 7) == 7);
  CHECK(wrap(100, 7, 7) == 7);
  CHECK(wrap(-100, 7, 7) == 7);
}

TEST_CASE("wrap is idempotent for values already in range across types",
          "[util][wrap]")
{
  CHECK(wrap<long>(123L, -1000L, 1000L) == 123L);
  CHECK(wrap<long long>(-1LL, -10LL, 10LL) == -1LL);
}