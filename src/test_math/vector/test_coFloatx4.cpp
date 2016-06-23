#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/vector/coFloatx4_f.h"

coTEST("coFloatx4")
{
	coFloatx4 a = {0.0f, 1.0f, 2.0f, 3.0f};
	coFloatx4 d = coDot(a, a);
}
