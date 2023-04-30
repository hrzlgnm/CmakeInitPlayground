#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>

enum class cell
{
  dead,
  alive,
};

auto delay() -> auto  {
  constexpr auto fps = 30;
  constexpr auto usecs_in_sec = 1000*1000;
  usleep(usecs_in_sec / fps);
}

auto inline operator<<(std::ostream& out, cell cell) -> auto&
{
  switch (cell) {
    case cell::dead:
      return out << " ";
    case cell::alive:
      return out << "#";
  }
  return out;
}

struct board
{
  board(int width, int height);
  int cols;
  int rows;
  std::vector<std::vector<cell>> cells;

  auto cell_at(int col, int row) const -> auto { return cells.at(row).at(col); }

  auto cell_alive_at(int col, int row) const -> auto { return cells.at(row).at(col) == cell::alive; }

  void set(int col, int row, cell cell) { cells.at(row).at(col) = cell; }

  auto alive_neighbours(int col, int row) const -> auto
  {
    int alive = 0;
    for (int delta_y = -1; delta_y <= 1; ++delta_y) {
      for (int delta_x = -1; delta_x <= 1; ++delta_x) {
        auto looki_x = col + delta_x;
        auto looki_y = row + delta_y;
        if (looki_x < 0) {
          looki_x += cols;
        }
        if (looki_x >= cols) {
          looki_x %= cols;
        }
        if (looki_y < 0) {
          looki_y += rows;
        }
        if (looki_y >= cols) {
          looki_y %= rows;
        }
        if (looki_x != col || looki_y != row) {
          if (cell_alive_at(looki_x, looki_y)) {
            alive++;
          }
        }
      }
    }
    return alive;
  }
};

board::board(int width, int height)
    : cols(width)
    , rows(height)
    , cells(height, std::vector<enum cell>(width, cell::dead))
{
}

auto calculate_next_board(const board& current_board) -> auto
{
  board next_board(current_board.cols, current_board.rows);
  for (auto row = 0; row < current_board.rows; ++row) {
    for (auto col = 0; col < current_board.cols; ++col) {
      auto alive_nbors = current_board.alive_neighbours(col, row);
      if (current_board.cell_alive_at(col, row)) {
        if (alive_nbors == 2 || alive_nbors == 3) {
          next_board.set(col, row, cell::alive);
        }
      } else {
        if (alive_nbors == 3) {
          next_board.set(col, row, cell::alive);
        }
      }
    }
  }
  return next_board;
}

auto render_board(int generation, const board& board)
{
  std::cout << "\e[1;1H\e[2J";
  std::cout << "G: " << generation << "\n";
  for (auto col = 0; col < board.cols; ++col) {
    for (auto row = 0; row < board.rows; ++row) {
      std::cout << board.cell_at(col, row);
    }
    std::cout << "\n";
  }
}

auto main() -> int
{
  constexpr auto width = 60;
  constexpr auto height = 60;
  board current_board(width, height);
  current_board.set(width / 2 - 1, height / 2 - 1, cell::dead);
  current_board.set(width / 2 - 1, height / 2, cell::alive);
  current_board.set(width / 2 - 1, height / 2 + 1, cell::alive);
  current_board.set(width / 2, height / 2 - 1, cell::alive);
  current_board.set(width / 2, height / 2, cell::alive);
  current_board.set(width / 2, height / 2 + 1, cell::dead);
  current_board.set(width / 2 + 1, height / 2 - 1, cell::dead);
  current_board.set(width / 2 + 1, height / 2, cell::alive);
  current_board.set(width / 2 + 1, height / 2 + 1, cell::dead);
  auto generation = 1;
  constexpr auto max_generations = 10000;
  render_board(generation, current_board);
  for (; generation <= max_generations; ++generation) {
    auto next_board = calculate_next_board(current_board);
    render_board(generation++, next_board);
    current_board = next_board;
    delay();
  }
  return 0;
}
