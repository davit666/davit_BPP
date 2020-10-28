#include <ga/GASimpleGA.h>
#include <ga/GA1DArrayGenome.h>

#include <iostream>

// 評価関数
float Objective(GAGenome &g){
  GA1DArrayGenome<int>& genome = (GA1DArrayGenome<int>&)g;
  float value=0.0;
  for(int i=0; i<genome.length(); i++)
    // if (genome.gene(i) == 88)
	    value += genome.gene(i);
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
  GA1DArrayGenome<int> genome(10,Objective);
  genome.crossover(GA1DArrayGenome<int>::UniformCrossover);
  genome.initializer(::Initializer);

  GASimpleGA ga(genome);
  ga.populationSize(20);
  ga.nGenerations(100);
  ga.pMutation(0.01);
  ga.pCrossover(0.9);

  // 評価開始
  ga.evolve();

  // 最も評価の高い個体を表示 
  std::cout << ga.statistics().bestIndividual() << "\n";

  return 0;
}