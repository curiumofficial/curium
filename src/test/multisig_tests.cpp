// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "key.h"
#include "keystore.h"
#include "main.h"
#include "script/script.h"
#include "script/script_error.h"
#include "script/interpreter.h"
#include "script/sign.h"
#include "uint256.h"

#ifdef ENABLE_WALLET
#include "wallet_ismine.h"
#endif

#include <boost/assign/std/vector.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost::assign;

typedef vector<unsigned char> valtype;


BOOST_AUTO_TEST_SUITE_END()
