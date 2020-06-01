#ifndef BENCH_EXPRTK_H
#define BENCH_EXPRTK_H

#include <vector>
#include <string>

#include "Benchmark.h"

class BenchExprTk : public Benchmark
{
public:

   BenchExprTk();

   double DoBenchmark(const std::string& sExpr, long iCount);
};

#endif
