#include "CPlanning_box_davit.h"
#include "CPlanning_box_davit.cpp"

// #include <iostream>
// using namespace std;


int main()
{
	int xx, yy, zz; //托盘的三个维度
	//托盘尺寸
	xx = 113;
	yy = 95;
	zz = 104;

	//吸盘尺寸
	int s_x = 14;
	int s_y = 14;

	//吸盘放置高度
	int release_z = 2;



	//设置放置允许的Z方向误差
	int allow_err_z = 3;

	//设置寻找最低层的范围
	int low_gap = 45;

	// 层内码垛允许超过层高标准的限制值
	int allow_over_z = 0;

	// 每一层的层高
	int layer_high_one = 20;
	int layer_high_other = 20;

	//记录各个所有托盘的箱子
	vector<vector <boxinfo> > packed_box_all;

	int daqiao_num = 0;
	///////////////////////////////////////////////////////////
	vector<boxinfo> Box_size;

	boxinfo box_type[47];

	long long int all_volume = 0;
	//////////第一组
	//
		
	box_type[0].dim1 = 32;
	box_type[0].dim2 = 56;
	box_type[0].dim3 = 11;
	box_type[0].vol = box_type[0].dim1 * box_type[0].dim2 * box_type[0].dim3;
	Box_size.push_back(box_type[0]);

	box_type[1].dim1 = 22;
	box_type[1].dim2 = 33;
	box_type[1].dim3 = 20;
	box_type[1].vol = box_type[1].dim1 * box_type[1].dim2 * box_type[1].dim3;
	Box_size.push_back(box_type[1]);

	box_type[2].dim1 = 33;
	box_type[2].dim2 = 39;
	box_type[2].dim3 = 15;
	box_type[2].vol = box_type[2].dim1 * box_type[2].dim2 * box_type[2].dim3;
	Box_size.push_back(box_type[2]);

	box_type[3].dim1 = 23;
	box_type[3].dim2 = 23;
	box_type[3].dim3 = 18;
	box_type[3].vol = box_type[3].dim1 * box_type[3].dim2 * box_type[3].dim3;
	Box_size.push_back(box_type[3]);

	box_type[4].dim1 = 28;
	box_type[4].dim2 = 32;
	box_type[4].dim3 = 18;
	box_type[4].vol = box_type[4].dim1 * box_type[4].dim2 * box_type[4].dim3;
	Box_size.push_back(box_type[4]);

	box_type[5].dim1 = 29;
	box_type[5].dim2 = 27;
	box_type[5].dim3 = 12;
	box_type[5].vol = box_type[5].dim1 * box_type[5].dim2 * box_type[5].dim3;
	Box_size.push_back(box_type[5]);

	box_type[6].dim1 = 23;
	box_type[6].dim2 = 42;
	box_type[6].dim3 = 12;
	box_type[6].vol = box_type[6].dim1 * box_type[6].dim2 * box_type[6].dim3;
	Box_size.push_back(box_type[6]);

	box_type[7].dim1 = 32;
	box_type[7].dim2 = 28;
	box_type[7].dim3 = 15;
	box_type[7].vol = box_type[7].dim1 * box_type[7].dim2 * box_type[7].dim3;
	Box_size.push_back(box_type[7]);

	box_type[8].dim1 = 28;
	box_type[8].dim2 = 28;
	box_type[8].dim3 = 18;
	box_type[8].vol = box_type[8].dim1 * box_type[8].dim2 * box_type[8].dim3;
	Box_size.push_back(box_type[8]);

	box_type[9].dim1 = 26;
	box_type[9].dim2 = 55;
	box_type[9].dim3 = 11;
	box_type[9].vol = box_type[9].dim1 * box_type[9].dim2 * box_type[9].dim3;
	Box_size.push_back(box_type[9]);

	box_type[10].dim1 = 34;
	box_type[10].dim2 = 33;
	box_type[10].dim3 = 15;
	box_type[10].vol = box_type[10].dim1 * box_type[10].dim2 * box_type[10].dim3;
	Box_size.push_back(box_type[10]);

	box_type[11].dim1 = 34;
	box_type[11].dim2 = 32;
	box_type[11].dim3 = 10;
	box_type[11].vol = box_type[11].dim1 * box_type[11].dim2 * box_type[11].dim3;
	Box_size.push_back(box_type[11]);

	box_type[12].dim1 = 23;
	box_type[12].dim2 = 23;
	box_type[12].dim3 = 12;
	box_type[12].vol = box_type[12].dim1 * box_type[12].dim2 * box_type[12].dim3;
	Box_size.push_back(box_type[12]);

	box_type[13].dim1 = 25;
	box_type[13].dim2 = 23;
	box_type[13].dim3 = 20;
	box_type[13].vol = box_type[13].dim1 * box_type[13].dim2 * box_type[13].dim3;
	Box_size.push_back(box_type[13]);

	box_type[14].dim1 = 38;
	box_type[14].dim2 = 26;
	box_type[14].dim3 = 19;
	box_type[14].vol = box_type[14].dim1 * box_type[14].dim2 * box_type[14].dim3;
	Box_size.push_back(box_type[14]);

	box_type[15].dim1 = 39;
	box_type[15].dim2 = 33;
	box_type[15].dim3 = 10;
	box_type[15].vol = box_type[15].dim1 * box_type[15].dim2 * box_type[15].dim3;
	Box_size.push_back(box_type[15]);

	box_type[16].dim1 = 33;
	box_type[16].dim2 = 38;
	box_type[16].dim3 = 20;
	box_type[16].vol = box_type[16].dim1 * box_type[16].dim2 * box_type[16].dim3;
	Box_size.push_back(box_type[16]);

	box_type[17].dim1 = 23;
	box_type[17].dim2 = 22;
	box_type[17].dim3 = 18;
	box_type[17].vol = box_type[17].dim1 * box_type[17].dim2 * box_type[17].dim3;
	Box_size.push_back(box_type[17]);

	box_type[18].dim1 = 33;
	box_type[18].dim2 = 28;
	box_type[18].dim3 = 13;
	box_type[18].vol = box_type[18].dim1 * box_type[18].dim2 * box_type[18].dim3;
	Box_size.push_back(box_type[18]);

	box_type[19].dim1 = 23;
	box_type[19].dim2 = 23;
	box_type[19].dim3 = 13;
	box_type[19].vol = box_type[19].dim1 * box_type[19].dim2 * box_type[19].dim3;
	Box_size.push_back(box_type[19]);

	box_type[20].dim1 = 28;
	box_type[20].dim2 = 28;
	box_type[20].dim3 = 22;
	box_type[20].vol = box_type[20].dim1 * box_type[20].dim2 * box_type[20].dim3;
	Box_size.push_back(box_type[20]);

	box_type[21].dim1 = 33;
	box_type[21].dim2 = 28;
	box_type[21].dim3 = 22;
	box_type[21].vol = box_type[21].dim1 * box_type[21].dim2 * box_type[21].dim3;
	Box_size.push_back(box_type[21]);

	box_type[22].dim1 = 24;
	box_type[22].dim2 = 22;
	box_type[22].dim3 = 10;
	box_type[22].vol = box_type[22].dim1 * box_type[22].dim2 * box_type[22].dim3;
	Box_size.push_back(box_type[22]);

	box_type[23].dim1 = 34;
	box_type[23].dim2 = 33;
	box_type[23].dim3 = 20;
	box_type[23].vol = box_type[23].dim1 * box_type[23].dim2 * box_type[23].dim3;
	Box_size.push_back(box_type[23]);

	box_type[24].dim1 = 28;
	box_type[24].dim2 = 27;
	box_type[24].dim3 = 20;
	box_type[24].vol = box_type[24].dim1 * box_type[24].dim2 * box_type[24].dim3;
	Box_size.push_back(box_type[24]);

	box_type[25].dim1 = 44;
	box_type[25].dim2 = 42;
	box_type[25].dim3 = 15;
	box_type[25].vol = box_type[25].dim1 * box_type[25].dim2 * box_type[25].dim3;
	Box_size.push_back(box_type[25]);

	box_type[26].dim1 = 28;
	box_type[26].dim2 = 28;
	box_type[26].dim3 = 22;
	box_type[26].vol = box_type[26].dim1 * box_type[26].dim2 * box_type[26].dim3;
	Box_size.push_back(box_type[26]);

	box_type[27].dim1 = 33;
	box_type[27].dim2 = 28;
	box_type[27].dim3 = 10;
	box_type[27].vol = box_type[27].dim1 * box_type[27].dim2 * box_type[27].dim3;
	Box_size.push_back(box_type[27]);


	box_type[28].dim1 = 28;
	box_type[28].dim2 = 33;
	box_type[28].dim3 = 20;
	box_type[28].vol = box_type[28].dim1 * box_type[28].dim2 * box_type[28].dim3;
	Box_size.push_back(box_type[28]);

	box_type[29].dim1 = 23;
	box_type[29].dim2 = 23;
	box_type[29].dim3 = 10;
	box_type[29].vol = box_type[29].dim1 * box_type[29].dim2 * box_type[29].dim3;
	Box_size.push_back(box_type[29]);

	box_type[30].dim1 = 34;
	box_type[30].dim2 = 33;
	box_type[30].dim3 = 13;
	box_type[30].vol = box_type[30].dim1 * box_type[30].dim2 * box_type[30].dim3;
	Box_size.push_back(box_type[30]);

	box_type[31].dim1 = 34;
	box_type[31].dim2 = 42;
	box_type[31].dim3 = 10;
	box_type[31].vol = box_type[31].dim1 * box_type[31].dim2 * box_type[31].dim3;
	Box_size.push_back(box_type[31]);

	box_type[32].dim1 = 38;
	box_type[32].dim2 = 33;
	box_type[32].dim3 = 16;
	box_type[32].vol = box_type[32].dim1 * box_type[32].dim2 * box_type[32].dim3;
	Box_size.push_back(box_type[32]);

	box_type[33].dim1 = 22;
	box_type[33].dim2 = 37;
	box_type[33].dim3 = 20;
	box_type[33].vol = box_type[33].dim1 * box_type[33].dim2 * box_type[33].dim3;
	Box_size.push_back(box_type[33]);

	box_type[34].dim1 = 29;
	box_type[34].dim2 = 28;
	box_type[34].dim3 = 10;
	box_type[34].vol = box_type[34].dim1 * box_type[34].dim2 * box_type[34].dim3;
	Box_size.push_back(box_type[34]);

	box_type[35].dim1 = 38;
	box_type[35].dim2 = 34;
	box_type[35].dim3 = 12;
	box_type[35].vol = box_type[35].dim1 * box_type[35].dim2 * box_type[35].dim3;
	Box_size.push_back(box_type[35]);

	box_type[36].dim1 = 28;
	box_type[36].dim2 = 22;
	box_type[36].dim3 = 20;
	box_type[36].vol = box_type[36].dim1 * box_type[36].dim2 * box_type[36].dim3;
	Box_size.push_back(box_type[36]);

	box_type[37].dim1 = 33;
	box_type[37].dim2 = 43;
	box_type[37].dim3 = 20;
	box_type[37].vol = box_type[37].dim1 * box_type[37].dim2 * box_type[37].dim3;
	Box_size.push_back(box_type[37]);

	box_type[38].dim1 = 42;
	box_type[38].dim2 = 33;
	box_type[38].dim3 = 15;
	box_type[38].vol = box_type[38].dim1 * box_type[38].dim2 * box_type[38].dim3;
	Box_size.push_back(box_type[38]);

	box_type[39].dim1 = 30;
	box_type[39].dim2 = 23;
	box_type[39].dim3 = 20;
	box_type[39].vol = box_type[39].dim1 * box_type[39].dim2 * box_type[39].dim3;
	Box_size.push_back(box_type[39]);

	box_type[40].dim1 = 23;
	box_type[40].dim2 = 42;
	box_type[40].dim3 = 15;
	box_type[40].vol = box_type[40].dim1 * box_type[40].dim2 * box_type[40].dim3;
	Box_size.push_back(box_type[40]);

	box_type[41].dim1 = 25;
	box_type[41].dim2 = 28;
	box_type[41].dim3 = 17;
	box_type[41].vol = box_type[41].dim1 * box_type[41].dim2 * box_type[41].dim3;
	Box_size.push_back(box_type[41]);

	box_type[42].dim1 = 33;
	box_type[42].dim2 = 43;
	box_type[42].dim3 = 12;
	box_type[42].vol = box_type[42].dim1 * box_type[42].dim2 * box_type[42].dim3;
	Box_size.push_back(box_type[42]);

	box_type[43].dim1 = 28;
	box_type[43].dim2 = 33;
	box_type[43].dim3 = 12;
	box_type[43].vol = box_type[43].dim1 * box_type[43].dim2 * box_type[43].dim3;
	Box_size.push_back(box_type[43]);

	box_type[44].dim1 = 43;
	box_type[44].dim2 = 42;
	box_type[44].dim3 = 11;
	box_type[44].vol = box_type[44].dim1 * box_type[44].dim2 * box_type[44].dim3;
	Box_size.push_back(box_type[44]);

	box_type[45].dim1 = 33;
	box_type[45].dim2 = 29;
	box_type[45].dim3 = 11;
	box_type[45].vol = box_type[45].dim1 * box_type[45].dim2 * box_type[45].dim3;
	Box_size.push_back(box_type[45]);

	box_type[46].dim1 = 50;
	box_type[46].dim2 = 50;
	box_type[46].dim3 = 50;
	box_type[46].vol = box_type[46].dim1 * box_type[46].dim2 * box_type[46].dim3;
	Box_size.push_back(box_type[46]);


	//初始化
	CPlanning_Box PlanningBox(xx, yy, zz, s_x, s_y, release_z,allow_err_z, low_gap, allow_over_z,true);


	for (int i = 0;i < 46;i++)
		PlanningBox.BoxType.push_back(Box_size[i]);

	int box_num = 1;
	int packed_num = 0;
	long long file_num = 1;
	bool result_planning = true;
    boxinfo place_box;
	bool is_change_flag = false;

	for (int i = 0;i<100;i++)
	{
		srand((unsigned)time(NULL));
		int box_rand = rand() % 46;
    	place_box = Box_size[box_rand];
    	result_planning = PlanningBox.Place_Box_to_Gap(place_box);
		if (result_planning ==0)
		{
			break;
		}
	}
    
    cout<<"simulation ended"<<endl;


}