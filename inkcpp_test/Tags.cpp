#include "catch.hpp"
#include "../inkcpp_cl/test.h"

#include <story.h>
#include <globals.h>
#include <runner.h>
#include <compiler.h>
#include <choice.h>

using namespace ink::runtime;

void HandleLine(ink::runtime::runner thread2, std::string line)
{
	std::cout << std::endl;
	std::cout << line << std::endl;
	std::cout << "line tags: " << thread2->num_line_tags() << " all tags: " << thread2->num_tags();
	std::cout << " line tags: ";
	for (int i = 0; i < thread2->num_line_tags(); i++)
	{
		std::cout << thread2->get_line_tag(i) << " ";
	}
	std::cout << " alltags: ";
	for (int i = 0; i < thread2->num_tags(); i++)
	{
		std::cout << thread2->get_tag(i) << " ";
	}
	std::cout << std::endl;
}


SCENARIO("run story with tags", "[tags]")
{
	GIVEN("a story with tags")
	{
		inklecate("ink/TagsStory.ink", "TagsStory.tmp");
		ink::compiler::run("TagsStory.tmp", "TagsStory.bin");
		auto ink = story::from_file("TagsStory.bin");
		runner thread = ink->new_runner();
		WHEN("start thread")
		{
			THEN("No tags")
			{
				REQUIRE(thread->has_tags() == false);
			}
			WHEN("approach first line")
			{
				std::string line = thread->getall();
				THEN("print no tags")
				{
					REQUIRE(line == "Hello\n");
				}
				THEN("collect all previous Tags (global, knot, line) in correct order")
				{
					REQUIRE(thread->has_tags() == true);
					REQUIRE(thread->num_tags() == 4);
					REQUIRE(std::string(thread->get_tag(0)) == "global_tag");
					REQUIRE(std::string(thread->get_tag(1)) == "knot_tag_start");
					REQUIRE(std::string(thread->get_tag(2)) == "second_knot_tag_start");
					REQUIRE(std::string(thread->get_tag(3)) == "output_tag_h");
				}
				WHEN("print choices")
				{
					auto itr = thread->begin();
					std::string choices[2] = {
						(itr++)->text(),
						(itr++)->text()
					};
					THEN("choices won't print tags, tags are still the same")
					{
						REQUIRE(choices[0] == "a");
						REQUIRE(choices[1] == "b");
						REQUIRE(thread->has_tags() == true);
						REQUIRE(thread->num_tags() == 4);
						REQUIRE(std::string(thread->get_tag(0)) == "global_tag");
						REQUIRE(std::string(thread->get_tag(1)) == "knot_tag_start");
						REQUIRE(std::string(thread->get_tag(2)) == "second_knot_tag_start");
						REQUIRE(std::string(thread->get_tag(3)) == "output_tag_h");
					}
					WHEN("choose divert")
					{
						thread->choose(1);
						THEN("choosing won't add tags!")
						{
							REQUIRE(thread->has_tags() == false);
							REQUIRE(thread->num_tags() == 0);
						}
						WHEN("proceed")
						{
							std::string line = thread->getall();
							THEN("new knot tag and now line tag, also choice tag. AND dont print tag in choice")
							{
								REQUIRE(line == "bKnot2\n");
								REQUIRE(thread->has_tags() == true);
								REQUIRE(thread->num_tags() == 3);
								REQUIRE(std::string(thread->get_tag(0)) == "choice_tag_b");
								REQUIRE(std::string(thread->get_tag(1)) == "knot_tag_2");
								REQUIRE(std::string(thread->get_tag(2)) == "output_tag_k");
							}
							WHEN("choose choice without tag, and proceed to end")
							{
								thread->choose(0);
								thread->getall();
								THEN("no tags, tags behind END are ignored")
								{
									REQUIRE(thread->has_tags() == false);
									REQUIRE(thread->num_tags() == 0);
								}
							}
						}
					}
				}
			}
		}
	}
}
SCENARIO("line tags test")
{
	GIVEN("story with line tags")
	{
		inklecate("ink/TagsStory2.ink", "TagsStory2.tmp");
		ink::compiler::run("TagsStory2.tmp", "TagsStory2.bin");
		auto ink = story::from_file("TagsStory2.bin");
		runner thread2 = ink->new_runner();
		WHEN("start thread2")
		{
			THEN("moveto")
			{
				auto ret = thread2->move_to(ink::hash_string("knot3"));
				REQUIRE(thread2->has_tags() == false);
				REQUIRE(ret == true);
			}
			WHEN("first line")
			{
				std::string line = thread2->getline();
				HandleLine(thread2, line);
				REQUIRE(thread2->num_line_tags() == 3);
				REQUIRE(std::string(thread2->get_line_tag(0)) == "tag1");
				REQUIRE(std::string(thread2->get_line_tag(1)) == "t2");
				REQUIRE(std::string(thread2->get_line_tag(2)) == "t3");
				
				THEN("second line")
				{
					std::string line = thread2->getline();
					HandleLine(thread2, line);
					REQUIRE(thread2->num_line_tags() == 2);
					REQUIRE(std::string(thread2->get_line_tag(0)) == "t4");
					REQUIRE(std::string(thread2->get_line_tag(1)) == "t5");
					
					THEN("third line")
					{
						std::string line = thread2->getline();
						HandleLine(thread2, line);
						REQUIRE(thread2->num_line_tags() == 2);
						REQUIRE(std::string(thread2->get_line_tag(0)) == "t6");
						REQUIRE(std::string(thread2->get_line_tag(1)) == "t7");

					}
				}
			}
			
		}
	}
}
SCENARIO("tags dump")
{
	GIVEN("story with line tags")
	{
		inklecate("ink/AHF.ink", "AHF.tmp");
		ink::compiler::run("AHF.tmp", "AHFStory.bin");
		auto ink = story::from_file("AHFStory.bin");
		runner thread = ink->new_runner();
		thread->move_to(ink::hash_string("test_knot1"));
		do
		{
			auto line = thread->getline();
			std::cout << "line: " << line << std::endl;
		} while (thread->can_continue());
		REQUIRE(thread->can_continue() == false);
		
		thread->move_to(ink::hash_string("test_tweet1"));
		do
		{
			auto line = thread->getline();
			std::cout << "line: " << line << std::endl;
		} while (thread->can_continue());
		REQUIRE(thread->can_continue() == false);
		thread->move_to(ink::hash_string("test_tweet2"));
		do
		{
			auto line = thread->getline();
			std::cout << "line: " << line << std::endl;
		} while (thread->can_continue());
		REQUIRE(thread->can_continue() == false);
		thread->move_to(ink::hash_string("test_tweet3"));
		do
		{
			auto line = thread->getline();
			std::cout << "line: " << line << std::endl;
		} while (thread->can_continue());
		REQUIRE(thread->can_continue() == false);
		
	}
}