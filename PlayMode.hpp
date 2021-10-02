#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>
#include "TextRenderer.hpp"

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t pressed = 0;
	} button_a, button_b, button_c, enter;

	TextRenderer * fancyFont;
	TextRenderer * questionFont;
	TextRenderer * optionFont;
	TextRenderer * hintFont;

	//music coming from the tip of the leg (as a demonstration):
	std::shared_ptr< Sound::PlayingSample > leg_tip_loop;

	int questionIndex = -1; // -1 for title page; 6 for waiting page; 7 for result page
	int selectedOption = -1;
	size_t num_dot = 0;
	uint16_t acum = 0;
	size_t result_index1;
	size_t result_index2;
	size_t result_index3;
	float questionLineHeight = 40;
	float optionLineHeight = 80;
	// game texts
	std::vector<std::vector<std::string>> questions = {
		{"Question 1:", 
		"It is Saturday night, and your plan is to play a new released game.",
		" But suddenly you friend asks you to go to a party with him/her.",
		" You will ?"
		},
		{"Question 2:", 
		"Which of the following activities is the best for you to",
		" spend your holiday?"
		},
		{"Question 3:", 
		"You bought a box of your favorite ice-cream yesterday, and plan to eat",
		" it at night. But when you get back home, you realize your ",
		" roommate ate your ice-cream without asking your permission. ",
		" What will you do?"
		},
		{"Question 4:", 
		"You are in a long line to buy a new released game. And you have been",
		" waiting for it for so long. But the guy in front of you brings a group",
		" of people and cut-in the line. At the end, the store closes and you",
		" cannot buy the game because of them.",
		" You will?"
		},
		{"Question 5:", 
		"You are in your friend's birthday party. And he and some",
		"other people are having a great and fun discussion. You want to",
		"join them, but you are really hungry and the food is really good.",
		"What will you do?"
		},
		{"Question 6:", 
		"It is Sunday today and you are having a leisured day.",
		"What you going to do for dinner?"
		}
	};

	std::vector<std::vector<std::string>> options = {
		{"A. Tell your friend that you are not going.", 
		 "B. Ask your friend who else are going to the party, and decide base on that.",
		 "C. Go to the party."
		},
		{"A. Stay home and take a good rest.", 
		 "B. Play vedio games with your friends.",
		 "C. Go to Disneyland.",
		},
		{"A. Go find your roommate and be mad at him/her.", 
		 "B. Talk to your roommate peacefully about this.",
		 "C. Whatever. You will just go buy a new one.",
		},
		{"A. This won't happen because I would be mad already when they cut in the line.", 
		 "B. Complain to those people who cut in the line.",
		 "C. It's just a game. Not a big deal.",
		},
		{"A. The food is sooo good. I don't care about them.",
		 "B. Quickly eat something first, and then join them.",
		 "C. Go join the discussion. You can eat later.",
		},
		{"A. Order delivery.", 
		 "B. Go out and have dinner at some restaurant.",
		 "C. Cook a nice meal yourself.",
		},
	};

	std::string results[3][2][2] = {
		{{"Ox", "Lion"}, {"Sheep", "Elephant"}},	// group
		{{"Dog", "Monkey"}, {"Panda", "Rabbit"}},	// middle
		{{"Bear", "Tiger"}, {"Horse", "Cat"}}		// not group
	};


};
