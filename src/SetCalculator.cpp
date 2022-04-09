#include "SetCalculator.h"

#include "Union.h"
#include "Intersection.h"
#include "Difference.h"
#include "Product.h"
#include "Comp.h"
#include "Identity.h"

#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace rng = std::ranges;

SetCalculator::SetCalculator(std::istream& istr, std::ostream& ostr)
    : m_actions(createActions()),
    m_operations(createOperations()), m_istr(istr), m_ostr(ostr)
  
{
}

void SetCalculator::start()
{
    //TASK: ask user for max number of operations (check between 3 and 100) and takin
    readMaxOperations();
    //m_istr.ignore(); //Tali: this line caused half the problems in my life (really only after adding readInt function) but still
    run();
}

void SetCalculator::run()
{
    do
    {
        m_ostr << '\n';
        printOperations();
        m_ostr << "Enter command ('help' for the list of available commands): ";
        //m_line.clear(); //TASK: try block for reading action
        //readline();
        /*
            Tali: I think we should put both read and run in seperate function that catches all the 
            throws that we define in lower layers
            why: almost anytime something is not read write, loop needs to start again
        */
        const auto action = readAction();
        runAction(action);
    } while (m_running && m_istr);
}



void SetCalculator::read()
{
    std::ifstream myfile;
    std::string path;
    m_ostr << "Please enter file path: ";

    std::getline(m_istr, path);

    //int numOfArguments = 0;
    //const auto i = std::ranges::find(m_actions, Action::Read, &ActionDetails::action);

    //std::vector<int> trypath = readArguments(i->numOfArguments);

    //m_line << path;
    myfile.open(path); //TASK: exception
    //myfile.open(trypath.str());

    if (!myfile)
        m_ostr << "not file";

    auto fileCalc = SetCalculator(myfile, m_ostr);

    fileCalc.m_operations = this->m_operations;
    fileCalc.m_maxOperations = this->m_maxOperations;

    while (!myfile.eof())
    {
        fileCalc.run();
    }

    this->m_operations = fileCalc.m_operations;
    this->m_maxOperations = fileCalc.m_maxOperations;

}

//Tali if arguments are always returned as vectors, we would have to make sure that each place that recieves it is ok and doesnt yell at everyone
//void SetCalculator::eval()
//{
//    if (auto index = readOperationIndex(); index) //R: init ; condition
//    {
//        const auto& operation = m_operations[*index];
//        auto inputs = std::vector<Set>();
//        for (auto i = 0; i < operation->inputCount(); ++i)
//        {
//            inputs.push_back(Set(m_istr));
//        }
//
//        operation->print(m_ostr, inputs);
//        m_ostr << " = " << operation->compute(inputs) << '\n';
//    }
//}
void SetCalculator::eval()
{
    const auto i = std::ranges::find(m_actions, Action::Eval, &ActionDetails::action);
    auto index = getIndexes(i->numOfArguments)[0];

    const auto& operation = m_operations[index];
    auto inputs = std::vector<Set>();
    for (auto i = 0; i < operation->inputCount(); ++i)
    {
        inputs.push_back(Set(m_istr));
    }
    
    operation->print(m_ostr, inputs);
    m_ostr << " = " << operation->compute(inputs) << '\n';
   
}

void SetCalculator::del()
{
    if (auto i = readOperationIndex(); i)
    {
        m_operations.erase(m_operations.begin() + *i);
    }
}

void SetCalculator::help()
{
    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
    {
        m_ostr << "* " << action.command << action.description << '\n';
    }
    m_ostr << '\n';
}

void SetCalculator::exit()
{
    m_ostr << "Goodbye!\n";
    m_running = false;
}

void SetCalculator::printOperations() const
{
    m_ostr << "List of available set operations:\n";
    for (decltype(m_operations.size()) i = 0; i < m_operations.size(); ++i)
    {
        m_ostr << i << ".\t";
        auto gen = NameGenerator();
        m_operations[i]->print(m_ostr, gen);
        m_ostr << '\n';
    }
    m_ostr << '\n';
    m_ostr << "Maximum number of operations allowed: " << m_maxOperations << '\n';
}

std::optional<int> SetCalculator::readOperationIndex() 
{
    auto i = 0;
    m_istr >> i;
    
    //m_line >> i;

    //auto i = readArguments(1);

    //auto i = readInt();


    m_ostr << "readOperationIndex: i = " << i << "\n";
    if (i >= m_operations.size()) //TASK: add to exceptions
    {
        m_ostr << "Operation #" << i << " doesn't exist\n";
        return {};
    }
    return i;
}

std::vector<int> SetCalculator::getIndexes(int numOfArguments)
{
    //have to add here that if its wrong to request again
    auto i = std::vector<int>();
    try {
        i = readArguments(numOfArguments);
    }
    catch (std::istream::failure error)
    {
        m_ostr << "invalid input\n";
    }
    catch (std::invalid_argument error)
    {
        m_ostr << error.what();
    }

    //m_ostr << "readOperationIndex: i = " << i[0] << "\n";
    for (auto index : i)
    {
        if (index >= m_operations.size()) //TASK: add to exceptions
        {
            //tali: add throw
            m_ostr << "Operation #" << index << " doesn't exist\n";
            return {};
        }
    }
    return i;
}
SetCalculator::Action SetCalculator::readAction()
{
    auto action = std::string();
    m_istr >> action;

    //m_line >> action;
    //m_ostr << "readACtion: action = " << action << "\n";

    const auto i = std::ranges::find(m_actions, action, &ActionDetails::command);
    if (i != m_actions.end())
    {
        return i->action;
    }

    return Action::Invalid;
}

void SetCalculator::runAction(Action action)
{
    switch (action)
    {
        default:
            m_ostr << "Unknown enum entry used!\n";
            break;

        case Action::Invalid:
            m_ostr << "Command not found\n";
            break;
    
        case Action::Read:         read();                     break;   
        case Action::Eval:         eval();                     break;
        case Action::Union:        binaryFunc<Union>();        break;
        case Action::Intersection: binaryFunc<Intersection>(); break;
        case Action::Difference:   binaryFunc<Difference>();   break;
        case Action::Product:      binaryFunc<Product>();      break;
        case Action::Comp:         binaryFunc<Comp>();         break;
        case Action::Del:          del();                      break;
        case Action::Help:         help();                     break;
        case Action::Exit:         exit();                     break;
    }
}

SetCalculator::ActionMap SetCalculator::createActions()
{
    return ActionMap
    {
        //TASK: add here: 'read' 'resize'
        {
            "read",
            " SOMETHING "
            " SOMETHING ELSE",
            1,
            Action::Read
        },
        {
            "eval",
            "(uate) num ... - compute the result of function #num on the "
            "following set(s); each set is prefixed with the count of numbers to"
            " read",
            1,
            Action::Eval
        },
        {
            "uni",
            "(on) num1 num2 - Creates an operation that is the union of "
            "operation #num1 and operation #num2",
            2,
            Action::Union
        },
        {
            "inter",
            "(section) num1 num2 - Creates an operation that is the "
            "intersection of operation #num1 and operation #num2",
            2,
            Action::Intersection
        },
        {
            "diff",
            "(erence) num1 num2 - Creates an operation that is the "
            "difference of operation #num1 and operation #num2",
            2,
            Action::Difference
        },
        {
            "prod",
            "(uct) num1 num2 - Creates an operation that returns the product of"
            " the items from the results of operation #num1 and operation #num2",
            2,
            Action::Product
        },
        {
            "comp",
            "(osite) num1 num2 - creates an operation that is the composition "
            "of operation #num1 and operation #num2",
            2,
            Action::Comp
        },
        {
            "del",
            "(ete) num - delete operation #num from the operation list",
            1,
            Action::Del
        },
        {
            "help",
            " - print this command list",
            0,
            Action::Help
        },
        {
            "exit",
            " - exit the program",
            0,
            Action::Exit
        }
    };
}

SetCalculator::OperationList SetCalculator::createOperations()
{
    return OperationList
    {
        std::make_shared<Union>(std::make_shared<Identity>(), std::make_shared<Identity>()),
        std::make_shared<Intersection>(std::make_shared<Identity>(), std::make_shared<Identity>()),
        std::make_shared<Difference>(std::make_shared<Identity>(), std::make_shared<Identity>())
    };
}

void SetCalculator::readMaxOperations()
{
    int max;
    //m_istr.exceptions(m_istr.failbit | m_istr.badbit);

    //auto max = readArguments(1);

    while (true)
    {
        try {
            m_ostr << "Please enter maximum number of operations: ";
            //m_istr >> max;
            max = readInt();
            setMaxOperations(max);
            break;
        }
        catch (std::istream::failure error)
        {
            //m_istr.ignore();
            m_ostr << "invalid input\n";

        }
        catch (std::invalid_argument error)
        {
            m_ostr << error.what();
        }
    }
}

void SetCalculator::setMaxOperations(int max)
{
    if (max > 100 || max < 3)
    {
        throw std::invalid_argument("number must be between 3 and 100!\n");
    } 

    m_maxOperations = max;
}

int SetCalculator::readInt()
{
    //m_istr.exceptions(m_istr.failbit | m_istr.badbit);
    
    //Tali: tried soooo many different things, finally just used what i saw in another targil and it worked. i really tried to figure out why but couldnt :(
    std::string line;
    std::getline(m_istr, line);
    m_ostr << "The line read is: " << line << "\n";
    std::stringstream ss(line);
    ss.exceptions(ss.failbit | ss.badbit);

    int num;
    ss >> num;
    m_ostr << "The num is: " << num << "\n";

    return num;
}

std::string SetCalculator::readString()
{
    return std::string();
}


//void SetCalculator::readline()
//{
//    auto line = std::string();
//    m_line.clear();
//
//    std::getline(m_istr, line);
//   
//    m_line.str(line);
//}
//
std::vector<int> SetCalculator::readArguments(int numOfArguments)
{
    m_ostr << "num of arguments requested: " << numOfArguments << "\n";
    std::string line;
    std::getline(m_istr, line);
    m_ostr << "The line read is: " << line << "\n";
    std::stringstream ss(line);
    ss.exceptions(ss.failbit | ss.badbit);

    int arg;
    std::vector<int> arguments;
    int count = numOfArguments;
    while (count > 0)
    {
        ss >> arg;
        m_ostr << "The index is: " << arg << "\n";

        arguments.push_back(arg);
        count--;
    }

    if(arguments.size() != numOfArguments)
        throw std::invalid_argument("Wrong number of arguments");

    return arguments;
}