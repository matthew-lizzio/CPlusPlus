// Local
#include "Item.hpp"

Item::Item(
    const std::string &i_Id,
    const double i_UnitPrice,
    const double i_Tax,
    const std::pair<int, int> &i_BuyXGetY)
    : m_Id(i_Id),
      m_UnitPrice(i_UnitPrice),
      m_Tax(i_Tax),
      m_BuyXGetY(i_BuyXGetY),
      m_Bundle({std::string(), 0})
{
}
Item::Item(
    const std::string &i_Id,
    const double i_UnitPrice,
    const std::pair<std::string, double> &i_Bundle,
    const double i_Tax)
    : m_Id(i_Id),
      m_UnitPrice(i_UnitPrice),
      m_Tax(i_Tax),
      m_BuyXGetY({0, 0}),
      m_Bundle(i_Bundle)
{
}
Item::Item()
    : m_Id(),
      m_UnitPrice(0),
      m_Tax(0),
      m_BuyXGetY({0, 0}),
      m_Bundle({std::string(), 0})
{
}
Item::Item(const Item &i_Item)
    : m_Id(i_Item.m_Id),
      m_UnitPrice(i_Item.m_UnitPrice),
      m_Tax(i_Item.m_Tax),
      m_BuyXGetY(i_Item.m_BuyXGetY),
      m_Bundle(i_Item.m_Bundle)
{
}
Item::~Item()
{
}
void Item::operator=(const Item &i_Item)
{
    const_cast<std::string &>(m_Id) = i_Item.m_Id;
    const_cast<double &>(m_UnitPrice) = i_Item.m_UnitPrice;
    const_cast<double &>(m_Tax) = i_Item.m_Tax;
    const_cast<std::pair<int, int> &>(m_BuyXGetY) = i_Item.m_BuyXGetY;
    const_cast<std::pair<std::string, double> &>(m_Bundle) = i_Item.m_Bundle;
}