/*
 * DynamicLookupTable.h
 *
 *  Created on: Feb 6, 2017
 *      Author: rosbots
 */

#ifndef DYNAMICLOOKUPTABLE_H_
#define DYNAMICLOOKUPTABLE_H_

#include <vector>
#include <utility>
#include "tspl/bfgs.h"

int get_uniquity(double m){
	return 0;
}

double lin(double a, std::vector<double> b) {
	return b[0] * a + b[1];
}

class DynamicLookupTable {
public:
	DynamicLookupTable(double (*f)(double, std::vector<double>) = lin,
			unsigned size = 2, double default_ = 0.0) {
		func = f;
		weights.resize(size, default_);
	}
	~DynamicLookupTable() {
	}
	inline double operator()(double a) {
		return func(a, weights);
	}
	inline void operator()(double (*f)(double, std::vector<double>),
			unsigned size, double default_ = 0.5) {
		func = f;
		weights.resize(size, default_);
	}
	inline void operator()(double a, double b) {
		data.push_back(std::make_pair(a, b));
	}
	void operator()(int = 100, double = 0.001, bool = false);
	inline double cost() {
		double m = 0;
		for (unsigned i = 0; i < weights.size(); i++) {
			double yhat = operator()(data[i].first);
			double y = ((data[i].second));
			m += ((y - yhat) * (y - yhat)) / 2.0;
		}
		return m;
	}
	inline double cost(std::vector<double> a) {
		weights = a;
		return cost();
	}
	inline std::vector<double> w() {
		return weights;
	}
	inline std::vector<std::pair<double, double> > getData() {
		return data;
	}
	inline void checkZeros() {
		for (unsigned i = 0; i < weights.size(); i++) {
			double k = weights[i], a = cost();
			weights[i] = 0;
			double b = cost();
			if (a < b)
				weights[i] = k;
		}
	}
private:
	std::vector<std::pair<double, double> > data;
	double (*func)(double, std::vector<double>);
	std::vector<double> weights;
};

std::vector<double> toVec(splab::Vector<double> a) {
	std::vector<double> k(a.dim());
	for (int i = 0; i < a.dim(); i++)
		k[i] = a[i];
	return k;
}

class objective_function {
private:
	DynamicLookupTable *dlt;
public:
	objective_function(DynamicLookupTable* d) {
		dlt = d;
	}
	double operator()(splab::Vector<double> a) {
		std::vector<double> j(a.dim());
		for (int i = 0; i < a.dim(); i++)
			j[i] = a[i];
		return dlt->cost(j);
	}
	splab::Vector<double> grad(splab::Vector<double> a) {
		splab::Vector<double> g(a.dim());
		for (int i = 0; i < a.dim(); i++) {
			a[i] += 0.0001;
			double ha = dlt->cost(toVec(a));
			a[i] -= 0.0002;
			double hb = dlt->cost(toVec(a));
			a[i] += 0.0001;
			g[i] = (ha - hb) / 0.0002;
		}
		return g;
	}
};

inline void DynamicLookupTable::operator()(int maxItr, double tolerance,
		bool zero) {
	objective_function obj(this);
	splab::BFGS<double, decltype(obj)> bfgs;
	splab::Vector<double> v(weights.size());
	for (int i = 0; i < v.dim(); i++) {
		v[i] = weights[i];
	}
	bfgs.optimize(obj, v, tolerance, maxItr);
	for (int i = 0; i < v.dim(); i++) {
		weights[i] = bfgs.getOptValue()[i];
	}
	if (zero)
		checkZeros();
}

#endif /* DYNAMICLOOKUPTABLE_H_ */
