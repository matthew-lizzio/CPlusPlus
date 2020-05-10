// Local
#include <PricingScheme.hpp>
// Standard Library
#include <map>
#include <string>

#pragma once

///@brief This class emulates a checkout process in a Grocery store.
class Checkout
{

public:
    Checkout(const PricingScheme &i_PricingScheme);
    virtual ~Checkout();

    ///@brief Adds an item with the given ID to the virtual cart
    void scan(const std::string &i_ID);

    ///@brief Used once all items are scanned to calculate total cost
    ///@return Total cost of items in cart in cents (rounds to nearest cent)
    int getTotal();

private:
    ///@brief Helper function to encapsulate logic for calculating the cost of all the items
    /// in the cart
    void calculateTotal();
    ///@brief Helper function to encapsulate logic for calculating the Buy X, Get Y price
    /// scheme.
    ///@remarks Function is recursive
    void processBuyXGetY(double &io_Sum, const Item &i_Item, int &io_NumberOf);

    /// Tracks cumulative cost while total is being calculated
    double m_Total;

    /// Pricing Scheme that describes the cost of items
    PricingScheme m_PricingScheme;

    /// virtual cart to track items
    /// key=Item ID, value=# of item in cart
    std::map<std::string, int> m_Cart;
};