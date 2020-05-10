// Standard Library
#include <string>
#include <utility>
#pragma once

///@brief Class to describe an item in a supermarket inventory.
///       Currently only tracks pricing scheme data, but other metadata
///       could easily be added.
class Item
{

public:
    Item(
        const std::string &i_Id,
        const double i_UnitPrice,
        const double i_Tax = 0,
        const std::pair<int, int> &i_BuyXGetY = {0, 0});

    Item(
        const std::string &i_Id,
        const double i_UnitPrice,
        const std::pair<std::string, double> &i_Bundle = {std::string(), 0},
        const double i_Tax = 0);

    Item(const Item &i_Item);
    Item();

    virtual ~Item();

    void operator=(const Item &i_Item);

    std::string getId() const
    {
        return m_Id;
    }
    double getUnitPrice() const
    {
        return m_UnitPrice;
    }

    double getTax() const
    {
        return m_Tax;
    }
    std::pair<int, int> getBuyXGetY() const
    {
        return m_BuyXGetY;
    }
    std::pair<std::string, double> getBundle() const
    {
        return m_Bundle;
    }

private:
    /// 4-digit numerical ID
    const std::string m_Id;

    /// Price of individual item before tax (e.g., $2.49 == 2.49)
    const double m_UnitPrice;

    /// Tax to be added onto item (e.g., 9.25% == 0.0925)
    const double m_Tax;

    /// Buy X (.first), Get Y free (.second)
    const std::pair<int, int> m_BuyXGetY;

    /// Buying this item and item #xxxx (.first) for $y.yy (.second)
    const std::pair<std::string, double> m_Bundle;
};