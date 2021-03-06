/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Copyright (C) 2012 Sergey Lisitsyn
 */

#ifndef  FEATUREBLOCKLOGISTICREGRESSION_H_
#define  FEATUREBLOCKLOGISTICREGRESSION_H_

#include <shogun/lib/config.h>
#ifdef USE_GPL_SHOGUN
#include <shogun/lib/IndexBlockRelation.h>
#include <shogun/machine/LinearMachine.h>

namespace shogun
{
/** @brief class FeatureBlockLogisticRegression, a linear
 * binary logistic loss classifier for problems with complex feature relations.
 * Currently two feature relations are supported - feature group
 * (done via IndexBlockGroup) and feature tree (done via CIndexTree).
 * Handling of feature relations is done via L1/Lq (for groups) and L1/L2
 * (for trees) regularization.
 *
 * The underlying solver is based on the SLEP library.
 *
 * @see CIndexBlock
 * @see IndexBlockGroup
 * @see IndexBlockTree
 */
class FeatureBlockLogisticRegression : public LinearMachine
{

	public:
		MACHINE_PROBLEM_TYPE(PT_BINARY)

		/** default constructor */
		FeatureBlockLogisticRegression();

		/** constructor
		 *
		 * @param z regularization coefficient
		 * @param training_data training features
		 * @param training_labels training labels
		 * @param task_relation task relation
		 */
		FeatureBlockLogisticRegression(
		     float64_t z, const std::shared_ptr<Features>& training_data,
		     const std::shared_ptr<Labels>& training_labels, std::shared_ptr<IndexBlockRelation> task_relation);

		/** destructor */
		virtual ~FeatureBlockLogisticRegression();

		/** get name */
		virtual const char* get_name() const
		{
			return "FeatureBlockLogisticRegression";
		}

		/** getter for feature relation
		 * @return feature relation
		 */
		std::shared_ptr<IndexBlockRelation> get_feature_relation() const;

		/** setter for feature relation
		 * @param feature_relation feature relation
		 */
		void set_feature_relation(std::shared_ptr<IndexBlockRelation> feature_relation);

		virtual float64_t apply_one(int32_t vec_idx);

		/** get max iter */
		int32_t get_max_iter() const;
		/** get q */
		float64_t get_q() const;
		/** get regularization */
		int32_t get_regularization() const;
		/** get termination */
		int32_t get_termination() const;
		/** get tolerance */
		float64_t get_tolerance() const;
		/** get z */
		float64_t get_z() const;

		/** set max iter */
		void set_max_iter(int32_t max_iter);
		/** set q */
		void set_q(float64_t q);
		/** set regularization */
		void set_regularization(int32_t regularization);
		/** set termination */
		void set_termination(int32_t termination);
		/** set tolerance */
		void set_tolerance(float64_t tolerance);
		/** set z */
		void set_z(float64_t z);

	protected:

		virtual SGVector<float64_t> apply_get_outputs(std::shared_ptr<Features> data);

		/** train machine */
		virtual bool train_machine(std::shared_ptr<Features> data=NULL);

	private:

		/** register parameters */
		void register_parameters();

		/** Initializes Parameters to std values */
		void init();

	protected:

		/** feature tree */
		std::shared_ptr<IndexBlockRelation> m_feature_relation;

		/** regularization type */
		int32_t m_regularization;

		/** termination criteria */
		int32_t m_termination;

		/** max iteration */
		int32_t m_max_iter;

		/** tolerance */
		float64_t m_tolerance;

		/** q of L1/Lq */
		float64_t m_q;

		/** regularization coefficient */
		float64_t m_z;

};
}
#endif //USE_GPL_SHOGUN
#endif
