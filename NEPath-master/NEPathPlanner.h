#pragma once
#include "Curve.h"
#include "DirectionParallel.h"
#include "ContourParallel.h"
#include "PlanningOptions.h"

// ����ɡ���·���滮��
class NEPathPlanner {
public:
	NEPathPlanner();
	void set_contour(const double* x, const double* y, int length);
	void set_contour(const path& contour_new);
	void addhole(const path& hole);
	void addhole(const double* x, const double* y, int length);
	void addholes(const paths& holes_new);
	paths tool_compensate(double dis); // ����
	paths tool_compensate(const ContourParallelOptions& opts); // ����
public:
	paths Raster(double angle = 0); // ����դ�㷨��δ��������ƽ�л�ϵ��㷨
	paths Raster(const DirectParallelOptions& opts); // ����դ�㷨��δ��������ƽ�л�ϵ��㷨
	paths Zigzag(double angle = 0); // ��Zigzag�㷨��δ��������ƽ�л�ϵ��㷨
	paths Zigzag(const DirectParallelOptions& opts); // ��Zigzag�㷨��δ��������ƽ�л�ϵ��㷨
	paths CP(); // ����ƽ���㷨
	paths CP(const ContourParallelOptions& opts); // ����ƽ���㷨
public:
	path contour;
	paths holes;
	double delta;
	bool wash;
	double washdis;
	bool debug;
	int num_least;
};