/*
	Extended code from:

    This file is part of Leela Zero.

    Leela Zero is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leela Zero is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Leela Zero.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

/*
 * We need to check for input while we are thinking.
 * That code isn't portable, so select something appropriate for the system.
 */
#ifdef _WIN32
#undef HAVE_SELECT
#define NOMINMAX
#else
#define HAVE_SELECT
#endif

/*
 * Features
 *
 * USE_BLAS: Use a basic linear algebra library.
 * We currently require this, as not all operations are performed on
 * the GPU - some operations won't get any speedup from it.
 * Also used for OpenCL self-checks.
 */
/*
 * We use OpenBLAS by default, except on macOS, which has a fast BLAS
 * built-in. (Accelerate)
 */
#if !defined(__APPLE__) && !defined(__MACOSX)
#define USE_OPENBLAS
#endif
/*
 * USE_MKL: Optionally allows using Intel Math Kernel library as
 * BLAS implementation. Note that MKL's license is not compatible with the GPL,
 * so do not redistribute the resulting binaries. It is fine to use it on your
 * own system.
 */
//# define USE_MKL
/*
 * USE_OPENCL: Use OpenCL acceleration for GPUs. This makes the program a lot
 * faster if you have a recent GPU. Don't use it on CPUs even if they have
 * OpenCL drivers - the BLAS version is much faster for those.
 */
#define USE_OPENCL

/*
 * USE_TUNER: Expose some extra command line parameters that allow tuning the
 * search algorithm.
 */
// #define USE_TUNER

#define PROGRAM_NAME "Gmk1"
#define PROGRAM_VERSION "0.1"

/*
 * OpenBLAS limitation: the default configuration on some Linuxes
 * is limited to 64 cores.
 */
#if defined(USE_BLAS) && defined(USE_OPENBLAS)
#define MAX_CPUS 64
#else
#define MAX_CPUS 128
#endif

using net_t = float;

#if defined(USE_BLAS) && defined(USE_OPENCL) && !defined(USE_HALF)
// If both BLAS and OpenCL are fully usable, then check the OpenCL
// results against BLAS with some probability.
//#define USE_OPENCL_SELFCHECK
#define SELFCHECK_PROBABILITY 2000
#endif

#if (_MSC_VER >= 1400) /* VC8+ Disable all deprecation warnings */
    #pragma warning(disable : 4996)
#endif /* VC8+ */

#include "../Common.h"

const int BOARD_SIZE = BSIZE;
const int RESIDUAL_BLOCKS = 6;
const int RESIDUAL_FILTERS = 96;
const int FULL_CONNECT_SIZE = 128;
const int ACTION_SPACE_N = BLSIZE;
#ifdef RULE_GOMOKU
const int INPUT_CHANNELS = 2;
#endif
#ifdef RULE_RENJU
const int INPUT_CHANNELS = 3;
#endif
#endif
