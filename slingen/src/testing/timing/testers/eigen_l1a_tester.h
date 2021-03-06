/*
 * l1a_tester.h
 *
 *  Created on: Aug 12, 2017
 *      Author: danieles
 */

#pragma once

#include <iostream>
#include <sstream>
#include <ctime>
#include <list>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "params.h"

#include <Eigen/Dense>

#ifdef TEST
#ifndef ERM
#include "tsc.h"
#endif
#endif

#include "helpers.h"
#include "CommonDefs.h"

using namespace std;
using namespace Eigen;

typedef Map< Matrix<FLOAT, PARAM0, 1> > Vec_m;
typedef Map< Matrix<FLOAT, PARAM1, 1> > Vec_k;
typedef Map< Matrix<FLOAT, PARAM2, 1> > Vec_n;

typedef Map< Matrix<FLOAT, PARAM0, PARAM1, RowMajor> > Mat_mk;
typedef Map< Matrix<FLOAT, PARAM2, PARAM1, RowMajor> > Mat_nk;

/*
 * PARAM0 -> M
 * PARAM1 -> K
 * PARAM2 -> N
 */

static __attribute__((noinline)) void kernel(const FLOAT a, const FLOAT t, const Mat_nk& W, const Mat_mk& A, const Vec_k& x0,
		const FLOAT b, const Vec_m& y, Vec_n& y1, Vec_n& v1, Vec_n& z1, Vec_m& y2, Vec_m& v2, Vec_m& z2, Vec_k& x1, Vec_k& x) {

	y1 = a*v1 + t*z1;
	y2 = a*v2 + t*z2;
	x  = x0 + b*(W.transpose()*y1 - A.transpose()*y2);
	z1 = y1 - W*x;
	z2 = y2 - (y - A*x);
	v1 = a*v1 + t*z1;
	v2 = a*v2 + t*z2;

}

double rel_diff(FLOAT a, FLOAT b) {

	using std::max;
	double c = fabs(a), d = fabs(b);
	double m = max(c, d);

	return (m == 0.) ? 0. : fabs(a-b)/m;
}

void build(FLOAT ** a, FLOAT ** t, FLOAT ** W, FLOAT ** A, FLOAT ** x0, FLOAT ** b, FLOAT ** y,
		FLOAT ** y1, FLOAT ** init_v1, FLOAT ** v1, FLOAT ** init_z1, FLOAT ** z1,
		FLOAT ** y2, FLOAT ** init_v2, FLOAT ** v2, FLOAT ** init_z2, FLOAT ** z2, FLOAT ** x1, FLOAT ** x)
{
	srand(time(NULL));

	*a = static_cast<FLOAT *>(aligned_malloc(sizeof(FLOAT), ALIGN));
	*t = static_cast<FLOAT *>(aligned_malloc(sizeof(FLOAT), ALIGN));
	*x0 = static_cast<FLOAT *>(aligned_malloc(PARAM1*sizeof(FLOAT), ALIGN));
	*b = static_cast<FLOAT *>(aligned_malloc(sizeof(FLOAT), ALIGN));
	*W = static_cast<FLOAT *>(aligned_malloc(PARAM2*PARAM1*sizeof(FLOAT), ALIGN));
	*A = static_cast<FLOAT *>(aligned_malloc(PARAM0*PARAM1*sizeof(FLOAT), ALIGN));
	*y = static_cast<FLOAT *>(aligned_malloc(PARAM0*sizeof(FLOAT), ALIGN));

	*y1 = static_cast<FLOAT *>(aligned_malloc(PARAM2*sizeof(FLOAT), ALIGN));
	*init_v1 = static_cast<FLOAT *>(aligned_malloc(PARAM2*sizeof(FLOAT), ALIGN));
	*v1 = static_cast<FLOAT *>(aligned_malloc(PARAM2*sizeof(FLOAT), ALIGN));
	*init_z1 = static_cast<FLOAT *>(aligned_malloc(PARAM2*sizeof(FLOAT), ALIGN));
	*z1 = static_cast<FLOAT *>(aligned_malloc(PARAM2*sizeof(FLOAT), ALIGN));
	*y2 = static_cast<FLOAT *>(aligned_malloc(PARAM0*sizeof(FLOAT), ALIGN));
	*init_v2 = static_cast<FLOAT *>(aligned_malloc(PARAM0*sizeof(FLOAT), ALIGN));
	*v2 = static_cast<FLOAT *>(aligned_malloc(PARAM0*sizeof(FLOAT), ALIGN));
	*init_z2 = static_cast<FLOAT *>(aligned_malloc(PARAM0*sizeof(FLOAT), ALIGN));
	*z2 = static_cast<FLOAT *>(aligned_malloc(PARAM0*sizeof(FLOAT), ALIGN));
	*x1 = static_cast<FLOAT *>(aligned_malloc(PARAM1*sizeof(FLOAT), ALIGN));
	*x = static_cast<FLOAT *>(aligned_malloc(PARAM1*sizeof(FLOAT), ALIGN));


	rands(*a, 1, 1);
	rands(*t, 1, 1);
	rands(*x0, PARAM1, 1);
	rands(*b, 1, 1);
	rands(*W, PARAM2, PARAM1);
	rands(*A, PARAM0, PARAM1);
	rands(*y, PARAM0, 1);
	rands(*v1, PARAM2, 1);
	rands(*z1, PARAM2, 1);
	rands(*v2, PARAM0, 1);
	rands(*z2, PARAM0, 1);


	memcpy(*init_v1, *v1, PARAM2*sizeof(FLOAT));
	memcpy(*init_z1, *z1, PARAM2*sizeof(FLOAT));
	memcpy(*init_v2, *v2, PARAM0*sizeof(FLOAT));
	memcpy(*init_z2, *z2, PARAM0*sizeof(FLOAT));

}

void destroy(FLOAT * a, FLOAT * t, FLOAT * W, FLOAT * A, FLOAT * x0, FLOAT * b, FLOAT * y,
		FLOAT * y1, FLOAT * init_v1, FLOAT * v1, FLOAT * init_z1, FLOAT * z1,
		FLOAT * y2, FLOAT * init_v2, FLOAT * v2, FLOAT * init_z2, FLOAT * z2, FLOAT * x1, FLOAT * x)
{
	aligned_free(a);
	aligned_free(t);
	aligned_free(x0);
	aligned_free(b);
	aligned_free(W);
	aligned_free(A);
	aligned_free(y);
	aligned_free(y1);
	aligned_free(init_v1);
	aligned_free(v1);
	aligned_free(init_z1);
	aligned_free(z1);
	aligned_free(y2);
	aligned_free(init_v2);
	aligned_free(v2);
	aligned_free(init_z2);
	aligned_free(z2);
	aligned_free(x1);
	aligned_free(x);
}

long validate(FLOAT const * a, FLOAT const * t, FLOAT const * W, FLOAT const * A, FLOAT const * x0, FLOAT const * b, FLOAT const * y,
		FLOAT const * y1, FLOAT const * init_v1, FLOAT const * v1, FLOAT const * init_z1, FLOAT const * z1,
		FLOAT const * y2, FLOAT const * init_v2, FLOAT const * v2, FLOAT const * init_z2, FLOAT const * z2,
		FLOAT const * x1, FLOAT const * x, const double threshold)
{
	bool failed = false;
	std::list<string> soft_err_msgs;
	std::vector<string> errMsgs;

	_dlmwrite(a, 1, 1, 0, 0, 1, string(EXEC_PATH) + "/a.txt");
	_dlmwrite(b, 1, 1, 0, 0, 1, string(EXEC_PATH) + "/b.txt");
	_dlmwrite(t, 1, 1, 0, 0, 1, string(EXEC_PATH) + "/t.txt");
	_dlmwrite(init_v1, PARAM2, 1, 0, 0, 1, string(EXEC_PATH) + "/v1.txt");
	_dlmwrite(init_z1, PARAM2, 1, 0, 0, 1, string(EXEC_PATH) + "/z1.txt");
	_dlmwrite(init_v2, PARAM0, 1, 0, 0, 1, string(EXEC_PATH) + "/v2.txt");
	_dlmwrite(init_z2, PARAM0, 1, 0, 0, 1, string(EXEC_PATH) + "/z2.txt");
	_dlmwrite(x0, PARAM1, 1, 0, 0, 1, string(EXEC_PATH) + "/x0.txt");
	_dlmwrite(y, PARAM0, 1, 0, 0, 1, string(EXEC_PATH) + "/y.txt");

	_dlmwrite(x, PARAM1, 1, 0, 0, 1, string(EXEC_PATH) + "/x.txt");
	_dlmwrite(y1, PARAM2, 1, 0, 0, 1, string(EXEC_PATH) + "/y1.txt");
	_dlmwrite(y2, PARAM0, 1, 0, 0, 1, string(EXEC_PATH) + "/y2.txt");

	_dlmwrite(A, PARAM0, PARAM1, 0, 0, PARAM1, string(EXEC_PATH) + "/A.txt");
	_dlmwrite(W, PARAM2, PARAM1, 0, 0, PARAM1, string(EXEC_PATH) + "/W.txt");


	string s = string("/usr/local/bin/matlab -nodisplay -nosplash -nodesktop -r \"execpath='") + string(EXEC_PATH) + "'; addpath(execpath); run('validatel1a.m');\"";
	system(s.c_str());

	std::vector<FLOAT> v1out(PARAM2), z1out(PARAM2);
	std::vector<FLOAT> v2out(PARAM0), z2out(PARAM0);

	_dlmread(&v1out[0], PARAM2, 1, 0, 0, 1, string(EXEC_PATH) + "/v1-out.txt");
	_dlmread(&z1out[0], PARAM2, 1, 0, 0, 1, string(EXEC_PATH) + "/z1-out.txt");
	_dlmread(&v2out[0], PARAM0, 1, 0, 0, 1, string(EXEC_PATH) + "/v2-out.txt");
	_dlmread(&z2out[0], PARAM0, 1, 0, 0, 1, string(EXEC_PATH) + "/z2-out.txt");

	for (int i=0; i<PARAM2; i++) {
		double err = rel_diff(v1[i], v1out[i]);
		if (err > SOFTERRTHRESH) {
			stringstream ss;
			ss << "WARNING: Error = " << err << ": v1 | at i = "<< i << " v1 = " << v1[i] << " - v1out = " << v1out[i] << endl;
			soft_err_msgs.push_back(ss.str());
		}
		if (err > threshold) {
			failed = true;
			stringstream ss;
			ss << "Error = " << err << ": v1 | at i = "<< i << " v1 = " << v1[i] << " - v1out = " << v1out[i] << endl;
			errMsgs.push_back(ss.str());
		}
	}

	for (int i=0; i<PARAM2; i++) {
		double err = rel_diff(z1[i], z1out[i]);
		if (err > SOFTERRTHRESH) {
			stringstream ss;
			ss << "WARNING: Error = " << err << ": z1 | at i = "<< i << " z1 = " << z1[i] << " - z1out = " << z1out[i] << endl;
			soft_err_msgs.push_back(ss.str());
		}
		if (err > threshold) {
			failed = true;
			stringstream ss;
			ss << "Error = " << err << ": z1 | at i = "<< i << " z1 = " << z1[i] << " - z1out = " << z1out[i] << endl;
			errMsgs.push_back(ss.str());
		}
	}

	for (int i=0; i<PARAM0; i++) {
		double err = rel_diff(v2[i], v2out[i]);
		if (err > SOFTERRTHRESH) {
			stringstream ss;
			ss << "WARNING: Error = " << err << ": v2 | at i = "<< i << " v2 = " << v2[i] << " - v2out = " << v2out[i] << endl;
			soft_err_msgs.push_back(ss.str());
		}
		if (err > threshold) {
			failed = true;
			stringstream ss;
			ss << "Error = " << err << ": v2 | at i = "<< i << " v2 = " << v2[i] << " - v2out = " << v2out[i] << endl;
			errMsgs.push_back(ss.str());
		}
	}

	for (int i=0; i<PARAM0; i++) {
		double err = rel_diff(z2[i], z2out[i]);
		if (err > SOFTERRTHRESH) {
			stringstream ss;
			ss << "WARNING: Error = " << err << ": z2 | at i = "<< i << " z2 = " << z2[i] << " - z2out = " << z2out[i] << endl;
			soft_err_msgs.push_back(ss.str());
		}
		if (err > threshold) {
			failed = true;
			stringstream ss;
			ss << "Error = " << err << ": z2 | at i = "<< i << " z2 = " << z2[i] << " - z2out = " << z2out[i] << endl;
			errMsgs.push_back(ss.str());
		}
	}

	dumpList(soft_err_msgs, string(EXEC_PATH) + "/soft_errors.txt");

	if(failed)
		for(std::vector<string>::const_iterator i = errMsgs.begin(); i != errMsgs.end(); i++)
			cout << *i;

	return failed;

}


int test()
{

	FLOAT * a, * t, * ax0, * b, * aW, * aA, * ay, * ay1, * init_v1, * av1, * init_z1, * az1,
	* ay2, * init_v2, * av2, * init_z2, * az2, * ax1, * ax;
	long retCode = 0;

	build(&a, &t, &aW, &aA, &ax0, &b, &ay, &ay1, &init_v1, &av1, &init_z1, &az1, &ay2, &init_v2, &av2, &init_z2, &az2, &ax1, &ax);

	Mat_nk W(aW);
	Mat_mk A(aA);
	Vec_m y(ay), y2(ay2), v2(av2), z2(az2);
	Vec_k x0(ax0), x1(ax1), x(ax);
	Vec_n y1(ay1), v1(av1), z1(az1);

#ifdef VALIDATE
	kernel(*a, *t, W, A, x0, *b, y, y1, v1, z1, y2, v2, z2, x1, x);
	retCode = validate(a, t, aW, aA, ax0, b, ay, ay1, init_v1, av1, init_z1, az1, ay2, init_v2, av2, init_z2, az2, ax1, ax, ERRTHRESH);

	if (retCode > 0) {
		cout << "Validation failed." << endl;
		return retCode;
	}
#endif
	//Cache warm-up
	// RDTSCP reads ts register guaranteeing that the execution of all the code
	// we wanted to measure is completed. This way we avoid including the
	// execution of a CPUID in between. The last CPUID guarantees no other
	// instruction can be scheduled before it (and so also before RDTSCP)
#ifdef TEST
#ifdef ERM
	for(int i=0; i<2; i++)
		kernel(*a, *t, W, A, x0, *b, y, y1, v1, z1, y2, v2, z2, x1, x);
#endif

#ifndef ERM
	myInt64 start, end, overhead, overhead_tsc;
	double cycles = 0.;
	size_t num_runs = RUNS, multiplier = 1;

	init_tsc();
	overhead_tsc = get_tsc_overhead();

	do{
		num_runs = num_runs * multiplier;

		start = start_tsc();
		for(size_t i = 0; i < num_runs; i++) {
			memcpy(av1, init_v1, PARAM2*sizeof(FLOAT));
			memcpy(az1, init_z1, PARAM2*sizeof(FLOAT));
			memcpy(av2, init_v2, PARAM0*sizeof(FLOAT));
			memcpy(az2, init_z2, PARAM0*sizeof(FLOAT));
		}
		end = stop_tsc(start);
		overhead = overhead_tsc + end;

		start = start_tsc();
		for(size_t i = 0; i < num_runs; i++) {
			memcpy(av1, init_v1, PARAM2*sizeof(FLOAT));
			memcpy(az1, init_z1, PARAM2*sizeof(FLOAT));
			memcpy(av2, init_v2, PARAM0*sizeof(FLOAT));
			memcpy(az2, init_z2, PARAM0*sizeof(FLOAT));
			kernel(*a, *t, W, A, x0, *b, y, y1, v1, z1, y2, v2, z2, x1, x);
		}
		end = stop_tsc(start);
		if (end > overhead)
			end -= overhead;

		cycles = (double) end;
		multiplier = ceil (  (CYCLES_REQUIRED) / (cycles)  + 1.0 );

	}while (multiplier > 2);

	list< double > cycleList, flopList;
	size_t Rep = NUMREP;

	double flops = 4.*PARAM0*PARAM1 + 4.*PARAM1*PARAM2 + 7.*PARAM0 + PARAM1 + 6.*PARAM2;

	start = start_tsc();
	for(size_t i = 0; i < num_runs; i++) {
		memcpy(av1, init_v1, PARAM2*sizeof(FLOAT));
		memcpy(az1, init_z1, PARAM2*sizeof(FLOAT));
		memcpy(av2, init_v2, PARAM0*sizeof(FLOAT));
		memcpy(az2, init_z2, PARAM0*sizeof(FLOAT));
	}
	end = stop_tsc(start);
	overhead = overhead_tsc + end;

	for (int k = 0; k < Rep; k++) {

		start = start_tsc();
		for (int i = 0; i < num_runs; ++i) {
			memcpy(av1, init_v1, PARAM2*sizeof(FLOAT));
			memcpy(az1, init_z1, PARAM2*sizeof(FLOAT));
			memcpy(av2, init_v2, PARAM0*sizeof(FLOAT));
			memcpy(az2, init_z2, PARAM0*sizeof(FLOAT));
			kernel(*a, *t, W, A, x0, *b, y, y1, v1, z1, y2, v2, z2, x1, x);
		}
		end = stop_tsc(start);
		end -= overhead;

		cycles = ((double) end) / num_runs;

		cycleList.push_back(cycles);
		flopList.push_back(flops);

	}

	dumpList(cycleList, string(EXEC_PATH) + "/cycles.txt");
	dumpList(flopList, string(EXEC_PATH) + "/flops.txt");
#endif
#endif

destroy(a, t, aW, aA, ax0, b, ay, ay1, init_v1, av1, init_z1, az1, ay2, init_v2, av2, init_z2, az2, ax1, ax);

return retCode;
}
