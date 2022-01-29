/*
Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License only.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <main.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <algorithm/dijkstra>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

struct xy {
    int x, y;
    bool operator==(const xy& other) const {
        return ((this->x == other.x) && (this->y == other.y));
    }
};

struct map {
    constexpr static const int width = 10;
    constexpr static const int height = 10;
    bool grid[height][width] = {
        { 1, 1, 0, 0, 0, 1, 1, 1, 0, 0 },
        { 1, 1, 0, 1, 1, 1, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 0, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 0, 0, 0, 1, 0, 0 },
        { 0, 1, 1, 1, 0, 0, 0, 1, 1, 0 },
        { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 0, 1, 0, 1 },
        { 0, 1, 1, 0, 0, 1, 0, 1, 0, 1 },
        { 0, 1, 1, 0, 0, 1, 1, 1, 0, 1 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }
    };

    std::vector<xy> get_moves(const xy& current){
        std::vector<xy> result;
        result.reserve(4);
        result.push_back({current.x - 1, current.y});
        result.push_back({current.x + 1, current.y});
        result.push_back({current.x, current.y - 1});
        result.push_back({current.x, current.y + 1});
        result.erase(std::remove_if(result.begin(), result.end(), [this](const xy& node){
            if (node.x < 0) return true;
            if (node.x >= width) return true;
            if (node.y < 0) return true;
            if (node.y >= height) return true;
            if (this->grid[node.y][node.x] == false) return true;
            return false;
        }), result.end());
        return result;
    }

    void print(const xy& start, const xy& end, std::vector<xy> path = {}) {
        printf("#");
        for (int x = 0; x < width; ++x) {
            printf("#");
        }
        printf("#\n");
        for (int y = 0; y < height; ++y) {
            printf("#");
            for (int x = 0; x < width; ++x) {
                bool on_start = (x == start.x) && (y == start.y);
                bool on_end = (x == end.x) && (y == end.y);
                bool on_path = (std::find(path.begin(), path.end(), xy{x, y}) != path.end());
                if (on_start) { printf("S"); continue; }
                if (on_end) { printf("E"); continue; }
                if (on_path) { printf("o"); continue; }
                if (this->grid[y][x]) { printf(" "); continue; }
                printf("X");
            }
            printf("#\n");
        }
        printf("#");
        for (int x = 0; x < width; ++x) {
            printf("#");
        }
        printf("#\n");
        printf("\n");
    }
};

TEST(dijkstra, evaluate, solve) {

    xy start = {0, 0};
    xy end = {9, 9};
    std::vector<xy> path;
    map maze;

    maze.print(start, end, path);

    gtl::dijkstra<xy>::solve(
        start,
        end,
        [&maze](const xy& current)->std::vector<xy>{
            return maze.get_moves(current);
        },
        path
    );

    maze.print(start, end, path);
}
