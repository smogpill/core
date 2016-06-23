#include "test_math/pch.h"
#include "test/unit/test.h"
#include "math/vector/Floatx4_f.h"

using namespace co;

coTEST("Floatx4")
{
	Floatx4 a = {0.0f, 1.0f, 2.0f, 3.0f};

	Floatx4 d = dot(a, a);
}
