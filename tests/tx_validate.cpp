/*******************************************************************************
*   (c) 2018 ZondaX GmbH
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <jsmn.h>
#include <json/json_parser.h>
#include <json/tx_display.h>
#include <json/tx_validate.h>
#include <parser.h>
#include "util/common.h"

namespace {
    TEST(TxValidationTest, CorrectFormat) {
        auto transaction =
            R"({"jsonrpc":"2.0","id":1,"method":"contract.call","params":{"callSite":"member.transfer","callParams":{"amount":"20000000000","toMemberReference":"insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","publicKey":"-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEc+Vs4y+XWE77LR0QL1e1wCOFePFEHJIB\ndsPWPKMH5zGRhRWV1HCJXajENCV2bdG/YKKEOAzTdE5BGXNg2dRQpQ==\n-----END PUBLIC KEY-----","seed":"rOpiqgDHHDmr2PfI5UzZiWpjRyDMoFtBNFoOwyC+yJ8="}})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_ok) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, MissingMethod) {
        auto transaction =
            R"({"jsonrpc":"2.0","id":1,"params":{"callSite":"member.transfer","callParams":{"amount":"20000000000","toMemberReference":"insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","publicKey":"-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEc+Vs4y+XWE77LR0QL1e1wCOFePFEHJIB\ndsPWPKMH5zGRhRWV1HCJXajENCV2bdG/YKKEOAzTdE5BGXNg2dRQpQ==\n-----END PUBLIC KEY-----","seed":"rOpiqgDHHDmr2PfI5UzZiWpjRyDMoFtBNFoOwyC+yJ8="}})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);

        EXPECT_EQ(err, parser_json_missing_method) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, MissingCallsite) {
        auto transaction =
                R"({"jsonrpc":"2.0","id":1,"method":"contract.call","params":{"callParams":{"amount":"20000000000","toMemberReference":"insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","publicKey":"-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEc+Vs4y+XWE77LR0QL1e1wCOFePFEHJIB\ndsPWPKMH5zGRhRWV1HCJXajENCV2bdG/YKKEOAzTdE5BGXNg2dRQpQ==\n-----END PUBLIC KEY-----","seed":"rOpiqgDHHDmr2PfI5UzZiWpjRyDMoFtBNFoOwyC+yJ8="}})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_json_missing_callsite) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, UnsupportedMethod) {
        auto transaction =
                R"({"jsonrpc":"2.0","id":1,"method":"getseed"})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_json_unsupported_method) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, MissingSeed) {
        auto transaction =
            R"({"jsonrpc":"2.0","id":1,"method":"contract.call","params":{"callSite":"member.transfer","callParams":{"amount":"20000000000","toMemberReference":"insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","publicKey":"-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEc+Vs4y+XWE77LR0QL1e1wCOFePFEHJIB\ndsPWPKMH5zGRhRWV1HCJXajENCV2bdG/YKKEOAzTdE5BGXNg2dRQpQ==\n-----END PUBLIC KEY-----"}})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_json_missing_seed) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, MissingPublicKey) {
        auto transaction =
                R"({"jsonrpc":"2.0","id":1,"method":"contract.call","params":{"callSite":"member.transfer","callParams":{"amount":"20000000000","toMemberReference":"insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","seed":"rOpiqgDHHDmr2PfI5UzZiWpjRyDMoFtBNFoOwyC+yJ8="}})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_json_missing_public_key) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, Spaces_InTheMiddle) {
        auto transaction =
                R"({"jsonrpc":"2.0","id":1, "method":"contract.call", "params":{"callSite":"member.transfer","callParams":{"amount":"20000000000","toMemberReference":"insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","publicKey":"-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEc+Vs4y+XWE77LR0QL1e1wCOFePFEHJIB\ndsPWPKMH5zGRhRWV1HCJXajENCV2bdG/YKKEOAzTdE5BGXNg2dRQpQ==\n-----END PUBLIC KEY-----","seed":"rOpiqgDHHDmr2PfI5UzZiWpjRyDMoFtBNFoOwyC+yJ8="}})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_json_contains_whitespace) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, Spaces_AtTheFront) {
        auto transaction =
                R"({  "jsonrpc":"2.0","id":1,"method":"contract.call","params":{"callSite":"member.transfer","callParams":{"amount":"20000000000","toMemberReference":"insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","publicKey":"-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEc+Vs4y+XWE77LR0QL1e1wCOFePFEHJIB\ndsPWPKMH5zGRhRWV1HCJXajENCV2bdG/YKKEOAzTdE5BGXNg2dRQpQ==\n-----END PUBLIC KEY-----","seed":"rOpiqgDHHDmr2PfI5UzZiWpjRyDMoFtBNFoOwyC+yJ8="}})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_json_contains_whitespace) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, Spaces_AtTheEnd) {
        auto transaction =
                R"({"jsonrpc":"2.0","id":1,"method":"contract.call","params":{"callSite":"member.transfer","callParams":{"amount":"20000000000","toMemberReference":"insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","publicKey":"-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEc+Vs4y+XWE77LR0QL1e1wCOFePFEHJIB\ndsPWPKMH5zGRhRWV1HCJXajENCV2bdG/YKKEOAzTdE5BGXNg2dRQpQ==\n-----END PUBLIC KEY-----","seed":"rOpiqgDHHDmr2PfI5UzZiWpjRyDMoFtBNFoOwyC+yJ8=" }})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_json_contains_whitespace) << "Validation failed, error: " << parser_getErrorDescription(err);
    }

    TEST(TxValidationTest, AllowSpacesInString) {
        auto transaction =
                R"({"jsonrpc":" 2.0","id":1,"method":"contract.call","params":{"callSite":"member.transfer","callParams":{"amount":"20000000000","toMemberReference":" insolar:1AAEAATjWvxVC3DFEBqINu7JSKWxlcb_uJo7QdAHrcP8"},"reference":"insolar:1AAEAAY2zkW3pOTCIlYg6XqhWLR32AAeBpTKZ3vLdFhE","publicKey":"-----BEGIN PUBLIC KEY-----\nMFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEc+Vs4y+XWE77LR0QL1e1wCOFePFEHJIB\ndsPWPKMH5zGRhRWV1HCJXajENCV2bdG/YKKEOAzTdE5BGXNg2dRQpQ==\n-----END PUBLIC KEY-----","seed":"rOpiqgDHHDmr2PfI5UzZiWpjRyDMoFtBNFoOwyC+yJ8="}})";

        parsed_json_t json;
        parser_error_t err;

        err = JSON_PARSE(&json, transaction);
        ASSERT_EQ(err, parser_ok);

        err = tx_validate(&json);
        EXPECT_EQ(err, parser_ok) << "Validation failed, error: " << parser_getErrorDescription(err);
    }
}
