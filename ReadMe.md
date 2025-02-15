# Mac Modification:
1. remove all `register` keyword (depreciated in c++11, disabled in C++17)
2. comment out zigzag functions: `NEPath-master/NEPathPlanner.cpp`, `NEPath-master/NEPathPlanner.h`, `DirectionParallel.cpp`, `DirectionParallel.h`, `demo.cpp`, `demo.h`
3. Change sort input in `DirectionParallel.cpp`
	```cpp
	sort(intersection.data(), intersection.data() + intersection.size(), cmp_Raster);
	=> 
	sort(intersection.begin(), intersection.end(), cmp_Raster);
	```
4. remove `inline` for these two functions
	```cpp
	cInt ContourParallel::double2cInt(const double& d, double scale, double delta_pos/*=0.0*/)
	double ContourParallel::cInt2double(const cInt& c, double scale, double delta_pos/*=0.0*/)
	```
5. add const to comparator function in `DirectionParallel.cpp`
	```cpp
		struct InterPoint {
			IntPoint point;
			unsigned int idpath;
			unsigned int idpoint;
			InterPoint(cInt x, cInt y, unsigned int path, unsigned int point) :
				point(IntPoint(x, y)), idpath(path), idpoint(point) {}
			bool operator < (const InterPoint& p) const { // original no const: `bool operator < (const InterPoint& p) { `
				return DirectionParalle::cmp_Raster(this->point, p.point);
			}
			static bool nextto(const InterPoint& a, const InterPoint& b, int size) {
				// a and b is on the same continuous boundary and adjacent
				return (a.idpath == b.idpath) && ((a.idpoint == (b.idpoint + 1) % size) || (b.idpoint == (a.idpoint + 1) % size));
			}
		};
	```
6. cahnge `FileAgent.cpp` to be used on Linux.
7. change `demo_Raster();` to `demo_CP();`  in `demo.cpp`


# NEPath

## A Classical Toolpath and Optimization-Based Non-Equidistant Toolpath Planning Library (In C++)

[![License](https://img.shields.io/badge/License-Boost_1.0-lightblue.svg)](https://www.boost.org/LICENSE_1_0.txt)

The **NEPath** library plans toolpaths for [additive manufacturing (AM, 3D printing)]([3D printing - Wikipedia](https://en.wikipedia.org/wiki/3D_printing)) and [CNC milling](https://en.wikipedia.org/wiki/Numerical_control). Toolpath planning is to generate some 1D toolpaths to filling given 2D slices. The **NEPath** library is able to plan the following toolpaths:

+ Optimization-based non-equidistant toolpath:
	+ **Isoperimetric-Quotient-Optimal Toolpath (IQOP)**.
	+ Variants of IQOP, like toolpaths that minimizing the perimeter, the isoperimetric quotient, and the area.
+ Classical toolpath:
	+ **Contour-Parallel Toolpath (CP)**.
	+ **Zigzag Toolpath**.
	+ **Raster Toolpath**.
+ Toolpath connection. (Temporarily unavailable)
+ Other functions:
	+ Tool Compensating.
	+ Calculating underfill rate.
	+ Determining sharp corners.

Among them, the IQOP is proposed by Wang et al., with a journal article, i.e., 

```
Wang Y, Hu C, Wang Z, et al. Optimization-based non-equidistant toolpath planning for robotic additive manufacturing with non-underfill orientation[J]. Robotics and Computer-Integrated Manufacturing, 2023, 84: 102599.
```

or in BiBTeX:

```
@article{wang2023optimization,
  title={Optimization-based non-equidistant toolpath planning for robotic additive manufacturing with non-underfill orientation},
  author={Wang, Yunan and Hu, Chuxiong and Wang, Ze and Lin, Shize and Zhao, Ziyan and Zhao, Wenxiang and Hu, Kehui and Huang, Zhongyi and Zhu, Yu and Lu, Zhigang},
  journal={Robotics and Computer-Integrated Manufacturing},
  volume={84},
  pages={102599},
  year={2023},
  publisher={Elsevier}
}
```

After the article is published, the **NEPath** library would provide the API and details of IQOP. More non-equidistant toolpaths would be designed soon.

### Complier

C++17

### Statement and Dependence

+ This project cites [AngusJohnson/Clipper2](https://github.com/AngusJohnson/Clipper2) as a dependent package.

+ This project depends on [Gurobi](https://www.gurobi.com/) optimizer for solving [quadratically constrained quadratic program](https://en.wikipedia.org/wiki/Quadratically_constrained_quadratic_program) with [second-order cone constraints](https://en.wikipedia.org/wiki/Second-order_cone_programming). If you need to use another optimizer, you can rewrite the method in the `MyOptimization` function (Temporarily unavailable). If you don't need IQOP and other optimization-based toolpaths, you can comment out `#define IncludeGurobi` in `NEPath-master/setup_NEPath.h` to avoid the dependence on [Gurobi](https://www.gurobi.com/).

### About Citing

If you need to use the **NEPath** project, please cite  "Wang Y, Hu C, Wang Z, et al. Optimization-based non-equidistant toolpath planning for robotic additive manufacturing with non-underfill orientation[J]. Robotics and Computer-Integrated Manufacturing, 2023, 84: 102599."

## Introduction to IQOP

### Framework

IQOP is an optimization-based non-equidistant toolpath planning method for AM and CNC milling. IQOP tries to optimize the smoothness and material cost of the child toolpath from a parent toolpath. IQOP has the following advantages:

+ Compared with the equidistant toolpath, i.e., CP, IQOP can generate smooth toolpaths. Specially, toolpaths insides tends to transform into a smooth circle.
+ IQOP can be applied for slices with arbitrary shapes and topological holes. Extra toolpaths would be added if underfill with large area exists.
+ IQOP achieves obviously lower underfill rates, higher printing efficiency, and higher toolpath smoothness than CP.
+ A general framework of non-equidistant toolpath planning for complex slices is provided.

<p align="center">
	<img src="https://user-images.githubusercontent.com/75420225/229330834-c5703971-8983-416b-af8d-ca3f9a1ab629.png" alt="gallery" />
</p>
<p align="center">
	<b>Figure.</b> Some demos of IQOP.
</p>
<p align="center">
	<img src="https://user-images.githubusercontent.com/75420225/229330954-a3efbecf-21c8-4343-8951-ee5c9c322d86.png" alt="different_object_functions" width="500" />
</p>
<p align="center">
	<b>Figure.</b> Toolpaths generated by different object functions.
</p>
<p align="center">
	<img src="https://user-images.githubusercontent.com/75420225/229330959-f6500987-c805-4bfe-b291-896692b17eaf.png" alt="different_weight" width="500" />
</p>
<p align="center">
	<b>Figure.</b> Toolpaths generated by different weighting coefficient.
</p>
More details of IQOP would be provided after the article is published.

### Optimization Problem of IQOP

The toolpaths can be planned by offsetting non-equidistantly. The offsetting distances $\left\{\delta_i\right\}_{i=1}^n$ can be seen as optimization variables. $\delta_i$ is the offsetting distance at $\left(x_i,y_i\right)$.

<p align="center">
	<img src="https://github.com/WangY18/NEPath/assets/75420225/169ab971-35b6-42b3-a1ce-8fa356532902" alt="Underfill" height="200" />
</p>
<p align="center">
    <b>Figure.</b> Optimization variables.
</p>
Given $l$, the optimization problem for generating $\tilde{l}$ can be written as:
$$
\begin{align}
\min\quad& \lambda_QQ+\lambda_SS+\lambda_LL\\
\mathrm{s.t.}\quad& L\text{ is the length of }\tilde{l}\\
& S\text{ is the area of the region enclosed by }\tilde{l}\\
& Q=\frac{L^2}{4\pi S}\text{ is the isoperimetric quotient of }\tilde{l}\\
& \alpha\delta_{\mathrm{m}}\leq\delta_i\leq\delta_{\mathrm{m}},\forall i\in\left[n\right]\\
& \left|\dot{\delta}_i\right|\leq\dot{\delta}_{\mathrm{m}},\forall i\in\left[n\right]\\
& \left|\ddot{\delta}_i\right|\leq\ddot{\delta}_{\mathrm{m}},\forall i\in\left[n\right]\\
\end{align}
$$
In our paper `Optimization-Based Non-Equidistant Toolpath Planning for Robotic Additive Manufacturing with Non-Underfill Orientation`, the above optimization problem is convexified, and the problem of self-intersection is solved. The above method can be applied for slices with arbitrary shapes and topological structures.

## API

#### `NEPath-master/path.h`

+ `(struct)path` is a struct to store information of toolpaths. `(double*)path::x` and `(double*)path::y` are waypoints of a toolpath.
+ `paths` is a `vector` of `path`, i.e., `typedef vector<path> paths;`

#### `NEPath-master/NEPathPlanner.h`

The package `NEPathPlanner.h` include the key class of **NEPath**, i.e., `NEPathPlanner`. All operations on toolpath planning is based on `NEPathPlanner`. The API of `NEPathPlanner` is as follows:

+ `(void)NEPathPlanner::set_contour()`: Set the contour, i.e., the outer boundary, of the slice. Every slice only have one closed contour. The start point and the end point of the contour are connected by default. If you want to set the outmost toolpath has a distance from the actual outer boundary, you can call `NEPathPlanner::tool_compensate()` with a **negative** distance to obtain the outmost toolpath firstly, and set the obtained outmost toolpath as the boundary of a new slice. See the example of Zigzag and CP for details.
	+ `(const double*)x`, `(const double*)y`, `(int)length`: The number of waypoints is  `length`. The `i`-th waypoint is `(x[i],y[i])`. It can be substituted as `(const path&)contour_new`.
	+ `(bool)wash`, `(double)wash_dis`, `(int)num_least`: If `wash==true`, the contour would be resampled with a uniformly-distributed distance no more than `wash_dis`, and the number of waypoints are no less than `num_least`. By default, `wash=true, washdis=0.2, num_least=50  `.
	+ The contour is stored in a public member variable `(path)contour`.
+ `(void)NEPathPlanner::addhole()`: Add a new hole, i.e., the inner boundaries, onto the slice. The start point and the end point of every hole are connected by default. A slice is allowed to have no holes. The same as `(void)NEPathPlanner::set_contour()`, you can call `NEPathPlanner::tool_compensate()` to offset the added hole.
	+ `(const double*)x`, `(const double*)y`, `(int)length`: The number of waypoints is  `length`. The `i`-th waypoint is `(x[i],y[i])`. It can be substituted as `(const path&)hole_new`.
	+ `(bool)wash`, `(double)wash_dis`, `(int)num_least`: If `wash==true`, the contour would be resampled with a uniformly-distributed distance no more than `wash_dis`, and the number of waypoints are no less than `num_least`. By default, `wash=true, washdis=0.2, num_least=50  `.
	+ The holes are stored in a public member variable `(paths)holes`.
+ `(void)NEPathPlanner::addholes()`: Add some new holes, i.e., the inner boundaries, onto the slice. The start point and the end point of every hole are connected by default. A slice is allowed to have no holes. The same as `(void)NEPathPlanner::set_contour()`, you can call `NEPathPlanner::tool_compensate()` to offset the added hole.
	+ `(const paths&)holes_new`: Add `path`s in `holes_new` onto the slice.
	+ `(bool)wash`, `(double)wash_dis`, `(int)num_least`: If `wash==true`, the contour would be resampled with a uniformly-distributed distance no more than `wash_dis`, and the number of waypoints are no less than `num_least`. By default, `wash=true, washdis=0.2, num_least=50  `.
	+ The holes are stored in a public member variable `(paths)holes`.
+ `(paths)NEPathPlanner::tool_compensate()`: Offset the contour and holes of the slice with a distance, i.e., tool compensating.
	+ `(const ContourParallelOptions&)opts`:
		+ The offsetting distance is `opts.delta`. If `opts.delta>0`, the contour will be offset outside and the holes will be offset inside. If `opts.delta<0`, the contour will be offset inside and the holes will be offset outside.
		+ If `opts.wash==true`, the contour would be resampled with a uniformly-distributed distance no more than `opts.wash_dis`, and the number of waypoints are no less than `opts.num_least`.
	+ The order of outputs is the offsetting results of `contour`, `holes[0]`, `holes[1]`, ..., `holes[holes.size()-1]`. Note that the offsetting results of each toolpath can be one, serval, or even zero toolpath.
	+  `(paths)NEPathPlanner::tool_compensate()` is achieved based on [AngusJohnson/Clipper2](https://github.com/AngusJohnson/Clipper2).
+ `(paths)NEPathPlanner::IQOP()`: Generate the **IQOP** toolpath of a slice. The optimization problem of IQOP is provided above. If you don't need IQOP and other optimization-based toolpaths, you can comment out `#define IncludeGurobi` in `NEPath-master/setup_NEPath.h` to avoid the dependence on [Gurobi](https://www.gurobi.com/).
  + `(const NonEquidistantOptions&)opts`: 
    + `opts.delta` is the maximum distance between toolpaths. `opts.alpha` the scale of the minimum distance. The distances between toolpaths at every point are between `opts.alpha*opts.delta` and `opts.delta`, i.e., $\forall i,\delta_i\in$ (`opts.alpha*opts.delta`, `opts.delta`).  `opts.dot_delta` is $\dot\delta_\mathrm{m}$, i.e., the upper bound of $\frac{\mathrm{d}\delta}{\mathrm{d}s}$. `opts.dot_delta` is $\ddot\delta_\mathrm{m}$, i.e., the upper bound of $\frac{\mathrm{d}^2\delta}{\mathrm{d}s^2}$.
    + `opts.optimize_Q` is true if $Q$ is in the objective function. `opts.optimize_S` is true if $S$ is in the objective function. `opts.optimize_L` is true if $L$ is in the objective function.  `opts.lambda_Q`,  `opts.lambda_S`, and `opts.lambda_L` are $\lambda_Q,\lambda_S,\lambda_L$, respectively.
    + `opts.epsilon` is the upper bound of error in $\left\|\cdot\right\|_\infty$. `opts.set_max` is the maximum iteration steps.
    + If `opts.wash==true`, the contour would be resampled with a uniformly-distributed distance no more than `opts.wash_dis`, and the number of waypoints are no less than `opts.num_least`.
+ `(paths)NEPathPlanner::Raster()`: Generate the **Raster** toolpath of a slice.
	+ `(const DirectParallelOptions&)opts`: `opts.delta` is the distance between toolpaths. `opts.angle` is the angle between Raster toolpaths and the $x$-axis. The unit of `opts.angle` is rad, and you can use `acos(-1.0)` to obtain a accurate $\pi=3.1415926\cdots$.
	+ Every Raster toolpath has two waypoints, i.e., the start point and the end point.
+ `(paths)NEPathPlanner::Zigzag()`: Generate the **Zigzag** toolpath of a slice.
	+ `(const DirectParallelOptions&)opts`: `opts.delta` is the distance between toolpaths. `opts.angle` is the angle between Zigzag toolpaths and the $x$-axis. The unit of `opts.angle` is rad, and you can use `acos(-1.0)` to obtain a accurate $\pi=3.1415926\cdots$.
	+ Every Zigzag toolpath has an even numbers of waypoints.
+ `(paths)NEPathPlanner::CP()`: Generate the **CP** toolpath of a slice.
	+ `(const ContourParallelOptions&)opts`: `opts.delta` is the distance between toolpaths. If `opts.wash==true`, the contour would be resampled with a uniformly-distributed distance no more than `opts.wash_dis`, and the number of waypoints are no less than `opts.num_least`.
	+ `(paths)NEPathPlanner::CP()` is achieved based on [AngusJohnson/Clipper2](https://github.com/AngusJohnson/Clipper2).
+ Other toolpath generation algorithms and toolpath connection algorithm will be added into `NEPathPlanner` latter.

#### `NEPath-master/Curve.h`

`Curve.h` has some fundamental  methods on geometry.

+ **Underfill**. For a slice $D\subset\mathbb{R}^2$ and some toolpaths $\left\{l_i\right\}_{i=1}^N$, underfill is defined as $D\bigcap\left(\bigcup\limits_{i=1}^n B_{\frac{\delta}2}\left(l_i\right)\right)^C$, where $\delta>0$ is the line width.	
	
	+ `(static UnderFillSolution)Curve::UnderFill()`: API of calculate underfill. Return a `UnderFillSolution`.
	
		+ `(const path&)contour`: the contour of slice.
		+ `(const paths&)holes`: the holes of slice. If the slice has no hole, you can input `paths()` as an empty set of holes.
		+ `(const paths&)ps`: the toolpaths planned before.
		+ `(double)delta`: the line width $\delta$. Note that for every toolpath, only a width of $\frac\delta2$ on each side is determined as fill.
		+ `(double)reratio`: the resolution ratio. `xs` and `ys` are sampled with a distance of `reratio` between 2 points.
	
	+ `(struct)UnderFillSolution` is a struct to store information of underfill.
		
		+ `(double*)xs` and `(double*)ys` are discrete points on $x$-axis and $y$-axis.
		+ `(int)nx` and  `(int)ny` are the lengths of `xs` and `ys`.
		+ `(bool**)map_slice` stores information of slice $D$. `map_slice[i][j]==true` if and only if the point `(xs[i],ys[j])`$\in D$.
		+ `(bool**)map_delta` stores information of neighborhood of toolpaths $\bigcup\limits_{i=1}^n B_{\frac{\delta}2}\left(l_i\right)$. `map_delta[i][j]==true` if and only if the point `(xs[i],ys[j])`$\in\bigcup\limits_{i=1}^n B_{\frac{\delta}2}\left(l_i\right)$.
		+ `(double)underfillrate` is the underfill rate, i.e.,
		$$
		\text{underfill rate}=\frac{\text{area of underfill}}{\text{area of slice}}=1-\frac{\text{number of pixels in }D\bigcap\left(\bigcup\limits_{i=1}^n B_{\frac{\delta}2}\left(l_i\right)\right)}{\text{number of pixels in }D}.
		$$
	
+ **Sharp corner.** To avoid computational sensitivity, sharp corners are determined by [area invariant](https://doi.org/10.1016/j.cagd.2008.01.002) (Helmut Pottmann, et al. 2009).

	+ `(static SharpTurnSolution)Curve::SharpTurn_Invariant()`: determine sharp corners on a toolpath:
	  + `(const path&)p`: the input toolpath.
	  + `(double)radius`: the radius of the rolling circle.
	  + `(double)threshold`: the threshold to determine a sharp corner.
	  + `(bool)close`: `close` is true if and only if the toolpath is closed.
	  + `(bool)washdis`: sharp corners would be determined with a uniformly-distributed distance no more than `washdis`.
	+ `(struct)SharpTurnSolution` is a struct to store information of sharp corners for a toolpath `p`.
		+ `(int)length`: length of the toolpath.
		+ `(double)radius`: the radius of the rolling circle.
		+ `(double)threshold`: the threshold to determine a sharp corner.
		+ `(double*)AreaPercent`: `AreaPercent[i]` is the percent of area on one side of the toolpath at `(p.x[i],p.y[i])`.
		+ `(bool*)SharpTurn`: `SharpTurn[i]==ture` if and only if `AreaPercent[i]>threshold`.
		+ `(bool)close`: `close` is true if and only if the toolpath is closed.

## Examples

### Toolpath Generation

#### IQOP (Isoperimetric-Quotient-Optimal Toolpath, Wang Y et al., 2023)


```c++
	NEPathPlanner planner;

	// Obtain the contour of the outer boundary of slices
	path contour;
	contour.length = 1000; // the number of waypoints
	contour.x = new double[contour.length](); // x-coordinate of waypoints
	contour.y = new double[contour.length](); // y-coordinate of waypoints
	const double pi = acos(-1.0); // pi == 3.1415926...
	for (int i = 0; i < contour.length; ++i) {
		double theta = 2.0 * pi * i / contour.length;
		double r = 15.0 * (1.0 + 0.1 * cos(10.0 * theta));
		contour.x[i] = r * cos(theta);
		contour.y[i] = r * sin(theta);
	}

	// The out boundary should be offset with half of the line width to obtain the outmost toolpath
	NEPathPlanner planner_toolcompensate;
	planner_toolcompensate.set_contour(contour);
	ContourParallelOptions opts_toolcompensate;
	opts_toolcompensate.delta = -1.0 * 0.5; // half of the line width of toolpaths
	opts_toolcompensate.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts_toolcompensate.washdis = 0.2;
	paths path_outmost = planner_toolcompensate.tool_compensate(opts_toolcompensate);

	planner.set_contour(path_outmost[0]);
	// or `planner.set_contour(contour.x, contour.y, contour.length)`

	// Set the toolpath parameters
	NonEquidistantOptions opts;
	opts.delta = 1.0; // the line width of toolpaths
	opts.alpha = 0.5; // the scale of minimum distance
	opts.dot_delta = 1.0; // the upper bound of \dot{delta_i}
	opts.ddot_delta = 0.1; // the upper bound of \ddot{delta_i}

	opts.optimize_Q = true; // the isoperimetric quotient is in the objective function
	opts.optimize_S = false; // the area is not in the objective function
	opts.optimize_L = false; // the length is not in the objective function
	opts.lambda_Q = 1.0; // the weighting coefficient of the isoperimetric quotient

	opts.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts.washdis = 0.2;


	paths IQOP_paths = planner.IQOP(opts, true); // all IQOP paths
	cout << "There are " << IQOP_paths.size() << " continuous toolpaths in total." << endl;
```

<p align="center">
	<img src="https://github.com/WangY18/NEPath/assets/75420225/6f5a6369-1eda-444f-838b-c260aac58ecf" alt="IQOP" height="300" />
</p>
<p align="center">
	<b>Figure.</b> IQOP toolpath minimizing Q.
</p>

<p align="center">
	<img src="https://github.com/WangY18/NEPath/assets/75420225/65b5b1a7-ebc4-4f8b-b2d0-f72352afda9d" alt="IQSOP" height="300" />
</p>
<p align="center">
	<b>Figure.</b> IQOP toolpath minimizing Q+1.0S.
</p>

<p align="center">
	<img src="https://github.com/WangY18/NEPath/assets/75420225/aac675c2-644e-4ff2-b608-36d26226fc26" alt="IQSOP" height="300" />
</p>
<p align="center">
	<b>Figure.</b> IQOP toolpath minimizing L.
</p>


#### CP (Contour-Parallel)

```c++
#include "NEPath-master/NEPathPlanner.h"
#include <iostream>
using namespace std;

int main() {
	NEPathPlanner planner;

	// Obtain the contour of the outer boundary of slices
	path contour;
	contour.length = 1000; // the number of waypoints
	contour.x = new double[contour.length](); // x-coordinate of waypoints
	contour.y = new double[contour.length](); // y-coordinate of waypoints
	const double pi = acos(-1.0); // pi == 3.1415926...
	for (int i = 0; i < contour.length; ++i) {
		double theta = 2.0 * pi * i / contour.length;
		double r = 15.0 * (1.0 + 0.15 * cos(10.0 * theta));
		contour.x[i] = r * cos(theta);
		contour.y[i] = r * sin(theta);
	}

	// The out boundary should be offset with half of the line width to obtain the outmost toolpath
	NEPathPlanner planner_toolcompensate;
	planner_toolcompensate.set_contour(contour);
	ContourParallelOptions opts_toolcompensate;
	opts_toolcompensate.delta = -1.0 * 0.5; // half of the line width of toolpaths
	opts_toolcompensate.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts_toolcompensate.washdis = 0.2;
	paths path_outmost = planner_toolcompensate.tool_compensate(opts_toolcompensate);

	planner.set_contour(path_outmost[0]);
	// or `planner.set_contour(contour.x, contour.y, contour.length)`

	// Set the toolpath parameters
	ContourParallelOptions opts;
	opts.delta = 1.0; // the line width of toolpaths
	opts.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts.washdis = 0.2;

	paths CP_paths = planner.CP(opts); // all CP paths
	cout << "There are " << CP_paths.size() << " continuous toolpaths in total." << endl;
	for (int i = 0; i < CP_paths.size(); ++i) {
		// CP_paths[i] is the i-th continuous toolpath
		cout << "Toopath " << i << " has " << CP_paths[i].length << " waypoints." << endl;
	}
	
	return 0;
}
```

<p align="center">
	<img src="https://user-images.githubusercontent.com/75420225/229331109-58155b93-d897-4553-b923-28be4eecfee1.png" alt="CP" height="300" />
</p>
<p align="center">
	<b>Figure.</b> CP toolpath.
</p>


#### Zigzag

```c++
#include "NEPath-master/NEPathPlanner.h"
#include <iostream>
using namespace std;

int main() {
	NEPathPlanner planner;

	// Set the contour
	path contour;
	contour.length = 1000; // the number of waypoints
	contour.x = new double[contour.length](); // x-coordinate of waypoints
	contour.y = new double[contour.length](); // y-coordinate of waypoints
	const double pi = acos(-1.0); // pi == 3.1415926...
	for (int i = 0; i < contour.length; ++i) {
		double theta = 2.0 * pi * i / contour.length;
		double r = 15.0 * (1.0 + 0.15 * cos(10.0 * theta));
		contour.x[i] = r * cos(theta);
		contour.y[i] = r * sin(theta);
	}
	planner.set_contour(contour);
	// or `planner.set_contour(contour.x, contour.y, contour.length)`

	// Set the toolpath parameters
	DirectParallelOptions opts;
	opts.delta = 1.0; // the line width of toolpaths
	opts.angle = pi / 3.0; // the angle of Zigzag toolpaths, unit: rad

	paths zigzag_paths = planner.Zigzag(opts); // all zigzag paths
	cout << "There are " << zigzag_paths.size() << " continuous toolpaths in total." << endl;
	for (int i = 0; i < zigzag_paths.size(); ++i) {
		// zigzag_paths[i] is the i-th continuous toolpath
		cout << "Toopath " << i << " has " << zigzag_paths[i].length << " waypoints." << endl;
	}
	
	return 0;
}
```

<p align="center">
	<img src="https://user-images.githubusercontent.com/75420225/229331070-887c0172-7ad4-42dd-85a1-6c35f09a6cc4.png" alt="zigzag" height="300" />
</p>
<p align="center">
	<b>Figure.</b> Zigzag toolpath.
</p>


#### Raster

```c++
#include "NEPath-master/NEPathPlanner.h"
#include <iostream>
using namespace std;

int main() {
	NEPathPlanner planner;

	// Set the contour
	path contour;
	contour.length = 1000; // the number of waypoints
	contour.x = new double[contour.length](); // x-coordinate of waypoints
	contour.y = new double[contour.length](); // y-coordinate of waypoints
	const double pi = acos(-1.0); // pi == 3.1415926...
	for (int i = 0; i < contour.length; ++i) {
		double theta = 2.0 * pi * i / contour.length;
		double r = 15.0 * (1.0 + 0.15 * cos(10.0 * theta));
		contour.x[i] = r * cos(theta);
		contour.y[i] = r * sin(theta);
	}
	planner.set_contour(contour);
	// or `planner.set_contour(contour.x, contour.y, contour.length)`

	// Set the toolpath parameters
	DirectParallelOptions opts;
	opts.delta = 1.0; // the line width of toolpaths
	opts.angle = - pi / 3.0; // the angle of raster toolpaths, unit: rad

	paths raster_paths = planner.Raster(opts); // all raster paths
	cout << "There are " << raster_paths.size() << " continuous toolpaths in total." << endl;
	
	return 0;
}
```

<p align="center">
	<img src="https://user-images.githubusercontent.com/75420225/229333683-670c0890-1a34-4e2a-b1ca-5de918815e09.png" alt="raster" height="300" />
</p>
<p align="center">
	<b>Figure.</b> Raster toolpath.
</p>


### Toolpath Connection

The API and examples of toolpath connection would be available soon.

### Others

#### Tool  compensate

```c++
#include "NEPath-master/NEPathPlanner.h"
#include <iostream>
using namespace std;

int main() {
	NEPathPlanner planner;

	// Obtain the contour of the outer boundary of slices
	path contour;
	contour.length = 1000; // the number of waypoints
	contour.x = new double[contour.length](); // x-coordinate of waypoints
	contour.y = new double[contour.length](); // y-coordinate of waypoints
	const double pi = acos(-1.0); // pi == 3.1415926...
	for (int i = 0; i < contour.length; ++i) {
		double theta = 2.0 * pi * i / contour.length;
		double r = 15.0 * (1.0 + 0.15 * cos(10.0 * theta));
		contour.x[i] = r * cos(theta);
		contour.y[i] = r * sin(theta);
	}
	planner.set_contour(contour);

	// Obtain the hole
	double x_hole[] = { -5,5,5,0,-5 };
	double y_hole[] = { -5,-5,5,0,5 };
	planner.addhole(x_hole, y_hole, 5);

	// Tool compensate
	ContourParallelOptions opts;
	opts.delta = -1.5; // the offset distance
	opts.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts.washdis = 0.2;
	paths ps_toolcompensate = planner.tool_compensate(opts); // Tool compensate

	cout << "There are " << ps_toolcompensate.size() << " continuous toolpaths in total." << endl;
	for (int i = 0; i < ps_toolcompensate.size(); ++i) {
		// ps_toolcompensate[i] is the i-th continuous toolpath
		cout << "Toopath " << i << " has " << ps_toolcompensate[i].length << " waypoints." << endl;
	}
	
	return 0;
}
```

<p align="center">
	<img src="https://user-images.githubusercontent.com/75420225/229334415-da87d12b-8c18-4a2b-b646-a8fe7558b06e.png" alt="Tool compensate" height="300" />
</p>
<p align="center">
	<b>Figure.</b> Tool compensate.
</p>


#### Underfill

```c++
#include "NEPath-master/NEPathPlanner.h"
#include <iostream>
using namespace std;

int main() {
	NEPathPlanner planner;

	// Obtain the contour of the outer boundary of slices
	path contour;
	contour.length = 1000; // the number of waypoints
	contour.x = new double[contour.length](); // x-coordinate of waypoints
	contour.y = new double[contour.length](); // y-coordinate of waypoints
	const double pi = acos(-1.0); // pi == 3.1415926...
	for (int i = 0; i < contour.length; ++i) {
		double theta = 2.0 * pi * i / contour.length;
		double r = 15.0 * (1.0 + 0.15 * cos(10.0 * theta));
		contour.x[i] = r * cos(theta);
		contour.y[i] = r * sin(theta);
	}

	// The out boundary should be offset with half of the line width to obtain the outmost toolpath
	NEPathPlanner planner_toolcompensate;
	planner_toolcompensate.set_contour(contour);
	ContourParallelOptions opts_toolcompensate;
	opts_toolcompensate.delta = -1.0 * 0.5; // half of the line width of toolpaths
	opts_toolcompensate.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts_toolcompensate.washdis = 0.2;
	paths path_outmost = planner_toolcompensate.tool_compensate(opts_toolcompensate);

	planner.set_contour(path_outmost[0]);
	// or `planner.set_contour(contour.x, contour.y, contour.length)`

	// Set the toolpath parameters
	ContourParallelOptions opts;
	opts.delta = 1.0; // the line width of toolpaths
	opts.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts.washdis = 0.2;

	paths CP_paths = planner.CP(opts); // all CP paths

	double delta_underfill = opts.delta; // the line width for underfill computation
	double reratio = 0.03; // resolution ratio for underfill computation

	UnderFillSolution ufs = Curve::UnderFill(contour, paths(), CP_paths, delta_underfill, reratio); // Obtain the results of underfill

	cout << "The underfill rate is " << ufs.underfillrate * 100 << "%." << endl;
	
	return 0;
}
```

<p align="center">
	<img src="https://github.com/WangY18/NEPath/assets/75420225/e050a611-8108-4f4d-b293-b801443de746" alt="Underfill" height="300" />
</p>
<p align="center">
    <b>Figure.</b> Underfill. The underfill rate is 1.2401% in this example.
</p>


#### Sharp corner

```c++
#include "NEPath-master/NEPathPlanner.h"
#include <iostream>
using namespace std;

int main() {
	NEPathPlanner planner;

	// Obtain the contour of the outer boundary of slices
	path contour;
	contour.length = 1000; // the number of waypoints
	contour.x = new double[contour.length](); // x-coordinate of waypoints
	contour.y = new double[contour.length](); // y-coordinate of waypoints
	const double pi = acos(-1.0); // pi == 3.1415926...
	for (int i = 0; i < contour.length; ++i) {
		double theta = 2.0 * pi * i / contour.length;
		double r = 15.0 * (1.0 + 0.15 * cos(10.0 * theta));
		contour.x[i] = r * cos(theta);
		contour.y[i] = r * sin(theta);
	}

	// The out boundary should be offset with half of the line width to obtain the outmost toolpath
	NEPathPlanner planner_toolcompensate;
	planner_toolcompensate.set_contour(contour);
	ContourParallelOptions opts_toolcompensate;
	opts_toolcompensate.delta = -1.0 * 0.5; // half of the line width of toolpaths
	opts_toolcompensate.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts_toolcompensate.washdis = 0.2;
	paths path_outmost = planner_toolcompensate.tool_compensate(opts_toolcompensate);

	planner.set_contour(path_outmost[0]);
	// or `planner.set_contour(contour.x, contour.y, contour.length)`

	// Set the toolpath parameters
	ContourParallelOptions opts;
	opts.delta = 1.0; // the line width of toolpaths
	opts.wash = true; // it is recommended to set opt.wash=true
	// if wash==true, then all toolpaths would have yniformly distributed waypoints, with a distance near opts.washdis
	opts.washdis = 0.2;

	paths CP_paths = planner.CP(opts); // all CP paths

	double radius = 1.0; // radius of the rolling circle
	double threshold = 0.3; // threshold of area on one side to determine a sharp corner

	// Obtain the results of underfill
	int num = 0;
	for (int i = 0; i < CP_paths.size(); ++i) {
		SharpTurnSolution sol = Curve::SharpTurn_Invariant(CP_paths[i], radius, threshold, true, 0.5);
		for (int j = 0; j < sol.length; ++j) {
			num += sol.SharpTurn[j];
		}
	}

	cout << "There exist " << num << " sharp corners." << endl;
	
	return 0;
}
```

<p align="center">
	<img src="https://github.com/WangY18/NEPath/assets/75420225/4586213c-d5e2-415b-b81a-d09ed680ffe9" alt="Underfill" height="300" />
</p>
<p align="center">
    <b>Figure.</b> Sharp corners. There exist 44 sharp corners in this example.
</p>
