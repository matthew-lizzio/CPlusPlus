// Local
#include "PricingScheme.hpp"

PricingScheme::PricingScheme()
{
}

PricingScheme::~PricingScheme()
{
}

void PricingScheme::addItem(const Item &i_Item)
{
    m_ItemMap[i_Item.getId()] = i_Item;
}