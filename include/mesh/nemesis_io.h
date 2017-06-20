// The libMesh Finite Element Library.
// Copyright (C) 2002-2017 Benjamin S. Kirk, John W. Peterson, Roy H. Stogner

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



#ifndef LIBMESH_NEMESIS_IO_H
#define LIBMESH_NEMESIS_IO_H


// Local includes
#include "libmesh/libmesh_common.h"
#include "libmesh/mesh_input.h"
#include "libmesh/mesh_output.h"
#include "libmesh/parallel_object.h"

// C++ includes

namespace libMesh
{

// Forward declarations
class Nemesis_IO_Helper;

/**
 * The \p Nemesis_IO class implements reading parallel meshes in the
 * \p Nemesis file format from Sandia National Labs.  Nemesis files
 * are essentially in the Exodus format plus some additional information.
 * All the Nemesis files for a single mesh have the same basename, e.g.
 * cylinder.e, followed by ".size.rank", where size is the total number
 * of files the Mesh is split into and rank is the ID of the processor's
 * elements that were written to the file.
 *
 * \author John Peterson
 * \date 2008
 */
class Nemesis_IO : public MeshInput<MeshBase>,
                   public MeshOutput<MeshBase>,
                   public ParallelObject
{

public:

  /**
   * Constructor.  Takes a writable reference to a mesh object.
   * This is the constructor required to read a mesh.
   */
  explicit
  Nemesis_IO (MeshBase & mesh, bool single_precision=false);

  /**
   * Destructor.
   */
  virtual ~Nemesis_IO ();

  /**
   * Implements reading the mesh from several different files.
   * You provide the basename, then LibMesh appends the ".size.rank"
   * depending on this->n_processors() and this->processor_id().
   */
  virtual void read (const std::string & base_filename) libmesh_override;

  /**
   * This method implements writing a mesh to a specified file.
   */
  virtual void write (const std::string & base_filename) libmesh_override;

  /**
   * Write one timestep's worth of the solution.
   */
  void write_timestep (const std::string & fname,
                       const EquationSystems & es,
                       const int timestep,
                       const Real time);

  /**
   * Output a nodal solution.
   */
  virtual void write_nodal_data (const std::string & fname,
                                 const std::vector<Number> & soln,
                                 const std::vector<std::string> & names) libmesh_override;

  /**
   * Output a nodal solution in parallel, without localizing the soln vector.
   */
  virtual void write_nodal_data (const std::string & fname,
                                 const NumericVector<Number> & parallel_soln,
                                 const std::vector<std::string> & names) libmesh_override;

  /**
   * Set the flag indicationg if we should be verbose.
   */
  void verbose (bool set_verbosity);

  /**
   * Write out global variables.
   */
  void write_global_data (const std::vector<Number> &,
                          const std::vector<std::string> &);

  /**
   * Write out information records.
   */
  void write_information_records (const std::vector<std::string> &);

  /**
   * If true, this flag will cause the Nemesis_IO object to attempt to
   * open an existing file for writing, rather than creating a new file.
   * Obviously this will only work if the file already exists.
   */
  void append(bool val);

private:
#if defined(LIBMESH_HAVE_EXODUS_API) && defined(LIBMESH_HAVE_NEMESIS_API)
  UniquePtr<Nemesis_IO_Helper> nemhelper;

  /**
   * Keeps track of the current timestep index being written. Used
   * when calling write_nodal_data() and other functions.
   */
  int _timestep;
#endif

  /**
   * Controls whether extra debugging information is printed to the screen or not.
   */
  bool _verbose;

  /**
   * Default false.  If true, files will be opened with EX_WRITE
   * rather than created from scratch when writing.
   */
  bool _append;

  /**
   * Helper function containing code shared between the two different
   * versions of write_nodal_data which take std::vector and
   * NumericVector, respectively.
   */
  void prepare_to_write_nodal_data (const std::string & fname,
                                    const std::vector<std::string> & names);
};


} // namespace libMesh


#endif // LIBMESH_NEMESIS_IO_H
