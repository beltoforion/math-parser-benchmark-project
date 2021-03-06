#include "BenchTinyExpr.h"

#include <cmath>
#include <cstring>

#include "tinyexpr/tinyexpr.h"


BenchTinyExpr::BenchTinyExpr()
	: Benchmark()
{
	m_sName = "TinyExpr";
}

double BenchTinyExpr::DoBenchmark(const std::string& sExpr, long iCount)
{
	double a = 1.1;
	double b = 2.2;
	double c = 3.3;
	double x = 2.123456;
	double y = 3.123456;
	double z = 4.123456;
	double w = 5.123456;

	te_variable vars[] =
	{
	  {"a", &a},
	  {"b", &b},
	  {"c", &c},
	  {"x", &x},
	  {"y", &y},
	  {"z", &z},
	  {"w", &w}
	};

	// Perform basic tests for the variables used
	// in the expressions
	{
		bool test_result = true;

		auto tests_list = test_expressions();

		for (auto test : tests_list)
		{
			int test_error = 0;
			te_expr* test_expression = reinterpret_cast<te_expr*>(0);

			test_expression = te_compile(test.first.c_str(), vars, sizeof(vars) / sizeof(te_variable), &test_error);

			if (
				(0 == test_expression) ||
				test_error ||
				(!is_equal(test.second, te_eval(test_expression)))
				)
			{
				te_free(test_expression);
				test_result = false;
				break;
			}

			te_free(test_expression);
		}

		if (!test_result)
		{
			StopTimerAndReport("Failed variable test");
			return m_fTime1;
		}
	}

	int error = 0;
	te_expr* expression = reinterpret_cast<te_expr*>(0);

	expression = te_compile(sExpr.c_str(), vars, sizeof(vars) / sizeof(te_variable), &error);

	if ((0 == expression) || error)
	{
		char buf[128];
		sprintf(buf, "Parsing error at %d.", error);
		StopTimerAndReport(buf);
		return m_fTime1;
	}

	//Prime the I and D caches for the expression
	{
		double d0 = 0.0;
		double d1 = 0.0;

		for (std::size_t i = 0; i < priming_rounds; ++i)
		{
			if (i & 1)
				d0 += te_eval(expression);
			else
				d1 += te_eval(expression);
		}

		if (
			(d0 == std::numeric_limits<double>::infinity()) &&
			(d1 == std::numeric_limits<double>::infinity())
			)
		{
			printf("\n");
		}
	}

	// Perform benchmark then return results
	double fRes = 0;
	double fSum = 0;

	fRes = te_eval(expression);

	StartTimer();

	for (int j = 0; j < iCount; ++j)
	{
		fSum += te_eval(expression);
		std::swap(a, b);
		std::swap(x, y);
	}

	StopTimer(fRes, fSum, iCount);

	te_free(expression);

	return m_fTime1;
}
