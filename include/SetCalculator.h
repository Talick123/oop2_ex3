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
    void read();
    void eval();
    void del();
    void help();
    void exit();

    template <typename FuncType>
    void binaryFunc()
    {
        if (auto f0 = readOperationIndex(), f1 = readOperationIndex(); f0 && f1)
        {
            m_operations.push_back(std::make_shared<FuncType>(m_operations[*f0], m_operations[*f1]));
        }
    }

    void printOperations() const;

    enum class Action
    {
        Invalid,
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

    std::optional<int> readOperationIndex();
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

    int readInt();
    std::string readString();

    std::ifstream openfile(std::string path) const;

    int readNumOfWords(std::string line);
};
