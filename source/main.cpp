#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>

#ifdef _WIN32
#  define _WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#  define usleep(x) Sleep(x / 1000)
#else
#  include <unistd.h>
#endif

auto delay() -> auto
{
  constexpr auto fps = 25;
  constexpr auto usecsInSec = 1000 * 1000;
  usleep(usecsInSec / fps);
}

struct Cell
{
  int x;
  int y;
  Cell(int x, int y)
      : x(x)
      , y(y)
  {
  }
  auto operator<(const Cell& other) const -> bool
  {
    if (y == other.y) {
      return x < other.x;
    }
    return y < other.y;
  }
  auto operator+(const Cell& other) const -> Cell
  {
    return Cell {x + other.x, y + other.y};
  }
};

struct Dimension
{
  int width;
  int height;
};

using Cells = std::set<Cell>;

struct Neighbors
{
  Cells alive;
  Cells dead;
};

struct Grid
{
  Dimension dim;
  Cells aliveCells;
  Grid(Dimension dim, Cells cells)
      : dim {dim}
      , aliveCells {std::move(cells)}
  {
  }
  inline auto isAlive(const Cell& c) const -> bool
  {
    return aliveCells.find(c) != aliveCells.end();
  }
};

auto getNeighbours(const Grid& grid, int x, int y) -> Neighbors
{
  // NOLINTNEXTLINE(*-avoid-c-arrays):
  const Cell neighbourOffsets[] = {
      {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
  const Cell cell(x, y);
  Cells possibleNeighbours;
  std::transform(std::begin(neighbourOffsets),
                 std::end(neighbourOffsets),
                 std::inserter(possibleNeighbours, possibleNeighbours.begin()),
                 [&cell](const Cell& offset) -> Cell { return cell + offset; });
  Cells alive;
  std::copy_if(possibleNeighbours.begin(),
               possibleNeighbours.end(),
               std::inserter(alive, alive.begin()),
               [&grid](const Cell& neighbor) -> bool
               { return grid.isAlive(neighbor); });
  Cells dead;
  std::set_difference(possibleNeighbours.begin(),
                      possibleNeighbours.end(),
                      alive.begin(),
                      alive.end(),
                      std::inserter(dead, dead.begin()));
  return Neighbors {alive, dead};
}

auto updateGrid(const Grid& grid) -> Grid
{
  Grid newCells {grid};
  std::map<Cell, int> deaded;

  for (const auto& cell : grid.aliveCells) {
    const auto neighbors = getNeighbours(grid, cell.x, cell.y);
    if (neighbors.alive.size() != 2 && neighbors.alive.size() != 3) {
      newCells.aliveCells.erase(cell);
    }
    for (const auto& dead : neighbors.dead) {
      deaded[dead] += 1;
    }
  }
  for (const auto& doomer : deaded) {
    if (doomer.second == 3) {
      newCells.aliveCells.insert(doomer.first);
    }
  }
  return newCells;
}

auto drawGrid(const Grid& grid)
{
  std::cout << "\033[1;1H\033[2J";
  for (auto row = 0; row < grid.dim.height; ++row) {
    for (auto col = 0; col < grid.dim.width; ++col) {
      if (grid.isAlive(Cell(col, row))) {
        std::cout << '#';
      } else {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}

auto glider() -> Grid
{
  // NOLINTNEXTLINE(*-magic-*)
  return Grid {{10, 10}, {{0, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}}};
}

auto gosperGlider() -> Grid
{
  // NOLINTBEGIN(*-magic-*)
  return Grid {{50, 50},
               {{22, 8}, {12, 7},  {36, 7},  {17, 9},  {11, 8},  {1, 9},
                {25, 4}, {2, 8},   {16, 7},  {25, 10}, {21, 6},  {23, 9},
                {14, 6}, {36, 6},  {22, 7},  {14, 12}, {17, 8},  {11, 10},
                {25, 9}, {35, 7},  {1, 8},   {18, 9},  {22, 6},  {21, 8},
                {23, 5}, {12, 11}, {17, 10}, {11, 9},  {35, 6},  {25, 5},
                {2, 9},  {13, 6},  {13, 12}, {15, 9},  {16, 11}, {21, 7}}};
  // NOLINTEND(*-magic-*)
}

auto main() -> int
{
  constexpr auto maxGenerations = 1000;
  auto grid {gosperGlider()};
  for (auto i = 0; i < maxGenerations; ++i) {
    drawGrid(grid);
    delay();
    grid = updateGrid(grid);
  }
  return 0;
}
