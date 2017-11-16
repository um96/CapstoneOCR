//A file containing code for a hash table using quadratic probing.
//Part of this code was made previously in my (Uros') 335 class, and was edited and reused for this project,
//since I needed a way to store a large amount that is to be quickly accessed.





#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>


#include <iostream>



namespace {

// Internal method to test if a positive number is prime.
bool IsPrime(size_t n) {
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( size_t i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}


// Internal method to return a prime number at least as large as n.
int NextPrime(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!IsPrime(n)) n += 2;  
  return n;
}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class HashTable {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTable(size_t size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }
  
  int CurrentSize() {
  	return current_size_;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos)) {
    	return false;
	}
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash(); 
	  
	items_++;   
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos)) {
    	return false;
	} 
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

	items_++;
    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }
  
  int ArraySize() {
  	return array_.size();
  }
  
  int CollisionCounter() {
  	return collision_;
  }
  
  float get_LFA() {
  	return LoadFactorAmt();
  }
  
    
  
  int FindCol(const HashedObj & x) const {
  	int col_amt = 1;
  	size_t offset = 1;
    size_t current_pos = InternalHash(x);
      
    while (array_[current_pos].element_ != x) {
      current_pos += offset; 
	  col_amt++; // Compute ith probe.
      offset += 2;
      if (current_pos >= array_.size())
	current_pos -= array_.size();
    }
    return col_amt;
  }
  
  
  

 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;
  mutable int collision_ = 0;
  int items_ = 0;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }



  size_t FindPos(const HashedObj & x) const {
    size_t offset = 1;
    size_t current_pos = InternalHash(x);
      
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      current_pos += offset; 
	  collision_++; // Compute ith probe.
      offset += 2;
      if (current_pos >= array_.size())
	current_pos -= array_.size();
    }
    return current_pos;
  }
  

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    items_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
	

  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    
    return hf(x) % array_.size( );
    
  }
  
  float LoadFactorAmt() {

  	float LFA =  current_size_ / (float)array_.size();
  	//std::cout<<LFA;
  	return LFA;
  }
  
  
  
};

#endif  // QUADRATIC_PROBING_H
