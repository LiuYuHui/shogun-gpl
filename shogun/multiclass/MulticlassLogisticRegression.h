/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2012 Sergey Lisitsyn
 * Copyright (C) 2012 Sergey Lisitsyn
 */

#ifndef MULTICLASSLOGISTICREGRESSION_H_
#define MULTICLASSLOGISTICREGRESSION_H_
#include <shogun/lib/config.h>
#include <shogun/lib/common.h>
#include <shogun/features/DotFeatures.h>
#include <shogun/machine/LinearMulticlassMachine.h>

namespace shogun
{

/** @brief multiclass logistic regression
 *
 */
class MulticlassLogisticRegression : public LinearMulticlassMachine
{
	public:
		MACHINE_PROBLEM_TYPE(PT_MULTICLASS)

		/** default constructor  */
		MulticlassLogisticRegression();

		/** standard constructor
		 * @param z z regularization constant value
		 * @param feats features
		 * @param labs labels
		 */
		MulticlassLogisticRegression(float64_t z, const std::shared_ptr<DotFeatures>& feats, std::shared_ptr<Labels> labs);

		/** destructor */
		virtual ~MulticlassLogisticRegression();

		/** get name */
		virtual const char* get_name() const
		{
			return "MulticlassLogisticRegression";
		}

		/** set z
		 * @param z z value
		 */
		inline void set_z(float64_t z)
		{
			ASSERT(z>0)
			m_z = z;
		}
		/** get C
		 * @return C value
		 */
		inline float64_t get_z() const { return m_z; }

		/** set epsilon
		 * @param epsilon epsilon value
		 */
		inline void set_epsilon(float64_t epsilon)
		{
			ASSERT(epsilon>0)
			m_epsilon = epsilon;
		}
		/** get epsilon
		 * @return epsilon value
		 */
		inline float64_t get_epsilon() const { return m_epsilon; }

		/** set max iter
		 * @param max_iter max iter value
		 */
		inline void set_max_iter(int32_t max_iter)
		{
			ASSERT(max_iter>0)
			m_max_iter = max_iter;
		}
		/** get max iter
		 * @return max iter value
		 */
		inline int32_t get_max_iter() const { return m_max_iter; }

	protected:

		/** train machine */
		virtual bool train_machine(std::shared_ptr<Features> data = NULL);

private:

		/** init defaults */
		void init_defaults();

		/** register parameters */
		void register_parameters();

protected:

		/** regularization constant for each machine */
		float64_t m_z;

		/** tolerance */
		float64_t m_epsilon;

		/** max number of iterations */
		int32_t m_max_iter;

};
}
#endif
