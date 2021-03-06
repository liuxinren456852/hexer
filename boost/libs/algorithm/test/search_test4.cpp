/* 
   Copyright (c) Marshall Clow 2010-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org

    Testing the range-based interfaces
*/

#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/algorithm/searching/boyer_moore_horspool.hpp>
#include <boost/algorithm/searching/knuth_morris_pratt.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

typedef std::vector<std::string> vec;
#define NUM_TRIES   100

#define runOne(call, refDiff)   { \
    res = hexerboost::algorithm::call ( haystack, needle );  \
    if ( res != exp ) {                                 \
        std::cout << "Expected "                        \
            << exp - haystack.begin () << " got "       \
            << res - haystack.begin () << std::endl;    \
        throw std::runtime_error                        \
            ( "Unexpected result from " #call );        \
        }                                               \
    }
    
#define runObject(obj, refDiff) { \
    hexerboost::algorithm::obj <vec::const_iterator> s_o =   \
      hexerboost::algorithm::make_##obj  ( needle );         \
    res = s_o ( haystack );                             \
    if ( res != exp ) {                                 \
        std::cout << "Expected "                        \
        << exp - haystack.begin () << " got "           \
        << res - haystack.begin () << std::endl;        \
        throw std::runtime_error                        \
        ( "Unexpected result from " #obj " object" );   \
        }                                               \
    }

namespace {

    vec ReadFromFile ( const char *name ) {
        std::ifstream in ( name, std::ios_base::binary | std::ios_base::in );
        std::string temp;
        vec retVal;
        while ( std::getline ( in, temp ))
            retVal.push_back ( temp );
        
        return retVal;
        }
    
    void check_one ( const vec &haystack, const vec &needle, int expected ) {
        
        vec::const_iterator res;
        vec::const_iterator exp;        // the expected result
        
        if ( expected >= 0 )
            exp = haystack.begin () + expected;
        else if ( expected == -1 )
            exp = haystack.end ();      // we didn't find it1
        else if ( expected == -2 )
            exp = std::search ( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
        else    
            throw std::logic_error ( "Expected must be -2, -1, or >= 0" );

        std::cout << "Pattern is " << needle.size ()   << " entries long" << std::endl;
        std::cout << "Corpus  is " << haystack.size () << " entries long" << std::endl;

    //  First, the std library search
        res = std::search ( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
        if ( res != exp ) {
            std::cout << "Expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
            throw std::runtime_error ( "Unexpected result from std::search" );
            }

        runOne    ( boyer_moore_search,          stdDiff );
        runObject ( boyer_moore,                 stdDiff );
        runOne    ( boyer_moore_horspool_search, stdDiff );
        runObject ( boyer_moore_horspool,        stdDiff );
        runOne    ( knuth_morris_pratt_search,   stdDiff );
        runObject ( knuth_morris_pratt,          stdDiff );
        }
        
    }

int test_main( int , char* [] )
{
    vec c1  = ReadFromFile ( "search_test_data/0001.corpus" );
    vec p1b = ReadFromFile ( "search_test_data/0002b.pat" );
    vec p1e = ReadFromFile ( "search_test_data/0002e.pat" );
    vec p1n = ReadFromFile ( "search_test_data/0002n.pat" );
    vec p1f = ReadFromFile ( "search_test_data/0002f.pat" );

    std::cout << std::ios::fixed << std::setprecision(4);
//  std::cout << "Corpus is " << c1.size () << " entries long\n";
    std::cout << "--- Beginning ---" << std::endl;
    check_one ( c1, p1b, 0 );       //  Find it at position zero
    std::cout << "---- Middle -----" << std::endl;
    check_one ( c1, p1f, -2 );      //  Don't know answer
    std::cout << "------ End ------" << std::endl;
    check_one ( c1, p1e, c1.size() - p1e.size ());  
    std::cout << "--- Not found ---" << std::endl;
    check_one ( c1, p1n, -1 );      //  Not found
    
    return 0;
    }
