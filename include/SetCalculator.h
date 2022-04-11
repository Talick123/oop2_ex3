#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iosfwd>
#include <optional>

#include <sstream>
#include <exception>

class Operation;

class SetCalculator
{
public:
    SetCalculator(std::istream& istr, std::ostream& ostr);
    void run();
    void start();

private:
    void resize();
    void read();
    void eval();
    void del();
    void help();
    void exit();

    template <typename FuncType>
    void binaryFunc()
    {
       //Noga: I dont know why this is here. 
        if (m_operations.size() >= m_maxOperations)
            throw std::invalid_argument("You've reached the max number of operations!\n");

        auto indexes = getIndexes(2);
        
        m_operations.push_back(std::make_shared<FuncType>(m_operations[indexes[0]], m_operations[indexes[1]]));

    }

    void printOperations() const;

    enum class Action
    {
        Invalid,
        Resize,
        Read,
        Eval,
        Union,
        Intersection,
        Difference,
        Product,
        Comp,
        Del,
        Help,
        Exit,
    };

    struct ActionDetails
    {
        std::string command;
        std::string description;
        int numOfArguments; //Tali: maybe change this to unsigned int
        Action action;
    };

    using ActionMap = std::vector<ActionDetails>;
    using OperationList = std::vector<std::shared_ptr<Operation>>;

    const ActionMap m_actions;
    OperationList m_operations;
    bool m_running = true;
    std::istream& m_istr;
    std::ostream& m_ostr;

    std::vector<int> getIndexes(int numOfArguments);
    Action readAction();
    void runAction(Action action);

    static ActionMap createActions();
    static OperationList createOperations();
    
    //New:
    int m_maxOperations;
    void readMaxOperations();
    void setMaxOperations(int max);
    std::vector<int> readArguments(int numOfArguments);

    std::string readString();

    std::ifstream openfile(std::string path) const;

    int readNumOfWords(std::string line);

    bool m_fileMode = false;
    int m_lineNum = -1;

    bool checkToContinueRead()const;
    void checkValidMaxOperation(int max);
    void copyCalculatorData(SetCalculator &to, SetCalculator& from);

    std::vector<std::vector<int>> getSets(int numOfSets);
};
