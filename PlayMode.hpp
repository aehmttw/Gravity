#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
    virtual void die();

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, w, a, s, d;

	//player position:
	glm::vec2 player_at = glm::vec2(256.0f * 8, 240.0f * 8);
    glm::vec2 player_vel = glm::vec2(0.0f, 0.0f);

	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
