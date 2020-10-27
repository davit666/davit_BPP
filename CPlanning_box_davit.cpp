#include "CPlanning_box_davit.h"
//二维点(或向量)结构体定义
struct POINT { int x; int y; };
typedef vector<POINT> PTARRAY;
//判断两个点(或向量)是否相等
bool operator==(const POINT &pt1, const POINT &pt2) {
	return (pt1.x == pt2.x && pt1.y == pt2.y);
}

CPlanning_Box::CPlanning_Box(int px, int py, int pz, int s_x, int s_y, int r_z,int low_gap,bool have_wall)
{
    //pallet information
   	pallet_x = px;
	pallet_y = py;
	pallet_z = pz;
    pallet_volume = px * py * pz;

    //sucker information
	sucker_x = s_x;
	sucker_y = s_y;
	release_z = r_z; 

    //other information
	find_low_gap = low_gap;
    have_wall = have_wall;
    pallet_is_full = false;
    packed_volume = 0;
    volume_ratio = 0;
    box_count = 0;
    // gaps_height.insert(cur_box_bottom); 

	
    //初始化码垛高度图
    Height_Map = new Grid*[pallet_x+1];
    for (int i = 0; i < pallet_x + 1; i++)
    {
        Height_Map[i] = new Grid[pallet_y+1];
    }
    Initialize_Pallet(Height_Map,pallet_x,pallet_y,pallet_z,have_wall);
    
    //初始化存储空隙信息的二维向量
    gaps_set.resize(pallet_z + 1);
    //更新高度0上的所有空隙信息，放入gaps-set
    Find_Gaps_on_z(0);

    //初始化island矩阵
    island_matrix.resize(pallet_x + 1);
    for (int i = 0; i< pallet_x +1; i++)
    {
        island_matrix[i].resize(pallet_y +1);
    }

}
void CPlanning_Box::Initialize_Pallet(Grid **Height_Map, int px, int py, int pz,bool have_wall)
{
	//初始化托盘，清空托盘

    for (int i = 0; i <= px; i++)
		for (int j = 0; j <= py; j++)
			{
				Height_Map[i][j].Height = 0;
                
                //如果考虑墙壁，将托盘最外圈设置为墙壁高度pallet-z
                if (have_wall)
                {
                    if ((i == 0)||(j == 0))    
				    {
				    	Height_Map[i][j].Height = pz;
				    }
				    if ((i == px)||(j == py))
				    {
					    Height_Map[i][j].Height = pz;
				    }
                }
				

				Height_Map[i][j].Grid_x = i;
				Height_Map[i][j].Grid_y = j;
			}
    // Height_Map[pallet_x-10][10].Height = 12;
    cout <<"pallet initialized" << endl;
	return;
}

void CPlanning_Box::Clear_CPlanning_Box()
{
    //初始化所有参数，用于重新码垛
    

    pallet_is_full = false;
    packed_volume = 0;
    volume_ratio = 0;
    box_count = 0;

    packed_boxes.clear();
    gaps_set.clear();
    gaps_set.resize(pallet_z + 1);
    gaps_height.clear();
    temp_gaps_height.clear();
    Initialize_Pallet(Height_Map,pallet_x,pallet_y,pallet_z,have_wall);
    Find_Gaps_on_z(0);


}



void CPlanning_Box::Find_Gaps_on_z(int z)
{
    
    // 根据给定高度z
    
    //先清空高度z上的已有空隙信息
    if (gaps_set[z].size())
    {
        gaps_set[z].clear();
    }
    
    //构建01临时矩阵， 1、2代表该点当前高度小于等于高度z、可用来放置箱子，0代表该点超高，不能放箱子
    vector<vector<int>> available_matirx(pallet_x +1, vector<int>(pallet_y +1));
    for (int i =1; i <= pallet_x; i++)
    {
        for (int j = 1; j <= pallet_y; j++)
        {
            if (Height_Map[i][j].Height < z) //该点当前高度小于高度z、可用来放置箱子
            {
                available_matirx[i][j] = 1;
            }
            else if (Height_Map[i][j].Height == z) //该点高度与z相等，作特殊标记
            {
                available_matirx[i][j] = 2;
            }
            else //该点超高，不能放箱子
            {
                available_matirx[i][j] = 0;
            }
        }
    }
    int rec_num = 0;
    
    rec_num = Find_Rectangle_area(available_matirx,z);//根据高度z的01临时矩阵，寻找所有可利用空隙正方形，返回正方形数量
    
    if (rec_num>0)
    {
        gaps_height.insert(z); //如果有正方形，将z高度设置为可以放置箱子的高度，纳入记录所有可放置高度信息的堆结构
    }

}

int CPlanning_Box::Find_Rectangle_area(vector<vector<int>> available_matirx,int z)
{
    //寻找01矩阵中所有的正方形，要求每个正方形一定不含0，一定含有至少一个2
    //将所有的空隙的基础信息放入gap-set【z】中

    if (available_matirx.empty()) return 0;
    const int m = available_matirx.size();
    const int n = available_matirx[0].size();
    vector<int> left(n,0);
    vector<int> right(n,n);
    vector<int> height(n,0);
    vector<int> height2(n,0);

    int maxA = 0;
	for (int i = 0; i < m; i++)
	{
        int cur_left = 0, cur_right = n; //当前left，当前n

		// 计算高度compute height (can do this from either side)
		for (int j = 0; j < n; j++)
		{
			if (available_matirx[i][j] >= 1) 
            {
                height[j]++;
                height2[j] += available_matirx[i][j];
            }
			else 
            {
                height[j] = 0;
                height2[j] = 0;
            }
		}

		// 计算左边compute left (from left to right)
		for (int j = 0; j < n; j++)
		{
			if (available_matirx[i][j] >= 1) left[j] = max(left[j], cur_left);
			else { left[j] = 0; cur_left = j + 1; }
		}

		// 计算右边compute right (from right to left)
		for (int j = n - 1; j >= 0; j--)
		{
			if (available_matirx[i][j] >= 1) right[j] = min(right[j], cur_right);
			else { right[j] = n; cur_right = j; }
		}
		// 计算面积compute the area of rectangle (can do this from either side)
		for (int j = 0; j < n; j++)
		{
			int area2 = (right[j] - left[j])*height2[j];
            int area = (right[j] - left[j])*height[j];
			gap_range temp_range;

			
			temp_range.left = i - height[j] + 1;
			temp_range.right = i;
			temp_range.down = left[j];
			temp_range.top = right[j] - 1;
			temp_range.z_dim = z;
            temp_range.area = area;
			maxA = max(maxA, area);

			//判断信息是否可用，若空隙某两条边边长都大于最小箱子边长，则可用，记录在案
            if (((area != 0)&&(area2 > area))&&((temp_range.right - temp_range.left + 1>= Min_Box_Size)&&(temp_range.top - temp_range.down + 1>= Min_Box_Size)))
			{
				bool Push_flag = 1;
				if (gaps_set[z].size())
				{
					for (vector<gap_range>::iterator iter = gaps_set[z].begin(); iter != gaps_set[z].end(); )//遍历v_range_temp[z_dim]中的元素，如果其中的range被当前的range覆盖，则删除v_range_temp中的该元素
					{
						if ((*iter).left >= temp_range.left && (*iter).right <= temp_range.right && (*iter).down >= temp_range.down && (*iter).top <= temp_range.top)
							iter = gaps_set[z].erase(iter);
						else
							iter++;
					}

					for (vector<gap_range>::iterator iter = gaps_set[z].begin(); iter != gaps_set[z].end(); iter++)//遍历v_range_temp[z_dim]中的元素，如果当前的range被其中的某个range被覆盖，则不将当前range push到v_range_temp中
					{
						if ((*iter).left <= temp_range.left && (*iter).right >= temp_range.right && (*iter).down <= temp_range.down && (*iter).top >= temp_range.top)
							Push_flag = 0;
					}
				}

				if (Push_flag == 1)
                {
                    gaps_set[z].push_back(temp_range);
                }
					
			}

		}
	}
    // cout <<"now here is \t"<<z<<"\tfloor, size is\t"<< gaps_set[z].size()<<"\tmax area is:\t"<<maxA<<endl;
	return gaps_set[z].size();
}
bool CPlanning_Box::Place_Box_to_Gap(boxinfo &box)
{
    
    //主程序，判断当前箱子能否放入当前托盘，输出0、1


    cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
    cout <<"start"<<endl;
    cout <<"box size is:\t"<<box.dim1 <<"\t"<<box.dim2<<"\t"<<box.dim3<<endl;
    

    vector<gap_info> gap_solutions = Find_Gaps_Available(box); //从当前空隙信息中拿出最多x个可放置当前箱子的空隙，记录于gap-solutions中
    cout<< "solution number:\t "<<gap_solutions.size()<<endl;

    if (gap_solutions.size() <=0) // gap soution为空，即无解
    {
        cout << "no solution" << endl;
        pallet_is_full = true;
        // cout <<"heap, number:\t"<<gaps_height.size()<<endl;
        return false;
    }
    
    

    gap_info best_gap = Find_Best_Gap(box,gap_solutions);//从gap solutions中寻找最好gap
    cout <<"best gap info is :\t"<<best_gap.z<<"  "<<best_gap.index<<endl;
    cout<< "orientation and position:\t "<<best_gap.box_orientation<<" "<<best_gap.box_position<<endl;
    cout <<"best gap range is :\t"<<gaps_set[best_gap.z][best_gap.index].left<<"  "<<gaps_set[best_gap.z][best_gap.index].right<<"  "<<gaps_set[best_gap.z][best_gap.index].down<<"  "<<gaps_set[best_gap.z][best_gap.index].top<<endl;
    gap_solutions.clear();    
    
    
    Place_Box_and_Update(box,best_gap);//放置箱子，更新高度图与新生成的空隙信息
    cout<<"############################################################"<<endl;
    return true;

}

vector<gap_info> CPlanning_Box::Find_Gaps_Available(boxinfo box)
{
    //从当前空隙信息中拿出最多x个可放置当前箱子的空隙，记录于gap-solutions中
    
    vector<gap_info> gap_solutions;//用于存储空隙高级信息的向量
    temp_gaps_height.clear();//用于临时存储堆结构中的高度
    
    while (gap_solutions.size()< available_gaps_num_limit)//当目前找到的解数量小于x
    {

        //从堆结构中拿出最小的可放置高度z，寻找z中的可用空隙
        int z = *gaps_height.begin();
        int highest_z = *gaps_height.end();
        temp_gaps_height.insert(temp_gaps_height.end(),z);
        gaps_height.erase(z);
        
        //遍历 gaps-set【z】中所有空隙
        for (int i = 0; i < gaps_set[z].size();i++)
        {
            if (gaps_set[z][i].z_dim + box.dim3<= pallet_z + z_allowed_over_pallet && gaps_set[z][i].z_dim + box.dim3 >= highest_z -find_low_gap)//不超高
            {

                if (((gaps_set[z][i].right-gaps_set[z][i].left + 1)>= box.dim1)&&((gaps_set[z][i].top-gaps_set[z][i].down + 1)>= box.dim2))//箱子可以放进空隙
                {
                    gap_info temp_gap;
                    temp_gap.z = z;
                    temp_gap.index = i;
                    temp_gap.box_orientation = 0;
                    
                    gap_solutions.push_back(temp_gap);//将该空隙的高级信息计入gap-solutions中
                    
                }
                if (((gaps_set[z][i].right-gaps_set[z][i].left + 1)>= box.dim2)&&((gaps_set[z][i].top-gaps_set[z][i].down)>= box.dim1 + 1))//箱子旋转后可以放入空隙
                {
                    gap_info temp_gap_r;
                    temp_gap_r.z = z;
                    temp_gap_r.index = i;
                    temp_gap_r.box_orientation = 1;

                    gap_solutions.push_back(temp_gap_r);
                    
                }
            }
        }
        if (gaps_height.size()<1) break; //将堆机构中所有的可放置箱子高度全部取出，已经没有可用空隙了
    }
    for (int i = 0; i< temp_gaps_height.size();i++)
    {
        gaps_height.insert(temp_gaps_height[i]); //将在temp_gaps_height临时存储的可放置高度信息还给堆结构，堆结构自动排序
    }
    temp_gaps_height.clear(); //清空临时存储

    // cout << "number of availale gaps:\t"<<gap_solutions.size()<<endl;
    return gap_solutions;
}
gap_info CPlanning_Box::Find_Best_Gap(boxinfo box, vector<gap_info> gap_solutions)
{
    boxinfo temp_box = box;
    gap_info best_gap; //用于记录最好空隙
    float best_score = -10000;//用于记录最好空隙分数

    for(int i = 0;i < gap_solutions.size();i++)//遍历所有选中空隙
    {
        float tmp_score = Find_Best_Position_and_Evaluate_Gap(temp_box, gap_solutions[i]);//判断空隙内最好放置位置，返回分数
        if (tmp_score > best_score)
        {
            best_score = tmp_score;
            best_gap = gap_solutions[i];
        }
    }
    // cout <<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%best score is:\t"<<best_score<<endl;
    return best_gap;
}
float CPlanning_Box::Find_Best_Position_and_Evaluate_Gap(boxinfo box,gap_info &gap_solution)
{
    //判断空隙内最好放置位置，返回分数
    if (gap_solution.box_orientation == 1) //如果箱子需要旋转，将箱子长宽作临时调整，不影响原箱子信息
    {
        int tmp = box.dim1;
        box.dim1 = box.dim2;
        box.dim2 = tmp;
    }
    
    //根据空隙高级信息，从gap set中找到它的基础信息（坐标位置信息）
    float gap_score = 0;
    gap_range gap = gaps_set[gap_solution.z][gap_solution.index];
    

    gap_score += Find_Best_Pos_in_Gap(box,gap,gap_solution); //寻找空隙中最好位置，返回位置分数
    // cout<<"^^^^^^^^^^^^^^^^^^^conner best score is:\t"<<gap_score<<"\t"<<gap_solution.box_position<<endl;
    if (gap_score < 0) //分数为负数证明 支撑不稳 
    {
        // cout <<" cannot support"<<endl;
        return 0;
    } 
 
    
    gap_score += Evaluate_Height(box,gap) * weight_height; //计算高度分数
    gap_score += Evaluate_Range(box,gap) * weight_range; // 计算范围分数
    gap_score += Evaluate_Support(box,gap) * weight_support; //计算支撑分数


    return gap_score;
}

float CPlanning_Box::Evaluate_Height(boxinfo box, gap_range gap)
{
    //计算高度分数
    float score_height = 1 - (float)gap.z_dim/ (float) pallet_z;
    // cout<<"height score: \t"<< score_height<<endl;
    return  score_height;
}
float CPlanning_Box::Evaluate_Range(boxinfo box, gap_range gap)
{
    //计算范围分数，包括x轴范围与y轴范围

    float score_x,score_y;
    float delta_x = (float) ((gap.right - gap.left +1) - box.dim1);
    float delta_y = (float) ((gap.top - gap.down +1) - box.dim2);
    if (delta_x<0 || delta_y <0) return -10000;
    
    float px = (float) pallet_x;
    float py = (float) pallet_y;
    float mb = (float) Min_Box_Size;
    float ma = (float) Max_Allowed_Gap_Size;
    
    float cross_point_x = px * ma / (px - mb + ma);
    float cross_point_y = py * ma / (py - mb + ma);
    
    // if (delta_x <= cross_point_x) score_x = delta_x *(-1 / ma) + 1;
    if (delta_x <= ma) score_x = delta_x *(-1 / ma) + 1;
    else if (delta_x < mb) score_x = delta_x /(ma - mb) + ma/(mb -ma);
    else score_x = delta_x / (px - mb) - mb / (px - mb);
    // if (delta_y <= cross_point_y) score_y =  delta_y *(-1 / ma) + 1;
    if (delta_y <= ma) score_y =  delta_y *(-1 / ma) + 1;
    else if (delta_y < mb) score_x = score_y = delta_y /(ma - mb) + ma/(mb -ma);
    else score_y = delta_y / (py - mb) - mb / (py - mb);

    // cout<<"range score:\t"<<(score_x * pallet_y + score_y * pallet_x)/ (pallet_y + pallet_x)<<endl;
    
    return (score_x * pallet_y + score_y * pallet_x)/ (pallet_y + pallet_x);
}
float CPlanning_Box::Evaluate_Support(boxinfo box, gap_range gap) 
{
    //计算支撑分数
    return 0;
}

float CPlanning_Box::Find_Best_Pos_in_Gap(boxinfo box,gap_range gap,gap_info &gap_solution) 
{
    //寻找空隙中最好位置，返回位置分数
    
    //构建临时的 分数、空隙变量
    float score;
    gap_range temp_gap;
    

    vector<int> positions = {LEFT_DOWN,LEFT_TOP,RIGHT_TOP,RIGHT_DOWN};
    // cout <<"box info:\t"<< box.dim1<<"\t"<<box.dim2<<"\t"<<box.dim3<<endl;;
    // cout<<"\n@@@@@gap number\t"<<gap_solution.z<<"\t"<<gap_solution.index<<"\t"<<gap_solution.box_orientation<<endl;
    // cout <<"original gap:\t"<<gap.left<<"\t"<<gap.right<<"\t"<<gap.top<<"\t"<<gap.down<<"\t"<<endl;
    
    //初始化最好位置与最好位置分数
    int best_position = LEFT_DOWN;
    float best_score = 0;
    
    for (int i = 0;i < positions.size();i++) //遍历四个位置
    {
        score = 0;
        
        //构建临时空隙结构temp-gap，表示将箱子放置在空隙的某个角落时，该箱子在角落的具体位置信息

        temp_gap.z_dim = gap.z_dim;
        temp_gap.area = box.dim1 * box.dim2;
        
        switch (positions[i]) //根据选择的位置计算箱子在角落的位置信息
        {
            case LEFT_DOWN:
            {
                temp_gap.left = gap.left;
                temp_gap.down = gap.down;
                temp_gap.right = gap.left + box.dim1 - 1;
                temp_gap.top = gap.down + box.dim2 -1;
                break;
            }
            case LEFT_TOP:
            {
                temp_gap.left = gap.left;
                temp_gap.down = gap.top -box.dim2 +1;
                temp_gap.right = gap.left + box.dim1 - 1;
                temp_gap.top = gap.top;
                break;
            }
            case RIGHT_TOP:
            {
                temp_gap.left = gap.right - box.dim1 +1;
                temp_gap.down = gap.top -box.dim2 +1;
                temp_gap.right = gap.right;
                temp_gap.top = gap.top;
                break;
            }
            case RIGHT_DOWN:
            {
                temp_gap.left = gap.right - box.dim1 +1;
                temp_gap.down = gap.down;
                temp_gap.right = gap.right;
                temp_gap.top = gap.down + box.dim2 -1;
                break;
            }
            default:
            {
                cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!no box position . something goes wrong"<<endl;
                break;
            }
        }
        // cout <<endl<<"new\t"<<positions[i]<<"\tgap:\t"<<temp_gap.left<<"\t"<<temp_gap.right<<"\t"<<temp_gap.down<<"\t"<<temp_gap.top<<"\t"<<endl;
        
        
        score += Evaluate_Area_Supported(box,temp_gap);//计算角落的支撑分数
        if (score <0) return -10000;//分数负数表示无法支撑
        score += Evaluate_Area_Contacted(box,temp_gap) * weight_area_contacted;//计算角落的接触分数
        score += Evaluate_Area_Created(box,temp_gap) * weight_area_created; //计算角落的贡献分数
        score += Evaluate_Area_Corner(box,temp_gap) *weight_area_corner;

        if (score > best_score)//更新最好位置
        {
            best_score = score;
            best_position = positions[i];
        }
    }
    gap_solution.box_position = best_position;
    // cout <<endl<<"inside function best score:\t" <<best_score<<endl;
    // cout <<"inside function best orientation:\t" <<gap_solution.box_orientation<<endl;
    // cout <<"inside function best position:\t" <<gap_solution.box_position<<endl<<endl;
    // cout <<"inside function box dim"<<box.dim1<<" "<<box.dim2<<endl;

    return best_score;
}

float CPlanning_Box::Evaluate_Area_Supported(boxinfo box,gap_range gap)
{
    //计算角落的支撑分数
    //包括面积支撑与重心支撑
    float support_score = 0;

    //计算箱子重心位置
    float box_center_x = ((float)gap.left + (float)gap.right)/2;
    float box_center_y = ((float)gap.top + (float)gap.down)/2;
    
    //初始化参数
    int support_volume = 0; 
    int support_area = 0;//支撑面积
    float support_center_x = 0;//支撑面重心x坐标
    float support_center_y = 0;//支撑面重心y坐标

    int tmp_h;

    //遍历该角落箱子的支撑信息，即箱子位置范围内的托盘高度图
    for (int i = gap.left; i <= gap.right; i++)
    {
        for (int j = gap.down; j <= gap.top; j++)
        {
            tmp_h = Height_Map[i][j].Height;
            support_volume += tmp_h;
            if ((tmp_h <= gap.z_dim)&&( tmp_h >= gap.z_dim - allow_z_err))//该点高度在支撑高度容差内能够支撑箱子
            {
                support_center_x += i;
                support_center_y += j; 
                support_area ++;

            }
            // if(gap.z_dim ==18)
            // {
            //     if ((tmp_h <= gap.z_dim)&&( tmp_h >= gap.z_dim - allow_z_err))
            //     {
            //         cout<<1;
            //     }
            //     else
            //     {
            //         cout<<0;
            //     }
                
            // } 
        }
        // if(gap.z_dim ==18)
        // {
        //     cout<<endl;
        // }

    }


    //计算底部支撑面积的重心位置
    if (support_area >0)
    {
        support_center_x /= (float) support_area;
        support_center_y /= (float) support_area;
    }
    
    // cout <<"area\t"<<support_area<<"\tcenter\t"<<support_center_x <<"\t"<<support_center_y<<endl;

    float support_area_ratio = (float) support_area / (float) gap.area;//计算支撑面积利用率
    float support_volume_ratio;
    if (gap.area * gap.z_dim ==0)
    {
        support_volume_ratio = 1;
    }
    else
    {
        support_volume_ratio = (float) support_volume / (float) (gap.area * gap.z_dim);
    }

    float support_center_ratio_x = abs(support_center_x-box_center_x) / ((float)box.dim1/2);
    float support_center_ratio_y = abs(support_center_y-box_center_y) / ((float)box.dim2/2);
    // cout <<"support_center_ratio_x\t"<<support_center_ratio_x<<"\tsupport_center_ratio_y\t"<<support_center_ratio_y <<endl;

    float support_center_ratio = 1 - max(support_center_ratio_x,support_center_ratio_y);//计算支撑重心与箱子重心的偏移程度

    // cout <<"area ratio \t"<<support_area_ratio <<"\t center ratio\t"<< support_center_ratio<<endl;

    if (support_area_ratio < stability_threshold_area || support_center_ratio < stability_threshold_center)//若支撑面积利用率过小或重心偏移程度过大，视为不可支撑
    {
        // cout<<"!!!!!annot support"<<endl;
        support_score-= 10000;
        // return support_score;
    }
    support_score = support_area_ratio * weight_area_supported_area + support_center_ratio * weight_area_supported_center;//返回面积利用率分数与重心偏移率分数

    support_score +=  (support_volume_ratio * weight_area_supported_volume);

    // cout << "support score is\t" << support_score<<endl; 
    return support_score;
}
float CPlanning_Box::Evaluate_Area_Contacted(boxinfo box,gap_range gap)
{
    //计算箱子放在该角落时，箱子与四周其他物体的接触面积，只要箱子与旁边的箱子或墙壁距离在容许空隙最大值的范围内，视为接触

    int i,j,tmp_dh,tmp_area,contacted_area;
    contacted_area = 0;
    tmp_area = 0;
    for (j = gap.down; j <= gap.top; j++)//计算箱子左侧接触面积，取最右
    {
        tmp_dh = 0;
        for (i = max(gap.left - Max_Allowed_Gap_Size , 0); i < gap.left; i++)
        {
            if (Height_Map[i][j].Height > gap.z_dim)
            {
                tmp_dh = min(box.dim3, Height_Map[i][j].Height - gap.z_dim);
            } 
        }
        tmp_area += tmp_dh;
    }
    // cout <<"left contacted area:\t"<<tmp_area<<endl;
    contacted_area += tmp_area;
    
    tmp_area = 0;
    for (j = gap.down; j <= gap.top; j++)//计算箱子下侧接触面积，取最上
    {
        tmp_dh = 0;
        
        for (i = gap.right + 1; i <= min(gap.right + Max_Allowed_Gap_Size,pallet_x); i++)
        {
            if (Height_Map[i][j].Height > gap.z_dim)
            {
                tmp_dh = min(box.dim3, Height_Map[i][j].Height - gap.z_dim);
                break;
            } 
        }
        tmp_area += tmp_dh;
    }
    // cout <<"right contacted area:\t"<<tmp_area<<endl;
    contacted_area += tmp_area;
    
    tmp_area = 0;
    for (i = gap.left; i <= gap.right; i++)//计算箱子右侧接触面积，取最左
    {
        tmp_dh = 0;
        for (j = gap.top + 1; j <= min(gap.top + Max_Allowed_Gap_Size,pallet_y); j++)
        {
            if (Height_Map[i][j].Height > gap.z_dim)
            {
                tmp_dh = min(box.dim3, Height_Map[i][j].Height - gap.z_dim);
                break;
            } 
        }
        tmp_area += tmp_dh;
    }
    // cout <<"top contacted area:\t"<<tmp_area<<endl;
    contacted_area += tmp_area;
    
    tmp_area = 0;
    for (i = gap.left; i <= gap.right; i++)//计算箱子上侧接触面积，取最下
    {
        tmp_dh = 0;
        for (j = max(gap.down - Max_Allowed_Gap_Size, 0); j < gap.down; j++)
        {
            if (Height_Map[i][j].Height > gap.z_dim)
            {
                tmp_dh = min(box.dim3, Height_Map[i][j].Height - gap.z_dim);
            } 
        }
        tmp_area += tmp_dh;
    }
    // cout <<"down contacted area:\t"<<tmp_area<<endl;
    contacted_area += tmp_area;
    
    int all_area =  (box.dim1 + box.dim2) * box.dim3 * 2;
    // cout <<" all contacted area\t"<<contacted_area<<"\tall area\t"<<all_area<<endl;

    float contacted_area_score = (float)contacted_area / (float) all_area;//计算接触面积利用率
    // cout<<"contacted score is\t" << contacted_area_score<<endl;
    return contacted_area_score;
}
float CPlanning_Box::Evaluate_Area_Created(boxinfo box,gap_range gap)
{
    //计算该角落的贡献分数，即放置在该平面后，能否与其它箱子拼出新的平面，用于以后箱子的支撑

    for (int i =1; i < pallet_x; i++)//构建临时01矩阵island矩阵，1代表能拼出，0代表不能拼出
    {
        for (int j = 1; j < pallet_y; j++)
        {
            if ((i>= gap.left) && (i<= gap.right) && (j>= gap.down) && (j<= gap.top)) //该位置为箱子位置
            {
                island_matrix[i][j] = 1;
            }
            else if (Height_Map[i][j].Height>= box.dim3 + gap.z_dim - allow_z_err && Height_Map[i][j].Height<= box.dim3 + gap.z_dim + allow_z_err)//改位置在箱子外，但在容许高度误差内其高度能与放置的箱子拼出新平面
            {
                island_matrix[i][j] = 1;
            }
            else
            {
                island_matrix[i][j] = 0;
            }
            // cout<<island_matrix[i][j];
        }
        // cout<<endl;
    }
    
    int island_area = Find_Island_Area(gap.left,gap.down);//寻找01矩阵中包含原箱子的最大面积
    
    // island_area += Find_Island_Area(min(gap.left-5,0),gap.down);
    // island_area += Find_Island_Area(gap.left,min(gap.down-5,0));
    // island_area += Find_Island_Area(max(gap.right+5,pallet_x),gap.down);
    // island_area += Find_Island_Area(gap.right,min(gap.down-5,0));
    // island_area += Find_Island_Area(min(gap.left-5,0),gap.top);
    // island_area += Find_Island_Area(gap.left,max(gap.top+5,pallet_y));
    // island_area += Find_Island_Area(max(gap.right+5,pallet_x),gap.top);
    // island_area += Find_Island_Area(gap.right,max(gap.top+5,pallet_y)); 
    
    
    // cout <<"created island area\t"<<island_area<<endl;
    // cout <<"box area       \t"<<gap.area <<endl;
    // cout<<box.dim1 * box.dim2 <<endl;

    float ceated_area_ratio = (float)(island_area - box.dim1 * box.dim2) / (float) (pallet_x * pallet_y);//计算面积利用率
    // cout <<"created area ratio\t"<< ceated_area_ratio <<endl;
    return ceated_area_ratio;
}
int CPlanning_Box::Find_Island_Area(int x,int y)
{
    //DFS max island area

    int area = 0;
    if ((x <=0)||(y<=0)||(x>=pallet_x)||(y>=pallet_y)) return 0;
    if (island_matrix[x][y] == 1)
    {
        area ++;
        island_matrix[x][y] = 0;
        area += Find_Island_Area(x ,y - 1);
        area += Find_Island_Area(x - 1,y);
        area += Find_Island_Area(x ,y + 1);
        area += Find_Island_Area(x + 1,y);
        return area;
    }
    else
    {
        return 0;
    }
    
    

}
float CPlanning_Box::Evaluate_Area_Corner(boxinfo box, gap_range gap)
{
    int l = gap.left;
    int r = gap.right;
    int t = gap.top;
    int d = gap.down;
    float score_corner = 0;

    if (l <= 1) score_corner += 1;
    if (d <= 1) score_corner += 1;
    if (r >= pallet_x) score_corner += 1;
    if (t >= pallet_y) score_corner += 1;

    score_corner /= 2;
    //cout << "corner score \t"<<score_corner<<endl;
    return score_corner;
}






void CPlanning_Box::Place_Box_and_Update(boxinfo &box,gap_info &best_gap)
{
    //放置箱子，更新地图与空隙
    
    Calcul_Box_Coordinate(box,best_gap); //计算箱子放置为位置的真实坐标

    packed_boxes.insert(packed_boxes.end(),box);//将箱子纳入已放置箱子
    box_count ++;
    cout<<"box placed"<<endl;

    int lowest_height_to_update = Update_Height_Map(box); //更新高度图，返回该箱子放置前，改位置的最低高度

    // cout<<"map:"<<endl;
    // for (int i = 1;i< pallet_x;i++)
    // {
    //     for (int j = 1;j< pallet_y;j++)
    //     {
    //         if (Height_Map[i][j].Height ==0)
    //         {
    //             cout <<"x";
    //         }
    //         else
    //         {
    //             cout<<Height_Map[i][j].Height;
    //         }
            
    //     }
    //     cout<<endl;
    // } 

    Update_Gaps(box,lowest_height_to_update); //将从箱子放置前该位置的最低高度，到箱子放置后地图高度，二者之间的所有空隙信息更新

    float ratio = Calcul_Occupacy_Ratio(box);
    cout<<"current packed box number is:\t"<<box_count<<endl;
    cout <<"current pallet occupacy ratio is:\t"<<ratio<<endl;
}

void CPlanning_Box::Calcul_Box_Coordinate(boxinfo &box,gap_info best_gap)
{

    int box_orientation = best_gap.box_orientation;
    if (box_orientation)
    {
        box.packx = box.dim2;
        box.packy = box.dim1;
        box.packz = box.dim3;
    }
    else
    {
        box.packx = box.dim1;
        box.packy = box.dim2;
        box.packz = box.dim3;
    }
    
    gap_range gap = gaps_set[best_gap.z][best_gap.index];
    int box_position = best_gap.box_position;
    switch (box_position)
    {
        case LEFT_DOWN:
        {
            box.cox = gap.left;
            box.coy = gap.down;
            break;
        }
        case LEFT_TOP:
        {
            box.cox = gap.left;
            box.coy = gap.top -box.packy +1;
            break;
        }
        case RIGHT_TOP:
        {
            box.cox = gap.right -box.packx +1;
            box.coy = gap.top -box.packy +1;
            break;
        }
        case RIGHT_DOWN:
        {
            box.cox = gap.right -box.packx +1;
            box.coy = gap.down;
            break;
        }
        default:
        {
            cout<<"no box position . something goes wrong"<<endl;
            break;
        }
    }
    box.coz = gap.z_dim;
    cout<<"gap chosen is:\t"<<endl;
    cout<<gap.left<<" "<<gap.left + box.packx -1<<" "<<gap.down<<" "<<gap.down + box.packy -1<<endl;
    cout<<"box size:\t"<<box.packx<<"\t"<<box.packy<<"\t"<<box.packz<<endl;
    cout <<"box xyz position:\t"<<box.cox<<"\t"<<box.coy<<"\t"<<box.coz<<endl;
}
int CPlanning_Box::Update_Height_Map(boxinfo &box)
{
    int lowest_height_to_update = pallet_z;
    for (int i = box.cox; i< box.cox + box.packx; i++)
    {
        for (int j = box.coy; j < box.coy + box.packy; j++)
        {
            if (Height_Map[i][j].Height < lowest_height_to_update)
            {
                lowest_height_to_update = Height_Map[i][j].Height;
            }
            Height_Map[i][j].Height = box.coz + box.packz;
        }
    }
    

    return lowest_height_to_update;
}

void CPlanning_Box::Update_Gaps(boxinfo box, int lowest_height_to_update)
{
    //将从箱子放置前该位置的最低高度，到箱子放置后地图高度，二者之间的所有空隙信息更新

    if (gaps_height.size()==0)
    {
        cout<<"nothing in heap now, something goes wrong"<<endl;
    }
    

    while (gaps_height.size() >0)
    {
        
        //从堆结构中寻找所有小于新放置箱子顶面高度的可放置高度，将其全部取出
        int z = *gaps_height.begin();
        if (z < (box.packz + box.coz))
        {
            temp_gaps_height.insert(temp_gaps_height.end(),z); 
            gaps_height.erase(z);
        }
        else//堆结构由于自动排序，不满足条件证明已经全部取出完毕
        {
            break;
        }
        
    }
    for (int i = 0; i < temp_gaps_height.size();i++)//将所有取出的高度重新计算空隙，并更新空隙记录向量gaps-set，和可放置高度记录堆函数gaps-height
    {
        Find_Gaps_on_z(temp_gaps_height[i]);
    }
    temp_gaps_height.clear();

    Find_Gaps_on_z(box.coz+box.packz);//计算箱子放置后顶面新生成的高度的所有空隙，更新空隙信息
    // cout <<"current gap height size is:\t"<<gaps_height.size()<<endl;
    

    //print 当前所有可放置箱子高度信息
    // for (int h : gaps_height)
    // {
    //     cout << h <<"\t";
    // }
    // cout<<endl;
}
float CPlanning_Box::Calcul_Occupacy_Ratio(boxinfo &box)
{
    //计算容积率

    int box_volume = box.dim1 * box.dim2 * box.dim3;
    packed_volume += box_volume;
    volume_ratio = float(packed_volume)/float(pallet_volume);
    return volume_ratio; 
}