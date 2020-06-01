#include "Benchmark.h"

#include <cassert>
#include <cstdio>
#include <ctime>
#include <algorithm>

using namespace std;


Benchmark::Benchmark(EBaseType eBaseType)
	: m_sName(),
	m_sInfo(),
	m_nTotalBytecodeSize(0),
	m_nPoints(0),
	m_nNum(),
	m_fScore(0),
	m_fTime1(0),
	m_fResult(0),
	m_fSum(0),
	m_bFail(false),
	m_sFailReason(),
	m_eBaseType(eBaseType),
	m_allFails()
{
	rate_list.reserve(36000);
}

Benchmark::~Benchmark()
{}

void Benchmark::AddPoints(int pt)
{
	m_nPoints += pt;
}

//-------------------------------------------------------------------------------------------------
int Benchmark::GetPoints() const
{
	return m_nPoints;
}

//-------------------------------------------------------------------------------------------------
void Benchmark::AddScore(double fScore)
{
	m_fScore += fScore;
}

//-------------------------------------------------------------------------------------------------
double Benchmark::GetScore() const
{
	return m_fScore;
}

std::string Benchmark::GetBaseType() const
{
	switch (m_eBaseType)
	{
	case FLOAT: return "float";
	case DOUBLE: return "double";
	case LONG_DOUBLE: return "long double";
	case MPFR: return "mpfr";
	default:           return "unknown";
	}
}
//-------------------------------------------------------------------------------------------------
void Benchmark::PreprocessExpr(std::vector<std::string>&)
{}

//-------------------------------------------------------------------------------------------------
std::string Benchmark::GetName() const
{
	return m_sName;
}

//-------------------------------------------------------------------------------------------------
std::string Benchmark::GetShortName() const
{
	return m_sName;
}

//-------------------------------------------------------------------------------------------------
void Benchmark::Reset()
{
	m_fTime1 = 0;
	m_fResult = 0;
	m_allFails.clear();
}

//-------------------------------------------------------------------------------------------------
void Benchmark::StartTimer()
{
	m_timer.Start();
}

//-------------------------------------------------------------------------------------------------
double Benchmark::GetTime() const
{
	return m_fTime1;
}

//-------------------------------------------------------------------------------------------------
void Benchmark::StopTimerAndReport(const std::string& msg)
{
	m_timer.Stop();
	m_fResult = 0;
	m_fSum = 0;
	m_fTime1 = 0;
	m_bFail = true;
#ifndef disable_failure_messge
	m_sFailReason = msg;
#endif
	rate_list.push_back(0.0);
}

//-------------------------------------------------------------------------------------------------
void Benchmark::StopTimer(double fRes, double fSum, int iCount)
{
	m_fTime1 = m_timer.Stop() / (double)iCount;
	m_fResult = fRes;
	m_fSum = fSum;
	m_bFail = false;
	rate_list.push_back(1.0e9 / m_fTime1);
}

//-------------------------------------------------------------------------------------------------
double Benchmark::GetSum() const
{
	return m_fSum;
}

//-------------------------------------------------------------------------------------------------
double Benchmark::GetRes() const
{
	return m_fResult;
}

//-------------------------------------------------------------------------------------------------
bool Benchmark::DidNotEvaluate() const
{
	return m_bFail;
}

//-------------------------------------------------------------------------------------------------
const std::string& Benchmark::GetFailReason() const
{
	return m_sFailReason;
}

//-------------------------------------------------------------------------------------------------
bool Benchmark::ExpressionFailed(const std::string& expr) const
{
	return m_allFails.find(expr) != m_allFails.end();
}

//-------------------------------------------------------------------------------------------------
const std::map<std::string, std::string> Benchmark::GetFails() const
{
	return m_allFails;
}

//-------------------------------------------------------------------------------------------------
void Benchmark::AddFail(const std::string& sExpr)
{
	m_allFails[sExpr] = DidNotEvaluate() ? m_sFailReason : std::string("incorrect result");
}

//-------------------------------------------------------------------------------------------------
double Benchmark::GetRate(const std::size_t& index) const
{
	if (index < rate_list.size())
		return rate_list[index];
	else
		return 0.0;
}

//-------------------------------------------------------------------------------------------------
void Benchmark::IgnoreLastRate()
{
	if (!rate_list.empty())
		rate_list.pop_back();
}

void Benchmark::copy(Benchmark* b)
{
	m_sName = b->m_sName;
	m_sInfo = b->m_sInfo;
	m_nTotalBytecodeSize = b->m_nTotalBytecodeSize;
	m_nNum = b->m_nNum;
	m_fTime1 = b->m_fTime1;
	m_fResult = b->m_fResult;
	m_fSum = b->m_fSum;
	m_bFail = b->m_bFail;
	m_sFailReason = b->m_sFailReason;
	m_timer = b->m_timer;
	m_eBaseType = b->m_eBaseType;
	m_allFails = b->m_allFails;
	rate_list = b->rate_list;
}
