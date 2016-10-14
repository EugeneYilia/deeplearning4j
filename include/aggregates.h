//
// @author raver119@gmail.com
//

#ifndef LIBND4J_AGGREGATES_H
#define LIBND4J_AGGREGATES_H

#include <aggregate_ops.h>

#define AGGREGATE_OPS \
        (0, aggregateOps::HierarchicSoftmax) ,\
        (1, aggregateOps::Dot) ,\
        (2, aggregateOps::Axpy) ,\
        (3, aggregateOps::SkipGram)


namespace functions {
    namespace aggregate {

        template<typename T>
        class AggregatedFunction {

        public:
#ifdef __CUDACC__
            template<typename OpClass>
            __device__ inline static void execCuda(T **arguments, int numArguments, int **shapeArguments, int numShapeArguments, int *indexArguments, int numIndexArguments, T *realArguments, int numRealArguments) {
                OpClass::executeAggregateCuda(arguments, numArguments, shapeArguments, numShapeArguments, indexArguments, numIndexArguments, realArguments, numRealArguments);
            }
#endif

            template<typename OpClass>
            inline static void exec(T **arguments, int numArguments, int **shapeArguments, int numShapeArguments, int *indexArguments, int numIndexArguments, T *realArguments, int numRealArguments) {
                OpClass::executeAggregate(arguments, numArguments, shapeArguments, numShapeArguments, indexArguments, numIndexArguments, realArguments, numRealArguments);
            }

            inline static void exec(int opNum, T **arguments, int numArguments, int **shapeArguments, int numShapeArguments, int *indexArguments, int numIndexArguments, T *realArguments, int numRealArguments) {
                DISPATCH_BY_OPNUM(exec, PARAMS(arguments, numArguments, shapeArguments, numShapeArguments, indexArguments, numIndexArguments, realArguments, numRealArguments), AGGREGATE_OPS);
            }
		};
    }
}

#ifdef __CUDACC__

template <typename T, typename OpClass>
__device__ void aggregateGeneric(T **arguments, int numArguments, int **shapeArguments, int numShapeArguments, int *indexArguments, int numIndexArguments, T *realArguments, int numRealArguments) {
    functions::aggregate::AggregatedFunction<T>:: template execCuda<OpClass>(arguments, numArguments, shapeArguments, numShapeArguments, indexArguments, numIndexArguments, realArguments, numRealArguments);
};

DISPATCH_KERNEL_SIMPLE(aggregateSimple_, aggregateGeneric, float, INPUT(float **arguments, int numArguments, int **shapeArguments, int numShapeArguments, int *indexArguments, int numIndexArguments, float *realArguments, int numRealArguments), PARAMS(arguments, numArguments, shapeArguments, numShapeArguments, indexArguments, numIndexArguments, realArguments, numRealArguments), OPS_A(AGGREGATE_OPS))
DISPATCH_KERNEL_SIMPLE(aggregateSimple_, aggregateGeneric, double, INPUT(double **arguments, int numArguments, int **shapeArguments, int numShapeArguments, int *indexArguments, int numIndexArguments, double *realArguments, int numRealArguments), PARAMS(arguments, numArguments, shapeArguments, numShapeArguments, indexArguments, numIndexArguments, realArguments, numRealArguments), OPS_A(AGGREGATE_OPS))
DISPATCH_KERNEL_SIMPLE(aggregateSimple_, aggregateGeneric, float16, INPUT(float16 **arguments, int numArguments, int **shapeArguments, int numShapeArguments, int *indexArguments, int numIndexArguments, float16 *realArguments, int numRealArguments), PARAMS(arguments, numArguments, shapeArguments, numShapeArguments, indexArguments, numIndexArguments, realArguments, numRealArguments), OPS_A(AGGREGATE_OPS))

#endif

#endif //LIBND4J_AGGREGATES_H
