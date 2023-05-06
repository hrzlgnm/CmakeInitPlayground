#include <cassert>
#include <iostream>
#include <set>
#include <string>

#include <unistd.h>

auto delay() -> auto
{
  constexpr auto fps = 30;
  constexpr auto usecs_in_sec = 1000 * 1000;
  usleep(usecs_in_sec / fps);
}

struct cell
{
  int x;
  int y;
};

struct dimension
{
  int width;
  int height;
};

using cells = std::set<cell>;

struct neighbours
{
  cells alive;
  cells dead;
};

struct grid
{
  dimension dim;
  cells c;
  grid(dimension dim, cells c)
      : dim {dim}
      , c {std::move(c)}
  {
  }
};

auto render(const grid& board)
{
  std::cout << "Hello\n";
}

auto main() -> int
{
  constexpr auto width = 10;
  constexpr auto height = 10;

  grid gen0 {{.width = width, .height = height}, {}};

  render(gen0);
  return 0;
}
