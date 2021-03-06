/**
 * This program tests serial LinearElasticity with a 2D ball dropping case.
 * In a free falling case, we know the velocity and displacement at a certain
 * time.
 */
#include "linear_elasticity.h"
#include "parameters.h"
#include "utilities.h"

extern template class Solid::LinearElasticity<2>;
extern template class Solid::LinearElasticity<3>;
extern template class Utils::GridCreator<2>;
extern template class Utils::GridCreator<3>;

int main(int argc, char *argv[])
{
  using namespace dealii;

  try
    {
      std::string infile("parameters.prm");
      if (argc > 1)
        {
          infile = argv[1];
        }
      Parameters::AllParameters params(infile);

      double R = 0.25;
      Vector<double> u;

      if (params.dimension == 2)
        {
          Triangulation<2> solid_tria;
          Point<2> center(0, 0);
          Utils::GridCreator<2>::sphere(solid_tria, center, R);
          Solid::LinearElasticity<2> solid(solid_tria, params);
          solid.run();
          u = solid.get_current_solution();
        }
      else if (params.dimension == 3)
        {
          Triangulation<3> solid_tria;
          Point<3> center(0, 0, 0);
          Utils::GridCreator<3>::sphere(solid_tria, center, R);
          Solid::LinearElasticity<3> solid(solid_tria, params);
          solid.run();
          u = solid.get_current_solution();
        }
      else
        {
          AssertThrow(false, ExcNotImplemented());
        }

      double umin = *std::min_element(u.begin(), u.end());
      double uerror = std::abs(umin + 5.0) / 5.0;
      AssertThrow(uerror < 1e-3, ExcMessage("Incorrect min velocity!"));
    }
  catch (std::exception &exc)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    }
  return 0;
}
