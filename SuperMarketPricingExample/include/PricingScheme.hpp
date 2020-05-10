// Local
#include <Item.hpp>
// Standard Library
#include <map>
#include <string>
#pragma once

///@brief Class for storing the pricing schemes of supermarket items (e.g., unit price, tax, deals)
/// Assumptions:
/// - Do not have to account for overflow
/// - Expect non-contradicting items with valid entries and no duplicates (assume data comes from db and scanner only scans valid items)
/// - Bundle and BuyXGetY are mutually exclusive deals (i.e., an item can't have both)
/// - Bundle Deals only support pairs of items (e.g., chips and salsa, but not toothbrush, floss, and toothpaste)
/// - A Bundle is not taxed
/// - Free items are not taxed
/// - There is no limit on the # of times a customer can receive a given deal
/// .
class PricingScheme
{

public:
    PricingScheme();
    virtual ~PricingScheme();

    ///@brief Adds an item's pricing scheme to the inventory
    ///@post Overwrites pricing scheme if item already exists
    void addItem(const Item &i_Item);

    std::map<std::string, Item> getItemMap()
    {
        return m_ItemMap;
    }

private:
    ///Map to lookup Item's pricing scheme based on its ID.
    ///Note: Could an unordered map if this gets too big, but
    /// would use more memory.
    std::map<std::string, Item> m_ItemMap;
};