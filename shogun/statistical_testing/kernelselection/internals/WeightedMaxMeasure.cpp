/*
 * Copyright (c) The Shogun Machine Learning Toolbox
 * Written (W) 2012 - 2013 Heiko Strathmann
 * Written (w) 2014 - 2017 Soumyajit De
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the Shogun Development Team.
 */

#include <shogun/lib/SGVector.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/kernel/Kernel.h>
#include <shogun/kernel/CombinedKernel.h>
#include <shogun/statistical_testing/MMD.h>
#include <shogun/statistical_testing/internals/KernelManager.h>
#include <shogun/statistical_testing/kernelselection/internals/WeightedMaxMeasure.h>

#ifdef USE_GPL_SHOGUN
#include <shogun/statistical_testing/kernelselection/internals/OptimizationSolver.h>

#include <utility>

using namespace shogun;
using namespace internal;

WeightedMaxMeasure::WeightedMaxMeasure(KernelManager& km, std::shared_ptr<MMD> est) : MaxMeasure(km, std::move(est))
{
}

WeightedMaxMeasure::~WeightedMaxMeasure()
{
}

void WeightedMaxMeasure::compute_measures()
{
	MaxMeasure::compute_measures();
	const auto num_kernels=kernel_mgr.num_kernels();
	if (Q.num_rows!=num_kernels || Q.num_cols!=num_kernels)
		Q=SGMatrix<float64_t>(num_kernels, num_kernels);
	std::fill(Q.data(), Q.data()+Q.size(), 0);
	for (auto i=0; i<num_kernels; ++i)
		Q(i, i)=1;
}

SGMatrix<float64_t> WeightedMaxMeasure::get_measure_matrix()
{
	return Q;
}

std::shared_ptr<Kernel> WeightedMaxMeasure::select_kernel()
{
	init_measures();
	compute_measures();

	OptimizationSolver solver(measures, Q);
	SGVector<float64_t> weights=solver.solve();

	auto kernel=std::make_shared<CombinedKernel>();
	const size_t num_kernels=kernel_mgr.num_kernels();
	for (size_t i=0; i<num_kernels; ++i)
	{
		if (!kernel->append_kernel(kernel_mgr.kernel_at(i)))
			error("Error while creating a combined kernel! Please contact Shogun developers!");
	}
	kernel->set_subkernel_weights(weights);
	SG_DEBUG("Created a weighted kernel!");
	return kernel;
}
#endif // USE_GPL_SHOGUN
