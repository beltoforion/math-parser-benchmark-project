#ifndef BENCH_MUPARSER_3_H
#define BENCH_MUPARSER_3_H

#include "Benchmark.h"


class BenchMuParser3 : public Benchmark
{
public:

	BenchMuParser3();
	double DoBenchmark(const std::string& sExpr, long iCount);
	std::string GetShortName() const;
	virtual void PreprocessExpr(std::vector<std::string>& vExpr);
	virtual void PreprocessExpr(std::string& s);

private:

	double DoBenchmarkStd(const std::string& sExpr, long iCount);
};

#endif
