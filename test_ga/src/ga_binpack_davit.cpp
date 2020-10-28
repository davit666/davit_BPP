
#include "ga_CPlanning_box_davit.h"
#include "ga_CPlanning_box_davit.cpp"


#include <ga/GASimpleGA.h>
#include <ga/GA1DArrayGenome.h>
// #include <iostream>
// using namespace std;


vector<boxinfo> Get_Box_Sets(){
	vector<boxinfo> Box_size;

	boxinfo box_type[47];

	float all_volume_ratio = 0;
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

	return Box_size;
}

vector<vector<int>> Get_Rand_Matrix(int loop_num){
	long long file_num = 1;
	vector<int> rand_arr;
	rand_arr.resize(loop_num * 100);

	std::ifstream infile("../src/rand_numbers.txt");
	int rand_count = 0;
	int r;
	while(infile >> r)
	{
		rand_arr[rand_count] = r;
		rand_count ++;
		if(rand_count >= loop_num * 100)
		{
			break;
		}
	}
	
	vector<vector<int>> rand_mat;
	rand_mat.resize(loop_num);
	for (int i = 0;i < loop_num;i++)
	{
		rand_mat[i].resize(100);
	}
	for (int i = 0;i < loop_num;i++)
	{
		for(int j = 0; j < 100;j++)
		{
			rand_mat[i][j] = rand_arr[i * 100 + j ];
			// rand_mat[i][j] = rand() %46;
			// cout << rand_mat[i][j];
		}
	}

	return rand_mat;

}




int loop_num = 10; //总共测试码垛轮数 
vector<boxinfo> Box_size =  Get_Box_Sets();
vector<vector<int>> rand_mat = Get_Rand_Matrix(loop_num);



float Get_Ave_Volume_Ratio(vector<float> weights)
{

	int xx = 113;
	int yy = 95;
	int zz = 100;
	int s_x = 14;
	int s_y = 14;
	int release_z = 2;
	int low_gap = 45;
	//初始化
	CPlanning_Box PlanningBox(xx, yy, zz, s_x, s_y, release_z, low_gap,true,weights);
	

	
	
	int box_num = 0;//箱子数量
	int packed_num = 0;//当前码垛轮次已经放置箱子数量
	bool result_planning;
    boxinfo place_box;
	int loop  = 0;
	bool change_pallet_flag = false;//是否更换新垛

	float all_volume_ratio = 0;
	float ave_volume_ratio = 0;

	while(loop <loop_num)
	{
		
		//判断是否更换托盘（是否重新启动）
		if ((PlanningBox.pallet_is_full == true)||(change_pallet_flag == true))
		{
			PlanningBox.Clear_CPlanning_Box();
			change_pallet_flag = false;
		}

		//生成要放置的箱子
		int box_rand = rand_mat[loop][packed_num];
    	place_box = Box_size[box_rand];

		
		//判断是否能放
		result_planning = PlanningBox.Place_Box_to_Gap(place_box);

		
		//能放
		if (result_planning)
		{
			box_num++;
			packed_num++;
			// cout<<"box\t"<<box_num<<endl;
		}
		
		//不能放
		else
		{
			//当前轮次停止
			loop ++; 
			change_pallet_flag = true;
			packed_num = 0;

			all_volume_ratio += PlanningBox.volume_ratio;
			ave_volume_ratio = all_volume_ratio / (float)loop;
			// cout << "当前托盘空间利用率 Volume Ratio: " << PlanningBox.volume_ratio << endl;
			// cout << "目前为止平均空间利用率:" << ave_volume_ratio << endl;

		}

	}

    cout<<endl<<"simulation ended\t";
	return ave_volume_ratio;
}



float Objective(GAGenome &g)
{
	GA1DArrayGenome<int>& genome = (GA1DArrayGenome<int>&)g;
	
	vector<float> weights(9);
	// vector<float> weights = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
	
	for(int i=0; i<genome.length(); i++)
	{
		weights[i] = (float) genome.gene(i) /10;
		cout <<weights[i]<<"\t";
	}
	
	float value = Get_Ave_Volume_Ratio(weights);
	cout<<"!!!!!!!!!!!!!!!"<<value<<endl;
	return value;
}


// 初期化関数
void Initializer(GAGenome& g)
{
  GA1DArrayGenome<int>& genome = (GA1DArrayGenome<int>&)g;
	for(int i = 0;i < genome.length();i++){
  	genome.gene(i, GARandomInt(0,10));
	}
}



int main(int argc, char const* argv[])
{

  // ゲノムのインスタンスを生成
  GA1DArrayGenome<int> genome(9,Objective);
  genome.crossover(GA1DArrayGenome<int>::UniformCrossover);
  genome.initializer(::Initializer);

  GASimpleGA ga(genome);
  ga.populationSize(20);
  ga.nGenerations(100);
  ga.pMutation(0.1);
  ga.pCrossover(0.9);

  // 評価開始
  ga.evolve();

  // 最も評価の高い個体を表示 
  std::cout << ga.statistics().bestIndividual() << "\n";
	// float num = Objective(genome);

  return 0;
}