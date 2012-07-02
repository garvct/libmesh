// $Id: eigen_system.h,v 1.1 2005-05-02 13:12:28 spetersen Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2005  Benjamin S. Kirk, John W. Peterson
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributd in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


#ifndef __eigen_system_h__
#define __eigen_system_h__

#include "libmesh_config.h"

// Currently, the EigenSystem should only be available
// if SLEPc support is enabled. 
#if defined(HAVE_SLEPC)

// C++ includes

// Local Includes
#include "system.h"
#include "eigen_solver.h"


// Forward Declarations
template <typename T> class SparseMatrix;


/**
 * This class provides a specific system class.  It aims
 * at solving eigenvalue problems.  Currently, this class
 * is restricted to handle standard eigenvalue problems
 * \p A*x=lambda*x .
 */

// ------------------------------------------------------------
// EigenSystem class definition

class EigenSystem : public System
{
public:

  /**
   * Constructor.  Optionally initializes required
   * data structures.
   */
  EigenSystem (EquationSystems& es,
	       const std::string& name,
	       const unsigned int number);

  /**
   * Destructor.
   */
  ~EigenSystem ();
 
  /**
   * The type of system.
   */
  typedef EigenSystem sys_type;

  /**
   * The type of the parent
   */
  typedef System Parent;
  
  /**
   * @returns a clever pointer to the system.
   */
  sys_type & system () { return *this; }
  
  /**
   * Clear all the data structures associated with
   * the system. 
   */
  virtual void clear ();

  /**
   * Reinitializes the member data fields associated with
   * the system, so that, e.g., \p assemble() may be used.
   */
  virtual void reinit ();
  
  /**
   * Assembles & solves the eigen system. 
   */
  virtual void solve ();

  /**
   * Assembles the system matrix. 
   */
  virtual void assemble ();

  /**
   * Returns real and imaginary part of the ith eigenvalue and copies
   * the respective eigen vector to the solution vector.
   */
  virtual std::pair<Real, Real> get_eigenpair (unsigned int i);
  
  /**
   * @returns \p "Eigen".  Helps in identifying
   * the system type in an equation system file.
   */
  virtual std::string system_type () const { return "Eigen"; }

  /**
   * @returns the number of converged eigenpairs.
   */
  unsigned int get_n_converged () const {return _n_converged_eigenpairs;}

  /**
   * @returns the number of eigen solver iterations.
   */
  unsigned int get_n_iterations () const {return _n_iterations;}
  
  /**
   * The system matrix for standard eigenvalue problems.
   */
  SparseMatrix<Number> *matrix;

  /**
   * The EigenSolver, definig which interface, i.e solver
   * package to use.
   */
  AutoPtr<EigenSolver<Number> > eigen_solver;
 

protected:

  
  /**
   * Initializes the member data fields associated with
   * the system, so that, e.g., \p assemble() may be used.
   */
  virtual void init_data ();

  
private:

  /**
   * The number of converged eigenpairs.
   */
  unsigned int _n_converged_eigenpairs;

  /**
   * The number of iterations of the eigen solver algorithm.
   */
  unsigned int _n_iterations;

};



// ------------------------------------------------------------
// EigenSystem inline methods


#endif // HAVE_SLEPC

#endif