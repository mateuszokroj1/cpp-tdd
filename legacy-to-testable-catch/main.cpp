#include <iostream>
#include <data_analyzer.hpp>

using namespace std;

int main()
{    
    DataAnalyzer analyzer(Statistics::avg);
    analyzer.load_data("data.dat");
    analyzer.calculate();
    analyzer.save_results("results.txt");

    return 0;
}