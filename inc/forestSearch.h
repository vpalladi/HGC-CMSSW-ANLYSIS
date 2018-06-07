/* provided by Andrew W Rose, 08/03/2018 */

#ifndef FORESTSEARCH_HPP
#define FORESTSEARCH_HPP

#include <stdint.h>
#include <stdlib.h>
#include <array>
#include <stdexcept>

namespace ForestSearch
{
  // Forward declaration of the internal Node type
  template< class Data , uint8_t KeySize > class Node;

  // #################################################################################################################################
  // The user interface
  // #################################################################################################################################
  template< class Data , uint8_t KeySize1 , uint8_t KeySize2 >
  class Search
  {
    typedef Node< Data , KeySize2 > ChildType; 
    typedef std::array< ChildType* , 1<<KeySize1 > ForestType;

  public:
    // Constructor - takes no arguments, fully specified by template parameters
    Search();
    
    // Destructor - tidy up after ourselves
    virtual ~Search();

    // Store data in the search-structure
    void push( const Data& aData );

    // Find the closest piece of stored data to a piece of probe data, as defined by the quadratic sum of the differences between keys
    template< class Data2 > const Data& search( const Data2& aData );

    // size
//    uint32_t size();
// size
    unsigned _size;

  private:
    
    // Store our trees
    ForestType mForest;
  };
  // #################################################################################################################################
}

#include "forestSearchBase.h"
// ====================================================================================================================


#endif //FORESTSEARCH_HPP
