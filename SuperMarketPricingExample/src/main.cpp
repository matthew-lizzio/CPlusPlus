// Local
#include <Checkout.hpp>
#include <Item.hpp>
#include <PricingScheme.hpp>
// Platform Specific
#include <Windows.h>
// Standard Library
#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string>
#include <vector>

///@brief Namespace to hold test framework
///@todo This is a quick and dirty framework. To actually be usable in production,
///      would need to redirect output to an external file that could be parsed by
///      pipeline (e.g., Jenkins, Gitlab, etc.).
///@todo Could make MACROs to take care of class generation
///      and automate most of the test code generation (just like googletest)
///@todo Could add more test assertion functions like ASSERT_TRUE, EXPECT_FALSE, etc.
///@todo This should really live in its own DLL

#pragma region Testing_Namespace
namespace Testing
{
    ///@brief Namespace to hold common constants for testing framework
    namespace Const
    {
        static const int WHITE = 7;
        static const int GREEN = 10;
        static const int RED = 12;
        static const std::string SUCCESS_STR = "SUCCESS";
        static const std::string FAILURE_STR = "FAILURE";
        static const std::string INDENT = "    ";
        static const std::string DIVIDER = "|---------------------------------------";
        static const std::string SMALL_DIVIDER = "|-----------------------";
    } // namespace Const

    ///@brief Helper fnc to change the console text color
    ///@remarks This is a Windows specific function
    static void setTextColor(const int i_Color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, i_Color);
    }
    ///@brief Helper function to get the current date time as a string
    ///@remarks Returned string has a carriage in it (i.e., \\n)
    static std::string getCurrentDateTime()
    {
        auto now = std::time(nullptr);
        std::string s = std::ctime(&now);
        //s.erase(s.find("\n"), 1);
        return s;
    }
    ///@brief Base class for Test Cases. Client should inherit from this class
    /// and override the runTest function with the logic for the test case.
    class TestCaseBase
    {
    public:
        TestCaseBase(){}; //default ctor
        virtual ~TestCaseBase(){};
        void execute()
        {
            try
            {
                printTestCaseStart();
                runTest();
                printTestCaseFinish();
            }
            catch (std::exception &e)
            {
                setTextColor(Const::RED);
                std::cout << "Exception thrown in test: " << m_Name.c_str() << std::endl;
                std::cout << e.what() << std::endl;
            }
            catch (...)
            {
                setTextColor(Const::RED);
                std::cout << "Exception thrown in test: " << m_Name.c_str() << std::endl;
            }
        }
        ///@brief Function that indicates if part of a test case failed
        bool hasFailures() const
        {
            return m_HasFailures;
        }

    protected:
        TestCaseBase(const std::string &i_Name)
            : m_HasFailures(false),
              m_Name(i_Name)
        {
        }
        ///@brief Pure virtual to let client provide their test case logic
        virtual void runTest() = 0;

        ///@brief Test Function to expect two obj to be equal
        ///@pre None
        ///@post Sets m_HasFailures if comparison fails and writes error msg
        ///@return None
        ///@remarks None
        template <class T>
        void EXPECT_EQ(T a, T b)
        {
            bool success = (a == b);
            m_HasFailures |= !success;

            if (!success)
            {
                setTextColor(Const::RED);
                std::cout << Const::INDENT.c_str() << "| Expected: " << b << " Actual: " << a << std::endl;
            }
        }

    private:
        ///@brief Helper function to print msg for start of test case
        void printTestCaseStart() const
        {
            setTextColor(Const::WHITE);
            std::cout << Const::INDENT.c_str() << Const::SMALL_DIVIDER.c_str() << std::endl;
            std::cout << Const::INDENT.c_str() << "| Running Test Case: " << m_Name.c_str() << std::endl;
        }
        ///@brief Helper function to print msg for end of test case
        void printTestCaseFinish() const
        {
            const std::string status = m_HasFailures ? Const::FAILURE_STR : Const::SUCCESS_STR;
            const int color = m_HasFailures ? Const::RED : Const::GREEN;
            setTextColor(color);
            std::cout << Const::INDENT.c_str() << "| Finished Test Case: " << m_Name.c_str() << " - " << status.c_str() << std::endl;
            setTextColor(Const::WHITE);
            std::cout << Const::INDENT.c_str() << Const::SMALL_DIVIDER.c_str() << std::endl;
        }
        bool m_HasFailures;
        const std::string m_Name;
    };
    ///@brief Class that represents a Test. TestCases can be added to this class and then executed.
    class Test
    {
    public:
        Test(const std::string &i_Name)
            : m_Name(i_Name),
              m_TestCases(),
              m_FailedTestCases(0),
              m_SuccessfulTestCases(0)
        {
        }
        virtual ~Test(){};

        ///@brief Client uses to run all added test cases
        void runAllTests()
        {
            printTestStart();

            for (auto &testcase : m_TestCases)
            {
                testcase->execute();
                if (testcase->hasFailures())
                {
                    ++m_FailedTestCases;
                }
                else
                {
                    ++m_SuccessfulTestCases;
                }
            }
            printTestFinish();
        }
        ///@brief Client uses to add a test case to test
        ///@remarks This class takes over memory management of input test case
        void addTestCase(std::unique_ptr<TestCaseBase> &io_TestCase)
        {
            m_TestCases.push_back(std::move(io_TestCase));
        }

    private:
        ///@brief Helper function to print msgs at test startup
        void printTestStart() const
        {
            setTextColor(Const::WHITE);
            std::cout << Const::DIVIDER.c_str() << std::endl;
            std::cout << "| " << getCurrentDateTime().c_str() << "| Running Test: " << m_Name.c_str() << std::endl;
        }
        ///@brief Helper function to print msgs at end of test
        void printTestFinish() const
        {
            setTextColor(Const::RED);
            std::cout << "| Failed Test Cases: " << m_FailedTestCases << std::endl;
            setTextColor(Const::GREEN);
            std::cout << "| Succesful Test Cases: " << m_SuccessfulTestCases << std::endl;

            setTextColor(Const::WHITE);
            std::cout << "| Finished Test: " << m_Name.c_str() << std::endl;
            std::cout << "| " << getCurrentDateTime().c_str();
            std::cout << Const::DIVIDER.c_str() << std::endl;
        }
        const std::string m_Name;
        std::vector<std::unique_ptr<TestCaseBase>> m_TestCases;
        unsigned int m_FailedTestCases;
        unsigned int m_SuccessfulTestCases;
    };
} // namespace Testing
#pragma endregion Testing_Namespace

#pragma region TestCases
///@test Test Case for the given example in Supermarket Pricing Coding Exercise document
///      This case tests all 4 pricing schemes together.
class TestExample : public Testing::TestCaseBase
{
public:
    TestExample() : Testing::TestCaseBase(__FUNCTION__) {}
    virtual ~TestExample() {}

protected:
    virtual void runTest() override
    {
        PricingScheme ps;
        ps.addItem(Item("1983", 1.99, 0, {2, 1}));      //toothbrush
        ps.addItem(Item("6732", 2.49, {"4900", 4.99})); //chips
        ps.addItem(Item("4900", 3.49, {"6732", 4.99})); //salsa
        ps.addItem(Item("8873", 2.49, 0));              //milk
        ps.addItem(Item("0923", 15.49, 0.0925));        //wine
        Checkout c(ps);

        std::vector<std::string> items{
            "1983",  //toothbrush
            "4900",  //salsa
            "8873",  //milk
            "6732",  //chips
            "0923",  //wine
            "1983",  //toothbrush
            "1983",  //toothbrush
            "1983"}; //toothbrush
        for (const auto &item : items)
        {
            c.scan(item);
        }
        int cents = c.getTotal();
        EXPECT_EQ(cents, 3037);
    }
};
///@test Test Case for the Simple Pricing scheme
class SimpleTest : public Testing::TestCaseBase
{
public:
    SimpleTest() : Testing::TestCaseBase(__FUNCTION__) {}
    virtual ~SimpleTest() {}

protected:
    virtual void runTest() override
    {
        PricingScheme ps;
        ps.addItem(Item("0000", 2.23, 0));
        ps.addItem(Item("1111", 8.00, 0));
        ps.addItem(Item("2222", 0.49, 0));
        ps.addItem(Item("3333", 100.00, 0));
        ps.addItem(Item("4444", 1.00, 0));
        ps.addItem(Item("5555", 5.00, 0));

        Checkout c(ps);

        std::vector<std::string> items{
            "0000",
            "1111",
            "2222",
            "3333",
            "4444",
            "5555"};
        for (const auto &item : items)
        {
            c.scan(item);
        }
        int cents = c.getTotal();
        EXPECT_EQ(cents, 11672);
    }
};
///@test Test Case for the Buy X, Get Y Free pricing scheme
class BuyXGetYTest : public Testing::TestCaseBase
{
public:
    BuyXGetYTest() : Testing::TestCaseBase(__FUNCTION__) {}
    virtual ~BuyXGetYTest() {}

protected:
    virtual void runTest() override
    {
        PricingScheme ps;
        ps.addItem(Item("0000", 1.00, 0, {1, 2}));     //exact deal case
        ps.addItem(Item("1111", 1.00, 0, {1, 2}));     //forgot the free items case
        ps.addItem(Item("2222", 1.00, 0, {1, 1}));     //multiple deal, forgot free item case
        ps.addItem(Item("3333", 1.00, 0, {3, 2}));     //1 deal plus additional bought
        ps.addItem(Item("4444", 1.00, 0.1, {100, 1})); //no deal case plus tax case

        Checkout c(ps);

        std::vector<std::string> items{
            "0000",  //$1
            "0000",  //free
            "0000",  //free
            "1111",  //$1
            "1111",  //free
            "1111",  //free
            "1111",  //$1
            "1111",  //free
            "2222",  //$1
            "2222",  //free
            "2222",  //$1
            "2222",  //free
            "2222",  //$1
            "3333",  //$1
            "3333",  //$1
            "3333",  //$1
            "3333",  //free
            "3333",  //free
            "3333",  //$1
            "4444"}; //$1.1
        for (const auto &item : items)
        {
            c.scan(item);
        }
        int cents = c.getTotal();
        EXPECT_EQ(cents, 1110);
    }
};

///@test Test Case for the Bundled Pricing Scheme
class BundledTest : public Testing::TestCaseBase
{
public:
    BundledTest() : Testing::TestCaseBase(__FUNCTION__) {}
    virtual ~BundledTest() {}

protected:
    virtual void runTest() override
    {
        PricingScheme ps;
        ps.addItem(Item("0000", 5.00, {"1111", 1.00}, 0.1)); // no qualifying bundles (has tax)
        ps.addItem(Item("1111", 5.00, {"0000", 1.00}));      // none bought

        ps.addItem(Item("2222", 5.00, {"3333", 1.00})); // exactlty 1 bundle
        ps.addItem(Item("3333", 5.00, {"2222", 1.00})); // exactlty 1 bundle

        ps.addItem(Item("4444", 5.00, {"5555", 1.00}));  // has extra
        ps.addItem(Item("5555", 10.00, {"4444", 1.00})); // exactly 1 bundle

        ps.addItem(Item("6666", 5.00, {"7777", 1.00}));  // exactly 1 bundle
        ps.addItem(Item("7777", 10.00, {"6666", 1.00})); // has extra

        ps.addItem(Item("8888", 5.00, {"9999", 1.00}));  // exactly 2 bundles
        ps.addItem(Item("9999", 10.00, {"8888", 1.00})); // 2 extra

        Checkout c(ps);

        std::vector<std::string> items{
            "0000", //$5.50
            "0000", //$5.50
            "0000", //$5.50

            "2222", //$1 (part of bundle)
            "3333", //free (part of bundle)

            "4444", //$1 (part of bundle)
            "4444", //$5
            "5555", //free (part of bundle)

            "6666", //$1 (part of bundle)
            "7777", //free (part of bundle)
            "7777", //$10

            "8888", //$1 (part of bundle)
            "8888", //$1 (part of bundle)
            "9999", //free (part of bundle)
            "9999", //free (part of bundle)
            "9999", //$10
            "9999", //$10
        };
        for (const auto &item : items)
        {
            c.scan(item);
        }
        int cents = c.getTotal();
        EXPECT_EQ(cents, 5650);
    }
};
#pragma endregion Testing_Namespace

/// The following tests the Checkout, PricingScheme, and Item classes
/// The console will indicate what tests were ran and what succeeded/failed.
int main()
{
    Testing::Test superMarketTest("SuperMarket Checkout Test");
    std::unique_ptr<Testing::TestCaseBase> tc = std::make_unique<TestExample>();
    superMarketTest.addTestCase(tc);
    tc = std::make_unique<SimpleTest>();
    superMarketTest.addTestCase(tc);
    tc = std::make_unique<BuyXGetYTest>();
    superMarketTest.addTestCase(tc);
    tc = std::make_unique<BundledTest>();
    superMarketTest.addTestCase(tc);

    superMarketTest.runAllTests();
    system("pause");
}