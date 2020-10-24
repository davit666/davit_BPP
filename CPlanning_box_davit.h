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


using namespace std;
const int LEFT = 1;
const int RIGHT = 2;
const int DOWN = 3;
const int TOP = 4;
const int LEFT_DOWN = 5;
const int LEFT_TOP = 6;
const int RIGHT_DOWN = 7;
const int RIGHT_TOP = 8;
const int Min_Box_Size = 22;
const int available_gaps_num_limit = 10;



const int weight_height = 1;
const int weight_range = 1;
const int weight_support = 1;
const int weight_area_supported = 1;
const int weight_area_contacted = 1;
const int weight_area_created = 1;






//空间网格单元,data标记这个格子是否被占用,Grid_x, Grid_y, Grid_z表示格子所在位置
struct Grid
{
	int Height;
 	int Grid_x, Grid_y;
};

//箱子信息,packst表示此箱子是否被放入托盘,dim1, dim2, dim3表示箱子尺寸,cox, coy, coz,表示箱子放置位置,packx, packy, packz表示箱子放置姿态
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

struct gap_info
{
	int z;
	int index;
	int box_orientation;
    int box_position;
};
class CPlanning_Box
{
    public:
    CPlanning_Box(int px, int py, int pz, int s_x, int s_y, int r_z, int allow_z, int low_gap,int z_allowed_over_pallet,bool have_wall);
	// ~CPlanning_Box();
    bool Place_Box_to_Gap(boxinfo &box);
    void Update_Height_map();
    void Clear_CPlanning_Box();

    private:
    void Initialize_Pallet(Grid **Height_Map,int px, int py, int pz,bool have_wall);
    
    
    void Find_Gaps_on_z(int z);
    int Find_Rectangle_area(vector<vector<int>> available_matirx,int z);
    
    
    vector<gap_info> Find_Gaps_Available(boxinfo box);
    
    
    gap_info Find_Best_Gap(boxinfo box, vector<gap_info> gap_solutions);
    int Find_Best_Position_and_Evaluate_Gap(boxinfo box,gap_info &gap_solution);
    int Evaluate_Height(boxinfo box, gap_range gap);
    int Evaluate_Range(boxinfo box, gap_range gap);
    int Evaluate_Support(boxinfo box, gap_range gap);

    // int Find_Best_Pos_in_Gap(boxinfo box, gap_range gap,gap_info &gap_solution);
    int Evaluate_Area_Supported(boxinfo box,gap_range gap);
    int Evaluate_Area_Contacted(boxinfo box,gap_range gap);
    int Evaluate_Area_Created(boxinfo box,gap_range gap);
    int Find_Best_Pos_in_Gap(boxinfo box,gap_range gap,gap_info &gap_solution);


    void Place_Box_and_Update(boxinfo &box,gap_info &best_gap);
    void Calcul_Box_Coordinate(boxinfo &box,gap_info best_gap);
    int Update_Height_Map(boxinfo &box);
    void Update_Gaps(boxinfo box, int lowest_height_to_update);

    float Calcul_Occupacy_Ratio(boxinfo &box);

    public:
    int pallet_x,pallet_y,pallet_z;
    int sucker_x,sucker_y;
    int release_z;
    int find_low_gap;
    int z_allowed_over_pallet;
    int cur_box_bottom,cur_box_top;
    int box_count;
    int pallet_volume;
    int packed_volume; 
    float volume_ratio;
    bool pallet_is_full;
    bool have_wall;
    



    vector<boxinfo> BoxType;

    Grid ** Height_Map;
    vector<vector<gap_range>> gaps_set;
    set<int> gaps_height;
    vector<int> temp_gaps_height;
    
    vector<boxinfo> packed_boxes;




     

};








#endif