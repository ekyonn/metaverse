/**
 * Copyright (c) 2016-2017 mvs developers 
 *
 * This file is part of metaverse-explorer.
 *
 * metaverse-explorer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/property_tree/ptree.hpp>      
#include <boost/property_tree/json_parser.hpp>

#include <metaverse/bitcoin.hpp>
#include <metaverse/client.hpp>
#include <metaverse/explorer/define.hpp>
#include <metaverse/explorer/callback_state.hpp>
#include <metaverse/explorer/display.hpp>
#include <metaverse/explorer/prop_tree.hpp>
#include <metaverse/explorer/dispatch.hpp>
#include <metaverse/explorer/extensions/command_extension.hpp>
#include <metaverse/explorer/extensions/command_extension_func.hpp>
#include <metaverse/explorer/extensions/command_assistant.hpp>

namespace libbitcoin {
namespace explorer {
namespace commands {

namespace pt = boost::property_tree;

/************************ createrawtx *************************/

class createrawtx: public command_extension
{
public:
    static const char* symbol(){ return "createrawtx";}
    const char* name() override { return symbol();} 
    const char* category() override { return "EXTENSION"; }
    const char* description() override { return "createrawtx "; }

    arguments_metadata& load_arguments() override
    {
        return get_argument_metadata();
    }

    void load_fallbacks (std::istream& input, 
        po::variables_map& variables) override
    {
        const auto raw = requires_raw_input();
        //load_input(auth_.name, "ACCOUNTNAME", variables, input, raw);
        //load_input(auth_.auth, "ACCOUNTAUTH", variables, input, raw);
    }

    options_metadata& load_options() override
    {
        using namespace po;
        options_description& options = get_option_metadata();
        options.add_options()
		(
            BX_HELP_VARIABLE ",h",
            value<bool>()->zero_tokens(),
            "Get a description and instructions for this command."
        )
        (
            BX_CONFIG_VARIABLE ",c",
            value<boost::filesystem::path>(),
            "The path to the configuration settings file."
        )
		(
			"type,t",
			value<uint16_t>(&option_.type)->required(),
			"Transaction type. 0 -- transfer etp, 3 -- transfer asset, 6 -- just only send message"
		)
		(
			"senders,s",
			value<std::vector<std::string>>(&option_.senders)->required(),
			"Send from addresses"
		)
        (
            "receivers,r",
            value<std::vector<std::string>>(&option_.receivers)->required(),
            "Send to [address:amount]. amount is asset number if sybol option specified"
        )
        (
            "symbol,n",
            value<std::string>(&option_.symbol)->default_value(""),
            "asset name, not specify this option for etp tx"
        )
        (
            "mychange,m",
            value<std::string>(&option_.mychange_address),
            "Mychange to this address, includes etp and asset change"
        )
        (
            "message,i",
            value<std::string>(&option_.message),
            "Message/Information attached to this transaction"
        )
        (
            "fee,f",
            value<uint64_t>(&option_.fee)->default_value(10000),
            "The fee of tx. default_value 0.0001 etp"
        );

        return options;
    }

    void set_defaults_from_config (po::variables_map& variables) override
    {
    }

    console_result invoke (std::ostream& output,
        std::ostream& cerr, libbitcoin::server::server_node& node) override;

    struct argument
    {
        argument()
        {
        }
    } argument_;

    struct option
    {
        uint16_t type;
		std::vector<std::string> senders;
		std::vector<std::string> receivers;
		std::string symbol;
        std::string mychange_address;
        std::string message;
		uint64_t fee;
		
    } option_;

};

} // namespace commands
} // namespace explorer
} // namespace libbitcoin
