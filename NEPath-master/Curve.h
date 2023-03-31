#pragma once
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>
#include "Basic.h"
#include "path.h"

struct UnderFillSolution {
	bool** map_slice = NULL; // ����Ƭ��
	bool** map_delta = NULL; // ��·����delta������
	int nx = -1;
	int ny = -1;
	double* xs = NULL;
	double* ys = NULL;
	double underfillrate = -1.0;
	void clear();
};

struct SharpTurnSolution {
	int length = 0; // �������������·�����ȣ������Ƿ�պ�
	double radius = -1.0; // ���ְ뾶
	double* AreaPercent = NULL; // �����㴦����ٷֱȣ����պ�ʱԼ��AreaPercent[0], AreaPercent[end] = -1.0;
	double threshold = -1.0; // �ٷֱ���ֵ
	bool* SharpTurn = NULL; // ��ת���ǣ����պ�ʱԼ��SharpTurn[0], SharpTurn[end] = false;
	bool close = false; // �Ƿ�պ�
	void clear();
};

class Curve {
public: // ��������
	static double interp_id(const double* x, int length, double id);
	static double nearest_id(const double* x, const double* y, int length, double x0, double y0);
	static double distance_point2path(const double* x, const double* y, int length, double x0, double y0);
	static double furthest_id(const double* x, const double* y, int length, double x0, double y0);
	static double curves_nearest(const path& p, const path& q, double& id1, double& id2);
	static path* rotate(const path& p, double angle);
	static paths* rotate(const paths& ps, double angle);
	static bool interset_path(const path& a, const path& b);
	static bool interset_path_id(const path& a, const path& b, double& ida, double& idb);
	static void Ndir(const double* x, const double* y, int length, double*& nx, double*& ny); // ���㷨��
	static double AreaCal(const double* x, const double* y, int length); // �������
	static void DiffLength(double*& dl, const double* x, const double* y, int length, bool poly = true); // ���㳤��
	static double* DiffLength(const double* x, const double* y, int length, bool poly = true); // ���㳤��
	static double TotalLength(const double* x, const double* y, int length, bool poly = true); // ���㳤��
	static double LengthBetween(const double* x, const double* y, int length, int from, int to); // ����֮��ĳ���
	static double LengthBetween(const double* x, const double* y, int length, double from, double to); // ����֮��ĳ���
	static void OffsetNaive(const double* x, const double* y, const double* delta, int length, double*& xnew, double*& ynew, double* nx = NULL, double* ny = NULL); // ��ƫ��
	static void OffsetNaive(const double* x, const double* y, double delta, int length, double*& xnew, double*& ynew, double* nx = NULL, double* ny = NULL); // ��ƫ��
public: // ����ָ�����
	static UnderFillSolution UnderFill(const path& contour, const paths& holes, const paths& ps, double delta, double reratio); // Ƿ��������
	static double UnderFillRate(const path& contour, const paths& holes, const paths& ps, double delta, double reratio); // Ƿ��������
	static SharpTurnSolution SharpTurn_Invariant(const path& p, double radius, double threshold = 0.3, bool close = false, double washdis = -1.0); // ��ת�����
	static int SharpTurnNum_Invariant(const path& p, double radius, double threshold = 0.3, bool close = false, double washdis = -1.0); // ��ת����������
private:
	static double AreaInvariant_OnePoint(const path& p, double radius, int id, bool close); // ����һ���㴦�����������
private:
	struct point {
		double x;
		double y;
		point(): x(0), y(0) {}
		point(double X, double Y) :x(X), y(Y) {}
		point(const point& p) :x(p.x), y(p.y) {}
	};
	static bool cmp_Raster(const point& a, const point& b);
public: // ��ֵ�������
	static path wash_dis(const path& p, double dis);
	static path wash_dis(const double* x, const double* y, int length, double dis, bool output_poly = false);
public: // CFS���
	static double BackDis(const double* x, const double* y, int length, double id, double dis);
	static double ForDis(const double* x, const double* y, int length, double id, double dis);
public: // ����
	static void write_xy(const char* outfilename, const path& p);
};