#ifndef BENCH_MUPARSER_230_H
#define BENCH_MUPARSER_230_H

#include "Benchmark.h"


class BenchMuParser230 : public Benchmark
{
public:

	BenchMuParser230(bool bEnableOptimizer = true);
	double DoBenchmark(const std::string& sExpr, long iCount);
	std::string GetShortName() const;
	virtual void PreprocessExpr(std::vector<std::string>& vExpr);
	virtual void PreprocessExpr(std::string& s);

private:

	double DoBenchmarkBulk(const std::string& sExpr, long iCount);
	double DoBenchmarkStd(const std::string& sExpr, long iCount);

	bool m_bUseBulkMode;
};

#endif
