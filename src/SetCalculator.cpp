#include "SetCalculator.h"

#include "Union.h"
#include "Intersection.h"
#include "Difference.h"
#include "Product.h"
#include "Comp.h"
#include "Identity.h"
#include "Exceptions.h"

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

//----------------------------------------------------------------------------------

void SetCalculator::start()
{
    readMaxOperations();
    run();
}

//----------------------------------------------------------------------------------

void SetCalculator::run()
{
    do
    {
        m_ostr << '\n';
        printOperations();
        m_ostr << "Enter command ('help' for the list of available commands): ";
        if (m_fileMode) m_lineNum++;
        try 
        {
            const auto action = readAction();
            runAction(action);
        }
        catch (std::istream::failure& error)
        {
            m_ostr << "invalid input\n";
            if(m_fileMode) throw;
        }
        catch (std::invalid_argument& error)
        {
            m_ostr << error.what();
            m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (m_fileMode) throw;
        }
        catch (std::out_of_range& error)
        {
            m_ostr << error.what();
            if (m_fileMode) throw;
        }

    } while (m_running && m_istr);
}

//----------------------------------------------------------------------------------


void SetCalculator::resize()
{
    readMaxOperations();
}

void SetCalculator::read()
{
    std::ifstream myfile;
    std::string path = readString();
 
    try
    {
        myfile = openfile(path);

        auto fileCalc = SetCalculator(myfile, m_ostr);

        copyCalculatorData(fileCalc, *this);
        
        
        fileCalc.m_fileMode = true;

        while (!myfile.eof() && fileCalc.m_running) //TODO: ADD myfile.m_running
        {
            try
            {
                fileCalc.run();
            }
            catch (...)
            {
                /*std::string line;
                myfile.seekg(0);
                std::getline(myfile, line);*/
                //printCurrFileLine();
                int counter = 0;
                std::string currLine;
                myfile.seekg(0);
                while (fileCalc.m_lineNum > counter)
                {
                    myfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    counter++;
                }

                std::getline(myfile, currLine);
                m_ostr << "\nError at line: " << currLine << "\n";
                //noga: no. always print the first line in file
                //m_ostr << "\nThe problem was this line: " << line << "\n";
                //Noga: i think to move this ugly part to different function
                //Noga: now I think that its stupid to make function that we need to use only in file mode so I dont Know
                //Noga: eventually i moved it to new function but i dont know ok. ok
                if (checkToContinueRead())
                    continue;
                else
                    break;
                
            }
        }

        //fileCalc.m_fileMode = false;

        copyCalculatorData(*this, fileCalc);


    }
    catch (InvalidPath &error)
    {
        m_ostr << error.what();
    }
    
    myfile.close();
}

//----------------------------------------------------------------------------------

void SetCalculator::eval()
{
    const auto i = std::ranges::find(m_actions, Action::Eval, &ActionDetails::action);
    auto index = getIndexes(i->numOfArguments)[0];

    const auto& operation = m_operations[index];
    auto inputs = std::vector<Set>();

    std::vector<std::vector<int>> sets = getSets(operation->inputCount());

    for (auto i = 0; i < operation->inputCount(); ++i)
    {
        inputs.push_back(Set(sets[i]));
        //inputs.push_back(Set(m_istr));
        //if (m_fileMode) m_lineNum++;
    }
    
    operation->print(m_ostr, inputs);
    m_ostr << " = " << operation->compute(inputs) << '\n';
   
}

std::vector<std::vector<int>> SetCalculator::getSets(int numOfSets)
{
    int size, numOfValues, num;
    std::string line;
    std::getline(m_istr, line);
    std::stringstream ss(line);
    ss.exceptions(ss.failbit | ss.badbit);
    std::vector<std::vector<int>> sets;
    std::vector<int> singleset;

    numOfValues = readNumOfWords(line);

    if(numOfValues <= numOfSets)
        throw std::out_of_range("Not enough values!\n");

    int valuesLeft = numOfValues;
    for (auto i = 0; i < numOfSets; i++)
    {
        if(valuesLeft <= 1)
            throw std::out_of_range("Not enough values!\n");

        ss >> size;
        --valuesLeft;

        if (size > valuesLeft)
            throw std::out_of_range("Not enough values!\n");

        int tempsize = size;
        m_ostr << "temp size is: " << tempsize << "\n";
        while (tempsize > 0)
        {
            ss >> num;
            singleset.push_back(num);
            --tempsize;
        }
        sets.push_back(singleset);
        singleset.clear();
        valuesLeft -= size;
    }

    return sets;
}



//----------------------------------------------------------------------------------

void SetCalculator::del()
{
    const auto i = std::ranges::find(m_actions, Action::Del, &ActionDetails::action);
    auto index = getIndexes(i->numOfArguments)[0];

    m_operations.erase(m_operations.begin() + index);
}
//----------------------------------------------------------------------------------

void SetCalculator::help()
{
    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
    {
        m_ostr << "* " << action.command << action.description << '\n';
    }
    m_ostr << '\n';
}

//----------------------------------------------------------------------------------

void SetCalculator::exit()
{
    m_ostr << "Goodbye!\n";
    m_running = false;
}

//----------------------------------------------------------------------------------

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


//----------------------------------------------------------------------------------

//NEW INDEX READER
std::vector<int> SetCalculator::getIndexes(int numOfArguments)
{
    auto i = std::vector<int>();

    i = readArguments(numOfArguments);
    
    for (auto index : i)
    {
        if (index >= m_operations.size() || index < 0) //TASK: add to exceptions and add if smaller than 0
        {
            std::string error_msg = "Operation ";
            error_msg = error_msg + std::to_string(index);
            error_msg = error_msg + " doesn't exist";
            throw std::out_of_range(error_msg);
        }
    }
    return i;
}

//----------------------------------------------------------------------------------

SetCalculator::Action SetCalculator::readAction()
{
    auto action = readString();

    const auto i = std::ranges::find(m_actions, action, &ActionDetails::command);
    if (i != m_actions.end())
    {
        return i->action;
    }

    return Action::Invalid;
}

//----------------------------------------------------------------------------------

void SetCalculator::runAction(Action action)
{
    switch (action)
    {
        default:
            throw std::invalid_argument("Unknown enum entry used!\n");
            break;

        case Action::Invalid:
            throw std::invalid_argument("Command not found\n");
            break;
    
        case Action::Resize:       resize();                     break;
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

//----------------------------------------------------------------------------------

SetCalculator::ActionMap SetCalculator::createActions()
{
    return ActionMap
    {
        //TASK: add here: 'read' 'resize'
        {
            "resize",
            " SOMETHING resize"
            " SOMETHING ELSE resize",
            1,
            Action::Resize
        },
        {
            "read",
            " SOMETHING read"
            " SOMETHING ELSE read",
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

//----------------------------------------------------------------------------------

SetCalculator::OperationList SetCalculator::createOperations()
{
    return OperationList
    {
        std::make_shared<Union>(std::make_shared<Identity>(), std::make_shared<Identity>()),
        std::make_shared<Intersection>(std::make_shared<Identity>(), std::make_shared<Identity>()),
        std::make_shared<Difference>(std::make_shared<Identity>(), std::make_shared<Identity>())
    };
}

//----------------------------------------------------------------------------------

void SetCalculator::readMaxOperations()
{
    int max;

    while (true)
    {
        try {
            m_ostr << "Please enter maximum number of operations: "; 
            max = readArguments(1)[0];

            checkValidMaxOperation(max);
            setMaxOperations(max);
            break;
        }
        catch (std::istream::failure error)
        {
            m_ostr << "invalid input\n";
        }
        catch (std::out_of_range error)
        {
            m_ostr << error.what();
        }
    }
}

//----------------------------------------------------------------------------------

void SetCalculator::setMaxOperations(int max)
{
    m_maxOperations = max;
}

//----------------------------------------------------------------------------------

//TODO: read actions with this function
std::string SetCalculator::readString()
{
    std::string line;
    m_istr >> line;
    std::stringstream ss(line);
    ss.exceptions(ss.failbit | ss.badbit);

    ss >> line;
    m_ostr << "line is: " << line << "\n";
    return line;
}

//----------------------------------------------------------------------------------

std::ifstream SetCalculator::openfile(std::string path) const
{
    std::ifstream myfile;

    myfile.open(path);

    if (!myfile)
        throw InvalidPath();

    return myfile;

}

//----------------------------------------------------------------------------------

//NEW function that reads int input (indexes)
std::vector<int> SetCalculator::readArguments(int numOfArguments)
{
    int arg;
    std::vector<int> arguments;

    std::string line;
    std::getline(m_istr, line);
    std::stringstream ss(line);
    ss.exceptions(ss.failbit | ss.badbit);

    int count = readNumOfWords(line);
    
    if (count != numOfArguments)
        throw std::out_of_range("Incorrect number of arguments!\n");

    while (count > 0)
    {
        ss >> arg;
        m_ostr << "The index is: " << arg << "\n";

        arguments.push_back(arg);
        count--;
    }
    return arguments;
}

//----------------------------------------------------------------------------------


int SetCalculator::readNumOfWords(std::string line)
{
    auto count = 0;
    std::string word;
    std::stringstream temp(line);

    while (temp >> word)
    {
        count++;
    }
    m_ostr << "number of words in line: " << count << "\n";
    return count;
}

bool SetCalculator::checkToContinueRead() const
{
    std::string to_continue;

    m_ostr << "\nWould you like to continue to "
        "read the file? (Y - yes | N - no): ";
    m_istr >> to_continue;

    //Noga: check for "y" and "yes" and "YeS" and etc........
    //Noga: this is how we do "Case-insensitive string comparison" mush
    //convert string to upper case
    std::for_each(to_continue.begin(), to_continue.end(), [](char& c) {
        c = ::toupper(c);
        });

    return (to_continue == "Y" || to_continue == "YES");
}

void SetCalculator::checkValidMaxOperation(int max)
{
   if (max > 100 || max < 3)
   {
       throw std::out_of_range("number must be between 3 and 100!\n");
   }
   if (max < m_operations.size())
   {
       std::string answer;
       m_ostr << "Would you like to delete the last" << (int(m_operations.size()) - max) << " operation? (Y - yes):\n";
       m_istr >> answer;
       if (answer == "Y" || answer == "yes")
           m_operations.resize(m_operations.size() - (int(m_operations.size()) - max));
   }
}

void SetCalculator::copyCalculatorData(SetCalculator& to, SetCalculator& from)
{
    to.m_operations = from.m_operations;
    to.m_maxOperations = from.m_maxOperations;
}


