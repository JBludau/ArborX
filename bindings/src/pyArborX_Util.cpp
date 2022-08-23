#ifndef PYARBORX_UTIL_CPP
#define PYARBORX_UTIL_CPP

#include "pyArborX_Util.hpp"
#include <ArborX_Exception.hpp>
#include <fstream>

namespace py = pybind11;

Predicates generateWithinQueries_device(ExecutionSpace execution_space,
                                        Primitives query_pts_view,
                                        int n_queries, float radius)
{
  Kokkos::View<decltype(ArborX::intersects(ArborX::Sphere{})) *, MemorySpace>
      within_queries("within_queries", n_queries);
  Kokkos::parallel_for(
      "Setup queries",
      Kokkos::RangePolicy<ExecutionSpace>(execution_space, 0, n_queries),
      KOKKOS_LAMBDA(int i) {
        within_queries(i) =
            ArborX::intersects(ArborX::Sphere{query_pts_view(i), radius});
      });

  return within_queries;
}

PointViewType vec2view (std::vector<ArborX::Point> const & in, std::string const & label)
{
  PointViewType  out(
      Kokkos::view_alloc(label, Kokkos::WithoutInitializing), in.size());
  Kokkos::deep_copy(out, Kokkos::View<ArborX::Point const *, Kokkos::HostSpace,
                                      Kokkos::MemoryTraits<Kokkos::Unmanaged>>{
                             in.data(), in.size()});
  return out;
}


std::vector<ArborX::Point> loadData(std::string const &filename,
                                    bool binary, int max_num_points)
{
  std::cout << "Reading in \"" << filename << "\" in "
            << (binary ? "binary" : "text") << " mode...";
  std::cout.flush();

  std::ifstream input;
  if (!binary)
    input.open(filename);
  else
    input.open(filename, std::ifstream::binary);
  ARBORX_ASSERT(input.good());

  std::vector<ArborX::Point> v;

  int num_points = 0;
  int dim = 0;
  if (!binary)
  {
    input >> num_points;
    input >> dim;
  }
  else
  {
    input.read(reinterpret_cast<char *>(&num_points), sizeof(int));
    input.read(reinterpret_cast<char *>(&dim), sizeof(int));
  }

  // For now, only allow reading in 2D or 3D data. Will relax in the future.
  ARBORX_ASSERT(dim == 2 || dim == 3);

  if (max_num_points > 0 && max_num_points < num_points)
    num_points = max_num_points;

  if (!binary)
  {
    v.reserve(num_points);

    auto it = std::istream_iterator<float>(input);
    auto read_point = [&it, dim]() {
      float xyz[3] = {0.f, 0.f, 0.f};
      for (int i = 0; i < dim; ++i)
        xyz[i] = *it++;
      return ArborX::Point{xyz[0], xyz[1], xyz[2]};
    };
    std::generate_n(std::back_inserter(v), num_points, read_point);
  }
  else
  {
    v.resize(num_points);

    if (dim == 3)
    {
      // Can directly read into ArborX::Point
      input.read(reinterpret_cast<char *>(v.data()),
                 num_points * sizeof(ArborX::Point));
    }
    else
    {
      std::vector<float> aux(num_points * dim);
      input.read(reinterpret_cast<char *>(aux.data()),
                 aux.size() * sizeof(float));

      for (int i = 0; i < num_points; ++i)
      {
        ArborX::Point p{0.f, 0.f, 0.f};
        for (int d = 0; d < dim; ++d)
          p[d] = aux[i * dim + d];
        v[i] = p;
      }
    }
  }
  input.close();
  std::cout << "done\nRead in " << num_points << " " << dim << "D points"
            << std::endl;

  return v;
}

void generateUtilWrapper(py::module &m)
{

  m.def("generateWithinQueries_device", &generateWithinQueries_device,
        py::arg("ExecutionSpace"), py::arg("Primitives"), py::arg("size"),
        py::arg("radius"));

  py::bind_vector<std::vector<ArborX::Point>>(m, "VectorArborXPoint");

  m.def("Util_vec2view", &vec2view, py::arg("vectorIn"),py::arg("label"));
  m.def("Util_loadData", &loadData, py::arg("filename"),py::arg("binary"),py::arg("max_num_points"));

  m.def("ArborX_version", &ArborX::version);
  m.def("ArborX_hash", &ArborX::gitCommitHash);
  m.def("Kokkos_version", &KokkosExt::version);
}

#endif
