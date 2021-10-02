#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

Load< Sound::Sample > dusty_floor_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("dusty-floor.opus"));
});

PlayMode::PlayMode(){

	//start music loop playing:
	// (note: position will be over-ridden in update())
	leg_tip_loop = Sound::loop_3D(*dusty_floor_sample, 1.0f, glm::vec3(0), 10.0f);

	questionFont = new TextRenderer(data_path("SeratUltra-1GE24.ttf"));
	fancyFont = new TextRenderer(data_path("BroadwayRegular-7Bpow.ttf"));
	optionFont = new TextRenderer(data_path("ReallyFree-ALwl7.ttf"));
	hintFont = new TextRenderer(data_path("OpenSans-B9K8.ttf"));
}

PlayMode::~PlayMode() {
	delete questionFont;
	delete fancyFont;
	delete optionFont;
	delete hintFont;
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_a) {
			button_a.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_b) {
			button_b.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_c) {
			button_c.pressed = true;
			return true;
		}else if (evt.key.keysym.sym == SDLK_RETURN) {
			enter.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			button_a.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_b) {
			button_b.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_c) {
			button_c.pressed = false;
			return true;
		}else if (evt.key.keysym.sym == SDLK_RETURN) {
			enter.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {

	{ //update listener 
		Sound::listener.set_position_right(glm::vec3(0),glm::vec3(0),0);
	}

	// key press event
	if (enter.pressed){
		// enter questions
		if(questionIndex == -1){
			questionIndex = 0;
		}
		// next question
		else if (questionIndex < 6){
			// nothing selected
			if (selectedOption == -1){
				return;
			}
			else{
				acum += selectedOption;
				// q1-2
				if(questionIndex == 1){
					// not group
					if(acum < 2)
						result_index1 = 2;
					// mid
					else if (acum == 2)
						result_index1 = 1;
					// group
					else	
						result_index1 = 0;
					acum = 0;
				}
				// q3-4
				else if(questionIndex == 3){
					// not angry
					if(acum >= 2)
						result_index2 = 1;
					// angry
					else	
						result_index2 = 0;
					acum = 0;
				}
				// q5-6
				else if(questionIndex == 5){
					// lazy eater
					if(acum < 2)
						result_index3 = 0;
					// not lazy eater
					else	
						result_index3 = 1;
					acum = 0;
				}
				questionIndex += 1;
				selectedOption = -1;
			}
		}
	}
	if(questionIndex >=0 && questionIndex <= 5){
		if(button_a.pressed)
			selectedOption = 0;
		else if(button_b.pressed)
			selectedOption = 1;
		else if (button_c.pressed)
			selectedOption = 2;
	}

	// prepare for result
	if(questionIndex == 6){
		// total time check
		static float prepareTime = 0;
		prepareTime += elapsed;
		if(prepareTime >= 3 )
			questionIndex = 7;
		// anim time time
		static float acumTime = 0;
		acumTime += elapsed;
		if(acumTime >= 0.25f){
			num_dot = (num_dot + 1) % 7;
			acumTime = 0;
		}
	}

}

void PlayMode::draw(glm::uvec2 const &drawable_size) {

	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw title page
	if(questionIndex == -1){
		// title
		fancyFont->draw("JUST    LIKE", 110.0f, 400.0f, glm::vec2(1,0.9), glm::vec3(250.0/255, 192.0/255, 137.0/255));
		fancyFont->draw("    ANIMAL   ", 110.0f, 250.0f, glm::vec2(1,0.9), glm::vec3(250.0/255, 192.0/255, 137.0/255));

		// draw hint text
		hintFont->draw("A test that tells you what animal you could be, if not human.", 20.0f, 60.0f, glm::vec2(0.2,0.25), glm::vec3(0.8, 0.8f, 0.2f));
		hintFont->draw("Press Enter to start", 20.0f, 20.0f, glm::vec2(0.2,0.25), glm::vec3(0.2, 0.4f, 0.9f));

		// bg color
		glClearColor(174.0f/255, 136.0f/255, 184.0f/255, 1.0f);
	}

	// draw question page
	if (questionIndex >=0 && questionIndex <=5){
		int offset = 0;
		// draw questions text
		for (auto line : questions[questionIndex]){
			questionFont->draw(line, 25.0f, 550.0f - offset*questionLineHeight, glm::vec2(0.23,0.28), glm::vec3(184.0/255, 248.0/255, 255.0/255));
			offset += 1;
		}
		offset = 0;
		// draw optiuons text
		for (size_t i = 0; i < options[questionIndex].size(); i++){
			optionFont->draw(options[questionIndex][i], 30.0f, 310.0f - offset*optionLineHeight, glm::vec2(0.36,0.36), glm::vec3(0.5, 0.8f, 0.2f));
			// render selected mark
			if(i == selectedOption)
				fancyFont->draw("X", 10.0f, 310.0f - offset*optionLineHeight, glm::vec2(0.2,0.25), glm::vec3(1.0f, 0.2f, 0.2f));
			offset += 1;
		}
		// draw hint text
		hintFont->draw("* Press A,B,C on your keyboard to select. * Press Enter to move on.", 10.0f, 10.0f, glm::vec2(0.15,0.18), glm::vec3(188.0/255, 184.0/255, 255.0/255));

		// bg color
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	}

	// prepare for result
	if(questionIndex == 6){
		questionFont->draw("Calculating result", 40.0f, 330.0f, glm::vec2(0.5,0.5), glm::vec3(184.0/255, 248.0/255, 255.0/255));
		hintFont->draw(std::string(num_dot, '*'), 40.0f, 240.0f, glm::vec2(0.5,0.5), glm::vec3(184.0/255, 248.0/255, 255.0/255));
	}

	// display result
	if(questionIndex == 7){
		optionFont->draw("You are a " + results[result_index1][result_index2][result_index3] + " !!!", 
			20.0f, 310.0f, glm::vec2(1,1), glm::vec3(247.0/255, 241.0/255, 52.0/255));
		glClearColor(203.0f/255, 150.0f/255, 217.0f/255, 1.0f);
	}
	GL_ERRORS();
}

