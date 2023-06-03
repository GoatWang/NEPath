#pragma once
#include "gurobi_c++.h"
#include "Curve.h"
#include "PlanningOptions.h"

class NonEquidistant {
public:
	NonEquidistant(bool debug = false);
	//paths NEpaths(const path& contour, const paths& holes, double delta, double dot_delta, double ddot_delta, double alpha, double err = 0.1, int N_maxiter = 5);
	paths NEpaths(const path& contour, const paths& holes, const NonEquidistantOptions& opts);
private:
	double* Optimize_QSL(path p, const NonEquidistantOptions& opts);
	double* L2A_optimize(double const* x, double const* y, int length, double delta, double dot_delta, double ddot_delta, double alpha, double* delta0 = NULL, double err = 0.1, int N_maxiter = 5); // ���Ż�Բ�ȣ�ʵ���������б𣬵������ǳ�ʼ������·���⻬�ȡ������Ż������deltas
	double* L_optimize(double const* x, double const* y, int length, double delta, double dot_delta, double ddot_delta, double alpha, double* delta0 = NULL, double err = 0.1, int N_maxiter = 5); // ���Ż�����
	double* IQA_optimize(double const* x, double const* y, int length, double delta, double dot_delta, double ddot_delta, double alpha, double lambda, double* delta0 = NULL, double err = 0.1, int N_maxiter = 5); // �Ż�Բ�Ⱥ����
	double* QSL_optimize_outward(double const* x, double const* y, int length, double delta, double dot_delta, double ddot_delta, double lambdaQ, double lambdaS, double lambdaL, double err = 0.1, int N_maxiter = 10); // �߽�ƽ���õ��Ż����⣬Բ�ȡ�����ͳ���
	void addcons_ddot_deltas(GRBModel& model, double const* x, double const* y, GRBVar* deltas, int length, double dot_delta, double ddot_delta);
	// ����һ��·���滮�����ǳ�ʼ������·���⻬�ȡ�
	paths do1offset(const path& contour, const NonEquidistantOptions& opts);
	pathnode* root_offset(const path& contour, const paths& holes, const NonEquidistantOptions& opts);
private:
	GRBEnv gurobi;
};