#include <chrono>
#include <fstream>

#ifdef ALGO_FAST
#include "../LibFastVC/fastvcp.hpp"
using SOLVER = FastVC;
#else
#include "../LibNuMVC/numvc.hpp"
using SOLVER = NuMVC;
#endif

#include "ParallelSolverAdapter.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  char title[1024];

  int optimal_size;
  int cutoff_time;

  sscanf(argv[2], "%d", &optimal_size);  // if you want to stop the algorithm
                                         // only cutoff time is reached, set
                                         // optimal_size to 0.
  sscanf(argv[3], "%d", &cutoff_time);

  sprintf(title, "@title file: %s        optimal_size: %d", argv[1],
          optimal_size);

  std::string filename(argv[1]);
  std::ifstream file(filename, std::ios::in);
  ParallelSolver<SOLVER> solver(file, optimal_size, std::chrono::seconds(cutoff_time), true);

  cout << "c Improved NuMVC Local Search Solver" << endl;
  cout << "c Using " << solver.get_concurrency() << " parallel instances" << std::endl;
  cout << "c Start local search..." << endl;
  solver.cover_LS(ParallelSolver<SOLVER>::default_stats_printer);
  auto solution(solver.get_independent_set());

  // check solution
  if (solver.check_solution()) {
    cout << "c Best found vertex cover size = " << solver.get_best_cover_size()
         << endl;

    cout << "c independent set:" << endl;
    for (auto v : solution) cout << v << ' ';
    cout << endl;

    auto solver_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        solver.get_best_duration());
    auto total_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        solver.get_total_duration());
    double performance = static_cast<double>(solver.get_best_step()) /
                         solver_time_ms.count() / 1000.0;
    cout << "c searchSteps = " << solver.get_best_step() << endl;
    cout << "c solveTime = " << solver_time_ms.count() << "ms" << endl;
    cout << "c totalTime = " << total_time_ms.count() << "ms" << endl;
    cout << "c performance = " << performance << "MT/s" << endl;
  } else {
    cout << "the solution is wrong." << endl;
  }

  return 0;
}