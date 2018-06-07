/* provided by Andrew W Rose, 08/03/2018 */

// #################################################################################################################################
// Declaration of the internal Node type - should probably be in the hpp file for consistency, 
// but putting it here keeps the hpp file clean for users.
// Should really document this...
namespace ForestSearch
{
  template< class Data , uint8_t KeySize >
  class Node {
    typedef Node< Data , KeySize-1> ChildType; 
    friend class Node< Data , KeySize + 1>;
    template< class T , uint8_t KeySize1 , uint8_t KeySize2 > friend class Search;

  public:
    virtual ~Node();   
  private:
    Node( const Data* aData , uint32_t aKey );
    void push( const Data* aData , uint32_t aKey );
    void push_down( const Data* aData , uint32_t aKey );
    void search( const uint32_t aKey , const Data** aMinData , int32_t& aMinDist );

    const Data* mData;
    uint32_t mKey, mDistanceToCentre;
    enum { mKeyMask = (1<<(KeySize-1)) };
    ChildType *mLeft , *mRight;
  };

  template< class Data >
  class Node< Data , 0 > {
    friend class Node< Data , 1>;
    template< class T , uint8_t KeySize1 , uint8_t KeySize2 > friend class Search;

    class BottomOfTreeException: public std::exception { virtual const char* what() const throw() { return "Key fell off bottom of tree"; } };

  public:
    virtual ~Node();   
  private:
    Node( const Data* aData , uint32_t aKey );
    void push( const Data* aData , uint32_t aKey );
    void search( const uint32_t aKey , const Data** aMinData , int32_t& aMinDist );

    const Data* mData;
    uint32_t mKey, mDistanceToCentre;
  };
}
// #################################################################################################################################



// #################################################################################################################################
// Below here are the definitions of the template Search and Node class
// I use preprocessor directives to make the code more readable
// #################################################################################################################################


// #################################################################################################################################
#define TEMPLATE_PARAMS template < class Data , uint8_t KeySize >
#define CLASS_NAME ForestSearch::Node< Data , KeySize >

TEMPLATE_PARAMS
CLASS_NAME::Node( const Data* aData , uint32_t aKey ) : mData( aData ), mKey( aKey ) , mDistanceToCentre( abs( int32_t( aKey - mKeyMask ) ) ) , mLeft(NULL), mRight(NULL) {}
    
TEMPLATE_PARAMS
CLASS_NAME::~Node() {
  if( mLeft )  delete mLeft;                              // Tidy up after ourselves
  if( mRight ) delete mRight;                             // Tidy up after ourselves
  if( mData ) delete mData;
  mLeft = mRight = NULL;
  mData = NULL;
}

TEMPLATE_PARAMS
void CLASS_NAME::push( const Data* aData , uint32_t aKey ) {     
 
  uint32_t lDistanceToCentre   ( abs( int32_t( aKey - mKeyMask ) ) );

  if( lDistanceToCentre < mDistanceToCentre ) {  // Pass the current data to the child & store the new data
    push_down( mData , mKey );
    mData = aData , mKey = aKey , mDistanceToCentre = lDistanceToCentre;
  } else {                                                // Keep the current data & pass the new data to the child
    push_down( aData , aKey );
  }   
}

TEMPLATE_PARAMS
void CLASS_NAME::push_down( const Data* aData , uint32_t aKey ) {
  ChildType** lChild( (aKey & mKeyMask) ? &mRight : &mLeft );   // Select the left or right child using the MSB
  if( !(*lChild) ) (*lChild) = new ChildType( aData , aKey & ~mKeyMask );  // Create a child node with the MSB shifted one bit right
  else             (**lChild).push( aData , aKey & ~mKeyMask );                     // Or access the existing child
}

TEMPLATE_PARAMS
void CLASS_NAME::search( const uint32_t aKey , const Data** aMinData , int32_t& aMinDist ) {
  int32_t lDelta( aKey - mKey );
  int32_t lDelta2( lDelta * lDelta );
  
  if( lDelta2 < aMinDist ) *aMinData = mData , aMinDist = lDelta2; // If closer than previous minimum, update minimum

  ChildType** lChild( (aKey & mKeyMask) ? &mRight : &mLeft );               // Select the left or right child using the MSB
  if( *lChild ) (**lChild).search( aKey & ~mKeyMask , aMinData , aMinDist ); // If the child exists, visit it
}

#undef TEMPLATE_PARAMS
#undef CLASS_NAME
// #################################################################################################################################


// #################################################################################################################################
#define TEMPLATE_PARAMS template < class Data >
#define CLASS_NAME ForestSearch::Node< Data , 0 >

TEMPLATE_PARAMS
CLASS_NAME::Node( const Data* aData , uint32_t aKey ) : mData( aData ), mKey( aKey ) {}
    
TEMPLATE_PARAMS
CLASS_NAME::~Node() {
  if( mData ) delete mData;
  mData = NULL;
}

TEMPLATE_PARAMS
void CLASS_NAME::push( const Data* aData , uint32_t aKey ) {     
  if( mData ) {  
    throw BottomOfTreeException();
  } else {                                                
    mData = aData , mKey = aKey;
  }   
}

TEMPLATE_PARAMS
void CLASS_NAME::search( const uint32_t aKey , const Data** aMinData , int32_t& aMinDist ) {
  int32_t lDelta( aKey - mKey );
  int32_t lDelta2( lDelta * lDelta );
  
  if( lDelta2 < aMinDist ) *aMinData = mData , aMinDist = lDelta2; // If closer than previous minimum, update minimum
}

#undef TEMPLATE_PARAMS
#undef CLASS_NAME
// #################################################################################################################################



// #################################################################################################################################
#define TEMPLATE_PARAMS template < class Data , uint8_t KeySize1 , uint8_t KeySize2 >
#define CLASS_NAME ForestSearch::Search< Data , KeySize1 , KeySize2 >

TEMPLATE_PARAMS
CLASS_NAME::Search(): mForest( ForestType{{NULL}} ), _size(0) { }

TEMPLATE_PARAMS
CLASS_NAME::~Search() {
  for( auto i(mForest.begin()); i!=mForest.end(); ++i ) if (*i) delete (*i);
  mForest = ForestType{{NULL}};
}

TEMPLATE_PARAMS
void CLASS_NAME::push( const Data& aData ) {
  _size++;
  uint32_t lKey1 , lKey2;
  GetKeys( aData , lKey1 , lKey2 );

  if ( lKey1 >> KeySize1 ) throw std::out_of_range( "Key1 is outside range allowed by specified key size" );
  if ( lKey2 >> KeySize2 ) throw std::out_of_range( "Key2 is outside range allowed by specified key size" );
  
  ChildType** lChild( & mForest[ lKey1 ] );
  if( !(*lChild) ) (*lChild) = new ChildType( new Data( aData ) , lKey2 ); // Create a child node
  else             (**lChild).push( new Data( aData ) , lKey2 );                          // Or access the existing child
}

// ---------------------------------------------------------------------------------------------------
// Successive squares are spaced 2n+1 apart: Since we are scanning outwards by successive rows,
// for each new row we add an additional "2n+1" cost to the previously found minimum vertical distance, 
// to take into account the additional lateral distance, rather than repeatedly calculating the square
// of the known distance each time.
// The break condition is when MinDistance <= 0, since the squared distance can never go negative
// ---------------------------------------------------------------------------------------------------
TEMPLATE_PARAMS
template< class Data2 >
const Data& CLASS_NAME::search( const Data2& aData ) {
  uint32_t lKey1 , lKey2;
  GetKeys( aData , lKey1 , lKey2 );

  if ( lKey1 >> KeySize1 ) throw std::out_of_range( "Key1 is outside range allowed by specified key size" );
  if ( lKey2 >> KeySize2 ) throw std::out_of_range( "Key2 is outside range allowed by specified key size" );
  
  const Data* lMinData( NULL );
  typename ForestType::const_iterator lPos( mForest.begin() + lKey1 );
  typename ForestType::const_reverse_iterator lNeg( mForest.begin() + lKey1 );
  for( int32_t lMinDist( 0x7FFFFFFF ) , lCost(1) ; lMinDist > 0 ; lMinDist -= lCost , lCost += 2 ) {
    if( lPos != mForest.end() ){
      if( *lPos ) (**lPos).search( lKey2 , &lMinData , lMinDist );
      lPos++;
    }
    if( lNeg != mForest.rend() ){
      if( *lNeg ) (**lNeg).search( lKey2 , &lMinData , lMinDist );
      lNeg++;
    }
  }

  return *lMinData;
}

//TEMPLATE_PARAMS
//template< class Data2 >
//uint32_t CLASS_NAME::size() {
//    return _size;
//}

#undef TEMPLATE_PARAMS
#undef CLASS_NAME
// #################################################################################################################################
