/***************************************************************************
    File                 : nsl_dft.c
    Project              : LabPlot
    Description          : NSL discrete Fourier transform functions
    --------------------------------------------------------------------
    Copyright            : (C) 2016 by Stefan Gerlach (stefan.gerlach@uni.kn)

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include "nsl_common.h"
#include "nsl_dft.h"
#ifdef HAVE_FFTW3
#include <fftw3.h>
#endif

const char* nsl_dft_result_type_name[] = {i18n("Magnitude"), i18n("Amplitude"), i18n("real part"), i18n("imaginary part"), i18n("Power"), i18n("Phase"),
		i18n("Amplitude in dB"), i18n("normalized amplitude in dB"), i18n("Magnitude squared"), i18n("Amplitude squared"), i18n("raw")};
const char* nsl_dft_xscale_name[] = {i18n("Frequency"), i18n("Index"), i18n("Period")};

int nsl_dft_transform_window(double data[], size_t stride, size_t n, int two_sided, nsl_dft_result_type type, nsl_sf_window_type window_type) {
	/* apply window function */
	if (window_type != nsl_sf_window_uniform)
		nsl_sf_apply_window(data, n, window_type);

	/* transform */
	int status = nsl_dft_transform(data, stride, n, two_sided, type);

	return status;
}

int nsl_dft_transform(double data[], size_t stride, size_t n, int two_sided, nsl_dft_result_type type) {
	size_t i;
	double result[2*n];
	size_t N=n/2;	/* number of resulting data points */
	if(two_sided)
		N=n;
#ifdef HAVE_FFTW3
	/* stride ignored */
	(void)stride;

	fftw_plan plan = fftw_plan_dft_r2c_1d(n, data, (fftw_complex *) result, FFTW_ESTIMATE);
	fftw_execute(plan);
	fftw_destroy_plan(plan);

	/* 2. unpack data */
	if(two_sided) {
		for(i = 1; i < n-i; i++) {
			result[2*(n - i)] = result[2*i];
			result[2*(n - i)+1] = -result[2*i+1];
		}
		if (i == n - i) {
			result[2*i] = result[2*(n-1)];
			result[2*i+1] = 0;
		}
	}
#else
	/* 1. transform */
	gsl_fft_real_wavetable *real = gsl_fft_real_wavetable_alloc(n);
	gsl_fft_real_workspace *work = gsl_fft_real_workspace_alloc(n);

	gsl_fft_real_transform(data, stride, n, real, work);
	gsl_fft_real_wavetable_free(real);
	gsl_fft_real_workspace_free(work);

	/* 2. unpack data */
	gsl_fft_halfcomplex_unpack(data, result, stride, n);
#endif

	/* 3. write result */
	switch(type) {
	case nsl_dft_result_magnitude:
		for (i = 0; i < N; i++)
			data[i] = sqrt(gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]));
		break;
	case nsl_dft_result_amplitude:
		for (i = 0; i < N; i++) {
			data[i] = sqrt(gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]))/n;
			if(i>0)
				data[i] *= 2.;
		}
		break;
	case nsl_dft_result_real:
		for (i = 0; i < N; i++)
			data[i] = result[2*i];
		break;
	case nsl_dft_result_imag:
		for (i = 0; i < N; i++)
			data[i] = result[2*i+1];
		break;
	case nsl_dft_result_power:
		for (i = 0; i < N; i++) {
			data[i] = (gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]))/n;
			if (i > 0)
				data[i] *= 2.;
		}
		break;
	case nsl_dft_result_phase:
		for (i = 0; i < N; i++)
			data[i] = -atan2(result[2*i+1],result[2*i]);
		break;
	case nsl_dft_result_dB:
		for (i = 0; i < N; i++) {
			if (i == 0)
				data[i] = 20.*log10(sqrt(gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]))/(double)n);
			else
				data[i] = 20.*log10(2.*sqrt(gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]))/(double)n);
		}
		break;
	case nsl_dft_result_normdB: {
		double maxdB=0;
		for (i = 0; i < N; i++) {
			if (i == 0)
				data[i] = 20.*log10(sqrt(gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]))/(double)n);
			else
				data[i] = 20.*log10(2.*sqrt(gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]))/(double)n);

			if (i == 0 || maxdB < data[i])
				maxdB = data[i];
		}
		for (i = 0; i < N; i++)
			data[i] -= maxdB;
		break;
	}
	case nsl_dft_result_squaremagnitude:
		for (i = 0; i < N; i++)
			data[i] = gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]);
		break;
	case nsl_dft_result_squareamplitude:
		for (i = 0; i < N; i++) {
			data[i] = (gsl_pow_2(result[2*i])+gsl_pow_2(result[2*i+1]))/gsl_pow_2(n);
			if (i > 0)
				data[i] *= 4.;
		}
		break;
	case nsl_dft_result_raw:
		break;
	}

	return 0;
}

