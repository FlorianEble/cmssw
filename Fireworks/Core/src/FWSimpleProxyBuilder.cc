// -*- C++ -*-
//
// Package:     Core
// Class  :     FWGlimpseSimpleProxyBuilder
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones, Alja Mrak-Tadel
//         Created:  Tue March 28 09:46:41 EST 2010
// $Id: FWSimpleProxyBuilder.cc,v 1.3 2010/04/15 20:15:15 amraktad Exp $
//

// system include files
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/mem_fn.hpp>

// user include files
#include "TEveCompound.h"
#include "Fireworks/Core/interface/FWSimpleProxyBuilder.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWInteractionList.h"



//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FWSimpleProxyBuilder::FWSimpleProxyBuilder(const std::type_info& iType) :
   m_helper(iType)
{
}

// FWSimpleProxyBuilder::FWSimpleProxyBuilder(const FWSimpleProxyBuilder& rhs)
// {
//    // do actual copying here;
// }

FWSimpleProxyBuilder::~FWSimpleProxyBuilder()
{
}

//
// assignment operators
//
// const FWSimpleProxyBuilder& FWSimpleProxyBuilder::operator=(const FWSimpleProxyBuilder& rhs)
// {
//   //An exception safe implementation is
//   FWSimpleProxyBuilder temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void
FWSimpleProxyBuilder::itemChangedImp(const FWEventItem* iItem)
{
   if (iItem)
   {
      m_helper.itemChanged(iItem);
   }
}

void
FWSimpleProxyBuilder::build(const FWEventItem* iItem,
                            TEveElementList* product)
{
   size_t size = iItem->size();
   for (int index = 0; index < static_cast<int>(size); ++index)
   {
      TEveCompound* itemHolder = createCompound();
      product->AddElement(itemHolder);
      if (iItem->modelInfo(index).displayProperties().isVisible())
      {
         const void* modelData = iItem->modelData(index);
         build(m_helper.offsetObject(modelData),index, *itemHolder);
      }
   }
}

bool
FWSimpleProxyBuilder::specialModelChangeHandling(const FWModelId& iId, TEveElement* iCompound) {
   const FWEventItem::ModelInfo& info = iId.item()->modelInfo(iId.index());
   bool returnValue = false;
   if(info.displayProperties().isVisible() && iCompound->NumChildren()==0) {
      const void* modelData = iId.item()->modelData(iId.index());      
      build(m_helper.offsetObject(modelData),iId.index(),*iCompound);
      returnValue=true;
   }
   return returnValue;
}


//
// const member functions
//

//
// static member functions
//
std::string
FWSimpleProxyBuilder::typeOfBuilder()
{
   return std::string("simple#");
}
