#include "BenchMuParser3.h"

#include <cmath>

#include "muparser3/muParser.h"

using namespace mp;


BenchMuParser3::BenchMuParser3()
	:Benchmark()
{
	m_sName = "muparser3";
}

double BenchMuParser3::DoBenchmarkStd(const std::string& sExpr, long iCount)
{
	Parser<double> p;

	double fRes(0);
	double fSum(0);
	double a = 1.1;
	double b = 2.2;
	double c = 3.3;
	double x = 2.123456;
	double y = 3.123456;
	double z = 4.123456;
	double w = 5.123456;

	p.SetExpr(sExpr.c_str());
	p.DefineVar("a", &a);
	p.DefineVar("b", &b);
	p.DefineVar("c", &c);

	p.DefineVar("x", &x);
	p.DefineVar("y", &y);
	p.DefineVar("z", &z);
	p.DefineVar("w", &w);

	p.DefineConst("pi", (double)M_PI);
	p.DefineConst("e", (double)M_E);

	try
	{

		fRes = p.Eval(); // create bytecode on first time parsing, don't want to have this in the benchmark loop
						 // since fparser does it in Parse(...) wich is outside too
						 // (Speed of bytecode creation is irrelevant)
	}
	catch (ParserError<std::string>& exc)
	{
		StopTimerAndReport(exc.GetMsg());
		return std::numeric_limits<double>::quiet_NaN();
	}
	catch (...)
	{
		StopTimerAndReport("unexpected exception");
		return std::numeric_limits<double>::quiet_NaN();
	}

	// Perform basic tests for the variables used
	// in the expressions
	{
		bool test_result = true;

		auto tests_list = test_expressions();

		for (auto test : tests_list)
		{
			Parser<double> test_p;

			test_p.SetExpr(test.first.c_str());
			test_p.DefineVar("a", &a);
			test_p.DefineVar("b", &b);
			test_p.DefineVar("c", &c);

			test_p.DefineVar("x", &x);
			test_p.DefineVar("y", &y);
			test_p.DefineVar("z", &z);
			test_p.DefineVar("w", &w);

			try
			{
				if (!is_equal(test.second, test_p.Eval()))
				{
					test_result = false;
					break;
				}
			}
			catch (...)
			{
				test_result = false;
				break;
			}
		}

		if (!test_result)
		{
			StopTimerAndReport("Failed variable test");
			return m_fTime1;
		}
	}

	//Prime the I and D caches for the expression
	{
		double d0 = 0.0;
		double d1 = 0.0;

		for (std::size_t i = 0; i < priming_rounds; ++i)
		{
			if (i & 1)
				d0 += p.Eval();
			else
				d1 += p.Eval();
		}

		if ( (d0 == std::numeric_limits<double>::infinity()) &&
			 (d1 == std::numeric_limits<double>::infinity())	)
		{
			printf("\n");
		}
	}

	StartTimer();

	for (int j = 0; j < iCount; ++j)
	{
		fSum += p.Eval();
		std::swap(a, b);
		std::swap(x, y);
	}

	StopTimer(fRes, fSum, iCount);

	return m_fTime1;
}

//-------------------------------------------------------------------------------------------------
void BenchMuParser3::PreprocessExpr(std::vector<std::string>& vExpr)
{
	for (std::size_t i = 0; i < vExpr.size(); ++i)
	{
		PreprocessExpr(vExpr[i]);
	}
}

//-------------------------------------------------------------------------------------------------
void BenchMuParser3::PreprocessExpr(std::string&)
{
}


//-------------------------------------------------------------------------------------------------
double BenchMuParser3::DoBenchmark(const std::string& sExpr, long iCount)
{
	return DoBenchmarkStd(sExpr, iCount);
}

//-------------------------------------------------------------------------------------------------
std::string BenchMuParser3::GetShortName() const
{
	return "muparser 3";
}
