#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;
vector<vector<int>> chessboard;
int board_size = 0;

struct NODE
{
    int x;
    int y;
    int number;
    bool isBackTracking;
    vector<NODE> available_next_step;
    NODE(int loc_x, int loc_y) : x(loc_x), y(loc_y)
    {
        number = 0;
        isBackTracking = false;
    }
    NODE(int loc_x, int loc_y, const vector<NODE> list) : x(loc_x), y(loc_y), available_next_step(list)
    {
        number = 0;
        isBackTracking = false;
    }
    NODE()
    {
        x = 0;
        y = 0;
        number = 0;
        isBackTracking = false;
    }
};
//8个方向
int x_axis[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
int y_axis[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

//判断下一步能否走
bool check(int x, int y)
{
    //x,y没有越界且下一步没被走过
    if (x >= 0 && y >= 0 && x < board_size && y < board_size && chessboard[x][y] == 0)
    {
        return true;
    }
    return false;
}

//下一步有几种走法
int next_step_ways(int x, int y)
{
    int steps = 0;
    for (int i = 0; i < 8; i++)
    {
        if (check(x + x_axis[i], y + y_axis[i]))
        {
            steps++;
        }
    }
    return steps;
}

//距离中心的距离(平方)
int distance_to_center(int x, int y)
{
    int x_mid = board_size / 2 - 1;
    int y_mid = board_size / 2 - 1;

    return (x - x_mid) * (x - x_mid) + (y - y_mid) * (y - y_mid);
}

//排序的比较函数
bool cmp_next_step_ways(NODE n1, NODE n2)
{
    if (next_step_ways(n1.x, n1.y) < next_step_ways(n2.x, n2.y))
    {
        return true;
    }
    else if (next_step_ways(n1.x, n1.y) == next_step_ways(n2.x, n2.y))
    {
        if (distance_to_center(n1.x, n1.y) > distance_to_center(n2.x, n2.y))
        {
            return true;
        }
    }
    return false;
}

//判断最后是否回到起点
bool back_to_start(int x, int y)
{

    int x_mid = board_size / 2 - 1;
    int y_mid = board_size / 2 - 1;
    for (int i = 0; i < 8; i++)
    {
        if (x + x_axis[i] == x_mid && y + y_axis[i] == y_mid)
        {
            return true;
        }
    }

    return false;
}

void print_output(int original_x, int original_y)
{

    int all_cells = board_size * board_size;
    int length = all_cells - chessboard[original_x - 1][original_y - 1];

    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            chessboard[i][j] = (chessboard[i][j] + length) % all_cells + 1;
            cout << setw(3) << chessboard[i][j];
            if (j + 1 != board_size)
            {
                cout << " ";
            }
        }
        if (i + 1 != board_size)
        {
            cout << endl;
        }
    }
}
void find_next_available_jump(vector<NODE> &branch, int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        if (check(x + x_axis[i], y + y_axis[i]))
        {
            branch.emplace_back(x + x_axis[i], y + y_axis[i]);
        }
    }
}
void print()
{
    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            cout << setw(3) << chessboard[i][j];
            if (j + 1 != board_size)
            {
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void find_way(int x, int y, int original_x, int original_y, int cur_loc)
{

    stack<NODE> my_stack;
    vector<NODE> branch;
    NODE start_node(x, y);
    /*******************    初始化     ********************/
    for (int i = 0; i < 8; i++)
    {
        if (check(x + x_axis[i], y + y_axis[i]))
        {
            branch.emplace_back(NODE(x + x_axis[i], y + y_axis[i]));
        }
    }
    //对搜索顺序进行排序
    sort(branch.begin(), branch.end(), cmp_next_step_ways);
    start_node.available_next_step = branch;
    my_stack.push(start_node);
    /********************************************************/

    while (!my_stack.empty())
    {
        NODE top_node = my_stack.top();
        for (int i = top_node.number; i <= top_node.available_next_step.size();)
        {

            //回到了起点，结束
            if (cur_loc == board_size * board_size && back_to_start(top_node.available_next_step[i - 1].x, top_node.available_next_step[i - 1].y))
            {
                //输出结果
                print_output(original_x, original_y);
                exit(0);
            }
            else //没回到起点
            {
                if (cur_loc < board_size * board_size && !top_node.isBackTracking) //还没走完,且不用回溯
                {
                    vector<NODE> temp;
                    find_next_available_jump(temp, top_node.available_next_step[i].x, top_node.available_next_step[i].y);
                    my_stack.top().number = i + 1;
                    if (!temp.empty())
                    {
                        sort(temp.begin(), temp.end(), cmp_next_step_ways);
                        my_stack.push(NODE(top_node.available_next_step[i].x, top_node.available_next_step[i].y, temp));
                    }
                    else//没有下一跳了，该回溯了
                    {
                        my_stack.top().isBackTracking=true;
                        //break;
                    }
                    chessboard[top_node.available_next_step[i].x][top_node.available_next_step[i].y] = ++cur_loc;
                    i++;
                    //print();
                    break;
                }
                else //需要回溯
                {
                    my_stack.top().isBackTracking = true;
                    if (top_node.number < top_node.available_next_step.size()) //还有备选路线
                    {
                        if (check(top_node.available_next_step[i].x, top_node.available_next_step[i].y)) //下一个备选路线可行
                        {
                            chessboard[top_node.available_next_step[i - 1].x][top_node.available_next_step[i - 1].y] = 0; //之前的跳的点置为0
                            chessboard[top_node.available_next_step[i].x][top_node.available_next_step[i].y] = cur_loc;

                            vector<NODE> temp;
                            find_next_available_jump(temp, top_node.available_next_step[i].x, top_node.available_next_step[i].y);
                            if (!temp.empty())
                            {
                                sort(temp.begin(), temp.end(), cmp_next_step_ways);
                            }

                            my_stack.top().number = ++i;

                            //将新跳法放入栈中
                            my_stack.push(NODE(top_node.available_next_step[i - 1].x, top_node.available_next_step[i - 1].y, temp));
                            //print();
                            break;
                        }
                    }
                    else //没有备选路线了
                    {
                        chessboard[top_node.available_next_step[i - 1].x][top_node.available_next_step[i - 1].y] = 0;
                        cur_loc--;
                        my_stack.pop();
                        my_stack.top().isBackTracking = true;
                        //print();
                        break;
                    }
                }
            }
        }
    }
}

int main()
{
    int n, a, b;
    cin >> n >> a >> b;
    board_size = n;
    chessboard = vector<vector<int>>(n, vector<int>(n));
    chessboard[n / 2 - 1][n / 2 - 1] = 1;
    find_way(n / 2 - 1, n / 2 - 1, a, b, 1); //从中心点开始
    return 0;
}