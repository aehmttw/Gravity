#include "PlayMode.hpp"
#include "load_save_png.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#define null NULL
#include <random>

struct Palette
{
    uint8_t id;
    std::string file;
    uint8_t xOff = 0;
    uint8_t yOff = 0;

    Palette(uint8_t p, std::string pf)
    {
        id = p;
        file = "data/" + pf;
    }

    Palette(uint8_t p, std::string pf, uint8_t x, uint8_t y)
    {
        id = p;
        file = "data/" + pf;
        xOff = x;
        yOff = y;
    }

    Palette()
    {
        id = 0;
        file = "";
    }
};

struct Texture
{
    uint8_t id;
    std::string file;
    uint8_t xOff = 0;
    uint8_t yOff = 0;

    Texture(uint8_t t, std::string tf)
    {
        id = t;
        file = "data/" + tf;
    }

    Texture(uint8_t p, std::string pf, uint8_t x, uint8_t y)
    {
        id = p;
        file = "data/" + pf;
        xOff = x;
        yOff = y;
    }

    Texture()
    {
        id = 0;
        file = "";
    }
};

struct GameTile
{
    mutable Palette palette;
    mutable Texture texture;
    bool isSolid = false;
    bool isBad = false;

    GameTile(Palette p, Texture t)
    {
        palette = p;
        texture = t;
    }

    GameTile(Palette p, Texture t, bool solid)
    {
        palette = p;
        texture = t;
        isSolid = solid;
    }

    GameTile(Palette p, Texture t, bool solid, bool bad)
    {
        palette = p;
        texture = t;
        isSolid = solid;
        isBad = bad;
    }

    GameTile()
    {

    }

    void setPalette(Palette p)
    {
        palette = p;
    }
};

struct Sprite
{
    mutable Palette palette;
    mutable Texture texture;
    mutable int posX;
    mutable int posY;
    int spriteId;

    Sprite(Palette p, Texture t, int x, int y, int id)
    {
        palette = p;
        texture = t;
        posX = x;
        posY = y;
        spriteId = id;
    }
};

void updateSprite(Sprite s, PPU466 &ppu)
{
    int x = s.posX + ppu.background_position.x;
    int y = s.posY + ppu.background_position.y;

    if (x >= 256 || x < 0 || y >= 240 || y < 0)
    {
        x = 0;
        y = 240;
    }

    ppu.sprites[s.spriteId].x = (uint8_t) x;
    ppu.sprites[s.spriteId].y = (uint8_t) y;
    ppu.sprites[s.spriteId].index = s.texture.id;
    ppu.sprites[s.spriteId].attributes = s.palette.id;
}

const Palette ground_palette = Palette(0, "ground_palette.png");
Palette sky_palette_arrow = Palette(2, "sky_palettes.png", 0, 0);
Palette sky_palette_bg = Palette(3, "sky_palettes.png", 0, 2);
Palette astronaut_palette = Palette(4, "astronaut_palette.png", 0, 0);
const Palette win_palette = Palette(5, "win_palette.png");

const GameTile ground = GameTile(ground_palette, Texture(0, "ground.png"), true);
const GameTile grass = GameTile(ground_palette, Texture(1, "grass.png"), true);
const GameTile antimatter = GameTile(Palette(1, "antimatter_palette.png"), Texture(2, "antimatter.png"), false, true);
const GameTile sky[4] =
        {
                GameTile(sky_palette_arrow, Texture(3, "sky1.png")),
                GameTile(sky_palette_arrow, Texture(4, "sky2.png")),
                GameTile(sky_palette_bg, Texture(5, "sky3.png")),
                GameTile(sky_palette_bg, Texture(6, "sky4.png"))
        };

const Texture win_tex = Texture(56, "win.png");
const Texture astronaut_tex[8] =
        {
            Texture(32, "astronaut1.png", 0, 0),
            Texture(33, "astronaut1.png", 8, 0),
            Texture(34, "astronaut1.png", 0, 8),
            Texture(35, "astronaut1.png", 8, 8),
            Texture(36, "astronaut2.png", 0, 0),
            Texture(37, "astronaut2.png", 8, 0),
            Texture(38, "astronaut2.png", 0, 8),
            Texture(39, "astronaut2.png", 8, 8)
        };

const Texture gravity_tex[16] =
        {
                Texture(40, "s.png", 0, 0),
                Texture(41, "s.png", 8, 0),
                Texture(42, "s.png", 0, 8),
                Texture(43, "s.png", 8, 8),
                Texture(44, "a.png", 0, 0),
                Texture(45, "a.png", 8, 0),
                Texture(46, "a.png", 0, 8),
                Texture(47, "a.png", 8, 8),
                Texture(48, "w.png", 0, 0),
                Texture(49, "w.png", 8, 0),
                Texture(50, "w.png", 0, 8),
                Texture(51, "w.png", 8, 8),
                Texture(52, "d.png", 0, 0),
                Texture(53, "d.png", 8, 0),
                Texture(54, "d.png", 0, 8),
                Texture(55, "d.png", 8, 8)
        };

const Sprite gravity_left[4] = {
        Sprite(astronaut_palette, gravity_tex[4], 325 * 8, 263 * 8, 8),
        Sprite(astronaut_palette, gravity_tex[5], 325 * 8 + 8, 263 * 8, 9),
        Sprite(astronaut_palette, gravity_tex[6], 325 * 8, 263 * 8 + 8, 10),
        Sprite(astronaut_palette, gravity_tex[7], 325 * 8 + 8, 263 * 8 + 8, 11),
};

const Sprite gravity_up[4] = {
        Sprite(astronaut_palette, gravity_tex[8], 253 * 8, 294 * 8, 12),
        Sprite(astronaut_palette, gravity_tex[9], 253 * 8 + 8, 294 * 8, 13),
        Sprite(astronaut_palette, gravity_tex[10], 253 * 8, 294 * 8 + 8, 14),
        Sprite(astronaut_palette, gravity_tex[11], 253 * 8 + 8, 294 * 8 + 8, 15),
};

const Sprite gravity_right[4] = {
        Sprite(astronaut_palette, gravity_tex[12], 328 * 8, 273 * 8, 16),
        Sprite(astronaut_palette, gravity_tex[13], 328 * 8 + 8, 273 * 8, 17),
        Sprite(astronaut_palette, gravity_tex[14], 328 * 8, 273 * 8 + 8, 18),
        Sprite(astronaut_palette, gravity_tex[15], 328 * 8 + 8, 273 * 8 + 8, 19),
};

const Sprite win = Sprite(win_palette, win_tex, 417 * 8, 285 * 8, 63);

uint8_t gravity = 0;
int backgroundX[2] = {0, 0};
int backgroundY[2] = {0, 0};

bool unlockedGravities[4] = {true, false, false, false};

void loadTex(PPU466 &ppu, Texture t, int ox, int oy) {
    PPU466::Tile tile;
    glm::uvec2 size;
    std::vector< glm::u8vec4 > data;
    load_png(t.file, &size, &data, LowerLeftOrigin);

    for (uint8_t i = 0; i < 8; i++)
    {
        tile.bit0[i] = 0;
        tile.bit1[i] = 0;
        for (uint8_t j = 0; j < 8; j++)
        {
            // Set bit0/1 to R/G channel in tile
            tile.bit0[i] |= (data[(i + t.yOff + oy) * size[0] + j + t.xOff + ox][0] & 1) << j;
            tile.bit1[i] |= (data[(i + t.yOff + oy) * size[0] + j + t.xOff + ox][1] & 1) << j;
        }
    }

    ppu.tile_table[t.id] = tile;
}

void loadTex(PPU466 &ppu, Texture t)
{
    loadTex(ppu, t, 0, 0);
}


void loadPalette(PPU466 &ppu, Palette p) {
    glm::uvec2 size;
    std::vector< glm::u8vec4 > data;
    load_png(p.file, &size, &data, UpperLeftOrigin);

    PPU466::Palette palette;
    palette[0] = data[p.xOff + p.yOff * size[0]];
    palette[1] = data[(p.xOff + 1) + p.yOff * size[0]];
    palette[2] = data[p.xOff + (p.yOff + 1) * size[0]];
    palette[3] = data[(p.xOff + 1) + (p.yOff + 1) * size[0]];
    ppu.palette_table[p.id] = palette;
}

void loadTile(PPU466 &ppu, GameTile t)
{
    loadTex(ppu, t.texture);
    loadPalette(ppu, t.palette);
}

struct GameMap
{
    GameTile* tiles[512][480];

    GameMap(std::string file)
    {
        for (int i = 0; i < 512; i++)
        {
            for (int j = 0; j < 480; j++)
            {
                uint8_t mx = i % 2;
                uint8_t my = j % 2;
                uint8_t tile = mx + 2 * my;

                tiles[i][j] = const_cast<GameTile*>(&sky[tile]);
            }
        }

        glm::uvec2 size;
        std::vector< glm::u8vec4 > data;
        load_png(file, &size, &data, LowerLeftOrigin);

        for (int i = 0; i < 512; i++)
        {
            for (int j = 0; j < 480; j++)
            {
                glm::u8vec4 t = data[i + j * 512];

                const GameTile* ti = tiles[i][j];

                if (t[0] == 0 && t[1] == 0 && t[2] == 0)
                    ti = &ground;
                else if (t[0] == 0 && t[1] == 255 && t[2] == 0)
                    ti = &grass;
                else if (t[0] == 255 && t[1] == 0 && t[2] == 0)
                    ti = &antimatter;

                tiles[i][j] = const_cast<GameTile*>(ti);
            }
        }
    }

    void setSky(PPU466 &ppu, uint8_t grav)
    {
        sky_palette_arrow = Palette(2, "sky_palettes.png", 2 * grav, 0);
        sky_palette_bg = Palette(3, "sky_palettes.png", 2 * grav, 2);

        if (grav == 0)
        {
            sky[0].palette = sky_palette_arrow;
            sky[1].palette = sky_palette_arrow;
            sky[2].palette = sky_palette_bg;
            sky[3].palette = sky_palette_bg;
        }
        else if (grav == 1)
        {
            sky[0].palette = sky_palette_arrow;
            sky[1].palette = sky_palette_bg;
            sky[2].palette = sky_palette_arrow;
            sky[3].palette = sky_palette_bg;
        }
        else if (grav == 2)
        {
            sky[0].palette = sky_palette_bg;
            sky[1].palette = sky_palette_bg;
            sky[2].palette = sky_palette_arrow;
            sky[3].palette = sky_palette_arrow;
        }
        else if (grav == 3)
        {
            sky[0].palette = sky_palette_bg;
            sky[1].palette = sky_palette_arrow;
            sky[2].palette = sky_palette_bg;
            sky[3].palette = sky_palette_arrow;
        }

        loadPalette(ppu, sky_palette_arrow);
        loadPalette(ppu, sky_palette_bg);
    }

    void loadRegions(PPU466 &ppu)
    {
        for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y)
        {
            for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x)
            {
                int xr = x / (PPU466::BackgroundWidth / 2);
                int yr = y / (PPU466::BackgroundHeight / 2);

                int i = (int) x + backgroundX[xr] * (int) PPU466::BackgroundWidth / 2;
                int j = (int) y + backgroundY[yr] * (int) PPU466::BackgroundHeight / 2;

                GameTile t = *(tiles[(i % 512 + 512) % 512][(j % 480 + 480) % 480]);
                ppu.background[x + PPU466::BackgroundWidth * y] = t.texture.id + (t.palette.id << 8);
            }
        }
    }

    bool isSolid(int i, int j)
    {
        GameTile t = *(tiles[(i % 512 + 512) % 512][(j % 480 + 480) % 480]);
        return t.isSolid;
    }

    bool isBad(int i, int j)
    {
        GameTile t = *(tiles[(i % 512 + 512) % 512][(j % 480 + 480) % 480]);
        return t.isBad;
    }
};

GameMap map = GameMap("data/level.png");

PlayMode::PlayMode()
{
    loadTile(ppu, ground);
    loadTile(ppu, grass);
    loadTile(ppu, antimatter);
    loadTile(ppu, sky[0]);
    loadTile(ppu, sky[1]);
    loadTile(ppu, sky[2]);
    loadTile(ppu, sky[3]);
    loadPalette(ppu, astronaut_palette);
    loadPalette(ppu, win_palette);

    for (int i = 0; i < 8; i++)
    {
        loadTex(ppu, astronaut_tex[i]);
    }

    for (int i = 0; i < 16; i++)
    {
        loadTex(ppu, gravity_tex[i]);
    }

    loadTex(ppu, win_tex);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
            w.pressed = true;
            return true;
        } else if (evt.key.keysym.sym == SDLK_a) {
            a.pressed = true;
            return true;
        } else if (evt.key.keysym.sym == SDLK_s) {
            s.pressed = true;
            return true;
        } else if (evt.key.keysym.sym == SDLK_d) {
            d.pressed = true;
            return true;
        }
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
        } else if (evt.key.keysym.sym == SDLK_w) {
            w.pressed = false;
            return true;
        } else if (evt.key.keysym.sym == SDLK_a) {
            a.pressed = false;
            return true;
        } else if (evt.key.keysym.sym == SDLK_s) {
            s.pressed = false;
            return true;
        } else if (evt.key.keysym.sym == SDLK_d) {
            d.pressed = false;
            return true;
        }
	}

	return false;
}

const float gravity_force = 500.0f;

float max(float i, float j)
{
    return i > j ? i : j;
}

float min(float i, float j)
{
    return i < j ? i : j;
}

void PlayMode::die()
{
    gravity = 0;
    unlockedGravities[1] = false;
    unlockedGravities[2] = false;
    unlockedGravities[3] = false;
    player_at.x = 256.0f * 8;
    player_at.y = 240.0f * 8;
    player_vel.x = 0;
    player_vel.y = 0;
    for (int i = 0; i < 8; i++)
    {
        loadTex(ppu, astronaut_tex[i]);
    }
}

bool walking = false;
float timeToSwitchSprite = 0.1f;

void PlayMode::update(float elapsed2) {
	constexpr float speed = 60.0f;
    constexpr float jump_power = 200.0f;
    float threshold = 2.0f;

    float vel = max(abs(player_vel.x), abs(player_vel.y)) * elapsed2 * 100;

    int count = (int) max(1, min(20, vel / 100));
    for (int e = 0; e < count; e++)
    {
        float elapsed = elapsed2 / count;
        if (gravity == 0)
            player_vel.y -= gravity_force * elapsed;
        else if (gravity == 1)
            player_vel.x -= gravity_force * elapsed;
        else if (gravity == 2)
            player_vel.y += gravity_force * elapsed;
        else if (gravity == 3)
            player_vel.x += gravity_force * elapsed;

        if (gravity == 0 || gravity == 2)
        {
            walking = true;

            if (right.pressed)
                player_vel.x = speed;
            else if (left.pressed)
                player_vel.x = -speed;
            else
            {
                player_vel.x = 0;
                walking = false;
            }
        }

        if (gravity == 1 || gravity == 3)
        {
            walking = true;

            if (up.pressed)
                player_vel.y = speed;
            else if (down.pressed)
                player_vel.y = -speed;
            else
            {
                player_vel.y = 0;
                walking = false;
            }
        }

        int x = (int) (player_at.x / 8);
        int y = (int) (player_at.y / 8);

        int x2 = (int) ((player_at.x - 0.0001) / 8);
        int y2 = (int) ((player_at.y - 0.0001) / 8);

        bool grounded = false;

        if (map.isBad((int) ((player_at.x + 4) / 8), (int) ((player_at.y + 4) / 8)) ||
                map.isBad((int) ((player_at.x + 12) / 8), (int) ((player_at.y + 12) / 8)) ||
                map.isBad((int) ((player_at.x + 4) / 8), (int) ((player_at.y + 12) / 8)) ||
                map.isBad((int) ((player_at.x + 12) / 8), (int) ((player_at.y + 4) / 8)))
            die();

        // Win condition
        if (x >= 416 && x <= 418 && y <= 286 && y >= 284)
        {
            gravity = 0;
            player_at.x = 418 * 8;
            player_at.y = 244 * 8;
            player_vel.x = 0;
            player_vel.y = 0;
            for (int i = 0; i < 8; i++)
            {
                loadTex(ppu, astronaut_tex[i]);
            }
        }
        updateSprite(win, ppu);

        if (player_at.x >= gravity_left[0].posX - 16 && player_at.x <= gravity_left[0].posX + 16 && player_at.y >= gravity_left[0].posY - 16 && player_at.y <= gravity_left[0].posY + 16)
        {
            unlockedGravities[1] = true;
        }

        if (player_at.x >= gravity_up[0].posX - 16 && player_at.x <= gravity_up[0].posX + 16 && player_at.y >= gravity_up[0].posY - 16 && player_at.y <= gravity_up[0].posY + 16)
        {
            unlockedGravities[2] = true;
        }

        if (player_at.x >= gravity_right[0].posX - 16 && player_at.x <= gravity_right[0].posX + 16 && player_at.y >= gravity_right[0].posY - 16 && player_at.y <= gravity_right[0].posY + 16)
        {
            unlockedGravities[3] = true;
        }

        for (int i = x; i <= x2 + 2; i++)
        {
            if (map.isSolid(i, y2) && !map.isSolid(i, y2 + 1) && 8.0f * (y2 + 1) - player_at.y < threshold)
            {
                player_vel.y = max(player_vel.y, 0.0f);
                player_at.y = max(player_at.y, 8.0f * (y2 + 1));

                if (up.pressed)
                    player_vel.y = jump_power;

                if (gravity == 0)
                    grounded = true;
            }

            if (map.isSolid(i, y2 + 2) && !map.isSolid(i, y2 + 1) &&
                player_at.y - (int) (ceil(player_at.y) / 8) * 8 < threshold)
            {
                player_vel.y = min(player_vel.y, 0);
                player_at.y = min(player_at.y, (float) ((int) (ceil(player_at.y) / 8) * 8));

                if (down.pressed)
                    player_vel.y = -jump_power;

                if (gravity == 2)
                    grounded = true;
            }
        }

        for (int i = y; i <= y2 + 2; i++)
        {
            if (map.isSolid(x2, i) && !map.isSolid((x2 + 1), i) && 8.0f * (x2 + 1) - player_at.x < threshold)
            {
                player_vel.x = max(player_vel.x, 0.0f);
                player_at.x = max(player_at.x, 8.0f * (x2 + 1));

                if (right.pressed)
                    player_vel.x = jump_power;

                if (gravity == 1)
                    grounded = true;
            }

            if (map.isSolid((x2 + 2), i) && !map.isSolid((x2 + 1), i) &&
                player_at.x - (int) (ceil(player_at.x) / 8) * 8 < threshold)
            {
                player_vel.x = min(player_vel.x, 0.0f);
                player_at.x = min(player_at.x, (int) (ceil(player_at.x) / 8) * 8.0f);

                if (left.pressed)
                    player_vel.x = -jump_power;

                if (gravity == 3)
                    grounded = true;
            }
        }

        if (grounded)
        {
            int gravityBefore = gravity;

            if (w.pressed && unlockedGravities[2])
                gravity = 2;
            if (a.pressed && unlockedGravities[1])
                gravity = 1;
            if (s.pressed && unlockedGravities[0])
                gravity = 0;
            if (d.pressed && unlockedGravities[3])
                gravity = 3;

            if (gravityBefore != gravity)
            {
                for (int i = 0; i < 8; i++)
                {
                    loadTex(ppu, astronaut_tex[i], gravity * 16, 0);
                }
            }
        }

        player_at += player_vel * elapsed;

        if (walking)
            timeToSwitchSprite -= elapsed;

        if (timeToSwitchSprite < -0.1f)
            timeToSwitchSprite += 0.2f;
    }
    //reset button press counters:
    left.downs = 0;
    right.downs = 0;
    up.downs = 0;
    down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	ppu.background_color = glm::u8vec4(
		0,
        0,
        0,
		255
	);

    map.setSky(ppu, gravity);
    map.loadRegions(ppu);

    int playerOffX = PPU466::ScreenWidth / 2 - 8;
    int playerOffY = PPU466::ScreenHeight / 2 - 8;

	//background scroll:
	ppu.background_position.x = int32_t(-player_at.x + playerOffX);
	ppu.background_position.y = int32_t(-player_at.y + playerOffY);

    int gridX = (ppu.background_position.x / (int) PPU466::ScreenWidth);
    int gridY = (ppu.background_position.y / (int) PPU466::ScreenHeight);
    backgroundX[0] = ((-gridX + 1) / 2) * 2;
    backgroundX[1] = (-gridX / 2) * 2;
    backgroundY[0] = ((-gridY + 1) / 2) * 2;
    backgroundY[1] = (-gridY / 2) * 2;

    uint8_t extra = (uint8_t) ((timeToSwitchSprite < 0 && walking) ? 0 : 4);
    //player sprite:
	ppu.sprites[0].x = (uint8_t) playerOffX;
	ppu.sprites[0].y = (uint8_t) playerOffY;
	ppu.sprites[0].index = (uint8_t) (32 + extra);
	ppu.sprites[0].attributes = 4;
    ppu.sprites[1].x = (uint8_t) (playerOffX + 8);
    ppu.sprites[1].y = (uint8_t) playerOffY;
    ppu.sprites[1].index = (uint8_t) (33 + extra);
    ppu.sprites[1].attributes = 4;
    ppu.sprites[2].x = (uint8_t) playerOffX;
    ppu.sprites[2].y = (uint8_t) playerOffY + 8;
    ppu.sprites[2].index = (uint8_t) (34 + extra);
    ppu.sprites[2].attributes = 4;
    ppu.sprites[3].x = (uint8_t) (playerOffX + 8);
    ppu.sprites[3].y = (uint8_t) (playerOffY + 8);
    ppu.sprites[3].index = (uint8_t) (35 + extra);
    ppu.sprites[3].attributes = 4;

    int gravityOffX = 20;
    int gravityOffY = 20;
    int tileOffset = 8;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (gravity == i)
                ppu.sprites[(i + 1) * 4 + j].attributes = astronaut_palette.id;
            else
                ppu.sprites[(i + 1) * 4 + j].attributes = antimatter.palette.id;
        }
    }

    if (unlockedGravities[1] || unlockedGravities[2] || unlockedGravities[3])
    {
        // down
        int downY = -16;
        ppu.sprites[4].x = (uint8_t) gravityOffX;
        ppu.sprites[4].y = (uint8_t) (gravityOffY + downY);
        ppu.sprites[4].index = 40;
        ppu.sprites[5].x = (uint8_t) (gravityOffX + tileOffset);
        ppu.sprites[5].y = (uint8_t) (gravityOffY + downY);
        ppu.sprites[5].index = 41;
        ppu.sprites[6].x = (uint8_t) gravityOffX;
        ppu.sprites[6].y = (uint8_t) (gravityOffY + downY + tileOffset);
        ppu.sprites[6].index = 42;
        ppu.sprites[7].x = (uint8_t) (gravityOffX + tileOffset);
        ppu.sprites[7].y = (uint8_t) (gravityOffY + downY + tileOffset);
        ppu.sprites[7].index = 43;
    }
    else
    {
        ppu.sprites[4].y = 240;
        ppu.sprites[5].y = 240;
        ppu.sprites[6].y = 240;
        ppu.sprites[7].y = 240;
    }

    if (unlockedGravities[1])
    {
        // left
        int leftX = -16;
        ppu.sprites[8].x = (uint8_t) (gravityOffX + leftX);
        ppu.sprites[8].y = (uint8_t) gravityOffY;
        ppu.sprites[8].index = 44;
        ppu.sprites[9].x = (uint8_t) (gravityOffX + tileOffset + leftX);
        ppu.sprites[9].y = (uint8_t) gravityOffY;
        ppu.sprites[9].index = 45;
        ppu.sprites[10].x = (uint8_t) (gravityOffX + leftX);
        ppu.sprites[10].y = (uint8_t) (gravityOffY + tileOffset);
        ppu.sprites[10].index = 46;
        ppu.sprites[11].x = (uint8_t) (gravityOffX + tileOffset + leftX);
        ppu.sprites[11].y = (uint8_t) (gravityOffY + tileOffset);
        ppu.sprites[11].index = 47;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            updateSprite(gravity_left[i], ppu);
        }
    }

    if (unlockedGravities[2])
    {
        // up
        int upY = 16;
        ppu.sprites[12].x = (uint8_t) gravityOffX;
        ppu.sprites[12].y = (uint8_t) (gravityOffY + upY);
        ppu.sprites[12].index = 48;
        ppu.sprites[13].x = (uint8_t) (gravityOffX + tileOffset);
        ppu.sprites[13].y = (uint8_t) (gravityOffY + upY);
        ppu.sprites[13].index = 49;
        ppu.sprites[14].x = (uint8_t) gravityOffX;
        ppu.sprites[14].y = (uint8_t) (gravityOffY + upY + tileOffset);
        ppu.sprites[14].index = 50;
        ppu.sprites[15].x = (uint8_t) (gravityOffX + tileOffset);
        ppu.sprites[15].y = (uint8_t) (gravityOffY + upY + tileOffset);
        ppu.sprites[15].index = 51;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            updateSprite(gravity_up[i], ppu);
        }
    }

    if (unlockedGravities[3])
    {
        // right
        int rightX = 16;
        ppu.sprites[16].x = (uint8_t) (gravityOffX + rightX);
        ppu.sprites[16].y = (uint8_t) gravityOffY;
        ppu.sprites[16].index = 52;
        ppu.sprites[17].x = (uint8_t) (gravityOffX + tileOffset + rightX);
        ppu.sprites[17].y = (uint8_t) gravityOffY;
        ppu.sprites[17].index = 53;
        ppu.sprites[18].x = (uint8_t) (gravityOffX + rightX);
        ppu.sprites[18].y = (uint8_t) (gravityOffY + tileOffset);
        ppu.sprites[18].index = 54;
        ppu.sprites[19].x = (uint8_t) (gravityOffX + tileOffset + rightX);
        ppu.sprites[19].y = (uint8_t) (gravityOffY + tileOffset);
        ppu.sprites[19].index = 55;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            updateSprite(gravity_right[i], ppu);
        }
    }

	//--- actually draw ---
	ppu.draw(drawable_size);
}
