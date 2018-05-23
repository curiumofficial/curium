// Copyright (c) 2012-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "key.h"
#include "keystore.h"
#include "main.h"
#include "script/script.h"
#include "script/script_error.h"
#include "script/sign.h"

#ifdef ENABLE_WALLET
#include "wallet_ismine.h"
#endif

#include <vector>

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE_END()
