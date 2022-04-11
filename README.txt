﻿OOP2 ex3 - Set Calculator Exceptions

Written by: Tali Kalev (טלי כלב), ID:208629691
            Noga Levy (נוגה לוי), ID:315260927

General Description of the Assignment:
    In this assignment we were asked to implement exceptions on a programmable 
    calculator for sets. The calculator starts with 3 basic operations on sets; Union,
    Intersection and Difference. The user may use the program to calculate
    different operations on sets from the list of operations as well as add new
    operations using the available commands.

Design:
    
    Classes: SetCalculator, Operation, BinaryOperation, NonCompBinaryOperation, Exceptions, 
            NameGenerator, Union, Intersection, Difference, Product, Comp, Sets, Identity

    SetCalculator:
        - responsible for dealing with input of commands and holding/executing
         all the available operations
    Operation:
        - represents an operation on sets
    BinaryOperation:
        - base class for all basic operations, deals with compound operations
        created using one of the basic operations (union, intersection etc)
    NonCompBinaryOperation:
        - Represents a binary operation that it isn't composition
        - They have similar print format and same implementation for inputCount()
    Identity:
        - Represents the identity operation
        - Returns the same set that it gets as input
        - Used as the leaf in every operation tree
    Union, Intersection, Difference, Product, Comp:
        - all are classes that inherit from NonCompBinaryOperation and take the outcomes
        of the 2 Operations received and executes on them the required operation
    Sets:
        - class that deals with the data structure that is the sets received and
        printed, makes sure to remove multiple occurrences of digits as well as
        sorts the set
    Exceptions:
        - class that inherits from exceptions and deals with incorrect file paths
    NameGenerator:
        - generates set names, A-Z

Files:
    - SetCalculator.h/SetCalculator.cpp: contains class SetCalculator (changed from original)
    - Set.h/Set.cpp: contains class Set 
    - Operation.h/Opertation.cpp: contains base class Operation
    - BinaryOperation.h/BinaryOperation.cpp: contains class BinaryOperation
    - NonCompBinaryOperation.h/NonCompBinaryOperation.cpp: contains class NonCompBinaryOperation
    - Identity.h/Identity.cpp: contains class Identity
    - Union.h/Union.cpp: contains class Union
    - Difference.h/Difference.cpp: contains class Difference
    - Intersection.h/Intersection.cpp: contains class Intersection
    - Product.h/Product.cpp: contains class Product (multiplies the sets)
    - Comp.h/Comp.cpp: contains class Comp
    - Exceptions.h/Exceptions.cpp: contains class Exceptions (new files)
    - NameGenerator.h/NameGenerator.cpp: contains class NameGenerator (changed from original)

Main Data Structures and their Jobs:
    - vector of shared_ptr of class Operations that contains all the operations
    in the calculator available for the user
    - all other main data structures and their jobs: see Design above

Notable Algorithms:
    - the following algorithms were used from the <algorithm> library:
        - std::ranges::set_union
        - std::ranges::set_intersection
        - std::ranges::set_difference
        - std::ranges::unique
        - std::ranges::sort
        - std::ranges::find
    - access to the data in the binary search tree of operations is done in order

Known Bugs:
    - no known bugs

Other General Notes:
     - printing of letters as representations of sets in operations list is completed
        in a cyclical way
    - if the user encounters an exception while reading from a file, if the user says
        they no longer want to continue then the program will return to the basic
        set calculator we began with (not reading from file)
    - all commands are written in the same line as the values of the operations
    - after the eval command, the sets required will be represented all in the
        following line together. 
        For example: 
        eval 0
        2 1 2 3 4 1 2
        (two sets first size 2, second size 3)
    - the program will read a line in its entirety, if within the line not all arguments
        are present, the program will let the user know and request the user to input again
        (if in file, will ask if user wants to continue reading)
    - read command supported while reading from file
    - first reads command then, if valid, read the rest of the line
