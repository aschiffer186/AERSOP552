#include "util.h"

void test_DFS()
{
    std::cout << "DFS Test 1\n";
    Vector<bool> row;
    row.push_back(true);
    Vector<Vector<bool>> map;
    map.push_back(row);

    maze m(map);

    auto path = m.solve_DFS();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = 0; i < path.size(); ++i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";

    std::cout << "DFS Test 2\n";
    Vector<bool> row2(1, false);
    Vector<Vector<bool>> map2(1, row2);
    maze m2(map2);

    path = m2.solve_DFS();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = 0; i < path.size(); ++i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";

    std::cout << "DFS Test 3\n";
    Vector<bool> row3(4, true);
    Vector<Vector<bool>> map3(6, row3);
    map3[0][3] = false;

    maze m3(map3);
    path = m3.solve_DFS();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = path.size() - 1; int(i) >= 0; --i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";

    std::cout << "DFS Test 4\n";
    Vector<bool> row4(4, true);
    Vector<Vector<bool>> map4(6, row4);
    map4[1][1] = false;
    map4[1][2] = false;
    map4[3][0] = false;
    map4[3][1] = false;
    map4[3][3] = false;
    map4[5][0] = false;
    map4[5][1] = false;
    map4[5][2] = false;

    maze m4(map4);
    path = m4.solve_DFS();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = path.size() - 1; int(i) >= 0; --i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";

    std::cout << "DFS Test 5\n";
    Vector<bool> row5(5, true);
    Vector<Vector<bool>> map5(6, row5);
    map5[1][1] = false;
    map5[1][2] = false;
    map5[1][3] = false;
    map5[2][1] = false;
    map5[3][1] = false;
    map5[3][3] = false;
    map5[4][3] = false;
    map5[5][1] = false;
    map5[5][2] = false;
    map5[5][3] = false;

    maze m5(map5);
    path = m5.solve_DFS();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = path.size() - 1; int(i) >= 0; --i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";
}

void test_a_star()
{
    std::cout << "A STAR TEST 1\n";
    Vector<bool> row;
    row.push_back(true);
    Vector<Vector<bool>> map;
    map.push_back(row);

    maze m(map);

    auto path = m.solve_a_star();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = 0; i < path.size(); ++i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";

    std::cout << "A STAR Test 2\n";
    Vector<bool> row2(1, false);
    Vector<Vector<bool>> map2(1, row2);
    maze m2(map2);

    path = m2.solve_a_star();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = 0; i < path.size(); ++i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";

    std::cout << "A STAR Test 3\n";
    Vector<bool> row3(4, true);
    Vector<Vector<bool>> map3(6, row3);
    map3[0][3] = false;

    maze m3(map3);
    path = m3.solve_a_star();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = path.size() - 1; int(i) >= 0; --i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";

    std::cout << "A STAR Test 4\n";
    Vector<bool> row4(4, true);
    Vector<Vector<bool>> map4(6, row4);
    map4[1][1] = false;
    map4[1][2] = false;
    map4[3][0] = false;
    map4[3][1] = false;
    map4[3][3] = false;
    map4[5][0] = false;
    map4[5][1] = false;
    map4[5][2] = false;

    maze m4(map4);
    path = m4.solve_a_star();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = path.size() - 1; int(i) >= 0; --i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";

    std::cout << "A STAR Test 5\n";
    Vector<bool> row5(5, true);
    Vector<Vector<bool>> map5(6, row5);
    map5[1][1] = false;
    map5[1][2] = false;
    map5[1][3] = false;
    map5[2][1] = false;
    map5[3][1] = false;
    map5[3][3] = false;
    map5[4][3] = false;
    map5[5][1] = false;
    map5[5][2] = false;
    map5[5][3] = false;

    maze m5(map5);
    path = m5.solve_a_star();
    if(path.is_empty())
        std::cout << "Error: no path found." << std::endl;
    for(size_t i = path.size() - 1; int(i) >= 0; --i)
    {
        std::cout << path[i];
    }
    std::cout << "\n";
}

int main(int argc, char** argv)
{
    test_DFS();
    test_a_star();
    return 0;
}

