// Local
#include "Checkout.hpp"
// Standard Library
#include <algorithm>
#include <cmath>

Checkout::Checkout(const PricingScheme &i_PricingScheme)
    : m_Total(0),
      m_PricingScheme(i_PricingScheme)
{
}

Checkout::~Checkout()
{
}

void Checkout::scan(const std::string &i_ID)
{
    auto it = m_Cart.find(i_ID);
    if (m_Cart.end() != it)
    {
        ++(it->second);
    }
    else
    {
        m_Cart[i_ID] = 1;
    }
}
int Checkout::getTotal()
{
    calculateTotal();
    return (std::round(m_Total * 100));
}
void Checkout::calculateTotal()
{
    std::map<std::string, Item> itemPricesMap = m_PricingScheme.getItemMap();
    for (auto &i : m_Cart)
    {
        if (0 == i.second)
        {
            continue;
        }

        Item item = itemPricesMap[i.first];
        std::pair<std::string, double> bundle = item.getBundle();

        double costOfItems = 0;
        if (item.getBuyXGetY() != std::pair<int, int>{0, 0})
        {
            processBuyXGetY(costOfItems, item, i.second);
        }
        else if (bundle != std::pair<std::string, double>{std::string(), 0})
        {
            auto it = m_Cart.find(bundle.first);
            int numberOfSecondItem = 0;
            if (m_Cart.end() != it)
            {
                numberOfSecondItem = it->second;
                it->second = 0;
            }

            const int numberOfBundles = std::min(i.second, numberOfSecondItem);
            const int numberOfUnbundlables = std::abs(i.second - numberOfSecondItem);
            const double priceOfUnbundlables =
                (i.second > numberOfSecondItem) ? item.getUnitPrice() : itemPricesMap[bundle.first].getUnitPrice();
            const double taxOfUnbundables =
                (i.second > numberOfSecondItem) ? item.getTax() : itemPricesMap[bundle.first].getTax();

            costOfItems += bundle.second * numberOfBundles;
            costOfItems += (taxOfUnbundables + 1) * priceOfUnbundlables * numberOfUnbundlables;
        }
        else
        {
            costOfItems += (item.getTax() + 1) * item.getUnitPrice() * i.second;
        }
        i.second = 0;
        m_Total += costOfItems;
    }
}
void Checkout::processBuyXGetY(double &io_Sum, const Item &i_Item, int &io_NumberOf)
{
    const int buyX = i_Item.getBuyXGetY().first;
    const int getY = i_Item.getBuyXGetY().second;
    /// Default to # of remaining items in case there aren't enough left to be eligible
    /// for the deal
    int numberOfItemsToCalculate = io_NumberOf;

    /// If we still have enough to be eligible for the deal, subtract the buyX
    /// from the # of items (because we're about to sum them) and also subtract the
    /// getY (because they are free)
    if (io_NumberOf >= buyX)
    {
        numberOfItemsToCalculate = buyX;
        io_NumberOf -= buyX;
        io_NumberOf -= getY;
    }
    else
    {
        io_NumberOf = 0;
    }

    /// Update sum for cost of this batch of items
    io_Sum += (i_Item.getTax() + 1) * i_Item.getUnitPrice() * numberOfItemsToCalculate;

    /// If we still have items left at this point, just use recursion until we are done
    if (io_NumberOf > 0)
    {
        processBuyXGetY(io_Sum, i_Item, io_NumberOf);
    }
}