/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2011 Heiko Strathmann
 * DS-Kernel implementation Written (W) 2008 Sébastien Boisvert under GPLv3
 * Copyright (C) 2011 Berlin Institute of Technology and Max-Planck-Society
 */

#include <shogun/kernel/string/DistantSegmentsKernel.h>
#include <string>

using namespace shogun;

DistantSegmentsKernel::DistantSegmentsKernel() : StringKernel<char>(),
	m_delta(0), m_theta(0)
{
	init();
}

DistantSegmentsKernel::DistantSegmentsKernel(int32_t size, int32_t delta,
		int32_t theta) : StringKernel<char>(size), m_delta(delta),
		m_theta(theta)
{
	init();
}

DistantSegmentsKernel::DistantSegmentsKernel(const std::shared_ptr<StringFeatures<char>>& l,
		const std::shared_ptr<StringFeatures<char>>& r, int32_t size, int32_t delta, int32_t theta) :
	StringKernel<char>(size), m_delta(delta), m_theta(theta)
{
	init();
	StringKernel<char>::init(l, r);
}

bool DistantSegmentsKernel::init(std::shared_ptr<Features> l, std::shared_ptr<Features> r)
{
	Kernel::init(l, r);
	return init_normalizer();
}

void DistantSegmentsKernel::init()
{
	SG_ADD(&m_delta, "delta", "Delta parameter of the DS-Kernel", ParameterProperties::HYPER);
	SG_ADD(&m_theta, "theta", "Theta parameter of the DS-Kernel", ParameterProperties::HYPER);
}

float64_t DistantSegmentsKernel::compute(int32_t idx_a, int32_t idx_b)
{
	bool free_a, free_b;
	int32_t aLength=0, bLength=0;
	char* a=lhs->as<StringFeatures<char>>()->get_feature_vector(idx_a, aLength,
			free_a);
	char* b=rhs->as<StringFeatures<char>>()->get_feature_vector(idx_b, bLength,
			free_b);
	ASSERT(a && b)

	if ((aLength<1)||(bLength<1))
		error("Empty sequences");

	float64_t result=compute(a, aLength, b, bLength, m_delta, m_theta);

	lhs->as<StringFeatures<char>>()->free_feature_vector(a, idx_a, free_a);
	rhs->as<StringFeatures<char>>()->free_feature_vector(b, idx_b, free_b);

	return result;
}

int32_t DistantSegmentsKernel::bin(int32_t j, int32_t i)
{
	if (i>j)
		return 0;
	if (i==3 && j>=3)
	{
		return j*(j-1)*(j-2)/6;
	}
	else if (i==2 && j>=2)
	{
		return j*(j-1)/2;
	}
	return 0;
}

int32_t DistantSegmentsKernel::compute(char* s, int32_t sLength, char* t,
		int32_t tLength, int32_t delta_m, int32_t theta_m)
{
	int32_t c=0;
	int32_t* i_=SG_MALLOC(int32_t, delta_m+1);
	int32_t* l_=SG_MALLOC(int32_t, delta_m+1);
	for (int32_t j_s=0; j_s<=(int32_t) sLength-1; j_s++)
	{
		for (int32_t j_t=0; j_t<=(int32_t) tLength-1; j_t++)
		{
			if (s[j_s-1+1]==t[j_t-1+1])
			{
				int32_t n=Math::min(Math::min(sLength-j_s, tLength-j_t), delta_m);
				int32_t k=-1;
				int32_t i=1;
				while (i<=n)
				{
					k++;
					i_[2*k]=i;
					i++;
					while (i<=n&&s[j_s-1+i]==t[j_t-1+i])
						i++;
					i_[2*k+1]=i;
					l_[k]=i_[2*k+1]-i_[2*k]+1;
					i++;
					while (i<=n&&s[j_s-1+i]!=t[j_t-1+i])
						i++;
				}
				c+=bin(l_[0], 3)-2*bin(l_[0]-theta_m, 3)
						+bin(l_[0]-2*theta_m, 3);
				int32_t c1=0;
				for (int32_t r=1; r<=k; r++)
				{
					c1+=bin(l_[r], 2)-bin(l_[r]-theta_m, 2);
				}
				c+=Math::min(theta_m, i_[1]-i_[0])*c1;
			}
		}
	}
	SG_FREE(l_);
	SG_FREE(i_);
	return c;
}
