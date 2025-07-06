#include <dpp/dpp.h>
#include "botInfo.hpp"
#include "filter.hpp"
#include "messageCleaner.hpp"
#include <algorithm>
#include <vector>
#include <sstream> 
#include <random>
#include <chrono>
#include <thread>
#include <functional>
#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <future>
#include <set>

filter Filter;

messageCleaner message_cleaner;


int main()
{
	
	/* Create bot cluster */
	dpp::cluster bot(botInfo().get_token(), dpp::i_default_intents | dpp::i_direct_messages | dpp::i_direct_message_reactions | dpp::i_direct_message_typing | dpp::i_message_content);
	/* Output simple log messages to stdout */
	bot.on_log(dpp::utility::cout_logger());
	
	/* Register slash command here in on_ready */
	bot.on_ready([&bot](const dpp::ready_t& event) {
		/* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
		if (dpp::run_once<struct register_bot_commands>()) {
			std::vector<dpp::slashcommand> commands {
				{ "ping", "Ping pong!", bot.me.id }
			};

			bot.global_bulk_command_create(commands);
		}
	});

	/* Handle slash command with the most recent addition to D++ features, coroutines! */
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
         if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
    });
    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        std::string message_content = event.msg.content;                 // content of user messages that'll be changed to lower
        const std::string original_message_content = message_content;     // original message content that won't change
        // if a phrase is found first don't check individual words
        bool cont{ false };
		std::string announcement = "*Announcement*";
		std::string announcement2 = "_Announcement_";
		if (event.msg.author.id != dpp::snowflake(1320617546168139888) && event.msg.channel_id == dpp::snowflake(1297692999462948877)) {
			if (original_message_content.length() < announcement.length() || ((original_message_content.substr(0, announcement.length()) != announcement) && (original_message_content.substr(0, announcement.length()) != announcement2)) ) {
				bot.message_delete(event.msg.id, event.msg.channel_id);
			}
		}


        if (!(event.msg.author.is_bot())) { // checks if message is from a bot
            std::transform(message_content.begin(), message_content.end(), message_content.begin(), // transforms message to lower
                [](unsigned char c) { return std::tolower(c); });
			

            std::stringstream str_stream;
            str_stream << message_content;
			std::string word;

			message_cleaner.clean_message(message_content);



			if (original_message_content.length() > 13 && original_message_content.substr(0, 13) == "Anon message:") {
				if (Filter.check_message(message_content)) {
					bot.message_create(dpp::message(dpp::snowflake(1332450361021038716), original_message_content.substr(13, original_message_content.length())));
				}
				
			}
            while (str_stream >> word && !cont) {


                if (word == "pepperoni") {
                    bot.message_create(dpp::message(event.msg.channel_id, "Ban that guy"));
                }
            }
        }
        });

	/* Start the bot */
	bot.start(dpp::st_wait);

	return 0;
}
