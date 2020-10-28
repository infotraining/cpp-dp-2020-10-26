#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>

struct StatResult
{
    std::string description;
    double value;
    StatResult(const std::string& desc, double val) : description(desc), value(val)
    {
    }
};

using Data = std::vector<double>;
using Results = std::vector<StatResult>;

class Strategy
{
public:
    virtual Results calculate(Data& data) const = 0;
    virtual ~Strategy() = default;
};

class AvgStrategy : public Strategy
{
public:
    Results calculate(Data& data) const override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double avg = sum / data.size();
        return {{ "AVG", avg }};
    }
};

class MinMaxStrategy : public Strategy
{
public:
    Results calculate(Data& data) const override
    {
        Results res;
        double min = *(std::min_element(data.begin(), data.end()));
        double max = *(std::max_element(data.begin(), data.end()));
        res.push_back(StatResult("MIN", min));
        res.push_back(StatResult("MAX", max));
        return res;
    }
};

class SumStrategy : public Strategy
{
public:
    Results calculate(Data& data) const override
    {
        return { {"SUM", std::accumulate(data.begin(), data.end(), 0.0) } };
    }
};

template <typename... StrategyTs>
class StatGroup : public Strategy
{
    std::vector<std::shared_ptr<Strategy>> stats_;
public:
    StatGroup()
    {
        (stats_.emplace_back(std::make_shared<StrategyTs>()), ...);
    }

    Results calculate(Data& data) const override
    {
        Results stat_results;

        for(const auto& stat : stats_)
        {
             auto result = stat->calculate(data);
            
            stat_results.insert(stat_results.end(), std::make_move_iterator(result.begin()), std::make_move_iterator(result.end()));
        }

        return stat_results;
    }
};  

class DataAnalyzer
{
    std::shared_ptr<Strategy> statistics_; 
    Data data_;
    Results results_;
public:
    DataAnalyzer(std::shared_ptr<Strategy> stat) : statistics_{stat}    
    {
    }

    void load_data(const std::string& file_name)
    {
        data_.clear();
        results_.clear();
        std::ifstream fin(file_name.c_str());
        if (!fin)
            throw std::runtime_error("File not opened");
        double d;
        while (fin >> d)
        {
            data_.push_back(d);
        }
        std::cout << "File " << file_name << " has been loaded...\n";
    }

    void set_statistics(std::shared_ptr<Strategy> stat)
    {
        statistics_ = stat;
    }

    void calculate()
    {
        auto result = statistics_->calculate(data_);
            
        results_.insert(results_.end(), std::make_move_iterator(result.begin()), std::make_move_iterator(result.end()));
    }

    const Results& results() const
    {
        return results_;
    }
};

void show_results(const Results& results)
{
    for (const auto& rslt : results)
        std::cout << rslt.description << " = " << rslt.value << std::endl;
}

int main()
{
    auto  std_stats = std::make_shared<StatGroup<AvgStrategy, MinMaxStrategy, SumStrategy>>();
    DataAnalyzer da{std_stats};
    da.load_data("data.dat");
    da.calculate();

    show_results(da.results());
    
    std::cout << "\n\n";
    
    da.load_data("new_data.dat");
    da.calculate();
    
    show_results(da.results());
}