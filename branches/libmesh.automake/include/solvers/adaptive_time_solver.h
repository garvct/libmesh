// The libMesh Finite Element Library.
// Copyright (C) 2002-2012 Benjamin S. Kirk, John W. Peterson, Roy H. Stogner

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __adaptive_time_solver_h__
#define __adaptive_time_solver_h__

// Local includes
#include "system_norm.h"
#include "unsteady_solver.h"

// C++ includes

namespace libMesh
{

// Forward declarations
class System;

/**
 * This class wraps another UnsteadySolver derived class, and compares
 * the results of timestepping with deltat and timestepping with
 * 2*deltat to adjust future timestep lengths.
 *
 * Currently this class only works on fully coupled Systems
 *
 * This class is part of the new DifferentiableSystem framework,
 * which is still experimental.  Users of this framework should
 * beware of bugs and future API changes.
 *
 * @author Roy H. Stogner 2007
 */

// ------------------------------------------------------------
// Solver class definition
class AdaptiveTimeSolver : public UnsteadySolver
{
public:
  /**
   * The parent class
   */
  typedef UnsteadySolver Parent;

  /**
   * Constructor. Requires a reference to the system
   * to be solved.
   */
  explicit
  AdaptiveTimeSolver (sys_type& s);

  /**
   * Destructor.
   */
  virtual ~AdaptiveTimeSolver ();

  virtual void init();

  virtual void reinit();

  virtual void solve() = 0;

  virtual void advance_timestep();

  /**
   * This method is passed on to the core_time_solver
   */
  virtual Real error_order () const;

  /**
   * This method is passed on to the core_time_solver
   */
  virtual bool element_residual (bool get_jacobian,
                                 DiffContext&);

  /**
   * This method is passed on to the core_time_solver
   */
  virtual bool side_residual (bool get_jacobian,
                              DiffContext&);

  /**
   * An implicit linear or nonlinear solver to use at each timestep.
   */
  virtual AutoPtr<DiffSolver> &diff_solver();

  /**
   * This object is used to take timesteps
   */
  AutoPtr<UnsteadySolver> core_time_solver;

  /**
   * Error calculations are done in this norm, DISCRETE_L2 by default.
   */
  SystemNorm component_norm;

  /**
   * If component_norms is non-empty, each variable's contribution to the error
   * of a system will also be scaled by component_scale[var], unless
   * component_scale is empty in which case all variables will be weighted
   * equally.
   */
  std::vector<float> component_scale;

  /**
   * This tolerance is the target relative error between double-deltat
   * and single-deltat timesteps, scaled by deltat.  If this error tolerance
   * is exceeded or not met, future timesteps will be run with deltat shrunk
   * or grown to compensate.
   *
   * The default value is 1.0e-2; obviously users should select their
   * own tolerance.
   */
  Real target_tolerance;

  /**
   * This tolerance is the maximum relative error between double-deltat and
   * single-deltat timesteps, scaled by deltat.  If this error tolerance is
   * exceeded, the current timestep will be repeated with a smaller deltat.
   *
   * If you use the default upper_tolerance=0.0,
   */
  Real upper_tolerance;

  /**
   * Do not allow the adaptive time solver to select deltat > max_deltat.
   * If you use the default max_deltat=0.0, then deltat is unlimited.
   */
  Real max_deltat;

  /**
   * Do not allow the adaptive time solver to select deltat < min_deltat.
   * The default value is 0.0.
   */
  Real min_deltat;

  /**
   * Do not allow the adaptive time solver to select
   * a new deltat greater than max_growth times the old deltat.
   * If you use the default max_growth=0.0, then the deltat growth is
   * unlimited.
   */
  Real max_growth;

  /**
   * This flag, which is true by default, grows (shrinks) the timestep
   * based on the expected global accuracy of the timestepping scheme.
   * Global in this sense means the cumulative final-time accuracy of
   * the scheme.  For example, the backward Euler scheme's truncation
   * error is locally of order 2, so that after N timesteps of size
   * deltat, the result is first-order accurate.  If you set this to
   * false, you can grow (shrink) your timestep based on the local
   * accuracy rather than the global accuracy of the core TimeSolver.
   * Note that by setting this value to false you may fail to achieve
   * the predicted convergence in time of the underlying method, however
   * it may be possible to get more fine-grained control over step sizes
   * as well.
   */
  bool global_tolerance;

protected:

  /**
   * We need to store the value of the last deltat used, so
   * that advance_timestep() will increment the system time
   * correctly.
   */
  Real last_deltat;

  /**
   * A helper function to calculate error norms
   */
  virtual Real calculate_norm(System &, NumericVector<Number> &);
};


} // namespace libMesh


#endif // #define __adaptive_time_solver_