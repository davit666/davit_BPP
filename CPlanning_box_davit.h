#ifndef _TEST_H_
#define _TEST_H_


#include <vector>
#include <time.h>
#include <stack>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <math.h>
#include <set>
#include<errno.h>

using namespace std;
const int LEFT = 1;
const int RIGHT = 2;
const int DOWN = 3;
const int TOP = 4;

const int LEFT_DOWN = 5;
const int LEFT_TOP = 6;
const int RIGHT_DOWN = 7;
const int RIGHT_TOP = 8;

const int Min_Box_Size = 22; //最小箱子最小边长
const int Max_Allowed_Gap_Size = 5;  //可接受的不可用缝隙最大边长
const int available_gaps_num_limit = 20; //每次判断放置位置时最多比较的箱子数量

const float stability_threshold_area = 0.4; //箱子在支撑面积上的容许程度
const float stability_threshold_center = 0.6;//箱子在中心偏移上的容许程度
const int allow_z_err = 2; //箱子判断支撑底面时的高度容差
const int z_allowed_over_pallet = 0; //允许箱子超过龙车的高度


//用于遗传算法的权重参数
// const float weight_height = 0.6 * 5;
// const float weight_range = 0.9;
// const float weight_support = 0;
// const float weight_area_supported_area = 0.1;
// const float weight_area_supported_center = 0.1;
// const float weight_area_supported_volume = 0.1;
// const float weight_area_contacted = 0.9;
// const float weight_area_created = 0.6;
// const float weight_area_corner = 0.05;
const float weight_height = 1 * 5;
const float weight_range = 0.8;
const float weight_support = 0.96;
const float weight_area_supported_area = 0.06;
const float weight_area_supported_center = 0.44;
const float weight_area_supported_volume = 0.11;
const float weight_area_contacted = 0.87;
const float weight_area_created = 0.03;
const float weight_area_corner = 0.56;





//高度图网格单元,height表示该格高度
struct Grid
{
	int Height;
 	int Grid_x, Grid_y;
};

//箱子信息,packst表示此箱子是否被放入托盘,dim1, dim2, dim3表示箱子尺寸,cox, coy, coz,表示箱子放置位置,packx, packy, packz表示箱子放置姿态（即是否旋转）
struct boxinfo
{
	int packst;
	int dim1, dim2, dim3, cox, coy, coz, packx, packy, packz;
	long int vol;
};

//记录空隙信息的基础单位，分别记录了空隙的左、右、上、下以及处于的Z值，包括空隙的面积
struct gap_range
{
	int left;
	int right;
	int down;
	int top;
	int z_dim;
	int area;
};
//记录选择的空隙的高级信息，z表示空隙高度，index表示该空隙在z高度下所有空隙中的编号，orientation表示箱子是否旋转，position表示箱子紧贴空隙的哪个角放置
struct gap_info
{
	int z;
	int index;
	int box_orientation;
    int box_position = LEFT_DOWN;
};



class CPlanning_Box
{
    public:
    CPlanning_Box(int px, int py, int pz, int s_x, int s_y, int r_z,int low_gap,bool have_wall);//初始化
	// ~CPlanning_Box();
    bool Place_Box_to_Gap(boxinfo &box);//将给定箱子放入当前托盘，输出能否放置
    // void Update_Height_map();
    void Clear_CPlanning_Box();//清空，初始化所有参数

    private:
    void Initialize_Pallet(Grid **Height_Map,int px, int py, int pz,bool have_wall);//清空托盘
    
    
    void Find_Gaps_on_z(int z); //寻找在高度z上的所有空隙，将所有空隙放入 gaps-set【z】
    int Find_Rectangle_area(vector<vector<int>> available_matirx,int z); //寻找高度z空间内所有有支撑的正方形空隙
    
    
    vector<gap_info> Find_Gaps_Available(boxinfo box); //在gaps-set中由低到高寻找前x个能够放入箱子的空隙（包括旋转箱子的）
    
    
    gap_info Find_Best_Gap(boxinfo box, vector<gap_info> gap_solutions); //计算给定空隙集中所有空隙的分数，选取最高分空隙输出
    
    float Find_Best_Position_and_Evaluate_Gap(boxinfo box,gap_info &gap_solution);  //在给定单个空隙中寻找最好的放置位置（角落），并给出评分
    
    float Find_Best_Pos_in_Gap(boxinfo box, gap_range gap,gap_info &gap_solution);//计算给定空隙所有放置位置（角落）评分，选择最好的输出
    float Evaluate_Area_Supported(boxinfo box,gap_range gap); //计算给定角落支撑评分
    float Evaluate_Area_Contacted(boxinfo box,gap_range gap);//计算给定角落接触评分
    float Evaluate_Area_Created(boxinfo box,gap_range gap); //计算给定角落贡献评分
    int Find_Island_Area(int x,int y);//寻找01矩阵中包含点xy的最大面积
    float Evaluate_Area_Corner(boxinfo box,gap_range gap);

    float Evaluate_Height(boxinfo box, gap_range gap); //计算单个空隙高度评分
    float Evaluate_Range(boxinfo box, gap_range gap); //计算单个空隙范围评分
    float Evaluate_Support(boxinfo box, gap_range gap); //计算单个空隙支撑评分



    // float Find_Best_Pos_in_Gap(boxinfo box,gap_range gap,gap_info &gap_solution);


    void Place_Box_and_Update(boxinfo &box,gap_info &best_gap); //可以放置箱子的话，放置箱子并更新高度图
    void Calcul_Box_Coordinate(boxinfo &box,gap_info best_gap); //计算箱子放置具体坐标
    int Update_Height_Map(boxinfo &box); //更新高度图
    void Update_Gaps(boxinfo box, int lowest_height_to_update); //更新可用空隙信息

    float Calcul_Occupacy_Ratio(boxinfo &box); //计算容积率


    public:
    int pallet_x,pallet_y,pallet_z; //托盘大小
    int sucker_x,sucker_y; //吸盘大小
    int release_z; //吸盘放置高度
    int find_low_gap; //设置寻找最低层的范围
    int cur_box_bottom,cur_box_top; //当前箱子的底面与顶面高度
    int box_count; //箱子计数
    int pallet_volume; //托盘容积
    int packed_volume;  //放置箱子总容积
    float volume_ratio; //容积率
    bool pallet_is_full; //托盘是否已满
    bool have_wall; //是否有墙壁（是龙车）
    



    vector<boxinfo> BoxType;  //箱子信息

    Grid ** Height_Map;  //高度图矩阵
    vector<vector<gap_range>> gaps_set; //空隙记录二维矩阵，第一位z代表托盘高度，第二位index代表z高度下的所有空隙
    set<int> gaps_height; //当前托盘可用来放置箱子的所有高度，堆结构，可自动排序
    vector<int> temp_gaps_height; //用于缓存高度信息的临时向量
    
    vector<boxinfo> packed_boxes; //用于存储所有已经放置箱子信息的箱子序列向量

    vector<vector<int>> island_matrix;  //用于寻找 max island的临时矩阵




     

};








#endif