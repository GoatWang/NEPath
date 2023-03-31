#pragma once
#include "Curve.h"
#include "clipper.hpp"
using namespace ClipperLib;

// ����ƽ��·��������Raster��Zigzag
class DirectionParalle { // TODO
public:
	static paths Raster(const path& contour, const paths& holes, double dis, double angle = 0); // ����դ�㷨
	static paths Zigzag(const path& contour, const paths& holes, double dis, double angle = 0); // ��Zigzag�㷨
private:
	static Paths Raster(const Path& contour, const Paths& holes, double dis, double scale); // ��դ�㷨
	static bool cmp_Raster(const IntPoint& a, const IntPoint& b);
	static Paths Zigzag(const Path& contour, const Paths& holes, double dis, double scale); // ����zigzag�㷨
};