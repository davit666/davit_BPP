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

	find_low_gap = low_gap;
    have_wall = have_wall;
    
    pallet_is_full = false;
    packed_volume = 0;
    volume_ratio = 0;
    box_count = 0;
    // gaps_height.insert(cur_box_bottom); 

	
    //initialze pallet height map
    Height_Map = new Grid*[pallet_x+1];
    for (int i = 0; i < pallet_x + 1; i++)
    {
        Height_Map[i] = new Grid[pallet_y+1];
    }
    Initialize_Pallet(Height_Map,pallet_x,pallet_y,pallet_z,have_wall);
    gaps_set.resize(pallet_z + 1);
    Find_Gaps_on_z(0);
}
void CPlanning_Box::Initialize_Pallet(Grid **Height_Map, int px, int py, int pz,bool have_wall)
{
	for (int i = 0; i <= px; i++)
		for (int j = 0; j <= py; j++)
			{
				Height_Map[i][j].Height = 0;
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
    // Height_Map[10][10].Height = 2;
    cout <<"pallet initialized" << endl;
	return;
}

void CPlanning_Box::Clear_CPlanning_Box()
{
    
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
    
    if (gaps_set[z].size())
    {
        gaps_set[z].clear();
    }
    vector<vector<int>> available_matirx(pallet_x +1, vector<int>(pallet_y +1));
    for (int i =1; i <= pallet_x; i++)
    {
        for (int j = 1; j <= pallet_y; j++)
        {
            if (Height_Map[i][j].Height < z)
            {
                available_matirx[i][j] = 1;
            }
            else if (Height_Map[i][j].Height == z)
            {
                available_matirx[i][j] = 2;
            }
            else
            {
                available_matirx[i][j] = 0;
            }
        }
    }
    int rec_num = 0;
    
    rec_num = Find_Rectangle_area(available_matirx,z);
    if (rec_num>0)
    {
        gaps_height.insert(z);
    }

}

int CPlanning_Box::Find_Rectangle_area(vector<vector<int>> available_matirx,int z)
{
    
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
    cout <<"now here is \t"<<z<<"\tfloor, size is\t"<< gaps_set[z].size()<<"\tmax area is:\t"<<maxA<<endl;
	return gaps_set[z].size();
}
bool CPlanning_Box::Place_Box_to_Gap(boxinfo &box)
{
    cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
    cout <<"start"<<endl;
    cout <<"box size is:\t"<<box.dim1 <<"\t"<<box.dim2<<"\t"<<box.dim3<<endl;
    

    vector<gap_info> gap_solutions = Find_Gaps_Available(box);
    if (gap_solutions.size() <=0)
    {
        cout << "no solution" << endl;
        pallet_is_full = true;
        cout <<"heap, number:\t"<<gaps_height.size()<<endl;
        return false;
    }
    cout<< "solution number:\t "<<gap_solutions.size()<<endl;
    gap_info best_gap = Find_Best_Gap(box,gap_solutions);
    gap_solutions.clear();
    cout <<"best gap info is :\t"<<best_gap.z<<"  "<<best_gap.index<<endl;
    cout<< "orientation and position:\t "<<best_gap.box_orientation<<" "<<best_gap.box_position<<endl;
    cout <<"best gap range is :\t"<<gaps_set[best_gap.z][best_gap.index].left<<"  "<<gaps_set[best_gap.z][best_gap.index].right<<"  "<<gaps_set[best_gap.z][best_gap.index].top<<"  "<<gaps_set[best_gap.z][best_gap.index].down<<endl;
    
    Place_Box_and_Update(box,best_gap);
    cout<<"############################################################"<<endl;
    return true;

}

vector<gap_info> CPlanning_Box::Find_Gaps_Available(boxinfo box)
{

    vector<gap_info> gap_solutions;
    cout<<gap_solutions.size()<<endl;;
    temp_gaps_height.clear();
    while (gap_solutions.size()< available_gaps_num_limit)
    {

        int z = *gaps_height.begin();
        temp_gaps_height.insert(temp_gaps_height.end(),z);
        gaps_height.erase(z);
        for (int i = 0; i < gaps_set[z].size();i++)
        {
            if (gaps_set[z][i].z_dim + box.dim3<= pallet_z)
            {

                if (((gaps_set[z][i].right-gaps_set[z][i].left + 1)>= box.dim1)&&((gaps_set[z][i].top-gaps_set[z][i].down + 1)>= box.dim2))
                {
                    gap_info temp_gap;
                    temp_gap.z = z;
                    temp_gap.index = i;
                    temp_gap.box_orientation = 0;
                    if (z+ box.dim3 <= pallet_z +z_allowed_over_pallet)
                    {
                        gap_solutions.push_back(temp_gap);
                    }
                }
                if (((gaps_set[z][i].right-gaps_set[z][i].left + 1)>= box.dim2)&&((gaps_set[z][i].top-gaps_set[z][i].down)>= box.dim1 + 1))
                {
                    gap_info temp_gap_r;
                    temp_gap_r.z = z;
                    temp_gap_r.index = i;
                    temp_gap_r.box_orientation = 1;

                    if (z+ box.dim3 <= pallet_z +z_allowed_over_pallet)
                    {
                        cout<<gap_solutions.size()<<endl;
                        gap_solutions.push_back(temp_gap_r);
                    }
                }
            }
        }
        if (gaps_height.size()<1) break;
    }
    for (int i = 0; i< temp_gaps_height.size();i++)
    {
        gaps_height.insert(temp_gaps_height[i]);
    }
    temp_gaps_height.clear();

    cout << "number of availale gaps:\t"<<gap_solutions.size()<<endl;
    return gap_solutions;
}
gap_info CPlanning_Box::Find_Best_Gap(boxinfo box, vector<gap_info> gap_solutions)
{
    boxinfo temp_box = box;
    gap_info best_gap;
    float best_score = -10000;

    for(int i = 0;i < gap_solutions.size();i++)
    {
        float tmp_score = Find_Best_Position_and_Evaluate_Gap(temp_box, gap_solutions[i]);
        if (tmp_score > best_score)
        {
            best_score = tmp_score;
            best_gap = gap_solutions[i];
        }
    }
    cout <<"best score is:\t"<<best_score<<endl;
    return best_gap;
}
float CPlanning_Box::Find_Best_Position_and_Evaluate_Gap(boxinfo box,gap_info &gap_solution)
{
    if (gap_solution.box_orientation == 1)
    {
        int tmp = box.dim1;
        box.dim1 = box.dim2;
        box.dim2 = tmp;
    }
    float gap_score = 0;
    gap_range gap = gaps_set[gap_solution.z][gap_solution.index];
    int box_direction = gap_solution.box_orientation;
    // cout << gap.left<<"\t"<< gap.right<<"\t"<< gap.down<<"\t"<< gap.top<<"\t"<<endl;
    // cout <<box_direction<<endl;

    gap_score += Find_Best_Pos_in_Gap(box,gap,gap_solution);
    if (gap_score < 0) return 0;    
    
    gap_score += Evaluate_Height(box,gap) * weight_height;
    gap_score += Evaluate_Range(box,gap) * weight_range;
    gap_score += Evaluate_Support(box,gap) * weight_support;

    
    // cout <<"midside function best position:\t" <<gap_solution.box_position<<endl;
    // cout <<"midside function box dim"<<box.dim1<<" "<<box.dim2<<endl;

    return gap_score;
}

float CPlanning_Box::Evaluate_Height(boxinfo box, gap_range gap)
{
    float score_height = 1 - (float)gap.z_dim/ (float) pallet_z;
    cout<<"height score: \t"<< score_height<<endl;
    return  score_height;
}
float CPlanning_Box::Evaluate_Range(boxinfo box, gap_range gap)
{
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
    if (delta_x <= cross_point_x) score_x = delta_x *(-1 / ma) + 1;
    else score_x = delta_x / (px - mb) - mb / (px - mb);
    if (delta_y <= cross_point_y) score_y =  delta_y *(-1 / ma) + 1;
    else score_y = delta_y / (py - mb) - mb / (py - mb);

    cout<<"range score:\t"<<(score_x * pallet_y + score_y * pallet_x)/ (pallet_y + pallet_x)<<endl;
    
    return (score_x * pallet_y + score_y * pallet_x)/ (pallet_y + pallet_x);
}
float CPlanning_Box::Evaluate_Support(boxinfo box, gap_range gap) 
{
    return 0;
}

float CPlanning_Box::Find_Best_Pos_in_Gap(boxinfo box,gap_range gap,gap_info &gap_solution)
{
    
    float score;
    gap_range temp_gap;
    float best_score = 0;
    int best_position = LEFT_DOWN;
    vector<int> positions = {LEFT_DOWN,LEFT_TOP,RIGHT_TOP,RIGHT_DOWN};
    cout <<"box info:\t"<< box.dim1<<"\t"<<box.dim2<<"\t"<<box.dim3<<endl;;
    cout <<"original gap:\t"<<gap.left<<"\t"<<gap.right<<"\t"<<gap.top<<"\t"<<gap.down<<"\t"<<endl;
    for (int i = 0;i < positions.size();i++)
    {
        score = 0;
        
        temp_gap.z_dim = gap.z_dim;
        temp_gap.area = box.dim1 * box.dim2;
        
        switch (positions[i])
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
        cout <<"new\t"<<positions[i]<<"\tgap:\t"<<temp_gap.left<<"\t"<<temp_gap.right<<"\t"<<temp_gap.top<<"\t"<<temp_gap.down<<"\t"<<endl;
        
        score += Evaluate_Area_Supported(box,temp_gap);
        if (score <0) return -10000;
        score += Evaluate_Area_Contacted(box,temp_gap) * weight_area_contacted;
        score += Evaluate_Area_Created(box,temp_gap) * weight_area_created;

        if (score > best_score)
        {
            best_score = score;
            best_position = positions[i];
        }
    }
    gap_solution.box_position = best_position;
    // cout <<"inside function best score:\t" <<best_score<<endl;
    // cout <<"inside function best orientation:\t" <<gap_solution.box_orientation<<endl;
    // cout <<"inside function best position:\t" <<gap_solution.box_position<<endl;
    // cout <<"inside function box dim"<<box.dim1<<" "<<box.dim2<<endl;

    return best_score;
}

float CPlanning_Box::Evaluate_Area_Supported(boxinfo box,gap_range gap)
{
    float box_center_x = ((float)gap.left + (float)gap.right)/2;
    float box_center_y = ((float)gap.top + (float)gap.down)/2;
    
    int support_area = 0;
    float support_center_x;
    float support_center_y;

    int tmp_h;

    for (int i = gap.left; i <= gap.right; i++)
    {
        for (int j = gap.down; j <= gap.top; j++)
        {
            tmp_h = Height_Map[i][j].Height;
            if ((tmp_h <= gap.z_dim)&&( tmp_h >= gap.z_dim - allow_z_err))
            {
                support_center_x += i;
                support_center_y += j; 
                support_area ++;
            }
        }
    }

    support_center_x /= (float) support_area;
    support_center_y /= (float) support_area;
    cout <<"area\t"<<support_area<<"\tcenter\t"<<support_center_x <<"\t"<<support_center_y<<endl;

    float support_area_ratio = (float) support_area / (float) gap.area;

    // float center_diff = sqrt(pow((support_center_x-box_center_x),2) + pow((support_center_y-box_center_y),2));
    // float box_half_diagnal_length = sqrt(pow((float)box.dim1,2) + pow((float)box.dim2,2))/2;
    // cout <<"diff\t"<<center_diff<<"\thalf_diag\t"<<box_half_diagnal_length<<endl;
    
    // float support_center_ratio = 1 - center_diff / box_half_diagnal_length;
    float support_center_ratio_x = abs(support_center_x-box_center_x) / ((float)box.dim1/2);
    float support_center_ratio_y = abs(support_center_y-box_center_y) / ((float)box.dim2/2);
    float support_center_ratio = 1 - max(support_center_ratio_x,support_center_ratio_y);

    cout <<"area ratio \t"<<support_area_ratio <<"\t center ratio\t"<< support_center_ratio<<endl;

    if (support_area_ratio < stability_threshold_area || support_center_ratio < stability_threshold_center) return -10000;

    float support_score = support_area_ratio * weight_area_supported_area + support_center_ratio * weight_area_supported_center;
    cout << "support score is\t" << support_score<<endl;
    return support_score;
}
float CPlanning_Box::Evaluate_Area_Contacted(boxinfo box,gap_range gap)
{
    int i,j,tmp_dh,tmp_area,contacted_area;
    contacted_area = 0;
    tmp_area = 0;
    for (j = gap.down; j <= gap.top; j++)
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
    cout <<"left contacted area:\t"<<tmp_area<<endl;
    contacted_area += tmp_area;
    
    tmp_area = 0;
    for (j = gap.down; j <= gap.top; j++)
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
    cout <<"right contacted area:\t"<<tmp_area<<endl;
    contacted_area += tmp_area;
    
    tmp_area = 0;
    for (i = gap.left; i <= gap.right; i++)
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
    cout <<"top contacted area:\t"<<tmp_area<<endl;
    contacted_area += tmp_area;
    
    tmp_area = 0;
    for (i = gap.left; i <= gap.right; i++)
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
    cout <<"down contacted area:\t"<<tmp_area<<endl;
    contacted_area += tmp_area;
    
    int all_area =  (box.dim1 + box.dim2) * box.dim3 * 2;

    cout <<" all contacted area\t"<<contacted_area<<"\tall area\t"<<all_area<<endl;

    float contacted_area_score = (float)contacted_area / (float) all_area;
    cout<<"contacted score is\t" << contacted_area_score<<endl;
    return contacted_area_score;
}
float CPlanning_Box::Evaluate_Area_Created(boxinfo box,gap_range gap)
{
    return 0;
}




void CPlanning_Box::Place_Box_and_Update(boxinfo &box,gap_info &best_gap)
{
    
    Calcul_Box_Coordinate(box,best_gap);

    packed_boxes.insert(packed_boxes.end(),box);
    box_count ++;
    cout<<"box placed"<<endl;

    int lowest_height_to_update = Update_Height_Map(box);

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

    Update_Gaps(box,lowest_height_to_update);

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
    cout<<gap.left<<" "<<gap.right<<" "<<gap.down<<" "<<gap.top<<endl;
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
    if (gaps_height.size()==0)
    {
        cout<<"nothing in heap now, something goes wrong"<<endl;
    }
    while (gaps_height.size() >0)
    {
        int z = *gaps_height.begin();
        if (z < (box.packz + box.coz))
        {
            temp_gaps_height.insert(temp_gaps_height.end(),z);
            gaps_height.erase(z);
        }
        else
        {
            break;
        }
        
    }
    for (int i = 0; i < temp_gaps_height.size();i++)
    {
        Find_Gaps_on_z(temp_gaps_height[i]);
    }
    temp_gaps_height.clear();

    Find_Gaps_on_z(box.coz+box.packz);
    cout <<"current gap height size is:\t"<<gaps_height.size()<<endl;
    for (int h : gaps_height)
    {
        cout << h <<"\t";
    }
    cout<<endl;
}
float CPlanning_Box::Calcul_Occupacy_Ratio(boxinfo &box)
{
    int box_volume = box.dim1 * box.dim2 * box.dim3;
    packed_volume += box_volume;
    volume_ratio = float(packed_volume)/float(pallet_volume);
    return volume_ratio; 
}