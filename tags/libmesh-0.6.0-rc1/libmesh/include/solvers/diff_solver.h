// $Id: diff_solver.h,v 1.7 2006-07-13 05:18:30 roystgnr Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2005  Benjamin S. Kirk, John W. Peterson
  
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



#ifndef __diff_solver_h__
#define __diff_solver_h__

// C++ includes

// Local includes
#include "auto_ptr.h"
#include "libmesh_common.h"
#include "reference_counted_object.h"

// Forward Declarations
class DiffSolver;
class DifferentiableSystem;
class TimeSolver;

/**
 * This is a generic class that defines a solver to handle
 * DifferentiableSystems  A user can define a solver by 
 * deriving from this class and implementing certain functions.
 *
 * This class is part of the new DifferentiableSystem framework,
 * which is still experimental.  Users of this framework should
 * beware of bugs and future API changes.
 *
 * @author Roy H. Stogner 2006
 */

// ------------------------------------------------------------
// Solver class definition
class DiffSolver : public ReferenceCountedObject<DiffSolver>
{
public:
  /**
   * The type of system
   */
  typedef DifferentiableSystem sys_type;
  
  /**
   * Constructor. Requires a reference to the system
   * to be solved.
   */
  DiffSolver (sys_type& s);

  /**
   * Factory.  Requires a reference to the system
   * to be solved.  Returns a NewtonSolver by default
   */
  static AutoPtr<DiffSolver> build(sys_type& s);
  
  /**
   * Destructor.
   */
  virtual ~DiffSolver () {}

  /**
   * The initialization function.  This method is used to
   * initialize internal data structures before a simulation begins.
   */
  virtual void init ();

  /**
   * The reinitialization function.  This method is used after
   * changes in the mesh.
   */
  virtual void reinit ();

  /**
   * This method performs a solve.  What occurs in
   * this method will depend on the type of solver.  See
   * the subclasses for more details.
   */
  virtual void solve () = 0;

  /**
   * @returns a constant reference to the system we are solving.
   */
  const sys_type & system () const { return _system; }

  /**
   * The DiffSolver should not print anything to std::cout
   * unless quiet is set to false
   */
  bool quiet;

  /**
   * Each linear solver step should exit after max_linear_iterations
   * is exceeded.
   */
  unsigned int max_linear_iterations;

  /**
   * The DiffSolver should exit in failure if max_nonlinear_iterations
   * is exceeded.
   */
  unsigned int max_nonlinear_iterations;

  /**
   * The DiffSolver should exit after the residual is
   * reduced to either less than absolute_residual_tolerance
   * or less than relative_residual_tolerance times the
   * initial residual.
   *
   * Users should increase any of these tolerances that they want to use for a
   * stopping condition.
   */
  Real absolute_residual_tolerance;
  Real relative_residual_tolerance;

  /**
   * The DiffSolver should exit after the full nonlinear step norm is
   * reduced to either less than absolute_step_tolerance
   * or less than relative_step_tolerance times the largest
   * nonlinear solution which has been seen so far.
   *
   * Users should increase any of these tolerances that they want to use for a
   * stopping condition.
   */
  Real absolute_step_tolerance;
  Real relative_step_tolerance;

  /**
   * Any required linear solves will at first be done with this tolerance;
   * the DiffSolver may tighten the tolerance for later solves.
   */
  Real initial_linear_tolerance;

protected:

  /**
   * The largest solution norm which the DiffSolver has yet seen will be stored
   * here, to be used for stopping criteria based on relative_step_tolerance
   */
  Real max_solution_norm;

  /**
   * The largest nonlinear residual which the DiffSolver has yet seen will be
   * stored here, to be used for stopping criteria based on
   * relative_residual_tolerance
   */
  Real max_residual_norm;

  /**
   * @returns a writeable reference to the system we are solving.
   */
  sys_type & system () { return _system; }
  
  /**
   * A reference to the system we are solving.
   */
  sys_type& _system;

};



#endif // #define __diff_solver_h__